#ifndef RS_MEM_UTILS_H
#define RS_MEM_UTILS_H

#include<cstddef> // For std::size_t

namespace RSMem
{
    void* GetAlignedAddress(void* curPtr, std::size_t alignment);

    bool HasEnoughMemory(void* starPtr, std::size_t arenaSize, void* curPtr, std::size_t bytesToAllocate);
}

#endif