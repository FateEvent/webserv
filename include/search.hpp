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
	typedef std::pair<std::string, std::string>		string_pair;

	void					comments_cleaner(string_map &map);
	string_pair				block_parser(std::string str, std::string::size_type &i);
	string_pair				string_parser(std::string str, std::string::size_type i);
	std::string::size_type	find_char(std::string str, char c, std::string::size_type i);
	string_map				split_block(std::string str);
	string_pair				cut_block(std::string target, std::vector<std::string> &arr);
	ssize_t					search(std::string target, char opening, char closure, string_vector &arr, ssize_t l = 0);
	int						config_file_reader(std::vector<t_search> &arr);
	int						cut_multiple_blocks(string_map &map);
	int						bracket_parser(char opening, char closure);
}

#endif