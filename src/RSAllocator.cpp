#include "RSAllocator.h"
#include "RSExceptions.h"

#include<cstdlib>

#ifdef RS_ENABLE_DEBUG_INFO
#include<iostream>
#endif

namespace RSMem
{
    Allocator::Allocator(): m_StartPtr(::malloc(1024)), 
        m_ArenaSize(1024)
    {
        if(!m_StartPtr)
            throw UnableToAllocateException();
        
        #ifdef RS_ENABLE_DEBUG_INFO
        std::cout << "Allocator: Allocated " << m_ArenaSize << " bytes.\n";
        #endif
    }

    Allocator::Allocator(std::size_t arenaSize): m_StartPtr(::malloc(arenaSize)),
        m_ArenaSize(arenaSize)
    {
        if(!m_StartPtr)
            throw UnableToAllocateException();

        #ifdef RS_ENABLE_DEBUG_INFO
        std::cout << "Allocator: Allocated " << m_ArenaSize << " bytes.\n";
        #endif
    }

    Allocator::~Allocator()
    {
        if(m_StartPtr)
        {
            ::free(m_StartPtr);

            #ifdef RS_ENABLE_DEBUG_INFO
            std::cout << "Allocator: Freed " << m_ArenaSize << " bytes.\n";
            #endif

            m_StartPtr = nullptr;
            m_ArenaSize = 0;
            m_Allocations = 0;
        }

    }

}