#include "RSMemUtils.h"

#include<cstdint>

namespace RSMem
{
    void* GetAlignedAddress(void* curPtr, std::size_t alignment)
    {
        auto misAlignment = reinterpret_cast<uintptr_t>(curPtr) & (alignment - 1);

        if(misAlignment == 0) // if no misalignment
        {
            return curPtr;
        }
        else
        {
            auto adjustment = alignment - misAlignment;

            auto adjustedPtr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(curPtr) + adjustment);

            return adjustedPtr;
        }
        
    }

    bool HasEnoughMemory(void* startPtr, std::size_t arenaSize, void* curPtr, std::size_t bytesToAllocate)
    {
        auto end = reinterpret_cast<uintptr_t>(startPtr) + arenaSize;
        auto requestedBlockEnd = reinterpret_cast<uintptr_t>(curPtr) + bytesToAllocate;

        return requestedBlockEnd <= end;
    }

}