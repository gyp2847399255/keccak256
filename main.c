#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "keccak256.h"

int main(int argc, char *argv[]) {
    assert(argc > 1);
    printf("The hash is ");
    Keccak256Result res = hash(argv[1], strlen(argv[1]));
    for (int i = 0; i < BYTES; i++) {
        printf("%02x", res.hash_result[i]);
    }
    printf("\n");
    return 0;
}
