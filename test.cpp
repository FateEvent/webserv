#include <iostream>
#include <cstring>

int	main()
{
	std::string locate = "/test";
	std::string dir = "/test/du/repertoire";
	std::cout << locate.length() << std::endl;

	std::cout << std::strncmp((locate + "/").c_str(), dir.c_str(), locate.length() + 1) << std::endl;
	std::cout << std::strncmp(locate.c_str(), dir.c_str(), locate.length()) << std::endl;

	return 0;
}