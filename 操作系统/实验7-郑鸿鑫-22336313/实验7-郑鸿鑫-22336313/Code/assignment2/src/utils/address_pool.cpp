#include "address_pool.h"
#include "os_constant.h"
#include "stdio.h"
AddressPool::AddressPool()
{
}

// 设置地址池BitMap
void AddressPool::initialize(char *bitmap, const int length, const int startAddress)
{
    resources.initialize(bitmap, length);
    this->startAddress = startAddress;
}

// 从地址池中分配count个连续页
int AddressPool::allocate(const int count)
{
    int start = resources.allocate(count);
    if(start == -1) printf("allocate failed");
    else  printf("allocate memory from 0x%x to 0x%x\n",start * PAGE_SIZE + startAddress,(start+count) * PAGE_SIZE + startAddress);
    return (start == -1) ? -1 : (start * PAGE_SIZE + startAddress);
}
// 释放若干页的空间
void AddressPool::release(const int address, const int amount)
{
    printf("release memory from Page%d to Page%d\n",address/PAGE_SIZE, address/PAGE_SIZE+amount);
    resources.release((address - startAddress) / PAGE_SIZE, amount);
}