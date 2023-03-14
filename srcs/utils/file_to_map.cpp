/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_to_map.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:24:35 by stissera          #+#    #+#             */
/*   Updated: 2023/03/11 01:12:08 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

void	ft::file_to_map(std::ifstream &file, std::multimap<std::string, std::string>& block)
{
	std::string line;
	size_t		start;
	size_t		end;
	char		space[] = " \v\f\r\n\t";

	size_t		bracket = 0;
	std::string key;
	std::string value;

	while (std::getline(file, line))
	{
		start = line.find_first_not_of(space);
		if (line[0] == '#')
			continue;
		if (line.find_last_not_of(space) < line.find_first_of("#"))
			end = line.find_last_not_of(space);
		else
			end = line.find_first_of("#") - 2;
		if (start == line.npos)
			continue;
		if (line.find("{") != line.npos)
				bracket++;
		if (key.empty())
		{
			start = line.find_first_not_of(" \v\f\r\n\t{");
			end = line.find_last_not_of(" \v\f\r\n\t{");
			key.append(line.substr(start, end + 1));
			continue;
		}
		if (bracket == 0 && value.empty())
			throw std::invalid_argument(line);
		if (line.find("}") != line.npos)
			bracket--;
		if (bracket != 0)
		{
			start = line.find_first_not_of(" \v\f\r\n\t{");
			if (start != line.npos && end != line.npos)
			{
				value.append(line.substr(start, end + 1) + "\n");
			}
		}
		if (bracket <= 0 && value.empty())
			throw std::invalid_argument(line);
		else if (bracket == 0)
		{
			value.rbegin()[0] = 0;
			block.insert(std::pair<std::string, std::string>(key, value));
			key.clear();
			value.clear();
			bracket = 0;
		}
	}
	if (bracket)
		throw std::invalid_argument("closing braket!");
}