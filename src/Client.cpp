/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/03/01 18:00:12 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(const config &config) : 	_ref_conf(config)
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
		//tmp.erase(tmp.length() - 2);	// on fait des bêtises
		//tmp.append("Cookie: stephane=guy_roux; chapeau=rouge; alex=se_debrouille\n");	// comme dit...
		memset(buffer, 0, 4096);
	}
	i = 0;
	for (std::string::iterator it = tmp.begin(); it != tmp.end() && *it != 0; it++)
	{
			for (; *it != '\n' && it != tmp.end() && *it != 0; it++)
				line.push_back(*it);
			header.push_back(line);
			std::cout << "line: " << line << std::endl;
			line.clear();
	}
	std::vector<std::string>::iterator it = header.begin();
	if (it->find("HTTP/1.1"))
	{
		if (!it->compare(0, 5, "GET /"))
			this->_header.method = "GET";
		else if (!it->compare(0, 6, "POST /"))
			this->_header.method = "POST";
		else if (!it->compare(0, 8, "DELETE /"))
			this->_header.method = "DELETE";
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

	if (!this->_header.method.compare("POST"))
	{
		std::cout << "POST" << std::endl;
		std::size_t pos = _header.directory.find('?');
		if(pos != std::string::npos)
		{
			pos += 1;
			std::string temp = _header.directory.substr(pos);
			this->_header.data = ft::str_to_map(temp, "&");
			std::map<std::string, std::string>::iterator	first = this->_header.data.begin();
			for (; first != this->_header.data.end(); ++first)
				std::cout << "key: " << first->first << ", value: " << first->second << std::endl;
		}
		std::cout << "POST_VAR OK" << std::endl;
	}

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
			it->erase(0, s_str);
			std::cout << "Accept OK" << std::endl;
			this->_header.accept = ft::str_to_vect(*it, ", ");
			std::vector<std::string>::iterator	first = this->_header.accept.begin();
			for (; first != this->_header.accept.end(); ++first)
				std::cout << "it: " << *first << std::endl;
			// USE TO FABIO'S SPLIT (<MIME_type>/<MIME_subtype>, ....)
		}
		else if (!it->find("User-Agent:"))
		{
			std::cout << "User-Agent" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			this->_header.user_agent = it->erase(0, s_str);
			std::cout << "User-Agent: " << this->_header.user_agent << std::endl;
			std::cout << "User-Agent OK" << std::endl;
		}
		else if (!it->find("Connection:"))
		{
			std::cout << "Connection" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			this->_header.connection = it->erase(0, s_str);
			std::cout << "Connection: " << this->_header.connection << std::endl;
			std::cout << "Connection OK" << std::endl;
		}
		else if (!it->find("Host:"))
		{
			std::cout << "Host" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			this->_header.host = it->erase(0, s_str);
			std::cout << "Host: " << this->_header.host << std::endl;
			std::cout << "Host OK" << std::endl;
		}
		else if (!it->find("Cookie:"))
		{
			std::cout << "COOKIE" << std::endl;
			s_str = it->find_first_of(' ') + 1;
			this->_header.cookie = ft::str_to_map(it->erase(0, s_str), "; ");
			std::cout << "Cookie:" << std::endl;
			std::map<std::string, std::string>::iterator	first = this->_header.cookie.begin();
			for (; first != this->_header.cookie.end(); ++first)
				std::cout << "key: " << first->first << ", value: " << first->second << std::endl;
			std::cout << "COOKIE OK" << std::endl;
			// USE TO FABIO'S SPLIT (name=value; ....)
		}
		else if (it->find("Content-Length:") == 0)
			this->_header.length = std::strtol(it->substr(it->find(' ') + 1).c_str(), NULL, 10);
		else if (it->find("Accept-Language:") == 0)
		{
			s_str = it->find_first_of(' ') + 1;
			it->erase(0, s_str);
			this->_header.language = ft::str_to_vect(*it, ", ");
			std::vector<std::string>::iterator	first = this->_header.language.begin();
			for (; first != this->_header.language.end(); ++first)
				std::cout << "it: " << *first << std::endl;
		}
		else if (it->find("Accept-Encoding:") == 0)
		{
			s_str = it->find_first_of(' ') + 1;
			it->erase(0, s_str);
			this->_header.encoding = ft::str_to_vect(*it, ", ");
			std::vector<std::string>::iterator	first = this->_header.encoding.begin();
			for (; first != this->_header.encoding.end(); ++first)
				std::cout << "it: " << *first << std::endl;
		}
	}
/*
	else if (!this->_header.method.compare("POST"))
		{
			std::cout << "TYPE POST" << std::endl;

			else
				this->_header.length = 0;
			std::cout << "POST OK" << std::endl;
		}
*/
	
		
	if (!this->_header.method.compare("POST") && this->_header.length > 0)
	{
		// NEED PARSE ELEMENT IN CONTENT AND PUT IN data, NEED TO CHANGE TYPE OF data TOO!
		this->_header.content_type.push_back((++it)->data());
	}
}
