/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_to_map.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:43:46 by stissera          #+#    #+#             */
/*   Updated: 2023/04/03 17:15:07 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

bool	ft::find_val(std::vector<std::string> &vector, std::string &var, std::string key)
{
	std::vector<std::string>::iterator first = vector.begin(), last = vector.end();

	for (first = vector.begin(); first != last; ++first)
	{
		while (std::isspace((*first)[0]))
			(*first).erase(0, 1);
		if ((*first).find(key + "=") != std::string::npos)
		{
			var = (*first).substr((*first).find_first_not_of(key + "="));
			return (true);
		}
	}
	return (false);
}

bool	ft::split_to_vectors(std::vector<std::string> &svector, std::string line, char sep)
{
	std::string	separator;

	separator += sep;
	line = line.substr(line.find_first_of(" ") + 1);
	size_t i = line.find_first_of(separator + "\r");

	for (; i != line.npos; i = line.find_first_of(separator + "\r"))
	{
		//std::cout << line.substr(0, line.find_first_of(";") < i ? line.find_first_of(";") : i ) << std::endl;
		svector.push_back(line.substr(0, line.find_first_of(";") < i ? line.find_first_of(";") : i )); // Possible problem when false
		line = line.substr(i + 1);
	}
	return (true);
}

bool	ft::split_to_mapss(std::map<std::string, std::string> &mapss, std::string line, char sep)
{
	if (line.find_first_of(" ") != line.npos)
		line = line.substr(line.find_first_of(" "));
	line = line.substr(line.find_first_not_of(" \t\v\n")); // + 1 change of to not off
	size_t i = line.find_first_of(sep);

	for (; i != line.npos; i = line.find_first_of(sep))
	{
		if (line.find_first_of("=") == line.npos)
			return (false);
	//	std::cout << "Key:   " << line.substr(0, line.find_first_of("=")) << std::endl;
	//	std::cout << "Value: " << line.substr(line.find_first_of("=") + 1,
	//											line.find_first_of(";") - line.find_first_of("=") - 1) << std::endl;
		mapss.insert(std::make_pair<std::string, std::string>(
						line.substr(line.find_first_not_of(" \t\v\n"), line.find_first_of("=") - line.find_first_not_of(" \t\v\n")),
						line.substr(line.find_first_of("=") + 1, line.find_first_of(sep) - line.find_first_of("=") - 1)));
		line = line.substr(i + 1);
	}
	//std::cout <<  "Key:   " << line.substr(0, line.find_first_of("=")) << std::endl;
	//std::cout <<  "Value: " << line.substr(line.find_first_of("=") + 1, line.length()) << std::endl;
	mapss.insert(std::make_pair<std::string, std::string>(
						line.substr(line.find_first_not_of(" \t\v\n"), line.find_first_of("=")),
						line.substr(line.find_first_of("=") + 1, line.find_last_not_of(" \r\n") - line.find_first_of("="))));
	return (true);
}

bool	ft::split_to_maposs(std::map<std::string, std::string> &mapss, std::string line)
{
	mapss.insert(std::make_pair<std::string, std::string>(
						line.substr(0, line.find_first_of(":")),
						line.substr(line.find_first_of(" ") + 1, line.find_last_not_of(" \r\n") - line.find_first_of(" "))));
	return (true);
}