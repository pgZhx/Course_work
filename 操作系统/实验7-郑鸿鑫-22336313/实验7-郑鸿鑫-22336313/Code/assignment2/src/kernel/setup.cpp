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
// void test_memory_allocation() {
//     unsigned int offset = 6313; // 学号后四位作为偏移量

//     // 初始化分页机制
//     MemoryManager mem_mgr;
//     mem_mgr.openPageMechanism();

//     // 申请内存
//     void* addr1 = mem_mgr.allocateVirtualMemory(AddressPoolType::KERNEL, 4096, offset); // 申请一页
//     void* addr2 = mem_mgr.allocateVirtualMemory(AddressPoolType::USER, 8192, offset); // 申请两页

//     // 打印申请结果
//     printf("Allocated memory at virtual address: %p\n", addr1);
//     printf("Allocated memory at virtual address: %p\n", addr2);

//     // 释放内存
//     mem_mgr.freeVirtualMemory(AddressPoolType::KERNEL, addr1, offset);
//     mem_mgr.freeVirtualMemory(AddressPoolType::USER, addr2, offset);

//     // 打印释放结果
//     printf("Freed memory at virtual address: %p\n", addr1);
//     printf("Freed memory at virtual address: %p\n", addr2);
// }
void first_thread(void *arg)
{
    // 第1个线程不可以返回
    // stdio.moveCursor(0);
    // for (int i = 0; i < 25 * 80; ++i)
    // {
    //     stdio.print(' ');
    // }
    // stdio.moveCursor(0);
    //test_memory_allocation();
    memoryManager.allocatePhysicalPages(KERNEL,15984);
    memoryManager.releasePhysicalPages(KERNEL,0x2233631,3);
    memoryManager.allocatePhysicalPages(KERNEL,1); 
    memoryManager.allocatePhysicalPages(KERNEL,5);
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
    memoryManager.openPageMechanism();
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
