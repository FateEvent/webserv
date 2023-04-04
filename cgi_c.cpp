#include <iostream>
#include <fstream>
#include <cstring>

int main(void)
{
	std::ifstream	file;
	file.open("site/video.html");
	std::string		text="<html><head><head/><body><p>Page not found!<p/><body/><html/>";
	std::string		buff;

	if (!file.is_open())
	{
		std::cout << "Content-Length: " + std::to_string(text.length()) + "\r\n\r\n";
		std::cout << text;
		return (0);
	}
	text.clear();
	while (std::getline(file, buff))
		text.append(buff + "\n");
	std::cout << "Content-Length: " + std::to_string(text.length()) + "\r\n\r\n";
	std::cout << text << std::endl;
	return (0);
}