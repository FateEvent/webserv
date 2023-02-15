/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/15 17:24:07 by faventur         ###   ########.fr       */
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

std::multimap<std::string, std::string>	clean_string(std::vector<std::string> *arr,
	std::multimap<std::string, std::string> *map)
{
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr->begin();
	while (first != arr->end())
	{
//		arr->erase(first);
//		if ((*first).find('{') != std::string::npos)
//			arr->erase(first);
		map->insert(string_parser(*first));
		arr->erase(first);
//		if ((*first).find('}') != std::string::npos)
//			arr->erase(first);
	}
	return (*map);
}

std::multimap<std::string, std::string>	split_string(std::vector<std::string> *arr)
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
	clean_string(arr, &map);
	return (map);
}

int	search(std::string target, char opening, char closure, std::vector<std::string> *arr)
{
	std::ifstream	inFlux("config/local.conf");
	std::string		buffer;
	bool			target_lookup(false);
	size_t			op(0);
	size_t			cl(0);

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (-1);
	}
	while (getline(inFlux, buffer))
	{
		std::string::size_type	pos = buffer.find(target);
		if (pos != std::string::npos)
			target_lookup = true;
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
	if (!target_lookup || arr->empty())
		return (0);
	return (1);
}

int	main()
{
	std::multimap<std::string, std::string>	map;
	std::vector<std::string>				arr;

	if (search("http", '{', '}', &arr) == 1)
	{
		map = split_string(&arr);
		for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
			std::cout << *first << std::endl;
		std::cout << std::endl << std::endl;
		std::cout << "bam" << std::endl;
		for (std::multimap<std::string, std::string>::iterator	first = map.begin(); first != map.end(); ++first)
			std::cout << first->first << ": " << first->second << std::endl;
	}
}
