#include "common_p.h"
#include <cstdint>
#include <stdlib.h>   
#include <stdint.h> 
#include <string>
#include <unordered_set>
#include <vector>

#ifndef _MN_TOKENIZER_P_H
#define _MN_TOKENIZER_P_H

namespace mathnotation {

//! Token type.
enum TokenType {
  TokenInvalid      =   0,  // <invalid>

  // Keywords & Identifiers
  TokenStart        = 256,  // Start assigning non-ASCII values
  TokenSymbol,
  TokenNumber,
  TokenVar,
  TokenReserved,

  // ASCII-symbols (match their character values)
  TokenLParen       = 40,   // (
  TokenRParen       = 41,   // )
  TokenMul          = 42,   // *
  TokenAdd          = 43,   // +
  TokenComma        = 44,   // ,
  TokenSub          = 45,   // -
  TokenDot          = 46,   // .
  TokenDiv          = 47,   // /
  TokenColon        = 58,   // :
  TokenSemicolon    = 59,   // ;
  TokenLt           = 60,   // <
  TokenEq           = 61,   // =
  TokenGt           = 62,   // >
  TokenQMark        = 63,   // ?
  TokenLBracket     = 91,   // [
  TokenBackslash    = 92,   // \ (optional)
  TokenRBracket     = 93,   // ]
  TokenCaret        = 94,   // ^
  TokenUnderscore   = 95,   // _ (optional)
  TokenLCurl        = 123,  // {
  TokenPipe         = 124,  // |
  TokenRCurl        = 125,  // }
  TokenTilde        = 126,  // ~
  TokenNot          = 33,   // !
  TokenMod          = 37,   // %
  TokenBitAnd       = 38,   // &
  TokenBitOr        = 124,  // |

  // Multi-char and compound operators (start at 300+ to avoid overlap)
  TokenPlusPlus     = 300,
  TokenMinusMinus,
  TokenNe,          // !=
  TokenLe,          // <=
  TokenGe,          // >=
  TokenLogAnd,      // &&
  TokenLogOr,       // ||

  TokenBitSar,      // >>
  TokenBitShr,      // >>>
  TokenBitShl,      // <<

  TokenAssign,      // =
  TokenAssignAdd,   // +=
  TokenAssignSub,   // -=
  TokenAssignMul,   // *=
  TokenAssignDiv,   // /=
  TokenAssignMod,   // %=

  TokenAssignBitAnd, // &=
  TokenAssignBitOr,  // |=
  TokenAssignBitXor, // ^=
  TokenAssignBitSar, // >>=
  TokenAssignBitShr, // >>>=
  TokenAssignBitShl, // <<=

  TokenEnd
};

enum TokenChar {
  // Digit.
  TokenChar0x0, TokenChar0x1, TokenChar0x2, TokenChar0x3,
  TokenChar0x4, TokenChar0x5, TokenChar0x6, TokenChar0x7,
  TokenChar0x8, TokenChar0x9,

  // Digit-Hex.
  TokenChar0xA, TokenChar0xB, TokenChar0xC, TokenChar0xD,
  TokenChar0xE, TokenChar0xF,

  // Non-Hex ASCII [A-Z] Letter and Underscore [_].
  TokenCharSym,

  // Punctuation.
  TokenCharDot = TokenDot,          // .
  TokenCharCom = TokenComma,        // ,
  TokenCharSem = TokenSemicolon,    // ;
  TokenCharQue = TokenQMark,        // ?
  TokenCharCol = TokenColon,        // :
  TokenCharLCu = TokenLCurl,        // {
  TokenCharRCu = TokenRCurl,        // }
  TokenCharLBr = TokenLBracket,     // [
  TokenCharRBr = TokenRBracket,     // ]
  TokenCharLPa = TokenLParen,       // (
  TokenCharRPa = TokenRParen,       // )

  TokenCharAdd = TokenAdd,          // +
  TokenCharSub = TokenSub,          // -
  TokenCharMul = TokenMul,          // *
  TokenCharDiv = TokenDiv,          // /
  TokenCharMod = TokenMod,          // %
  TokenCharNot = TokenNot,          // !
  TokenCharAnd = TokenBitAnd,       // &
  TokenCharOr  = TokenBitOr,        // |
  TokenCharXor = TokenAssignBitXor,       // ^
  TokenCharTilde = TokenTilde,       // ~
  TokenCharEq  = TokenAssign,       // =
  TokenCharLt  = TokenLt,           // <
  TokenCharGt  = TokenGt,           // >

  // Space.
  TokenCharSpc = 63,

  // Extended ASCII character (0x80 and above), acts as non-recognized.
  TokenCharExt,
  // Invalid (non-recognized) character.
  TokenCharInv,

  TokenCharSingleCharTokenEnd = TokenCharRPa
};

extern const std::unordered_set<uint32_t> tokenTypesClassTable;
extern const TokenChar tokenCharClassTable[256];

//! \internal
//!
//! Token class.
struct Token {
  // Members
  // -------

  //! Token type.
  uint32_t _tokenType;
  //! Token hash-code (only if the token is symbol or keyword).
  uint32_t _hashCode;
  //! Token position from the beginning of the input.
  size_t _position;
  //! Token size.
  size_t _size;
  //! Token value (if the token is a number).
  double _value;

  // Reset
  // -----

  inline void reset() {
    _position = 0;
    _size = 0;
    _value = 0.0;
    _tokenType = TokenInvalid;
  }

  // Accessors
  // ---------

  inline uint32_t setData(size_t position, size_t size, uint32_t hashCode, uint32_t tokenType) {
    _position = position;
    _size = size;
    _hashCode = hashCode;
    _tokenType = tokenType;

    return tokenType;
  }

  inline uint32_t tokenType() const noexcept { return _tokenType; }
  inline uint32_t hashCode() const noexcept { return _hashCode; }

  inline size_t position() const noexcept { return _position; }
  inline size_t size() const noexcept { return _size; }

  inline uint32_t positionAsUInt() const noexcept {
    MN_ASSERT(_position < ~static_cast<uint32_t>(0));
    return static_cast<uint32_t>(_position);
  }

  inline bool validTokenType(uint32_t tokenType)  {
    MN_ASSERT(tokenTypesClassTable.find(tokenType) != tokenTypesClassTable.end());
    return true;
  }

  inline double value() const noexcept { return _value; }
};

// ======================
// Lexical Analyser 
struct LexicalAnalyser {
    size_t pos = 0;
    std::string input;
    std::vector<Token> tokenStream;    
    
    inline char peek() const { return pos < input.size() ? input[pos] : '\0';}
    inline char next() { return pos < input.size() ? input[pos++] : '\0';}
    inline bool hasNext()  { return pos < input.size();}

    inline bool scan(const std::string& filename);
    inline bool analyse();
    
    inline auto begin() const { return tokenStream.begin(); }
    inline auto end() const { return tokenStream.end(); }

    inline const Token& at(size_t index) const { return tokenStream.at(index); }
    inline size_t size() const noexcept { return tokenStream.size(); }

    inline void clear() {
      tokenStream.clear();
      input.clear();
    }
};

}
// [Guard]
#endif // _MN_TOKENIZER_P_H
