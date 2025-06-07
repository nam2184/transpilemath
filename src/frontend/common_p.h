#ifndef _MN_UTILITY_P
#define _MN_UTILITY_P

namespace utility {
  void AssertionFailure(const char* file, int line, const char* msg);
}

#define MN_ASSERT(exp) do { \
  if (!(exp)) \
    ::utility::AssertionFailure(__FILE__, __LINE__, #exp); \
  } while (0)


#endif // _MN_UTILITY_P 
