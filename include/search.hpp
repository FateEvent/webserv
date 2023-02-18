#ifndef SEARCH_H
# define SEARCH_H

# include <iostream>
# include <fstream>
# include <string>
# include <algorithm>
# include <vector>
# include <map>
# include <utility>
# include <cctype>

typedef struct search {
	std::string	target;
	std::size_t	line;
}				t_search;

namespace ft {
	typedef std::vector<std::string>				string_vector;
	typedef std::multimap<std::string, std::string>	string_map;
	typedef std::pair<std::string, std::string>		pair;

	string_vector	block_cleaner(std::string str);
	void			comments_cleaner(string_map &map);
	pair			string_parser(std::string str);
	string_map		clean_string(std::string target, string_vector &arr, string_map &map);
	string_map		split_string(std::string target, string_vector &arr);
	string_map		cut_block(std::string target, string_vector &arr);
	ssize_t			search(std::string target, char opening, char closure, string_vector &arr, ssize_t l = 0);
}

#endif
