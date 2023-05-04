#include "../includes/common.h"

std::stringstream *ft::listing_creator(char const *directory, std::string here)
{
	DIR				*dir;
	struct dirent	*ent;
	int				closed;
	struct stat		filestat;
	std::string		path;
	std::string		time_str;

	if (!here.empty())
	{
		here = here.substr(1);
		here.append("/");
	}

	dir = opendir(directory);
	if (dir != NULL)
	{
		std::stringstream *str = new std::stringstream(); 
		*str << "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Autoindex Page</title>\n\t\t<style>\n\t\t\ttable {\n\t\t\tfont-family: arial, sans-serif;\n\t\t\tborder-collapse: collapse;\n\t\t\twidth: 100%;\n\t\t\t}\n\n\n\t\t\ttd, th {\n\t\t\tborder: 1px solid #dddddd;\n\t\t\ttext-align: left;\n\t\t\tpadding: 8px;\n\t\t\t}\n\n\n\t\t\ttr:nth-child(even) {\n\t\t\tbackground-color: #dddddd;\n\t\t\t}\n\t\t</style>\n\t</head>\n\t<body>\n\t\t<h1>Autoindex Page</h1>\n\t\t<table>\t\t\t<tr>\n\t\t\t\t<th>Listing</th>\n\t\t\t\t<th>Size</th>\n\t\t\t\t<th>Last accessed</th>\n\t\t\t</tr>\n";
		while ((ent = readdir(dir)) != NULL)
		{
			path = std::string(directory) + "/" + std::string(ent->d_name);
			stat(path.c_str(), &filestat);
			time_str = std::asctime(std::localtime(&filestat.st_mtime));
			time_str.pop_back();
			*str << "\t\t\t<tr>\n\t\t\t\t<td><a href=\"" << here << ent->d_name << "\">" << ent->d_name << "</a></td>\n\t\t\t\t<td>" << filestat.st_size << " bytes</td>\n\t\t\t\t<td>" << time_str << "</td>\n\t\t\t</tr>\n";
		}
		*str << "\t\t</table>\n\t</body>\n</html>\n";
		closed = closedir(dir);
		if (closed)
			throw std::invalid_argument("Couldn't close the directory stream!");
		return (str);
	}
	else
		throw std::invalid_argument("Invalid or unexisting directory!");
	return (0);
}