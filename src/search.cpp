/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/15 14:08:42 by faventur         ###   ########.fr       */
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

std::multimap<std::string, std::string>	split_string(std::vector<std::string> *arr)
{
	std::multimap<std::string, std::string>	map;
	std::string	key;
	std::string	val;

	std::vector<std::string>::iterator	first = arr->begin();
	while (first != arr->end())
	{
		size_t	pos = (*first).find("error_page");
		if (pos != std::string::npos)
		{
			(*first).erase(std::remove_if((*first).begin(), (*first).end(), ::isspace), (*first).end());
			key = (*first).substr(0, 10);
			std::cout << key << "\\n" << std::endl;
			val = (*first).substr(10, 6);
			val.insert(val.begin() + 3, ' ');
			std::cout << val << "\\n" << std::endl;
			map.insert(std::make_pair(key, val));
			arr->erase(first);
		}
		else
			++first;
	}
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
	std::vector<std::string>	arr;

	std::cout << search("server", '{', '}', &arr) << std::endl;
	split_string(&arr);
	for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
		std::cout << *first << std::endl;
	
}
