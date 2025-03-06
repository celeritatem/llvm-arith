#include <stdio.h>
#include <stdlib.h>

void arith_write(int v) {
    printf("result = %d\n", v);
}

int arith_read(char *s) {
    char buf[64];
    int val;
    printf("%s = ", s);
    fgets(buf, sizeof(buf), stdin);
    if (EOF == sscanf(buf, "%d", &val)) {
        printf("invalid value %s\n", buf);
        exit(1);
    }
    return val;
}