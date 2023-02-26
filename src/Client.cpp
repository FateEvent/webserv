/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/02/26 18:41:38 by stissera         ###   ########.fr       */
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
	for (std::string::iterator it = tmp.begin(); it != tmp.end() - 1; it++)
	{
		// put a if about it 
			for (; *it != '\n' && it != tmp.end(); it++) /// PROBLEM HERE
				line.push_back(*it);
			header.push_back(line);
			line.clear();
		// end of if, try like to see if segfault..
	}
for (std::vector<std::string>::iterator it = header.begin(); it != header.end(); it++)
{
	std::cout << *it << std::endl;
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
			throw ("unvailable header!"); // give maybe a error to the client!
		this->_header.directory.append(*it, s_str, e_str - s_str);
	}
	else
	 throw ("505 HTTP Version Not Supported");
	for (++it; it != header.end() && *it->data() != '\n'; it++)
	{
		// NO NEED BECAUSE ALREADY IN RIGHT SOCKET
/* 		if (!it->find("Host:"))
		{
			s_str = it->find_first_of(' ') + 1;
			if (!it->find(":"))
			{
				e_str = it->find_first_of(':');
				
			}
		} */
		
		if (!it->find("Accept:"))
		{
			s_str = it->find_first_of(' ') + 1;
			
			// USE TO FABIO'S SPLIT (<MIME_type>/<MIME_subtype>, ....)
		}
		else if (!it->find("Cookie:"))
		{
			s_str = it->find_first_of(' ') + 1;
			// USE TO FABIO'S SPLIT (name=value; ....)
		}
		else if (!it->find("Cookie:"))
		{
			// PUT COOKIE DIRECTLY ON INSTANCE -> this->_ref_conf.cookie/map<id, struct cookie>
		}
		else if (!this->_header.methode.compare("POST"))
		{
			if (!it->find("Content-Length:"))
			{
				this->_header.lenght = std::stoul(it[it->find(' ') + 1]);
			}
			else
				this->_header.lenght = 0;
		}
	}
std::cout << "END" << std::endl;
	if (!this->_header.methode.compare("POST") && this->_header.lenght > 0)
	{
		// NEED PARSE ELEMENT IN CONTENT AND PUT IN data, NEED TO CHANGE TYPE OF data TOO!
		this->_header.content_type.push_back((++it)->data());
	}
		std::cout << "DATA ARE ----->  " + this->_header.data << std::endl;
}
