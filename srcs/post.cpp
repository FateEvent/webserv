/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:54:10 by stissera          #+#    #+#             */
/*   Updated: 2023/04/06 22:31:21 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::execute_post()
{
	std::cout << "POST METHOD" << std::endl;
	if (!(this->_allow >> 1 & 1) && this->_allow != 0)
	{
		this->make_error(405);
		return (false);
	}
	if (this->_header.Content_Length == 0)
	{
		this->make_error(411);
		return (false);
	}
	else if (this->_header.Content_Length > this->_max_body && this->_max_body > 0)
	{
		this->make_error(413);
		return (false);
	}
	
	for (std::vector<std::string>::iterator it = this->_header.Transfer_Encoding.begin(); it != this->_header.Transfer_Encoding.end(); it++)
		if (!it->compare("chunked"))
		{
			this->_chunked = true;
			break;
		}

	if ((!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end()) ||
		(!this->_ref_conf.cgi.empty() && _ref_conf.cgi.find(_index.substr(_index.find_last_of("."))) != _ref_conf.cgi.end()))
	{
		std::cout << YELLOW << "Execute POST by a CGI" << RST << std::endl;
		(*static_cast<std::ifstream*>(this->_data.file)).close();
		this->_data.file = 0;
		this->_data.file = new std::stringstream();
		if (!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end())
			this->launch_cgi(this->_cgi_call.find(_index.substr(_index.find_last_of(".")))->second);
		else
			this->launch_cgi(this->_ref_conf.cgi.find(_index.substr(_index.find_last_of(".")))->second);
		// WARNING IF CGI VAR AS ONLY A KEY THAT MEANS THE KEY IS A PATH! TODO THAT!
	}
/* 	char buff[2];
	memset(buff, 0, 2);
	int recept = 0;
	recept = recv(this->_sock_fd, &buff, 1, 0);
	while (recept > 0)
	{
		std::cout << buff;
		recept = recv(this->_sock_fd, &buff, 1, 0);
	}
	std::cout << std::endl;
	std::cout << "Send: " << send(this->_sock_fd, "HTTP/1.1 405 Method Not Allowed\r\n\r\n\0", 36, MSG_OOB) << std::endl; // , NULL, 0);
	//close(this->_sock_fd);
	this->clear_header();
	this->_index.clear();
	this->_root.clear();
	this->_working = true;
	return (true); */
	return (false);
}

