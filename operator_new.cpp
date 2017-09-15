#include <iostream>

#define MAX_ALLOC_MEMORY_PER (1024*1024*1024)

void* operator new(std::size_t size)
{
    static unsigned long cur_top_addr = 0;
    static unsigned long  cur_addr_end = 0;
    void* res = NULL;
    if (cur_top_addr + size <= (unsigned long)cur_addr_end)
    {
        res = (void*)cur_top_addr;
        cur_top_addr += size;
    }else
    {
        void* newMem = (void*)malloc(MAX_ALLOC_MEMORY_PER);
        if (newMem == NULL)
            throw -1;
        cur_addr_end = (unsigned long)(newMem) + MAX_ALLOC_MEMORY_PER;
        cur_top_addr = (unsigned long)newMem;
        res = (void*)cur_top_addr;
        cur_top_addr += size;
    }
    return res;
}

void operator delete(void* pointer)
{

}

int main()
{
	return 0;
}
