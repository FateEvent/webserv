/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:54:10 by stissera          #+#    #+#             */
/*   Updated: 2023/04/13 19:20:50 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::execute_post()
{
	std::cout << "POST METHOD" << std::endl;
	if (!(this->_allow >> 1 & 1) && this->_allow != 0)
	{
		std::cout << RED << "False methode POST?" << RST << std::endl;
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
		std::cout << RED << "False content-length POST?" << RST << std::endl;
		this->make_error(411);
		return (false);
	}
	else if (this->_header.Content_Length > this->_max_body && this->_max_body > 0)
	{
		std::cout << RED << "False content-length 2 POST?" << RST << std::endl;
		this->make_error(413);
		return (false);
	}
	if ((!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end()) ||
		(!this->_ref_conf.cgi.empty() && _ref_conf.cgi.find(_index.substr(_index.find_last_of("."))) != _ref_conf.cgi.end()))
	{
		std::cout << YELLOW << "Execute POST by a CGI" << RST << std::endl;
		static_cast<std::fstream*>(this->_data.file)->close();
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
		char	tmpfile[11];

		//this->_data._in.tmpfile = TMP_FILE_NAMED;
		strcpy(tmpfile, ".tmpXXXXXX");
		if (mkstemp(tmpfile) == -1)
		{
			this->make_error(500);
			return (false);
		}
		this->_data._in.temporary = new std::fstream(tmpfile, std::ios::out | std::ios::in | std::ios::binary | std::ios::app);
		this->_data._in.receipt = 1;
		this->_data._in.file_buf.clear();
		this->_data._in.filename = NULL;
		this->_data._in.size = 0;
		this->_multipart = true;
		return (false);
	}
	else if (!this->is_chunk())
	{

		// TO THAT AND THINK ABOUT CHUNKED CGI
		std::cout << YELLOW << "Chunked POST" << RST << std::endl;
	}
	else
	{
		// SAME AS POST METHODE! CAN DO ONE FUNCTION USUALY
		send_success_status();
		//std::cout << BLUE << "\"" << this->_data.header << "\"" << RST << std::endl;
	}
	return (false);
}
