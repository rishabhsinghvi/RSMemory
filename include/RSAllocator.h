#ifndef RS_ALLOCATOR_H
#define RS_ALLOCATOR_H

#include<cstddef>

namespace RSMem
{
    class Allocator
    {
    protected:
        void* m_StartPtr = nullptr;
        std::size_t m_ArenaSize = 0;
        std::size_t m_Allocations = 0;

    public:

        Allocator();

        Allocator(std::size_t arenaSize);

        virtual ~Allocator();

        virtual void* Allocate(std::size_t numBytes, std::size_t alignment) = 0;

        virtual void Free() = 0;

        virtual void Reset() = 0;

    };
}

#endif