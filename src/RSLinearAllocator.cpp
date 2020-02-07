#include "RSLinearAllocator.h"
#include "RSMemUtils.h"
#include "RSExceptions.h"

#include<cstdint>

namespace RSMem
{
    LinearAllocator::LinearAllocator():
        Allocator(), m_CurPtr(m_StartPtr)
    {

    }   

    LinearAllocator::LinearAllocator(std::size_t arenaSize):
        Allocator(arenaSize), m_CurPtr(m_StartPtr)
    {

    } 

    void* LinearAllocator::Allocate(std::size_t numBytes, std::size_t alignment)
    {
        auto alignedAddr = GetAlignedAddress(m_CurPtr, alignment);

        if(HasEnoughMemory(m_StartPtr, m_ArenaSize, alignedAddr, numBytes))
        {
            m_CurPtr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(alignedAddr) + numBytes);
            m_Allocations += 1;
            return alignedAddr;
        }   
        else
        {
            #ifdef RS_THROW_LOW_MEMORY_EXP
                throw LowMemoryException();
            #else
                return nullptr;
            #endif
        }

    
             
    }

    void LinearAllocator::Free()
    {
        // A linear allocator does not free independent memory blocks. 
        // You can call Reset() on the Allocator
    }
    
    void LinearAllocator::Reset()
    {
        if(!m_StartPtr)
            return;
        
        m_CurPtr = m_StartPtr;
        m_Allocations = 0;
    }

}