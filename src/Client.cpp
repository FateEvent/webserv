/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/03/13 14:23:31 by faventur         ###   ########.fr       */
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
		memset(buffer, 0, 4096);
	}
	i = 0;
	for (std::string::iterator it = tmp.begin(); it != tmp.end() && *it != 0; it++)
	{
			for (; *it != '\n' && it != tmp.end() && *it != 0; it++)
				line.push_back(*it);
			header.push_back(line);
			#ifdef __DEBUG
				std::cout << "line: " << line << std::endl;
			#endif
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
		#ifdef __DEBUG
			std::cout << "Directory before:" << _header.directory << std::endl;
		#endif
	}
	else
	 throw ("505 HTTP Version Not Supported");

	//isoler les variables qui sont dans le directory
	if (!this->_header.method.compare("GET"))
	{
		std::size_t pos = _header.directory.find('?');
		if(pos != std::string::npos)
		{
			pos += 1;
			std::string temp = _header.directory.substr(pos);
			this->_header.data = ft::str_to_map(temp, "&");
			_header.directory.erase(pos - 1);
			#ifdef __DEBUG
				std::map<std::string, std::string>::iterator	first = this->_header.data.begin();
				std::map<std::string, std::string>::iterator	last = this->_header.data.end();
				for (; first != last; ++first)
					std::cout << "data key: " << first->first << ", data value: " << first->second << std::endl;
			#endif
		}
		#ifdef __DEBUG
			std::cout << "Directory after split variables:" << _header.directory << std::endl;
			std::cout << "recup variables OK" << std::endl;
		#endif
	}
	
	//recuperer le file et l'extension du directory dans une paire
	if(this->_header.directory.find(".") != std::string::npos)
	{
		std::string				key;
		std::string				val;
		std::string::size_type pos = _header.directory.find(".");
		val = _header.directory.substr(pos + 1);
		_header.directory.erase(pos);	
		while (_header.directory[pos] != '/')
			pos--;
		key = _header.directory.substr(pos + 1);
		_header.directory.erase(pos + 1);	
		_header.file = std::make_pair(key, val);
		#ifdef __DEBUG
			std::cout << "file key: " << key << std::endl;
			std::cout << "file value: " << val << std::endl;
		#endif
	}
	#ifdef __DEBUG
		std::cout << "Directory after:" << _header.directory << std::endl;
	#endif
	
	for (++it; it != header.end() && *it->data() != '\r' && *it->data() != 0; it++)
	{
		if (!it->find("Accept:"))
		{
			this->_header.accept = ft::str_to_vect(ft::string_cutter(*it), ", ");
			#ifdef __DEBUG
				std::cout << "Accept" << std::endl;
				std::vector<std::string>::iterator	first = this->_header.accept.begin();
				std::vector<std::string>::iterator	last = this->_header.accept.end();
				for (; first != last; ++first)
					std::cout << "it: " << *first << std::endl;
			#endif
			// USE TO FABIO'S SPLIT (<MIME_type>/<MIME_subtype>, ....)
		}
		else if (!it->find("User-Agent:"))
		{
			this->_header.user_agent = ft::string_cutter(*it);
			#ifdef __DEBUG
				std::cout << "User-Agent: " << this->_header.user_agent << std::endl;
			#endif
		}
		else if (!it->find("Connection:"))
		{
			this->_header.connection = ft::string_cutter(*it);
			#ifdef __DEBUG
				std::cout << "Connection: " << this->_header.connection << std::endl;
			#endif
		}
		else if (!it->find("Host:"))
		{
			this->_header.host = ft::string_cutter(*it);
			#ifdef __DEBUG
				std::cout << "Host: " << this->_header.host << std::endl;
			#endif

			std::size_t pos = _header.host.find(':');
			if(pos != std::string::npos)
			{
				pos += 1;
				_header.listen = _header.host.substr(pos);
				_header.host.erase(pos - 1);
				ft::space_eraser(_header.listen);
			}
			#ifdef __DEBUG
				std::cout << "Host after split: " << _header.host << '$' << std::endl;
				std::cout << "listen: " << _header.listen << '$' << std::endl;
				std::cout << "Host & listen OK" << std::endl;
			#endif
		}
		else if (it->find("Accept-Language:") == 0)
		{
			this->_header.language = ft::str_to_vect(ft::string_cutter(*it), ", ");
			#ifdef __DEBUG
				std::vector<std::string>::iterator	first = this->_header.language.begin();
				std::vector<std::string>::iterator	last = this->_header.language.end();
				for (; first != last; ++first)
					std::cout << "it: " << *first << std::endl;
			#endif
		}
		else if (it->find("Accept-Encoding:") == 0)
		{
			this->_header.encoding = ft::str_to_vect(ft::string_cutter(*it), ", ");
			#ifdef __DEBUG
				std::vector<std::string>::iterator	first = this->_header.encoding.begin();
				std::vector<std::string>::iterator	last = this->_header.encoding.end();
				for (; first != last; ++first)
					std::cout << "it: " << *first << std::endl;
			#endif
		}
		else if (!it->find("Cookie:"))
		{
			this->_header.cookie = ft::str_to_map(ft::string_cutter(*it), "; ");
			#ifdef __DEBUG
				std::cout << "Cookies" << std::endl;
				std::map<std::string, std::string>::iterator	first = this->_header.cookie.begin();
				for (; first != this->_header.cookie.end(); ++first)
					std::cout << "key: " << first->first << ", value: " << first->second << std::endl;
			#endif
			// USE TO FABIO'S SPLIT (name=value; ....)
		}
		else if (it->find("Content-Length:") == 0)
			this->_header.length = std::strtol(ft::string_cutter(*it).c_str(), NULL, 10);
		else if (!it->find("Content-Type:"))
		{
			this->_header.user_agent = ft::string_cutter(*it);
			#ifdef __DEBUG
				std::cout << "Content-Type: " << this->_header.user_agent << std::endl;
			#endif
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
		int i = 0;
		// NEED PARSE ELEMENT IN CONTENT AND PUT IN data, NEED TO CHANGE TYPE OF data TOO!
		this->_header.content.push_back((++it)->data());
		std::vector<std::string>::iterator	first = _header.content.begin();
		std::vector<std::string>::iterator	last = _header.content.end();
		for (; first != last; ++first, ++i)
			std::cout << i << ": " << *it << std::endl;
	}
}

static std::string	test_dir(std::string root, std::string index)
{
	if (index != "")	// how is a string initialised?
	{
		std::string	tok, rest;
		std::make_pair(tok, rest) = ft::string_parser(index, " ");

		while (tok != "")
		{
			std::string	tmp = root + tok;
			if (access(tmp.data(), F_OK) == 0)
				return (root + tok);
			std::make_pair(tok, rest) = ft::string_parser(rest, " ");
		}
	}
	return ("www/404.html");
}

// need the location struct to compare with each location directory to find the right location root! 
std::string	Client::find_path(void)
{
	std::string	path = this->_header.host;
	std::string	res;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator first = this->_ref_conf.location.begin();
	std::map<std::string, std::map<std::string, std::string> >::const_iterator last = this->_ref_conf.location.end();
	
	if (path == this->_ref_conf.root)
		res = this->_ref_conf.root;
	else
	{
		for (; first != last; ++first)
		{
			if (first->first == path)
			{
				std::map<std::string, std::string>::const_iterator root = first->second.find("root");

				if (root != first->second.end())
				{
					res = root->second;
					break ;
				}
			}
		}
		if (res == "")
		{
			std::vector<std::string> directory = ft::str_to_vect(path, "/");
			std::vector<std::string>::const_iterator rfirst = directory.begin();
			std::vector<std::string>::const_iterator rlast = directory.end() - 1;

			for (; rfirst != rlast; --rfirst)
			{
				for (; first != last; ++first)
				{
					if (first->first.find(*rfirst))
					{
						res = first->first;
						
						while (rfirst != rlast)
						{
							res += "/" + *rfirst;
							++rfirst;
						}
						break ;
					}
				}
			}
		}
	}
	return (test_dir(res, _ref_conf.index));	// if I don't find anything I return "www/404.html"
}
