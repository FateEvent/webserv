/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/17 17:21:02 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** The search() function looks for a target string in a file and pushes in a
** vector every line included between the opening and the closure characters.
** 
** Return value: The search() function returns -1 in case the file can't be
** opened, and the number of the line of the config file where it stopped
** otherwise.
** 
** The cut_block() function creates a multimap containing the block name as the
** key and the block as a value.
** 
** The split_string() function (together with the clean_string() function)
** creates a multimap from the vector containing every line of the config file.
** 
** The string_parser() function makes a key-value pair from a string.
** 
** The comments_cleaner() function takes out the comments (indicated by a '#'
** character) from a string.
** 
** The block_cleaner() function makes a vector out of a string containing
** a key and many key-value pairs.
*/

#include "../include/search.hpp"

void	comments_cleaner(std::multimap<std::string, std::string> &map)
{
	for (std::multimap<std::string, std::string>::iterator	first = map.begin();
		first != map.end(); ++first)
	{
		std::string::size_type	pos = first->second.find('#');
		if (pos != std::string::npos)
			first->second.erase(pos);
	}
}

std::pair<std::string, std::string>	block_parser(std::string str, std::string::size_type &i)
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
	pos = str.find_last_of('}');
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

std::pair<std::string, std::string>	string_parser(std::string str, std::string::size_type i)
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
	pos = str.find('}');
	while (str[i] && i <= pos)
	{
		if (!::isspace(str[i]))
			val += str[i];
		if (::isspace(str[i + 1]))
			val += ' ';
		++i;
	}
	pos = val.find(';');
	if (pos != std::string::npos)
		val.erase(pos);
	return (std::make_pair(key, val));
}

std::string::size_type	find_char(std::string str, char c, std::string::size_type i)
{
	std::string::size_type	count(0);

	while (str[i] != '\n')
	{
		if (str[i] == c)
			return (count);
		++i;
		++count;
	}
	return (std::string::npos);
}

std::multimap<std::string, std::string>	split_block(std::string str)
{
	std::multimap<std::string, std::string>	map;
	std::string								key;
	std::string								val;
	std::string::size_type					i(0);
	std::string::size_type					line_length(0);
	std::string::size_type					pos(0);

	while (str[i] && ::isspace(str[i]))
		++i;
	if (str[i] == '{')
	{
		++i;
		while (str[i] && ::isspace(str[i]))
			++i;
		while (str[i])
		{
			std::string::size_type	pos = find_char(str, '{', i);
			if (pos != std::string::npos)
			{
				map.insert(block_parser(str, i));
			}
			else if (str[i] == '\n')
			{
				map.insert(string_parser(str, i - line_length));
				line_length = 0;
			}
			if (str[i] == '}')
			{
				++i;
				++line_length;
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
		pos = str.find('}');
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

std::pair<std::string, std::string>	cut_block(std::string target, std::vector<std::string> &arr)
{
	std::string	key;
	std::string	val;
	std::vector<std::string>::iterator	first = arr.begin();
	std::string::size_type	pos((*first).find(target));

	(*first).erase(std::remove_if((*first).begin(), (*first).begin() + pos, ::isspace), (*first).begin() + pos);
	key = (*first).substr(0, target.length());
	pos = (*first).find('{');
	if (pos == std::string::npos)
	{
		++first;
		val = *first;
	}
	else
		val = (*first).substr(target.length());
	while (first != arr.end())
	{
		++first;
		val += '\n';
		val += *first;
	}
	++first;
	return (std::make_pair(key, val));
}

int	cut_multiple_blocks(std::multimap<std::string, std::string> &map)
{
	std::string								key;
	std::string								val;
	std::string								buffer;
	std::string								str;
	std::string::size_type					i(0);
	std::ifstream							inFlux("config/local.conf");

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (-1);
	}
	while (getline(inFlux, buffer))
	{
		while (buffer[i] && ::isspace(buffer[i]))
			++i;
		while (buffer[i] && !::isspace(buffer[i]))
			str += buffer[i];
	}
	/*
	(*first).erase(std::remove_if((*first).begin(), (*first).begin() + pos, ::isspace), (*first).begin() + pos);
	key = (*first).substr(0, target.length());
	pos = (*first).find('{');
	if (pos == std::string::npos)
	{
		++first;
		val = *first;
	}
	else
		val = (*first).substr(target.length());
	while (first != arr.end())
	{
		++first;
		val += '\n';
		val += *first;
	}
	++first;
	map.insert(std::make_pair(key, val));
	*/
	std::cout << str << std::endl;
	return (0);
}

// reads the config file and returns an array of strings containing the target keys
int	config_file_reader(std::vector<std::string> &arr)
{
	std::ifstream				inFlux("config/local.conf");
	std::string					buffer;
	std::string					target;
	size_t						op(0);
	size_t						cl(0);
	std::string::size_type		i(0);

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (-1);
	}
	while (getline(inFlux, buffer))
	{
		while (op == cl && !::isspace(buffer[i]))
		{
			target += buffer[i];
			++i;
		}
		std::string::size_type	pos = buffer.find('{');
		if (pos != std::string::npos)
			op++;
		std::string::size_type	pos2 = buffer.find('}');
		if (pos2 != std::string::npos)
			cl++;
		arr.push_back(target);
		target.clear();
	}
	return (0);
}

int	bracket_parser(char opening, char closure)
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

ssize_t	search(std::string target, char opening, char closure, std::vector<std::string> &arr, ssize_t l = 0)
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

int	main()
{
	std::multimap<std::string, std::string>	block_map;
	std::multimap<std::string, std::string>	map;
	std::vector<std::string>				arr;
	std::string target = "http";

	int i = bracket_parser('{', '}');
	std::cout << "brackets: " << i << std::endl;
	i = search(target, '{', '}', arr);
	std::cout << "line: " << i << std::endl;
	if (i >= 0)
	{
		std::cout << " --- the config file vector --- " << std::endl;
		for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
			std::cout << *first << std::endl;
		block_map.insert(cut_block(target, arr));
		std::cout << " --- the block --- " << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = block_map.begin(); first != block_map.end(); ++first)
			std::cout << first->first << ": " << first->second << std::endl;
		map = split_block(block_map.find(target)->second);
		comments_cleaner(map);
		std::cout << " --- the result --- " << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
			std::cout << first->first << ": " << first->second << '$' << std::endl;

		if (target == "server")
		{
			std::cout << " --- mais encore --- " << std::endl;
			std::multimap<std::string, std::string>::iterator	pos = map.find("location");
			map = split_block(pos->second);
			for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
				std::cout << first->first << ": " << first->second << '$' << std::endl;
		}

/*		config_file_reader(arr);
		for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
			std::cout << *first << std::endl;
*/
	}
}
