/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:50:29 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 02:15:59 by stissera         ###   ########.fr       */
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
		delete this->_data.file;
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
		send_success_status();
		//std::cout << BLUE << "\"" << this->_data.header << "\"" << RST << std::endl;
	}
	return (false);
}