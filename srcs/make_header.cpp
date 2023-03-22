/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_header.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:11:28 by stissera          #+#    #+#             */
/*   Updated: 2023/03/22 00:22:20 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common.h"
#include "../includes/utils.hpp"

std::string ft::make_header(int http_err)
{
	static std::map<int, std::string>	head_err;

	if (head_err.empty())
	{
		std::fstream	file("error_http.ini");
		std::string		line;
		if (!file.is_open())
			throw std::invalid_argument("Can't found error_http.ini!");
		while(std::getline(file, line))
		{
			std::string key = line.substr(line.find_first_not_of(" \t"), line.find_first_of("\t"));
			std::string	value = line.substr(line.find_first_of("\t") + 1);
			head_err.insert(std::make_pair(std::strtoul(key.data(), NULL, 10), value));
		}
		return ("");
	}

	std::string	header;
	std::map<int, std::string>::iterator it = head_err.find(http_err);

	if (it != head_err.end())
		header = "HTTP/1.1 " + std::to_string(it->first) + " " + it->second + "\r\n";

	return (header);
}

std::string ft::make_content_type(std::string type_file)
{
	static std::map<std::string, std::string>	content_type;

	if (content_type.empty())
	{
		std::fstream	file("content_type.ini");
		std::string		line;
		if (!file.is_open())
			throw std::invalid_argument("Can't found content_type.ini!");
		while(std::getline(file, line))
		{
			std::string key = line.substr(line.find_first_not_of(" \t."), line.find_first_of("\t") - line.find_first_not_of(" \t."));
			std::string	value = line.substr(line.find_first_of("\t") + 1);
			content_type.insert(std::make_pair(key, value));
		}
		return ("");
	}

	std::string	header;
	std::map<std::string, std::string>::iterator it = content_type.find(type_file);

	if (it != content_type.end())
		header = "Content-Type: " + it->second + "\r\n\r\n";
	else
		header = "Content-Type: application/octet-stream\r\n\r\n";

	return (header);
}

