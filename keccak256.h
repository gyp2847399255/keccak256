#ifndef _KECCAK256_H_
#define _KECCAK256_H_

#include <string.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;

#define BYTES 32

typedef struct Keccak256Result {
    u8 hash_result[BYTES];
} Keccak256Result;

Keccak256Result hash(char s[], u32 len);

#endif
