/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/02/28 16:23:46 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(const config &config) : _ref_conf(config)
{
	_socklen = sizeof(this->_addr);
	FD_ZERO(&this->_readfd);
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	if (this->_sock_fd == -1)
		throw ("Socket error in constructor Client!");
	_make_struct();
}

Client::~Client()
{}

int	Client::get_sockfd() const
{
	return (this->_sock_fd);
}

void	Client::test_client()
{
	sendto(this->_sock_fd, "SALUT", 6, MSG_OOB, NULL, 0);
}

void	Client::_make_struct()
{
	char 						buffer[4096];
	std::string					tmp;
	std::string					line;
	std::vector<std::string>	header;
	ssize_t						i = -1;
	size_t 						s_str;
	size_t 						e_str;

	while (i == -1 || i == 4095)
	{
		i = recv(this->_sock_fd, &buffer, 4095, 0);
		tmp.append(buffer);
		memset(buffer, 0, 4096);
	}
	i = 0;
	for (std::string::iterator it = tmp.begin(); it != tmp.end() && *it != 0; it++)
	{
			for (; *it != '\n' && it != tmp.end() && *it != 0; it++)
				line.push_back(*it);
			header.push_back(line);
			line.clear();
	}
	std::vector<std::string>::iterator it = header.begin();
	if (it->find("HTTP/1.1"))
	{
		if (!it->compare(0, 5, "GET /"))
			this->_header.methode = "GET";
		else if (!it->compare(0, 6, "POST /"))
			this->_header.methode = "POST";
		else if (!it->compare(0, 8, "DELETE /"))
			this->_header.methode = "DELETE";
		else
			throw ("Only POST, GET and DELETE are available!"); // give maybe a error to the client!
		s_str = it->find_first_of('/');
		e_str = it->find_last_of(' ');
		if (s_str == e_str)
			throw ("unavailable header!"); // give maybe a error to the client!
		this->_header.directory.append(*it, s_str, e_str - s_str);
	}
	else
	 throw ("505 HTTP Version Not Supported");

	for (++it; it != header.end() && *it->data() != '\r' && *it->data() != 0; it++)
	{
		std::cout << *it << std::endl;
// NO NEED BECAUSE ALREADY IN RIGHT SOCKET
/*
		if (!it->find("Host:"))
		{
			s_str = it->find_first_of(' ') + 1;
			if (!it->find(":"))
			{
				e_str = it->find_first_of(':');
				
			}
		}
*/
		if (!it->find("Accept:"))
		{
			std::cout << "Accept" << std::endl;	
			s_str = it->find_first_of(' ') + 1;
			std::cout << "Accept OK" << std::endl;	
			// USE TO FABIO'S SPLIT (<MIME_type>/<MIME_subtype>, ....)
		}
		else if (!it->find("User-Agent:"))
		{
			std::cout << "User-Agent" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			std::cout << "User-Agent OK" << std::endl;
			// USE TO FABIO'S SPLIT (name=value; ....)
		}
		else if (!it->find("Host:"))
		{
			std::cout << "Host" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			std::cout << "Host OK" << std::endl;
			// USE TO FABIO'S SPLIT (name=value; ....)
		}
		else if (!it->find("Cookie:"))
		{
			std::cout << "COOKIE" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			std::cout << "COOKIE OK" << std::endl;
			// USE TO FABIO'S SPLIT (name=value; ....)
		}
		else if (it->find("Content-Length:") == 0)
			this->_header.length = std::strtol(it->substr(it->find(' ') + 1).c_str(), NULL, 10);
	}	
/*
	else if (!this->_header.methode.compare("POST"))
		{
			std::cout << "TYPE POST" << std::endl;

			else
				this->_header.length = 0;
			std::cout << "POST OK" << std::endl;
		}
*/
	if (!this->_header.methode.compare("POST") && this->_header.length > 0)
	{
		// NEED PARSE ELEMENT IN CONTENT AND PUT IN data, NEED TO CHANGE TYPE OF data TOO!
		this->_header.content_type.push_back((++it)->data());
	}
}
