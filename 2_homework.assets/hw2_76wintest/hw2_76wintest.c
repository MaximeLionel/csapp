#include <stdio.h>
#include <limits.h>

void* calloc(size_t ele_count, size_t ele_size)
{
    if (ele_count == 0 || ele_size == 0) return NULL;

    size_t size = ele_count * ele_size;
    if (!ele_count || size / ele_count != ele_size) return NULL; // overflow check

    void* p = malloc(size);
    if (p == NULL) return NULL;
    memset(p, 0, size);
    return p;
}

int main()
{
    size_t ele_count = UINT_MAX;
    size_t ele_size = 2;
    printf("calloc(0x%X, 0x%X) returns 0x%p.\n", ele_count, ele_size, calloc(ele_count, ele_size));

    ele_count = 0;
    ele_size = 2;
    printf("calloc(0x%X, 0x%X) returns 0x%p.\n", ele_count, ele_size, calloc(ele_count, ele_size));

    ele_count = 5;
    ele_size = 2;
    printf("calloc(0x%X, 0x%X) returns 0x%p.\n", ele_count, ele_size, calloc(ele_count, ele_size));
    return 0;
}

