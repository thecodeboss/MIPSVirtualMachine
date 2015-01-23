#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <cassert>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#define __DEBUG 0

#define __debug(x) if (__DEBUG) x
#define DEBUG(a) __debug(std::cerr << a << std::endl);
#define DEBUG2(a,b) __debug(std::cerr << a << b << std::endl);
#define DEBUG3(a,b,c) __debug(std::cerr << a << b << c << std::endl);

#define PANIC(a) std::cerr << "Execution halted! " << a << std::endl;
#define PANIC2(a,b) std::cerr << "Execution halted! " << a << b << std::endl;
#define PANIC3(a,b,c) std::cerr << "Execution halted! " << a << b << c << std::endl;

#endif