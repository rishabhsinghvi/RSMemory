#include "RSPoolAllocator.h"
#include "RSExceptions.h"
#include "RSMemUtils.h"

#include<cstdlib>
#include<cstdint>

#ifdef RS_ENABLE_DEBUG_INFO
#include<iostream>
#endif

namespace RSMem
{
    PoolAllocator::PoolAllocator(std::size_t totalMemoryBlockSize, std::size_t objectSize, std::size_t alignment): m_StartPtr(::malloc(totalMemoryBlockSize)),
        m_ArenaSize(totalMemoryBlockSize)
    {
        // The PoolAllocator assumes that totalMemoryBlockSize % objectSize == 0
        // If not, it would have total number of available allocations = floor(totalMemoryBlockSize / objectSize)
        if(!m_StartPtr)
            throw UnableToAllocateException();
        
        m_PossibleAllocations = totalMemoryBlockSize / objectSize;
        m_RemainingAllocations = m_PossibleAllocations;
        
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
        // TODO: Check if block belongs to memory arena

        if(!block)
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





}