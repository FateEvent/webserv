#include <iostream>
#include <cstring>

int	main()
{
	int size = 0;
	std::string tmp("10\r\n");
	size = std::strtoul(tmp.c_str(), NULL, 16);
	std::cout << size << std::endl;
	return 0;
}