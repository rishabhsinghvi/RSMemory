#ifndef RS_EXCEPTIONS_H
#define RS_EXCEPTIONS_H

#include<exception>

namespace RSMem
{
    struct UnableToAllocateException: public std::exception
    {
        virtual const char* what() const noexcept
        {
            return "Unable to allocate requested memory!\n";
        }
    };

    
    struct LowMemoryException: public std::exception
    {
        virtual const char* what() const noexcept
        {
            return "Allocator does not have enough memory to fulfill request.\n";
        }
    };

}

#endif