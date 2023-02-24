/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/24 11:59:40 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** The bracket_parser() function verifies that opening and closing brackets
** are present.
**
** Return value: The bracket_parser() function returns 1 in case of error, 0
** otherwise.
**
** The search() function looks for a target string in a file and pushes in a
** vector every line included between the opening and the closure characters.
**
** Return value: The search() function returns -1 in case the file can't be
** opened, and the number of the line of the config file where it stopped
** otherwise.
**
** The config_file_reader() function reads the config file and fills the
** vector passed as a parameter with a structure containing the target keys
** and the number of the line in the file.
**
** Return value: The config_file_reader() function returns 1 in case of error,
** 0 otherwise.
**
** The cut_block() function creates a multimap containing the block name as
** the key and the block as a value.
**
** Return value: The cut_block() function returns a pair made up of the
** block name as the key and the block as a value.
**
** The cut_multiple_blocks() function returns a map containing the structure
** blocks located in the config file.
**
** The string_parser() function makes a key-value pair out of a string line.
**
** The block_parser() function makes a key-value pair out of a struct block.
**
** The split_block() function makes a multimap out of the string block passed
** as a parameter. It makes use of the string_parser() function and the
** block_parser() function.
**
** The find_char() function looks for a character in a particular line of a
** block of text; it takes as a parameter a string (containing the newline
** character '\n'), the character to research and the line number.
**
** Return value: The find_char() function returns a size_t indicating the
** position of the character on the line or the npos character (defined in
** the header <string>, the maximum size of a size_t).
**
** The comments_cleaner() function takes out the comments (indicated by a
** '#' character) from a string.
*/

#include "../include/search.hpp"

void	ft::space_eraser(std::multimap<std::string, std::string> &map)
{
	std::multimap<std::string, std::string>::iterator	first(map.begin()), last(map.end());
	std::string::size_type								i(0);

	for (; first != last; ++first)
	{
		i = 0;
		while (first->second[i])
		{
			if (::isspace(first->second[i]))
				first->second.erase(i, i + 1);
			++i;
		}
		if (first->second == "localhost")
			first->second = "127.0.0.1";
		if (first->second == ":")
			first->second = "0.0.0.0";
	}
}

void	ft::kärcherizer(std::multimap<std::string, std::string> &map)
{
	std::multimap<std::string, std::string>::iterator	first(map.begin()), last(map.end());
	std::string											str;
	std::string::size_type								i(0);

	for (; first != last; ++first)
	{
		std::string::size_type	pos = first->second.find_last_of(':');
		if (pos != std::string::npos)
		{
			i = pos + 1;
			while (first->second[i])
			{
				if (!::isspace(first->second[i]))
					str += first->second[i];
				else if (::isspace(first->second[i]))
				{
					map.insert(std::make_pair("listen", str));
					str.clear();
				}
				++i;
			}
			if (str == "tcp" || str == "udp")
				map.insert(std::make_pair("type", str));
			else
				map.insert(std::make_pair(first->first, str));
			first->second.erase(pos);
		}
	}
	ft::space_eraser(map);
}

void	ft::comments_cleaner(std::multimap<std::string, std::string> &map)
{
	std::multimap<std::string, std::string>::iterator first(map.begin()), last(map.end());

	for (; first != last; ++first)
	{
		std::string::size_type	pos = first->second.find('#');
		if (pos != std::string::npos)
			first->second.erase(pos);
	}
}

std::pair<std::string, std::string>	ft::block_parser(std::string str, char closure, std::string::size_type &i)
{
	std::string				key;
	std::string				val;
	std::string::size_type	pos(0);

	while (str[i] && ::isspace(str[i]))
		++i;
	while (str[i] && !::isspace(str[i]))
	{
		key += str[i];
		++i;
	}
	pos = str.find_last_of(closure);
	while (str[i] && i < pos)
	{
		if (!::isspace(str[i]))
			val += str[i];
		if (str[i] == '\n')
			val += '\n';
		if (::isspace(str[i]) && !::isspace(str[i + 1]))
			val += ' ';
		++i;
	}
	return (std::make_pair(key, val));
}

std::pair<std::string, std::string>	ft::string_parser(std::string str, char closure, std::string::size_type i)
{
	std::string				key;
	std::string				val;
	std::string::size_type	pos(0);

	std::cout << "str: " << str << std::endl;
	while (str[i] && ::isspace(str[i]))
		++i;
	while (str[i] && !::isspace(str[i]))
	{
		key += str[i];
		++i;
	}
	pos = str.find(closure);
	while (str[i] && i <= pos)
	{
		if (!::isspace(str[i]))
			val += str[i];
		if (::isspace(str[i]) && !::isspace(str[i + 1]))
			val += ' ';
		++i;
	}
	pos = val.find(';');
	if (pos != std::string::npos)
		val.erase(pos);
	std::cout << "kv: " << key << ": " << val << std::endl;
	return (std::make_pair(key, val));
}

std::string::size_type	ft::find_char(std::string str, char c, std::string::size_type i)
{
	std::string::size_type	count(0);

	while (str[i] && str[i] != '\n')
	{
		if (str[i] == c)
			return (count);
		++i;
		++count;
	}
	return (std::string::npos);
}

std::multimap<std::string, std::string>	ft::split_block(std::multimap<std::string, std::string>::iterator it, char opening, char closure)
{
	std::multimap<std::string, std::string>	map;
	std::string								key;
	std::string								val;
	std::string::size_type					i(0);
	std::string::size_type					line_length(0);
	std::string::size_type					pos(0);
	std::string								str(it->second);

//	map.insert(std::make_pair(it->first, it->second));
	while (str[i] && ::isspace(str[i]))
		++i;
	if (str[i] == opening)
	{
		++i;
		while (str[i] && ::isspace(str[i]))
			++i;
		while (str[i])
		{
			std::string::size_type	pos = find_char(str, opening, i);
			if (pos != std::string::npos)
				map.insert(ft::block_parser(str, closure, i));
			else if (str[i] == '\n')
			{
				map.insert(ft::string_parser(str, closure, i - line_length));
				line_length = 0;
			}
			++i;
			++line_length;
		}
	}
	else
	{
		while (str[i] && ::isspace(str[i]))
		++i;
		while (str[i] && !::isspace(str[i]))
		{
			key += str[i];
			++i;
		}
		pos = str.find(closure);
		while (str[i] && i <= pos)
		{
			if (!::isspace(str[i]))
				val += str[i];
			if (str[i] == '\n')
				val += '\n';
			if (::isspace(str[i]) && !::isspace(str[i + 1]))
				val += ' ';
			++i;
		}
		map.insert(std::make_pair(key, val));
	}
	return (map);
}

std::pair<std::string, std::string>	ft::cut_block(std::string target, char opening, std::vector<std::string> &arr)
{
	std::string	key;
	std::string	val;
	std::vector<std::string>::iterator	first = arr.begin();
	std::string::size_type	pos((*first).find(target));

	(*first).erase(std::remove_if((*first).begin(), (*first).begin() + pos, ::isspace), (*first).begin() + pos);
	key = (*first).substr(0, target.length());
	pos = (*first).find(opening);
	if (pos == std::string::npos)
	{
		++first;
		val = *first;
	}
	else
		val = (*first).substr(target.length());
	for (++first; first != arr.end(); first++)
	{
		val += '\n';
		val += *first;
	}
	return (std::make_pair(key, val));
}

ssize_t	ft::search(std::string target, char opening, char closure, std::vector<std::string> &arr, ssize_t l)
{
	std::ifstream	inFlux("config/local.conf");
	std::string		buffer;
	bool			target_lookup(false);
	size_t			op(0);
	size_t			cl(0);
	ssize_t			line(0);

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (-1);
	}
	while(line < l)
	{
		getline(inFlux, buffer);
		++line;
	}
	while (getline(inFlux, buffer))
	{
		std::string::size_type	pos = buffer.find(target);
		if (pos != std::string::npos)
		{
			target_lookup = true;
			if (buffer.find(opening) == std::string::npos)
				arr.push_back(buffer);
		}
		if (target_lookup)
		{
			pos = buffer.find(opening);
			if (pos != std::string::npos)
				op++;
			if (op >= 1)
				arr.push_back(buffer);
			std::string::size_type	pos2 = buffer.find(closure);
			if (pos2 != std::string::npos)
				cl++;
			if (op >= 1 && op == cl)
				break ;
		}
		++line;
	}
	return (line);
}

int	ft::config_file_reader(std::vector<t_search> &arr, char opening, char closure)
{
	std::ifstream				inFlux("config/local.conf");
	std::string					buffer;
	size_t						op(0);
	size_t						cl(0);
	std::string::size_type		i(0);
	t_search					s = {"", 0};

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (1);
	}
	while (getline(inFlux, buffer))
	{
		i = 0;
		while (buffer[i])
		{
			if (buffer[i] == opening)
				op++;
			else if (buffer[i] == closure)
				cl++;
			else if (op == cl && !::isspace(buffer[i]))
				s.target += buffer[i];
			++i;
		}
		if (s.target != "")
			arr.push_back(s);
		s.target.clear();
		++s.line;
	}
	return (0);
}

int	ft::cut_multiple_blocks(char opening, char closure, std::multimap<std::string, std::string> &map)
{
	std::vector<t_search>		conf_arr;
	std::vector<std::string>	str_arr;
	std::string					key;
	std::string					val;

	if (ft::config_file_reader(conf_arr, opening, closure) == 0)
	{
		for (std::vector<t_search>::iterator	first = conf_arr.begin(); first != conf_arr.end(); ++first)
		{
			search(first->target, opening, closure, str_arr, first->line);
			map.insert(cut_block(first->target, opening, str_arr));
			str_arr.clear();
		}
		return (0);
	}
	return (1);
}

int	ft::bracket_parser(char opening, char closure)
{
	std::ifstream	inFlux("config/local.conf");
	std::string		buffer;
	size_t			op(0);
	size_t			cl(0);

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (1);
	}
	while (getline(inFlux, buffer))
	{
		std::string::size_type	pos = buffer.find(opening);
		if (pos != std::string::npos)
			op++;
		std::string::size_type	pos2 = buffer.find(closure);
		if (pos2 != std::string::npos)
			cl++;
	}
	if (op >= 1 && op == cl)
		return (0);
	return (1);
}

// example of use
/*
int	main()
{
	std::multimap<std::string, std::string>	block_map;
	std::multimap<std::string, std::string>	map;

	int i = bracket_parser('{', '}');
	std::cout << "brackets: " << i << std::endl;
	if (i == 0)
	{
		std::cout << " --- the blocks --- " << std::endl;
		cut_multiple_blocks('{', '}', block_map);
		for (std::map<std::string, std::string>::iterator	first = block_map.begin(); first != block_map.end(); ++first)
			std::cout << first->first << ": " << first->second << '$' << std::endl;

		// here I split the block "server", for example
		std::cout << " --- splitting the block 'server' --- " << std::endl;
		map = split_block(block_map.find("server"), '{', '}');
		comments_cleaner(map);
		for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
			std::cout << first->first << ": " << first->second << '$' << std::endl;

		// with the same function I split the block "location"
		std::cout << " --- splitting the block 'location' --- " << std::endl;
		map = split_block(map.find("location"), '{', '}');
		for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
			std::cout << first->first << ": " << first->second << '$' << std::endl;
	}
}
*/
