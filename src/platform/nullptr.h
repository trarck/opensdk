#ifndef OPENSDK_PLATFORM_NULLPTR_H_
#define OPENSDK_PLATFORM_NULLPTR_H_

#include <iostream>
#include <memory>
 
// #define USE_CXX11_NULLPTR
 
#ifndef USE_CXX11_NULLPTR
////////////////////////////////////////////
// To avoid errors in latest GCC which is used by IDEOne.
// In my case I use GCC 4.5.3 where it supports some features of C++11, but not nullptr.
#define nullptr nullptr__
#define nullptr_t nullptr_t__
////////////////////////////////////////////
 
namespace std {
 
    const class nullptr_t
    {
    public:
        
        // Return 0 for any class pointer
        template<typename T>
        operator T*() const
        {
            return 0;
        }
        
        // Return 0 for any member pointer
        template<typename T, typename U>
        operator T U::*() const
        {
            return 0;
        }
        
        // Used for bool conversion
        operator void*() const
        {
            return 0;
        }
        
        // Comparisons with nullptr
        bool operator==(const nullptr_t&) const
        {
            return true;
        }
        bool operator!=(const nullptr_t&) const
        {
            return false;
        }
        
    private:
        
        // Not allowed to get the address
        void operator&() const;
        
    } nullptr = {};

}
 
using std::nullptr;

#endif //USE_CXX11_NULLPTR


#endif  // OPENSDK_PLATFORM_NULLPTR_H_

