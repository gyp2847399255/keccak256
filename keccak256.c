#include "keccak256.h"
#include <stdio.h>

const u32 rotation_constants[5][5] = {
    {0, 1, 62, 28, 27},
    {36, 44, 6, 55, 20},
    {3, 10, 43, 25, 39},
    {41, 45, 15, 21, 8},
    {18, 2, 61, 56, 14}
};

#define BITRATE_BYTES 136
#define BITRATE_CHUNKS (BITRATE_BYTES / 8)
typedef struct Block {
    u64 chunk[BITRATE_CHUNKS];
} Block;

typedef struct State {
    u64 state[5][5];
} State;

u64 rol(u64 value, u32 left) {
    if (left == 0) {
        return value;
    }
    u64 top = value >> (64 - left);
    u64 bottom = value << left;
    return top | bottom;
}

void read_block(char **data, Block *res) {
    memset(res, 0, sizeof(Block));
    u32 len = strlen(*data);
    u8 block[BITRATE_BYTES];
    if (len >= BITRATE_BYTES) {
        memcpy(block, *data, BITRATE_BYTES);
        *data += BITRATE_BYTES;
    } else {
        memcpy(block, *data, len);
        u32 pad_len = BITRATE_BYTES - len;
        if (pad_len > 1) {
            block[len] = 0x1;
            memset(block + len + 1, 0, pad_len - 2);
            block[BITRATE_BYTES - 1] = 0x80;
        } else {
            block[len] = 0x81;
        }
    }

    for (int i = 0; i < BITRATE_CHUNKS; i++) {
        u64 v = 0;
        for (int j = 7; j >= 0; j--) {
            v = (v << 8) + block[i * 8 + j];
        }
        res->chunk[i] = v;
    }
}

#define NR 24
const u64 round_constants[NR] = {
    0x0000000000000001,   0x0000000000008082,   0x800000000000808A,   0x8000000080008000,
    0x000000000000808B,   0x0000000080000001,   0x8000000080008081,   0x8000000000008009,
    0x000000000000008A,   0x0000000000000088,   0x0000000080008009,   0x000000008000000A,
    0x000000008000808B,   0x800000000000008B,   0x8000000000008089,   0x8000000000008003,
    0x8000000000008002,   0x8000000000000080,   0x000000000000800A,   0x800000008000000A,
    0x8000000080008081,   0x8000000000008080,   0x0000000080000001,   0x8000000080008008
};

void each_round(State *state, u64 rc) {
    u64 c[5];
    for (int i = 0; i < 5; i++) {
        c[i] = 0;
        for (int j = 0; j < 5; j++) {
            c[i] ^= state->state[i][j];
        }
    }
    for (int i = 0; i < 5; i++) {
        u64 d = c[(i + 4) % 5] ^ rol(c[(i + 1) % 5], 1);
        for (int j = 0; j < 5; j++) {
            state->state[i][j] ^= d;
        }
    }

    u64 b[5][5];
    memset(b, 0, sizeof(b));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            b[j % 5][(2 * i + 3 * j) % 5] = rol(state->state[i][j], rotation_constants[j][i]);
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            state->state[i][j] = b[i][j] ^ ((~b[(i + 1) % 5][j]) & b[(i + 2) % 5][j]);
        }
    }

    state->state[0][0] ^= rc;
}

void keccak_f(State *state) {
    for (int i = 0; i < NR; i++) {
        each_round(state, round_constants[i]);
    }
}

void absorb_block(State *state, Block *block) {
    for (int i = 0; i < BITRATE_CHUNKS; i++) {
        state->state[i % 5][i / 5] ^= block->chunk[i];
    }
    keccak_f(state);
}

Keccak256Result hash(char s[], u32 len) {
    State state;
    memset(&state, 0, sizeof(State));

    u32 n_blocks = len / BITRATE_BYTES + 1;
    for (int i = 0; i < n_blocks; i++) {
        Block block;
        read_block(&s, &block);
        absorb_block(&state, &block);
    }

    Keccak256Result res;
    for (int i = 0; i < 4; i++) {
        u64 v = state.state[i][0];
        for (int j = 0; j < 8; j++) {
            res.hash_result[i * 8 + j] = v & 255;
            v >>= 8;
        }
    }
    return res;
}
