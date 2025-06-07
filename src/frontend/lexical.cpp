#include <cstdint>
#include <unordered_set>
#include "lexical_p.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace mathnotation {

#define TC(ID) Token##ID
const std::unordered_set<uint32_t> tokenTypesClassTable = {
    // ASCII-symbols
    TC(LParen), TC(RParen), TC(Mul), TC(Add), TC(Comma),
    TC(Sub), TC(Dot), TC(Div), TC(Colon), TC(Semicolon),
    TC(Lt), TC(Eq), TC(Gt), TC(QMark), TC(LBracket),
    TC(Backslash), TC(RBracket), TC(Caret), TC(Underscore),
    TC(LCurl), TC(Pipe), TC(RCurl), TC(Tilde), TC(Not),
    TC(Mod), TC(BitAnd), TC(BitOr),

    // Keyword / Identifier / Other tokens
    TC(Invalid),
    TC(Symbol), TC(Number), TC(Var), TC(Reserved),

    // Compound tokens
    TC(PlusPlus), TC(MinusMinus),
    TC(Ne), TC(Le), TC(Ge), TC(LogAnd), TC(LogOr),
    TC(BitSar), TC(BitShr), TC(BitShl),
    TC(Assign), TC(AssignAdd), TC(AssignSub), TC(AssignMul),
    TC(AssignDiv), TC(AssignMod),
    TC(AssignBitAnd), TC(AssignBitOr), TC(AssignBitXor),
    TC(AssignBitSar), TC(AssignBitShr), TC(AssignBitShl)
};


const TokenChar tokenCharClassTable[256] = {
  // 0x00 – 0x07
  TC(CharInv), TC(CharInv), TC(CharInv), TC(CharInv),
  TC(CharInv), TC(CharInv), TC(CharInv), TC(CharInv),

  // 0x08 – 0x0F
  TC(CharInv), TC(CharSpc), TC(CharSpc), TC(CharSpc),
  TC(CharSpc), TC(CharSpc), TC(CharInv), TC(CharInv),

  // 0x10 – 0x1F
  TC(CharInv), TC(CharInv), TC(CharInv), TC(CharInv),
  TC(CharInv), TC(CharInv), TC(CharInv), TC(CharInv),
  TC(CharInv), TC(CharInv), TC(CharInv), TC(CharInv),
  TC(CharInv), TC(CharInv), TC(CharInv), TC(CharInv),

  // 0x20 – 0x2F (space and punctuation)
  TC(CharSpc), TC(CharNot), TC(CharInv), TC(CharInv),
  TC(CharInv), TC(CharMod), TC(CharAnd), TC(CharInv),
  TC(CharLPa), TC(CharRPa), TC(CharMul), TC(CharAdd),
  TC(CharCom), TC(CharSub), TC(CharDot), TC(CharDiv),

  // 0x30 – 0x39 (digits)
  TC(Char0x0), TC(Char0x1), TC(Char0x2), TC(Char0x3),
  TC(Char0x4), TC(Char0x5), TC(Char0x6), TC(Char0x7),
  TC(Char0x8), TC(Char0x9),

  // 0x3A – 0x40 (punctuation and @)
  TC(CharCol), TC(CharSem), TC(CharLt), TC(CharEq),
  TC(CharGt), TC(CharQue), TC(CharInv),

  // 0x41 – 0x5A (A–Z)
  TC(Char0xA), TC(Char0xB), TC(Char0xC), TC(Char0xD),
  TC(Char0xE), TC(Char0xF), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym),

  // 0x5B – 0x60 ([\]^_`)
  TC(CharLBr), TC(CharInv), TC(CharRBr), TC(CharXor),
  TC(CharSym), TC(CharInv),

  // 0x61 – 0x7A (a–z)
  TC(Char0xA), TC(Char0xB), TC(Char0xC), TC(Char0xD),
  TC(Char0xE), TC(Char0xF), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym), TC(CharSym), TC(CharSym),
  TC(CharSym), TC(CharSym),

  // 0x7B – 0x7E ({|}~)
  TC(CharLCu), TC(CharOr), TC(CharRCu), TC(CharTilde),

  // 0x7F (DEL)
  TC(CharInv),

  // 0x80 – 0xFF (Extended ASCII)
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt),
  TC(CharExt), TC(CharExt), TC(CharExt), TC(CharExt)
};

#undef TC

inline bool LexicalAnalyser::scan(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
      std::cerr << "Failed to open: " << filename << std::endl;
      return false;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();       
  input = buffer.str();
  
  
  while (peek() != '\0') {
    char c = peek();

    if (std::isspace(c)) {
        next();
        continue;
    }

    Token token;

    TokenChar charClass = tokenCharClassTable[static_cast<unsigned char>(c)];

    int hash = 0;
    if (charClass <= TokenChar0x0 && charClass >= TokenChar0x9 ) {
        hash = static_cast<unsigned char>(c);
    } else {
        token._value = c; 
    }

    token.setData(pos, 1, hash, charClass);
    tokenStream.push_back(token);

    next();
  }

  if (!analyse()) { //Build tokens from token chars in tokenStream
    return false;  
  }; 
  return true;
};

inline bool LexicalAnalyser::analyse() {
  return true;    
}

};
