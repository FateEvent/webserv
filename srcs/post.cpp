/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:54:10 by stissera          #+#    #+#             */
/*   Updated: 2023/05/04 16:04:13 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::execute_post()
{
	if (!(this->_allow >> 1 & 1) && this->_allow != 0)
	{
		if (shutdown(this->_sock_fd, SHUT_RD) == -1)
			std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		this->make_error(405);
		return (false);
	}
	for (std::vector<std::string>::iterator it = this->_header.Transfer_Encoding.begin(); it != this->_header.Transfer_Encoding.end(); it++)
		if (!it->compare("chunked"))
		{
			char	tmpfile[12];

			strcpy(tmpfile, "chunkXXXXXX");
			mkstemp(tmpfile);
			this->_data._in.temporary = new std::fstream(tmpfile, std::ios::out | std::ios::in | std::ios::binary | std::ios::app);
			this->_data._in.receipt = 1;
			this->_data._in.file_buf.clear();
			this->_data._in.size = 0;
			this->_chunked = true;
			return (false);
		}
	if (this->_header.Content_Length == 0 && !this->is_chunk()) // Maybe change Content_Length to ssize_t and initialize it to -1. Compare with -1 instead of 0  
	{
		if (shutdown(this->_sock_fd, SHUT_RD) == -1)
			std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		this->make_error(411);
		return (false);
	}
	else if (this->_header.Content_Length > this->_max_body && this->_max_body > 0)
	{
		if (shutdown(this->_sock_fd, SHUT_RD) == -1)
			std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		this->make_error(413);
		return (false);
	}
	if ((!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end()) ||
		(!this->_ref_conf.cgi.empty() && _ref_conf.cgi.find(_index.substr(_index.find_last_of("."))) != _ref_conf.cgi.end()))
	{
		std::cout << YELLOW << "Execute POST by a CGI" << RST << std::endl;
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
	else if (!this->_header.Boundary.empty())
	{
		std::cout << YELLOW << "Multipart POST" << RST << std::endl;
		strcpy(this->_data._in.tmpfile, "/tmp/tmp.XXXXXX");
		if (mkstemp(this->_data._in.tmpfile) == -1)
		{
			this->make_error(500);
			return (false);
		}
		this->_data._in.temporary = new std::fstream(this->_data._in.tmpfile, std::ios::out | std::ios::in | std::ios::binary);
		this->_data._in.file_buf.clear();
		this->_data._in.filename = NULL;
		this->_data._in.pos_seek = 0;
		this->_data._in.size = 0;
		this->_data._in.in_bound = 0;
		this->_data._in.receipt = 1;
		this->_multipart = true;
	}
	else if (this->is_chunk())
	{
		// TO THAT AND THINK ABOUT CHUNKED CGI
		std::cout << YELLOW << "Chunked POST" << RST << std::endl;
	}
	else
	{
		if (shutdown(this->_sock_fd, SHUT_RD) == -1)
			std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		send_success_status();
		//std::cout << BLUE << "\"" << this->_data.header << "\"" << RST << std::endl;
	}
	return (false);
}
