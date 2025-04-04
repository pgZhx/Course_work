#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#include "queue.h"
// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;
Semaphore chops_1;
Semaphore chops_2;
Semaphore chops_3;
Semaphore chops_4;
Semaphore chops_5;
void wait(){
    for (int i = 0;i < 0xfffffff;i++){}
}
void philo_1(void *arg)
{   
   do{
       chops_1.P();
      wait();
       chops_5.P();
       printf("philo_1 is eating. (6313) \n");
        wait();
       chops_1.V();
       chops_5.V();
       wait();
    }while(true);
}
void philo_2(void *arg)
{
    do{
      chops_1.P();
      wait();
     chops_2.P();
       printf("philo_2 is eating. \n");
        wait();
       chops_2.V(); 
       chops_1.V();
       wait();
    }while(true);
}
void philo_3(void *arg)
{
    do{
       chops_3.P();
      wait();
       chops_2.P();
       printf("philo_3 is eating. \n");
       wait();
       chops_3.V();
       chops_2.V();
       wait();
    }while(true);
}
void philo_4(void *arg)
{
   do{
     chops_3.P();
      wait();
      chops_4.P();
       printf("philo_4 is eating.\n");
       wait();
       chops_4.V();
        chops_3.V();
       wait();
    }while(true);
}
void philo_5(void *arg)
{
    do{
       chops_5.P();
       //wait();
       chops_4.P();
       printf("philo_5 is eating.\n");
       wait();
       chops_5.V();
        chops_4.V();
       wait();
    }while(true);
}
void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);
    chops_1.initialize(1);
    chops_2.initialize(1);
    chops_3.initialize(1);
    chops_4.initialize(1);
    chops_5.initialize(1);   
    programManager.executeThread(philo_1, nullptr, "second thread", 1);
    programManager.executeThread(philo_2, nullptr, "third thread", 1);
    programManager.executeThread(philo_3, nullptr, "fourth thread", 1); 
    programManager.executeThread(philo_4, nullptr, "fifth thread", 1); 
    programManager.executeThread(philo_5, nullptr, "sixth thread", 1);
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