/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 14:26:39 by faventur          #+#    #+#             */
/*   Updated: 2023/03/03 14:31:00 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/search.hpp"

void	ft::space_eraser(std::string &str, std::string::size_type i)
{
	while (str[i])
	{
		if (::isspace(str[i]))
			str.erase(i, i + 1);
		else
			++i;
	}
}

std::vector<std::string>	ft::str_to_vect(std::string str, std::string sep)
{
	std::vector<std::string>	vec;
	char	*tmp = new char[str.length() + 1];

	std::strcpy(tmp, str.c_str());
	char	*tok = std::strtok(tmp, sep.data());

	while (tok != NULL)
	{
		std::string(tok).erase(0, std::string(tok).find_first_not_of(" \t"));
   		std::string(tok).erase(std::string(tok).find_last_not_of(" \t") + 1);
		vec.push_back(std::string(tok));
		tok = std::strtok(NULL, sep.data());	
	}
	delete[] tmp;
	for (std::vector<std::string>::iterator first = vec.begin(); first != vec.end(); ++first)
	{
		if (first->find(";q") != std::string::npos)
		{
			first->erase(first->find(";q"));
		}
/*
		else if (first->find(";v") != std::string::npos)
		{
			first->erase(first->find(";v"));
		}
*/
	}
	return (vec);
}

std::map<std::string, std::string>	ft::str_to_map(std::string str, std::string sep)
{
	std::map<std::string, std::string> 	map;
	std::vector<std::string> 			vect;
	ft::string_pair 					pair;	
	
	vect = ft::str_to_vect(str, sep);
	
	for(std::vector<std::string>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		pair = ft::string_parser(*it ,"=");
		map.insert(pair);
	}
	return (map);
}
