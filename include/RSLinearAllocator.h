#ifndef RS_LINEAR_ALLOCATOR_H
#define RS_LINEAR_ALLOCATOR_H

#include "RSAllocator.h"

namespace RSMem
{
    class LinearAllocator: public Allocator
    {
    private:
        void* m_CurPtr = nullptr;
    
    public:

        LinearAllocator(); 

        LinearAllocator(std::size_t arenaSize);

        virtual ~LinearAllocator() = default;

        virtual void* Allocate(std::size_t numBytes, std::size_t alignment) override;

        virtual void Free() override;

        virtual void Reset() override;

    };
}

#endif