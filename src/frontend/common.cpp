#include <stdio.h>
#include <stdlib.h>
#include "./common_p.h"


namespace utility {
  void AssertionFailure(const char* file, int line, const char* msg) {
    fprintf(stderr,
      "[mathnot] Assertion failed at %s (line %d):\n"
      "[mathnot] %s\n", file, line, msg);
    ::abort();
  }
}

