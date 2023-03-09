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
	typedef std::multimap<std::string, std::string>	string_multimap;
	typedef std::map<std::string, std::string>		string_map;
	typedef std::pair<std::string, std::string>		string_pair;

	// config file parsing
	int						bracket_parser(char opening, char closure);
	int						cut_multiple_blocks(char opening, char closure, std::multimap<std::string, std::string> &map);
	int						config_file_reader(std::vector<t_search> &arr, char opening, char closure);
	ssize_t					search(std::string target, char opening, char closure, string_vector &arr, ssize_t l = 0);
	string_pair				cut_block(std::string target, char opening, std::vector<std::string> &arr);
	string_multimap			split_block(std::multimap<std::string, std::string>::iterator it, char opening, char closure);
	std::string::size_type	find_char(std::string str, char c, std::string::size_type i);
	string_pair				string_parser(std::string str, char closure, std::string::size_type i);
	int						string_looper(char c, std::string str);
	string_pair				block_parser(std::string str, char closure, std::string::size_type &i);
	void					comments_cleaner(std::multimap<std::string, std::string> &map);
	void					kärcherizer(std::multimap<std::string, std::string> &map);
	void					space_eraser(std::multimap<std::string, std::string> &map);

	// server-client communication parsing
	string_map				str_to_map(std::string str, std::string sep);
	string_vector			str_to_vect(std::string str, std::string sep);
	string_pair				string_parser(std::string str, std::string sep);
	void					space_eraser(std::string &str, std::string::size_type i = 0);
	std::string				string_cutter(std::string str);
}

#endif