//
// Copyright (C) 2014, 2017-2020 Chris McClelland
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright  notice and this permission notice  shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <stdexcept>
#include "rng.hpp"
#ifdef _WIN32
  #include <io.h>
  #include <fcntl.h>
#endif

#define EXPLICIT_SEED

namespace {
  // Values to initialize RNGs for ordinals 1, 2 & 3 (corresponding to bit-widths of 32, 64 & 96)
  struct Param {
    int n, r, t, maxt;
    uint32_t s;
    const char* seed;
  };
  const Param params[] = {
    {0,    0,  0, 0,  0,          ""},
    {1024, 32, 5, 32, 0x00001c48, "0110110000010010111101011010111010011010000010100011100111110100110001001111011101001101101010011000011010000010010110010100111001101101101001100010111010111011100010110011111010011100110000111010010000010011111101011111100001011001000010001001010100110001001000110001001001101101011101100110111111000011010011011111101000101011100110100101000101010111011011100001110111100011100010000011000110000111111011111000011100111110011110010010110101111101110111110010000001101101101000100011110101001000000000010010000110101111100100110001011001111001111101101101011101101001111000001000100011010010011111011000100100011101000000100010000010011011110100100111001001110000100010110000001010000001011011011111011010001100010101100000000010011101111001011110011000001000001000001001110100001100100111000100110010101010011100000001110010110011111010110110011111010000010100011110101010100011101001101011011111111110101110010001011100011000010000101100000101001010101110101011100010010101001011110101110001111000110100000110101100111101"},
    {2048, 64, 3, 32, 0x005f81cb, "11111001100110001001100001101110011000101001101010110011001110101110110000111110000000001101101100010101100100000100110000000111100100000010000000001100011110000011001110000100100111111001001100001111010001011001001000011010111000000101011000000011100110010110110110001110000011111011110000100011110011010110010100111010110100010101110001111001110101101011010001100111011110101010011111101111101100011001110000110101000001001101001011111011000010100100001010001010000001010011000011001110101100000111000111101011010111100110010101111011011000010101001000110101000000000111011110101111101111100101011110010111001001100001110111010000011101000010011111100100100001011010101011010011101100010110000001110110100111110111001111010111000001010100000101111010100111101001111001010101011111101101101011100001111011011011100001111011110101010000111011110011010011101001100001010000110101100011001110110111001001001001101100111101010111000100010111010101100011110111101011110110111100010100000100111000101000011111110010010101110110010110110000010010111101011010111010011010000010100011100111110100110001001111011101001101101010011000011010000010010110010100111001101101101001100010111010111011100010110011111010011100110000111010010000010011111101011111100001011001000010001001010100110001001000110001001001101101011101100110111111000011010011011111101000101011100110100101000101010111011011100001110111100011100010000011000110000111111011111000011100111110011110010010110101111101110111110010000001101101101000100011110101001000000000010010000110101111100100110001011001111001111101101101011101101001111000001000100011010010011111011000100100011101000000100010000010011011110100100111001001110000100010110000001010000001011011011111011010001100010101100000000010011101111001011110011000001000001000001001110100001100100111000100110010101010011100000001110010110011111010110110011111010000010100011110101010100011101001101011011111111110101110010001011100011000010000101100000101001010101110101011100010010101001011110101110001111000110100000110101100111101"},
    {3060, 96, 3, 32, 0x00079e56, "001011101111110100101011011011010011000000101001011000000011000101100011100101001100110101110100111000100111010110101100011001100010011111110011100110101010001000110100111111001001100101100101001101100001000001000101100010000001110110100010110100101100000011101001001010100010100011001011010001001001010011101101001001111000011101111111000010011111001101011001000000001001000011000011000000011000100001101101001100000010101110110100001000110110000011111100000000000101000010101111111111100010010000101101101001000001110000011010000000010100100001110001100111001101001000011111001110000111101010000101011001111111111100011110110101100011000100001010100100001100011101100011101010000010100011110010111011000101001010001111010010000000010111011000111001100000100001010101001111110000110000010000111010010100110011011011001111110000011001010110001001101000111001100001101011000001010111110010010111011011001111111100010010010010011101011000011111001010000001011010001100010000101101100111001101110101000010100100100111111001100110001001100001101110011000101001101010110011001110101110110000111110000000001101101100010101100100000100110000000111100100000010000000001100011110000011001110000100100111111001001100001111010001011001001000011010111000000101011000000011100110010110110110001110000011111011110000100011110011010110010100111010110100010101110001111001110101101011010001100111011110101010011111101111101100011001110000110101000001001101001011111011000010100100001010001010000001010011000011001110101100000111000111101011010111100110010101111011011000010101001000110101000000000111011110101111101111100101011110010111001001100001110111010000011101000010011111100100100001011010101011010011101100010110000001110110100111110111001111010111000001010100000101111010100111101001111001010101011111101101101011100001111011011011100001111011110101010000111011110011010011101001100001010000110101100011001110110111001001001001101100111101010111000100010111010101100011110111101011110110111100010100000100111000101000011111110010010101110110010110110000010010111101011010111010011010000010100011100111110100110001001111011101001101101010011000011010000010010110010100111001101101101001100010111010111011100010110011111010011100110000111010010000010011111101011111100001011001000010001001010100110001001000110001001001101101011101100110111111000011010011011111101000101011100110100101000101010111011011100001110111100011100010000011000110000111111011111000011100111110011110010010110101111101110111110010000001101101101000100011110101001000000000010010000110101111100100110001011001111001111101101101011101101001111000001000100011010010011111011000100100011101000000100010000010011011110100100111001001110000100010110000001010000001011011011111011010001100010101100000000010011101111001011110011000001000001000001001110100001100100111000100110010101010011100000001110010110011111010110110011111010000010100011110101010100011101001101011011111111110101110010001011100011000010000101100000101001010101110101011100010010101001011110101110001111000110100000110101100111101"}
  };
}

// An int wrapper that throws if it's out of a defined range
template<int min, int max> class RangedInt {
  const int value;
  const char* name;
public:
  RangedInt(int v, const char* n) : value(v), name(n) {
    if (v < min || v > max) {
      throw std::out_of_range(std::string(name) + " must be between " + std::to_string(min) + " and " + std::to_string(max));
    }
  }
  operator int() const { return value; }
};

// Wrapper class for the behavioural model
class RNG {
  const RangedInt<1, 3> ord;
  rng g;
  std::vector<int> state;
  std::vector<uint32_t> lw;
  std::pair<std::vector<int>, int> out;
public:
  explicit RNG(int o) :
    ord(o, "RNG ordinal"),
    g(params[ord].n, params[ord].r, params[ord].t, params[ord].maxt, params[ord].s),
    state(g.n, 1),
    lw(ord+1),
    out()
  {
    int i = g.n - 1;
    while (i >= 0) {
      #ifdef EXPLICIT_SEED
        out = g.Step(state, 1, params[ord].seed[i] - '0');
      #else
        out = g.Step(state, 1, rand() % 2);
      #endif
      --i;
    }
  }

  const uint32_t* getNext() {
    int i;
    for (int j = 0; j < ord; ++j) {
      lw[j] = 0;
      i = 31;
      while (i >= 0) {
        lw[j] <<= 1;
        if (out.first[j*32+i] == 1) {
          lw[j] |= 1;
        }
        --i;
      }
    }
    out = g.Step(state, 0, 0);
    return lw.data();
  }
};

int main(int argc, const char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Synopsis: " << argv[0] << " <ordinal> <numCycles>" << std::endl;
      return 1;
    }
    const int ord = std::strtoul(argv[1], nullptr, 10);
    const size_t limit = std::strtoul(argv[2], nullptr, 10);
    RNG g(ord);
    
    // Maybe make stdout binary
    #ifdef _WIN32
      setmode(fileno(stdout), O_BINARY);
    #endif

    // Get a stream of pseudorandom values
    if (limit) {
      for (size_t k = 0; k < limit; ++k) {
        std::fwrite(g.getNext(), 4*ord, 1, stdout);
      }
    } else {
      for (;;) {
        std::fwrite(g.getNext(), 4*ord, 1, stdout);
      }
    }
    return 0;
  }
  catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
}