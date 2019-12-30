

#include<stdio.h>
#include<stdlib.h>

#define min(x,y) x < y ? x : y
#define max(x,y) x > y ? x : y

int main(int argc, char const *argv[])
{
	if(argc < 2)	{
		puts("Usage: ./malloc_chunk_sizes <size>");
		exit(-1);
	}
	int size = atoi(argv[1]);
	int min_val = max(0, size-0x10);
	int max_val = min(size+0x10, 4096);
	for(int i = min_val; i <= max_val; i++)	{
		long long* malloc_pointer = malloc(i);
		printf("%u\t:\t0x%llx\n", i, *(malloc_pointer-1) & ~7);
	}
	return 0;
}