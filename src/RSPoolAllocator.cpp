#include "RSPoolAllocator.h"
#include "RSExceptions.h"
#include "RSMemUtils.h"

#include<cstdlib>
#include<cstdint>

#ifdef RS_ENABLE_DEBUG_INFO
#include<iostream>
#endif

#include<cassert>
#include<cmath>

namespace RSMem
{
    PoolAllocator::PoolAllocator(std::size_t objectSize, std::size_t maxObjects, std::size_t alignment):
        m_PossibleAllocations(maxObjects), m_RemainingAllocations(maxObjects)
    {
        // objectSize refers to size of each block
        // maxObjects is the upper bound on the number of objects you'd expect to create
        
        // Ensure alignment is a power of two
        if((alignment & (alignment - 1)) != 0)
            throw AlignmentException();

        auto totalSize = 0;
        if(objectSize % alignment == 0) // Normally using sizeof(T) would give you the totalSize
        {
            totalSize = objectSize;
        }
        else // In case we don't get that; for e.g. packed struct or user-defined objectSize
        {
            if(alignment >= objectSize)
            {
                // If alignment is greater than object size, each object would occupy 'alignment' bytes
                // This case is pretty rare; anything that can be put in an array must have alignof(T) <= sizeof(T).
                // It would only be valid if the user specified they want an alignment greater than the size.
                totalSize = alignment;
            }
            else
            {
                // Otherwise, totalSize to allocate per object/block is the multiple of alignment next highest to the objectSize.
                // If objectSize is 13 and alignment is 4, we would need to allocate 16 bytes per object.
                totalSize = static_cast<std::size_t>(::ceil(static_cast<double>(objectSize)/static_cast<double>(alignment))) * alignment; 
            }
        }


        m_ArenaSize = totalSize * maxObjects;

        m_StartPtr = ::malloc(m_ArenaSize);

        if(!m_StartPtr)
            throw UnableToAllocateException();
        
        
        m_FreeListHead = reinterpret_cast<Block*>(m_StartPtr);

        Block* curBlock = m_FreeListHead;

        for(auto i = 0; i < m_PossibleAllocations; i++)
        {
            auto end = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(curBlock) + objectSize);
            auto nextBlock = reinterpret_cast<Block*>(GetAlignedAddress(end, alignment));
            curBlock->m_Next = nextBlock;
            curBlock = nextBlock;
        }

        curBlock->m_Next = nullptr;

        #ifdef RS_ENABLE_DEBUG_INFO
        std::cout << "PoolAllocator: Allocated " << m_PossibleAllocations << " blocks.\n";
        #endif
    }


    PoolAllocator::~PoolAllocator()
    {
        if(m_StartPtr)
        {
            ::free(m_StartPtr);

            #ifdef RS_ENABLE_DEBUG_INFO
            std::cout << "PoolAllocator: Freed memory.\n";
            #endif

            m_StartPtr = nullptr;
            m_FreeListHead = nullptr;
            m_PossibleAllocations = 0;
            m_RemainingAllocations = 0;
        }

    }

    void* PoolAllocator::Allocate()
    {
        if(m_RemainingAllocations == 0)
        {
            #ifdef RS_THROW_LOW_MEMORY_EXP
                throw LowMemoryException();
            #else
                return nullptr;
            #endif
        }

        auto block = m_FreeListHead;
        m_FreeListHead = m_FreeListHead->m_Next;

        m_RemainingAllocations -= 1;

        return reinterpret_cast<void*>(block);
    }


    void PoolAllocator::Free(void* block)
    {
        if(!block || !IsInMemoryArena(block))
            return;
        
        auto listBlock = reinterpret_cast<Block*>(block);

        listBlock->m_Next = m_FreeListHead;
        m_FreeListHead = listBlock;

        m_RemainingAllocations += 1;
    }


    std::size_t PoolAllocator::GetPossibleAllocations() const
    {
        return m_PossibleAllocations;
    }

    std::size_t PoolAllocator::GetRemainingAllocations() const
    {
        return m_RemainingAllocations;
    }


    bool PoolAllocator::IsInMemoryArena(void* ptr) const
    {
        auto end = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_StartPtr) + m_ArenaSize);

        return ptr >= m_StartPtr && ptr < end;
    }


}