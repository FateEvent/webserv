/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_stock.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/17 15:47:57 by faventur         ###   ########.fr       */
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

// disons que je pars de block_cleaner pour sortir des pairs key-value
std::vector<std::string>	block_cleaner(std::string str)
{
	std::vector<std::string>	arr;
	std::string::size_type		i(0);
	std::string					buf;
	std::string::size_type		pos(0);

	while ((pos = str.find(';')) != std::string::npos)
		str.erase(pos, 1);
	while (str[i] && ::isspace(str[i]))
		++i;
	while (str[i] && !::isspace(str[i]) && str[i] != '{')
	{
		buf += str[i];
		++i;
	}
	pos = str.find('{');
	str.erase(pos, 1);
	arr.push_back(buf);
	buf.clear();
	while (str[i] && str[i] != '}')
	{
		if (!::isspace(str[i]))
			buf += str[i];
		if (!::isspace(str[i]) && ::isspace(str[i + 1]))
		{
			arr.push_back(buf);
			buf.clear();
		}
		++i;
	}
	return (arr);
}

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

std::pair<std::string, std::string>	string_parser(std::string str)
{
	std::string::size_type	i(0);
	std::string	key;
	std::string	val;
	std::string::size_type	pos(0);

	while (str[i] && ::isspace(str[i]))
		++i;
	while (str[i] && !::isspace(str[i]))
	{
		key += str[i];
		++i;
	}
	while (str[i])
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

std::multimap<std::string, std::string>	clean_string(std::string target,
	std::vector<std::string> &arr, std::multimap<std::string, std::string> &map)
{
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr.begin();
	while (first != arr.end())
	{
		if ((*first).find(target) != std::string::npos)
			arr.erase(first);
		if ((*first).find('{') != std::string::npos)
			arr.erase(first);
		map.insert(string_parser(*first));
		arr.erase(first);
		if ((*first).find('}') != std::string::npos)
			arr.erase(first);
	}
	return (map);
}

std::multimap<std::string, std::string>	split_string(std::string target, std::vector<std::string> &arr)
{
	std::multimap<std::string, std::string>	map;
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr.begin();
	while (first != arr.end())
	{
		std::string::size_type	pos((*first).find("location"));
		if (pos != std::string::npos)
		{
			(*first).erase(std::remove_if((*first).begin(), (*first).begin() + pos, ::isspace), (*first).begin() + pos);
			key = (*first).substr(0, 8);
			val = (*first).substr(8);
			while ((*first).find('}') == std::string::npos)
			{
				arr.erase(first);
				val += '\n';
				val += *first;
			}
			arr.erase(first);
			map.insert(std::make_pair(key, val));
		}
		else
			++first;
	}
	clean_string(target, arr, map);
	return (map);
}

std::multimap<std::string, std::string>	cut_block(std::string target, std::vector<std::string> &arr)
{
	std::multimap<std::string, std::string>	map;
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
	map.insert(std::make_pair(key, val));
	return (map);
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
	std::string target = "server";

	int	i = search(target, '{', '}', arr);
	std::cout << "line: " << i << std::endl;
	if (i >= 0)
	{
		std::cout << " --- the config file vector --- " << std::endl;
		for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
			std::cout << *first << std::endl;
		block_map = cut_block(target, arr);
		std::cout << " --- the block --- " << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = block_map.begin(); first != block_map.end(); ++first)
			std::cout << first->first << ": " << first->second << std::endl;
/*		map = split_string(target, arr);
		comments_cleaner(map);
		std::cout << " --- the result --- " << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
			std::cout << first->first << ": " << first->second << std::endl;

		if (target == "server")
		{
			std::multimap<std::string, std::string>::iterator	pos = map.find("location");
			arr = block_cleaner(pos->second);
			for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
				std::cout << *first << std::endl;
		}
*/
	}
}
