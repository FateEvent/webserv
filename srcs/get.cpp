/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:50:29 by stissera          #+#    #+#             */
/*   Updated: 2023/04/19 22:31:30 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::execute_get()
{
	std::cout << "GET METHOD" << std::endl;
	if (!(this->_allow & 1) && this->_allow != 0)
	{
		std::cout << RED << "False methode GET?" << RST << std::endl;
		this->make_error(405);
		return (false);
	}
	if (!this->_index.empty() && ((!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end()) ||
		(!this->_ref_conf.cgi.empty() && _ref_conf.cgi.find(_index.substr(_index.find_last_of("."))) != _ref_conf.cgi.end())))
	{
		std::cout << YELLOW << "Execute by a CGI" << RST << std::endl;
		static_cast<std::fstream*>(this->_data.file)->close();
		this->_data.file = 0;
		this->_data.file = new std::stringstream();
		if (!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end())
			this->launch_cgi(this->_cgi_call.find(_index.substr(_index.find_last_of(".")))->second);
		else
			this->launch_cgi(this->_ref_conf.cgi.find(_index.substr(_index.find_last_of(".")))->second);
		// WARNING IF CGI VAR AS ONLY A KEY THAT MEANS THE KEY IS A PATH! TODO THAT!
	}
	else
	{
		std::string	header;
		this->_data.header = ft::make_header(200);
		this->_data.header.append("Content-Length: " + std::to_string(this->_data.data_size) + "\r\n");
		this->_data.header.append(ft::make_content_type(this->_index.substr(this->_index.find_last_of(".") + 1)));
		int check = 0;
		check = send(this->_sock_fd, this->_data.header.c_str(), this->_data.header.length(), 0);
		if (check == -1)
			this->make_error(500);
		if (check == 0)
			this->make_error(501);
		//std::cout << BLUE << "\"" << this->_data.header << "\"" << RST << std::endl;
		this->_sedding = true;
	}
	return (false);
}