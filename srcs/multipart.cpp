/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipart.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:53:24 by stissera          #+#    #+#             */
/*   Updated: 2023/04/18 00:24:45 by stissera         ###   ########.fr       */
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
			std::cout << "Boundary found at position:" << std::endl;
			for (std::vector<int>::iterator it = this->_data._in.bound_seek.begin(); it != this->_data._in.bound_seek.end(); it++)
				std::cout << GREEN << *it << RST << std::endl;
			make_error(201);
			return (true);
		}
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
	return (false);
}

bool	Client::process_multipart()
{
	// first step check if size of vector is minimum 2
	// move cursor on start of boundary with first vector
	this->_data._in.temporary->seekg(this->_data._in.bound_seek.front(), this->_data._in.temporary->beg);
	this->_data._in.bound_seek.erase(this->_data._in.bound_seek.begin());
	std::string header;
	
	// getline first line to spring boundary line
	std::getline(*this->_data._in.temporary, header, '\n');
	std::cout << GREEN << "Extracted line is: " << RST << header << std::endl;
	std::cout << RED << "Cursor in position: " << this->_data._in.temporary->tellg() << RST << std::endl;

	// take header until \n is first element of line, means we have taked last line of header so we start on the first octet of data
	while (header.find("\r") != 0)
	{
		std::getline(*this->_data._in.temporary, header, '\n');
		std::cout << GREEN << "Extracted line is: " << RST << header << std::endl;
		std::cout << RED << "Cursor in position: " << this->_data._in.temporary->tellg() << RST << std::endl;
	}
	// need prepare type of data
	// if data is not file so variable, do that in one time
	// otherwise may call function to start writing data in file
	// end of first
	// start second step - repetitive step for data until seek is number of vector first.
	std::cout << PURPLE << "START OF DATA! " << this->_data._in.temporary->tellg() << " to " << this->_data._in.bound_seek.front() <<RST << std::endl;
	char bb[this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg()];
	memset(bb, 0, this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg());

	this->_data._in.temporary->read(bb, this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg() - 2);
	std::cout << RED << this->_data._in.temporary->gcount() << " char taked!" << RST << std::endl;
	std::cout << RED << this->_data._in.temporary->tellg() << " + 2 should the position where start next boundary." << RST << std::endl;
	std::cout << RED << bb << RST << std::endl;
	this->_data._in.temporary->read(bb, this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg() - 2);

	// end of first boudary and start of next boundary / if dont exist we should stay in eof.
	return (true);


	// Take first and second vector<int> in this->_data._in.bound_seek.
	// find fist of "\r\n\r\n" means end of instruction for boundary
	// then ex for a file take until second vector position - 1 or maybe -2 (\r\n??)
	// try to do buffer/buffer (no blocking state)
	// remove the first in vector.
	// do until second is end of vector. First can't be last vector!!!

/* 	for (int i = 0; i < this->_data._in.receipt; i++)
	{
		std::cout << PURPLE << buff[i] << RST;
		this->_data._in.file_buf += buff[i];
		if (buff[i] == '\n')
		{
			std::cout << GREEN << "\\n" << RST;
			if ((!this->_data._in.file_buf.find("--" + _header.Boundary + "\r\n"))
					|| (!this->_data._in.file_buf.find("--" + _header.Boundary + "--\r\n")))
				this->_data._in.file_buf.clear();
			if (!this->_data._in.file_buf.find("Content-Disposition"))
			{
				ft::split_to_vectors(_header.Content_Disposition, this->_data._in.file_buf, ";");
					this->_data._in.file_buf.clear();
			}
			if (!this->_data._in.file_buf.find("Content-Type"))
				this->_data._in.file_buf.clear();
			if (!this->_data._in.file_buf.find("\r\n"))
					this->_data._in.file_buf.clear();
			if (this->_data._in.file_buf.find("\r\n--" + _header.Boundary) != this->_data._in.file_buf.npos)
			{
				std::cout << YELLOW << "WRITE" << RST;
				this->_data._in.file_buf.erase(this->_data._in.file_buf.find("\r\n--" + _header.Boundary));
				ft::find_val(_header.Content_Disposition, _header.entry_name, "name");
				ft::find_val(_header.Content_Disposition, _header.filename, "filename");
				if (_header.entry_name != "" && _header.filename == "")
					_header.other.insert(std::make_pair(_header.entry_name, this->_data._in.file_buf));
				else if (_header.filename != "")
				{
					std::cout << RED << "WRITE FILE" << RST << std::endl;
					if (this->_data._in.filename == NULL)
						this->_data._in.filename = new std::fstream(_header.filename, std::ios::out);
					this->_data._in.filename->write(this->_data._in.file_buf.data(), this->_data._in.file_buf.length());
					this->_data._in.filename->close();
					this->_data._in.filename = NULL;
				}
				this->_data._in.file_buf.clear();
			}
		}
	} */
}