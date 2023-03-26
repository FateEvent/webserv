/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:09:13 by stissera          #+#    #+#             */
/*   Updated: 2023/03/26 12:49:11 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.h"

bool	ft::parse_header(int fd, s_header &head)
{
	char 						buffer[2];
	std::string					tmp;
	std::string					line;
	std::vector<std::string>	header;
	size_t 						s_str;
	size_t 						e_str;
	ssize_t						recept = 1;

	while (tmp.find("\r\n\r\n") == tmp.npos)
	{
		recept = recv(fd, &buffer, 1, 0);
		if (recept < 1 || buffer[0] == 0)
			break;
		tmp.push_back(buffer[0]);
	}
std::cout << PURPLE << tmp << RST << std::endl;
	if (recept == 0 || (tmp.find("\r\n\r\n") == tmp.npos)) // close client
	{
		std::cout << YELLOW << "Connexion closed by client" << RST << std::endl;
		return (false);
	}
	else if (recept == -1)	// empty
	{
		std::cout << strerror(errno) << std::endl;
		std::cout << RED << "Not HTTP/1.1 request, connexion closed!" << RST << std::endl;
		return (false);
	}

	// SET HEADER IN VECTOR BY LINE
	for (std::string::iterator it = tmp.begin(); it != tmp.end() && *it != 0; it++)
	{
		for (; *it != '\n' && it != tmp.end() && *it != 0; it++)
			line.push_back(*it);
		header.push_back(line);
		line.clear();
	}
	if (header.data() == 0)
	{
		head.Methode = "CLOSE";
		return (false);
	}

	std::vector<std::string>::iterator it = header.begin();
	if (it->find(" HTTP/1.1"))
	{
		if (!it->compare(0, 5, "GET /"))
			head.Methode = "GET";
		else if (!it->compare(0, 6, "POST /"))
			head.Methode = "POST";
		else if (!it->compare(0, 8, "DELETE /"))
			head.Methode = "DELETE";
		else
		{
			head.Methode = "BAD";
			return (true);
		}
		s_str = it->find_first_of("/");
		e_str = it->find_first_of(" \r\v\t\f\n", s_str);
		if (s_str == e_str || s_str == it->npos || e_str == it->npos)
		{
			std::cout << RED << "unvailable header!" << RST << std::endl;
			return (false);
		}
		head.Dir = it->substr(s_str, it->find_first_of(" \v\t\f\n\r\?", s_str) - s_str);
		if (it->find_first_of("?") != it->npos && !head.Methode.compare("GET"))
			ft::split_to_mapss(head.other, it->substr(it->find_first_of("?") + 1, it->find_last_of(" ") - it->find_last_of("?") - 1), '&');
	}
	else
	{
		std::cout << RED << "Header not supported!" << RST << std::endl;
		head.Methode = "NOT_SUPPORTED";
		return (false);
	}

	for (++it; it != header.end() && *it->data() != '\r' && *it->data() != 0; it++)
	{
		if (!it->find("Host:"))
			head.Host = it->substr(it->find_first_of(' ') + 1, it->find_last_not_of("\r\n") - it->find_first_of(" "));
		else if (!it->find("Accept:")) 
			ft::split_to_vectors(head.Accept, it->data());
		else if (!it->find("User-Agent:"))
			head.User_Agent = it->substr(it->find_last_of(' ') + 1, it->find_last_of('\r') - it->find_last_of(" ") - 1);
		else if (!it->find("Cookie:"))
		{
			if (!ft::split_to_mapss(head.Cookie, it->data()))
				return (false);
		}
		else if (!it->find("Keep-Alive:"))
			head.Connexion = std::stoul(it->substr(it->find_last_of(' ') + 1));
		else if (!it->find("Connection:"))
			head.Connexion = it->substr(it->find_last_of(' ') + 1, it->find_last_of('\r') - it->find_last_of(" ") - 1);
		else if (it->find("Content-Length:") == 0)
			head.Content_Length = std::strtol(it->substr(it->find(' ') + 1).c_str(), NULL, 10);
		else if (!it->find("Content_Type:")) 
			ft::split_to_vectors(head.Content_Type, it->data());
		else if (!it->find("Content_Encoding:")) 
			ft::split_to_vectors(head.Content_Encoding, it->data());
		else if (!it->find("Transfer_Encoding:")) 
			ft::split_to_vectors(head.Transfer_Encoding, it->data());
		else
			ft::split_to_maposs(head.other, it->data());
	}
	head.split_dir();
	#ifdef DEBUG
		head.print_all();
	#endif
	return (true);
}