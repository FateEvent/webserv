#include "../includes/common.h"

int ft::listing_creator(char const *directory)
{
	DIR				*dir;
	struct dirent	*ent;
	int				closed;
//	s_clt_data		data;

	dir = opendir(directory);
	if (dir != NULL)
	{
//		data.file = new std::stringstream();
		std::stringstream *str = new std::stringstream(); 
//		*static_cast<std::stringstream*>(data.file) << "<html>\n\t<head></head>\n\t<body>\n\t\t<div>\n\t\t\t<ul>\n";
		*str << "<html>\n\t<head></head>\n\t<body>\n\t\t<div>\n\t\t\t<ul>\n";
		while ((ent = readdir(dir)) != NULL)
		{
//			*static_cast<std::stringstream*>(data.file) << "\t\t\t\t<li>" << ent->d_name << "<li>\n";
			*str << "\t\t\t\t<li>" << ent->d_name << "<li>\n";
		}
//		*static_cast<std::stringstream*>(data.file) << "\t\t\t</ul>\n\t\t</div>\n\t</body>\t</html>\n";
		*str << "\t\t\t</ul>\n\t\t</div>\n\t</body>\n</html>\n";
		std::cout << str->str() << std::endl;
		closed = closedir(dir);
		if (closed)
		{
			std::cerr << strerror(errno) << std::endl;
			throw std::invalid_argument("Couldn't close the directory stream!");
		}
	}
	else
	{
		std::cerr << strerror(errno) << std::endl;
		throw std::invalid_argument("Invalid or unexisting directory!");
	}
	return (1);
}