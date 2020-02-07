#include "RSPoolAllocator.h"

#include<iostream>
#include<utility>
using namespace RSMem;

// template<typename T, typename ... Args>
// T* alloc(Allocator& allocator, Args&& ... args)
// {
//     auto ptr = allocator.Allocate(sizeof(T), alignof(T));
//     return new(ptr) T(std::forward<Args>(args)...);
// }

// #define RSMEM_NEW(Allocator, Type, ...) alloc<Type>(Allocator, __VA_ARGS__)

struct A
{
    int x;
    int y;
    int z;
    char c;
};


int main()
{   
    
    std::cout << sizeof(A) << '\n';
    std::cout << alignof(A) << "\n\n\n";
    PoolAllocator pa(sizeof(A), 10, alignof(A));
    for(auto i = 0; i < 10; i++)
    {
        std::cout << pa.Allocate() << '\n';
    }

}