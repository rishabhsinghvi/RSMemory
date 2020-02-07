#ifndef RS_POOL_ALLOCATOR_H
#define RS_POOL_ALLOCATOR_H

#include<cstddef>

namespace RSMem
{
    // The Pool Allocator interface is sufficiently different from the Allocator interface.
    // As such, I think it is best if PoolAllocator is a standalone class.

    struct Block
    {
        Block* m_Next;
    };

    class PoolAllocator
    {
    private:
        void* m_StartPtr = nullptr;
        Block* m_FreeListHead = nullptr;
        std::size_t m_ArenaSize = 0;
        std::size_t m_RemainingAllocations = 0;
        std::size_t m_PossibleAllocations = 0;
    
    public:

        PoolAllocator() = delete;

        PoolAllocator(std::size_t objectSize, std::size_t maxObjects, std::size_t alignment);

        ~PoolAllocator();

        void* Allocate();

        void Free(void* block);

        std::size_t GetPossibleAllocations() const;

        std::size_t GetRemainingAllocations() const;
    };
}

#endif