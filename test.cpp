#include <iostream>
#include <string>

int main (int ac, char **av)
{


	{
	unsigned int test = 0;
	unsigned int ip[4];
	sscanf(av[1], "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
	test = (ip[0] % 256 << 0 | 0) | (ip[1] % 256 << 8 | 0) | (ip[2] % 256 << 16 | 0) | (ip[3] % 256 << 24 | 0);
	std::cout << test << std::endl;
	}

	unsigned ret;

    int a, b, c, d;
    char arr[4];
    sscanf(av[1], "%d.%d.%d.%d", &a, &b, &c, &d);
    arr[0] = a; arr[1] = b; arr[2] = c; arr[3] = d;

    ret = *(unsigned int *)arr;
	
	std::cout << ret << std::endl;
	
	return 0;

}