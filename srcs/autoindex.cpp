#include "../includes/common.h"

int ft::listing_creator(char const *directory)
{
	DIR				*dir;
	struct dirent	*ent;
	int				closed;
	struct stat		filestat;
	std::string		path;
	std::string		time_str;
//	s_clt_data		data;

	dir = opendir(directory);
	if (dir != NULL)
	{
//		data.file = new std::stringstream();
		std::stringstream *str = new std::stringstream(); 
//		*static_cast<std::stringstream*>(data.file) << "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Autoindex Page</title>\n\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"error_page.css\">\n\t</head>\n\t<body>\n\t\t<h1>Autoindex Page</h1>\n\t\t<div>\n\t\t\t<h2>Listing</h2>\n\t\t\t<ul>\n";
		*str << "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Autoindex Page</title>\n\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"error_page.css\">\n\t</head>\n\t<body>\n\t\t<h1>Autoindex Page</h1>\n\t\t<div>\n\t\t\t<h2>Listing</h2>\n\t\t\t<ul>\n";
		while ((ent = readdir(dir)) != NULL)
		{
			path = std::string(directory) + "/" + std::string(ent->d_name);
			std::cout << path << std::endl;
			stat(path.c_str(), &filestat);
			time_str = std::asctime(std::localtime(&filestat.st_mtime));
			time_str.pop_back();
//			*static_cast<std::stringstream*>(data.file) << "\t\t\t\t<li>" << "<div><a href=\"\">" << ent->d_name << "</a></div>" << "<div>" << ent->d_reclen << " bytes " << time_str << "</div><li>\n";
			*str << "\t\t\t\t<li>" << "<div><a href=\"\">" << ent->d_name << "</a></div>" << "<div>" << ent->d_reclen << " bytes " << time_str << "</div><li>\n";
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