#ifndef _MN_UTILITY
#define _MN_UTILITY

namespace utility {
  enum ErrorCode {
    //! No error.
    ErrorOk = 0,
    //! No memory.
    ErrorNoMemory = 1,
    //! Invalid argument.
    ErrorInvalidArgument,
    //! Invalid state.
    ErrorInvalidState,

    //! No expression was given.
    ErrorNoExpression,
    //! Invalid syntax.
    ErrorInvalidSyntax,

    //! Symbol not found.
    ErrorSymbolNotFound,
    //! Symbol already exists.
    ErrorSymbolAlreadyExists
  };
}

#endif // _MN_UTILITY 
