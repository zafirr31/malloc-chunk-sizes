

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<getopt.h>

#define min(x,y) x < y ? x : y
#define max(x,y) x > y ? x : y

void print_help()	{
	printf("%4s, %-25s %s\n", "-d", "--decimal", "output chunk sizes in decimal");
	printf("%4s, %-25s %s\n", "-h", "--help", "display this help and exit");
	printf("%4s, %-25s %s\n", "-l", "--lower", "with -r, lower bound of range");
	printf("%4s, %-25s %s\n", "-r", "--range", "set mode to range mode");
	printf("%4s, %-25s %s\n", "-s", "--size", "without -r, size(s) to get malloc size");
	printf("%4s, %-25s %s\n", "-u", "--upper", "with -r, upper bound of range");
	printf("%4s, %-25s %s\n", "-x", "--hex", "output chunk sizes in hexadecimal");
	exit(EXIT_SUCCESS);
}

void swap(int* x, int* y)	{
	int temp = *x;
	*x = *y;
	*y = temp;
	return;
}

int main(int argc, char *argv[])
{
	enum {DEC_MODE, HEX_MODE};
	bool verbose = false;
	bool range = false;
	int opt;
	int min_val = -1;
	int max_val = -1;
	int size = -1;
	int mode = DEC_MODE;
	const struct option longopts[] = {
		{.name = "upper", .has_arg = required_argument, .val = 'u'},
		{.name = "lower", .has_arg = required_argument, .val = 'l'},
		{.name = "size", .has_arg = required_argument, .val = 's'},
		{.name = "decimal", .has_arg = no_argument, .val = 'd'},
		{.name = "hex", .has_arg = no_argument, .val = 'x'},
		{.name = "range", .has_arg = no_argument, .val = 'r'},
		{.name = "help", .has_arg = no_argument, .val = 'h'},
		{},
	};

	if(argc < 2)	{
		fprintf(stderr, "Try \'./malloc_chunk_sizes -h\' for more information\n");
		exit(EXIT_FAILURE);
	}

	while ((opt = getopt_long(argc, argv, "hdxrvl:u:s:", longopts, NULL)) != -1)	{
		switch(opt)	{
			case 'u': max_val = atoi(optarg); break;
			case 'l': min_val = atoi(optarg); break;
			case 's': size = atoi(optarg); break;
			case 'd': mode = DEC_MODE; break;
			case 'x': mode = HEX_MODE; break;
			case 'r': range = true; break;
			case 'h': 
				print_help();
				break;
			default:
				fprintf(stderr, "Try \'./malloc_chunk_sizes --help\' for more information\n");
				exit(EXIT_FAILURE);
		}	
	}
	if(!range)	{
		if(size == -1)	{
			fprintf(stderr, "malloc_chunk_sizes requires size parameter. Try the \'-s\' option\n");
			fprintf(stderr, "Try \'./malloc_chunk_sizes -h\' for more information\n");
			exit(EXIT_FAILURE);
		}
		min_val = size;
		max_val = min_val;
	}
	else	{
		if(min_val == -1 || max_val == -1)	{
			fprintf(stderr, "Range parameter requires below and above parameters. Try \'-u\' and \'-l\' options.\n");
			fprintf(stderr, "Try \'./malloc_chunk_sizes -h\' for more information\n");
			exit(EXIT_FAILURE);
		}
		if(min_val > max_val)
			swap(&min_val, &max_val);
	}

	for(int i = min_val; i <= max_val; i++)	{
		long long* malloc_pointer = malloc(i);
		if(mode == DEC_MODE)
			printf("%u\t:\t%lld\n", i, *(malloc_pointer-1) & ~7);
		else if(mode == HEX_MODE)
			printf("%u\t:\t0x%llx\n", i, *(malloc_pointer-1) & ~7);
		free(malloc_pointer);
	}
	return 0;
}