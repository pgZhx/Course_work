#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#include "memory.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;
// 内存管理器
MemoryManager memoryManager;
void testBoundaryConditions(MemoryManager& memoryManager, AddressPoolType type) {
    // 分配0个页面
    int zeroPages = memoryManager.allocatePhysicalPages(type, 0);
    if (zeroPages != 0) {
        printf("Allocated 0 pages, but got %x\n", zeroPages);
        return;
    }
    // 分配超过总量的页面
    int tooManyPages = memoryManager.allocatePhysicalPages(type, memoryManager.getTotalMemory() / PAGE_SIZE + 1);
    if (tooManyPages != 0) {
        printf("Allocated too many pages, but got %x\n", tooManyPages);
        return;
    }
    printf("Boundary conditions test passed\n");
}

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    // 执行边界条件测试
    testBoundaryConditions(memoryManager, AddressPoolType::KERNEL);
    int page = memoryManager.allocatePhysicalPages(AddressPoolType::KERNEL, 1);
    if (page != 0) {
        printf("Allocated page at address: %x\n", page);
    } else {
        printf("Allocation failed\n");
    }
    // 释放页面
    memoryManager.releasePhysicalPages(AddressPoolType::KERNEL, page, 1);
    printf("Released page\n");
    asm_halt();
}

extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 内存管理器
    memoryManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}
