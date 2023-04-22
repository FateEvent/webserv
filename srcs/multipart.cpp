/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipart.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:53:24 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 01:53:28 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::multipart()
{
	if (this->_data._in.pos_seek == -1)
		return (true);

	if (this->_header.Content_Length != this->_data._in.size && this->_data._in.receipt != 0)
	{
		int size;
		socklen_t len = sizeof(size);
		if (getsockopt(this->_sock_fd, SOL_SOCKET, SO_RCVBUF, &size, &len) == -1)
		{
			std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
			return (false);
		}
		char	buff[size];
		memset(buff, 0, size);
		this->_data._in.receipt = recv(this->_sock_fd, &buff, size, 0);
		if (this->_data._in.receipt > 0)
		{
			this->_data._in.temporary->write(buff, this->_data._in.receipt);
			this->_data._in.size += this->_data._in.receipt; 
		}
	}
	
	if (this->_header.Content_Length == this->_data._in.size && this->_data._in.pos_seek != -1)
	{
		if (this->_data._in.temporary->eof())
		{
			this->_data._in.temporary->clear();
			this->_data._in.pos_seek = -1;
/*  			std::cout << "Boundary found at position:" << std::endl;
			for (std::vector<size_t>::iterator it = this->_data._in.bound_seek.begin(); it != this->_data._in.bound_seek.end(); it++)
				std::cout << GREEN << *it << RST << std::endl; */
			//make_error(201); // for test
			return (true);
		}
		else
		{
		char	buff[BUFFER_SIZE_MB];
		memset(&buff, 0, BUFFER_SIZE_MB);
		size_t	pos_str = 0;

		this->_data._in.temporary->seekg(this->_data._in.pos_seek, this->_data._in.temporary->beg);
		this->_data._in.temporary->read(buff, BUFFER_SIZE_MB);
		std::string search_b(buff, this->_data._in.temporary->gcount());
		
		while ((pos_str = search_b.find("--" + this->_header.Boundary, pos_str)) != search_b.npos)
		{
			this->_data._in.bound_seek.push_back(this->_data._in.pos_seek + pos_str);
			pos_str++;
		}
		if (this->_data._in.temporary->gcount() - pos_str == this->_data._in.temporary->gcount() - this->_header.Boundary.length())
			this->_data._in.pos_seek = this->_data._in.pos_seek + this->_data._in.temporary->gcount() - this->_header.Boundary.length() - 2;
		else
			this->_data._in.pos_seek = this->_data._in.pos_seek + this->_data._in.temporary->gcount();
		}
	}
	return (false);
}

bool	Client::process_multipart()
{
	if (static_cast<long unsigned int>(this->_data._in.temporary->tellg()) == this->_data._in.bound_seek.back())
		return (true);
	size_t buf_size = 0;

	// start second step - repetitive step for data until seek is number of vector first.
	if (static_cast<long unsigned int>(this->_data._in.temporary->tellg()) < this->_data._in.bound_seek.front())
	{
		if (BUFFER_SIZE_MB < this->_data._in.bound_seek[0] - this->_data._in.temporary->tellg())
			buf_size = BUFFER_SIZE_MB;
		else
			buf_size = this->_data._in.bound_seek[0] - this->_data._in.temporary->tellg() - 2;
		char	buff[buf_size];
		memset(&buff, 0, buf_size);

		this->_data._in.temporary->read(buff, buf_size);
		this->_data._in.filename->write(buff, buf_size);
		if (buf_size < BUFFER_SIZE_MB)
		{
			this->_data._in.temporary->seekg(this->_data._in.bound_seek.front(), this->_data._in.temporary->beg);
			this->_data._in.filename->close();
			delete this->_data._in.filename;
			this->_data._in.filename =  NULL;
		}
		//this->_data._in.in_bound = this->_data._in.temporary->tellg();
		return (false);
	}

	this->_data._in.temporary->seekg(this->_data._in.bound_seek.front(), this->_data._in.temporary->beg);
	this->_data._in.bound_seek.erase(this->_data._in.bound_seek.begin());

	// getline first line to spring boundary line
	std::getline(*this->_data._in.temporary, this->_data._in.file_buf, '\n');

	// take header until \n is first element of line, means we have taked last line of header so we start on the first octet of data
	while (this->_data._in.file_buf.find("\r") != 0)
	{
		std::getline(*this->_data._in.temporary, this->_data._in.file_buf, '\n');
		if (this->_data._in.file_buf.find("Content-Disposition") == 0)
		{
			if (this->_data._in.file_buf.find(" filename=\"") != this->_data._in.file_buf.npos)
			{
				std::string file = this->_data._in.file_buf.substr(this->_data._in.file_buf.find(" filename=\"") + 11,
																	this->_data._in.file_buf.find_last_of("\"") - (this->_data._in.file_buf.find(" filename=\"") + 11));
				file = this->_download + "/" + file;
				this->_data._in.filename = new std::fstream;
				this->_data._in.filename->open(file, std::ios::out | std::ios::binary | std::ios::trunc);
			}
			else
			{
				this->_data._in.temporary->seekg(this->_data._in.bound_seek.front(), this->_data._in.temporary->beg);
				this->_data._in.bound_seek.erase(this->_data._in.bound_seek.begin());
			}
		}
	}
	return (false);
}