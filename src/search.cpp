/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/16 11:40:37 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** The search() function looks for a target string in a file and pushes in a
** vector every line included between the opening and the closure characters.
** 
** The search() function returns -1 in case the file can't be opened, 0 in case
** the target string has not been found or the vector passed as a parameter has
** not been filled, 1 in case of success.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <cctype>

void	comments_cleaner(std::multimap<std::string, std::string> *map)
{
	for (std::multimap<std::string, std::string>::iterator	first = map->begin();
		first != map->end(); ++first)
	{
		std::string::size_type	pos = first->second.find('#');
		if (pos != std::string::npos)
		{
			first->second.erase(pos);
			std::cout << first->second << std::endl;
		}
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
	while (str[i] && ::isspace(str[i]))
		++i;
	while (str[i] && !::isspace(str[i]))
	{
		val += str[i];
		++i;
	}
	pos = val.find(';');
	if (pos != std::string::npos)
		val.erase(pos);
	return (std::make_pair(key, val));
}

std::multimap<std::string, std::string>	clean_string(std::string target,
	std::vector<std::string> *arr, std::multimap<std::string, std::string> *map)
{
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr->begin();
	while (first != arr->end())
	{
		if ((*first).find(target) != std::string::npos)
			arr->erase(first);
		if ((*first).find('{') != std::string::npos)
			arr->erase(first);
		map->insert(string_parser(*first));
		arr->erase(first);
		if ((*first).find('}') != std::string::npos)
			arr->erase(first);
	}
	return (*map);
}

std::multimap<std::string, std::string>	split_string(std::string target, std::vector<std::string> *arr)
{
	std::multimap<std::string, std::string>	map;
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr->begin();
	while (first != arr->end())
	{
		if ((*first).find("error_page") != std::string::npos)
		{
			(*first).erase(std::remove_if((*first).begin(), (*first).end(), ::isspace), (*first).end());
			key = (*first).substr(0, 10);

			val = (*first).substr(10, 11);
			val.insert(val.begin() + 3, ' ');

			map.insert(std::make_pair(key, val));
			arr->erase(first);
		}
		else if ((*first).find("location") != std::string::npos)
		{
			(*first).erase(std::remove_if((*first).begin(), (*first).end(), ::isspace), (*first).end());
			key = (*first).substr(0, 8);
			val = (*first).substr(8);
			while ((*first).find('}') == std::string::npos)
			{
				arr->erase(first);
				val += '\n';
				val += *first;
			}
			arr->erase(first);
			map.insert(std::make_pair(key, val));
		}
		else
			++first;
	}
	clean_string(target, arr, &map);
	return (map);
}

std::multimap<std::string, std::string>	cut_block(std::string target, std::vector<std::string> *arr)
{
	std::multimap<std::string, std::string>	map;
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr->begin();
	(*first).erase(std::remove_if((*first).begin(), (*first).end(), ::isspace), (*first).end());
	key = (*first).substr(0, target.length());
	std::string::size_type	pos = key.find('{');
	if (pos == std::string::npos)
	{
		++first;
		val = *first;
	}
	else
		val = (*first).substr(target.length());
	while (first != arr->end())
	{
		++first;
		val += '\n';
		val += *first;
	}
	++first;
	map.insert(std::make_pair(key, val));
	return (map);
}

ssize_t	search(std::string target, char opening, char closure, std::vector<std::string> *arr, ssize_t l = 0)
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
				arr->push_back(buffer);
		}
		if (target_lookup)
		{
			pos = buffer.find(opening);
			if (pos != std::string::npos)
				op++;
			if (op >= 1)
				arr->push_back(buffer);
			std::string::size_type	pos2 = buffer.find(closure);
			if (pos2 != std::string::npos)
				cl++;
			if (op >= 1 && op == cl)
				break ;
		}
	}
	return (line);
}

int	main()
{
	std::multimap<std::string, std::string>	block_map;
	std::multimap<std::string, std::string>	map;
	std::vector<std::string>				arr;
	std::string target = "http";

	if (search(target, '{', '}', &arr) >= 0)
	{
		std::cout << " --- the config file vector --- " << std::endl;
		for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
			std::cout << *first << std::endl;
		block_map = cut_block(target, &arr);
		std::cout << " --- the block --- " << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = block_map.begin(); first != block_map.end(); ++first)
			std::cout << first->first << ": " << first->second << std::endl;
		map = split_string(target, &arr);
		comments_cleaner(&map);
		std::cout << " --- the result --- " << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
			std::cout << first->first << ": " << first->second << std::endl;
	}
}
