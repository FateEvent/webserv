/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:54:10 by stissera          #+#    #+#             */
/*   Updated: 2023/04/12 12:38:04 by stissera         ###   ########.fr       */
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
			this->_chunked = true;
			break;
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
		(*static_cast<std::ifstream*>(this->_data.file)).close();
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
		// FABIO DOING THAT.
		std::cout << YELLOW << "Boundary POST" << RST << std::endl;
		char	*tmpfile = strdup(".tmpXXXXXX");
		mkstemp(tmpfile);
		std::fstream	temporary(tmpfile, std::ios::out | std::ios::in | std::ios::binary);

		std::string	file_buf;
		char		buff[1024];

		std::cout << "POST METHOD" << std::endl;
		int recept = 1;
		while (recept > 0)
		{
			memset(buff, 0, 1024);
			recept = recv(this->_sock_fd, &buff, 1023, 0);
			for (int i = 0; i < recept; i++)
			{
				temporary << buff[i];
				file_buf += buff[i];
				if (buff[i] == '\n')
				{
					if ((!file_buf.find("--" + _header.Boundary + "\r\n"))
						|| (!file_buf.find("--" + _header.Boundary + "--\r\n")))
						file_buf.clear();
					if (!file_buf.find("Content-Disposition"))
					{
						ft::split_to_vectors(_header.Content_Disposition, file_buf, ';');
						file_buf.clear();
					}
					if (!file_buf.find("Content-Type"))
						file_buf.clear();
					if (!file_buf.find("\r\n"))
						file_buf.clear();
					if (file_buf.find("\r\n--" + _header.Boundary) != file_buf.npos)
					{
						file_buf.erase(file_buf.find("\r\n--" + _header.Boundary));
						ft::find_val(_header.Content_Disposition, _header.entry_name, "name");
						ft::find_val(_header.Content_Disposition, _header.filename, "filename");
						if (_header.entry_name != "" && _header.filename == "")
							_header.other.insert(std::make_pair(_header.entry_name, file_buf));
						else if (_header.filename != "")
						{
							std::fstream	filename;

							filename.open(_header.filename, std::ios::out);
							filename << file_buf;
							filename.close();
						}
						file_buf.clear();
					}
				}
			}
		}
		remove(tmpfile);
		for (std::map<std::string, std::string>::iterator it = _header.other.begin(); it != _header.other.end(); ++it)
			std::cout << "clé: " << it->first << ", value: " << it->second << std::endl; 
		std::cout << std::endl;

		std::cout << "Send: " << send(this->_sock_fd, "HTTP/1.1 405 Method Not Allowed\r\n\r\n\0", 36, MSG_OOB) << std::endl; // , NULL, 0);
		//close(this->_sock_fd);
		this->clear_header();
		this->_index.clear();
		this->_root.clear();
		this->_working = true;
		return (true);
	}
	else if (!this->is_chunk())
	{
		// TO THAT AND THINK ABOUT CHUNKED CGI
		std::cout << YELLOW << "Chunked POST" << RST << std::endl;
	}
	else
	{
		// SAME AS POST METHODE! CAN DO ONE FUNCTION USUALY
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

