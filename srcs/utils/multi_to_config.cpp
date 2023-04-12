/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_to_config.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:12:49 by stissera          #+#    #+#             */
/*   Updated: 2023/04/05 09:46:37 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

static std::multimap<std::string, std::string>	parse_value(std::string str)
{
	std::string				key;
	std::string 			value;
	size_t					end_key;
	size_t					end_line = 0;
	std::multimap<std::string, std::string>	ret;

	while (str.begin() != str.end() && end_line != str.npos)
	{
		key.clear();
		value.clear();
		end_key = str.find_first_of(" \n\t\0");
		key.append(str.substr(0, end_key));
		str = str.substr(end_key + 1);
		str = str.substr(str.find_first_not_of(" \t"));
		end_line = str.find_first_of(";\n#\0");
		// if subsequence exist
		if (str.find_first_of("{") < end_line + 1)
		{
			size_t	bracket = 1;
			while (bracket != 0 && str.begin() != str.end())
			{
				value.append(str.substr(0, end_line));
				end_line = str.find_first_of("\n\0");
				str = str.substr(end_line + 1);
				end_line = str.find_first_of("#;\n") + 1;
				if (str.find_first_of("{") < end_line)
					bracket ++;
				if (str.find_first_of("}") < end_line)
					bracket--;
			}
			end_line--;
		}
		value.append(str.substr(0, end_line));
		end_line = str.find_first_of("\n\0");
		str = str.substr(end_line + 1);
		ret.insert(std::pair<std::string, std::string>(key, value));
	}
	ft::check_host(ret);
	return (ret);
}

void	ft::parse_map(std::multimap<std::string, std::string>::iterator start,
					std::multimap<std::string, std::string>::iterator end,
					std::multimap<std::string, std::multimap<std::string, std::string> >& config)
{
	for (; start != end; start++)
		 config.insert(std::make_pair(start->first, parse_value(start->second)));
}

void	ft::check_host(std::multimap<std::string, std::string> &config)
{
	size_t	end;
	std::multimap<std::string, std::string>::iterator it = config.find("host");
	if (it == config.end())
		it = config.find("default");
	if (it->second.find(":") != it->second.npos)
	{
		if (config.find("listen") != config.end())
			throw std::invalid_argument("Confilt in host, listen already exist");
		end = it->second.find_first_not_of("1234567890", it->second.find(":") + 1);
		config.insert(std::pair<std::string, std::string>("listen", it->second.substr(it->second.find(":") + 1, end - it->second.find(":") - 1)));
		it->second = it->second.substr(0, it->second.find_first_of(":"));
	}
}