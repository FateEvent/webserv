/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipart.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:53:24 by stissera          #+#    #+#             */
/*   Updated: 2023/04/18 17:25:26 by stissera         ###   ########.fr       */
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
			for (std::vector<size_t>::iterator it = this->_data._in.bound_seek.begin(); it != this->_data._in.bound_seek.end(); it++)
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
	if (this->_data._in.temporary->tellg() == (long long)this->_data._in.bound_seek.back())
		return (true);
	size_t buf_size = 0;

	// start second step - repetitive step for data until seek is number of vector first.
	if (this->_data._in.in_bound < this->_data._in.bound_seek.front())
	{
		if (BUFFER_SIZE_MB < this->_data._in.bound_seek[1] - this->_data._in.temporary->tellg())
			buf_size = BUFFER_SIZE_MB;
		else
			buf_size = this->_data._in.bound_seek[1] - this->_data._in.temporary->tellg() - 2;
		char	buff[buf_size];
		memset(&buff, 0, buf_size);
		this->_data._in.temporary->read(buff, buf_size);
		this->_data._in.filename->write(buff, buf_size);
		if (buf_size < BUFFER_SIZE_MB)
			this->_data._in.temporary->seekg(this->_data._in.bound_seek.front(), this->_data._in.temporary->beg);
		//this->_data._in.in_bound = this->_data._in.temporary->tellg();
		return (false);
	}

	this->_data._in.temporary->seekg(this->_data._in.bound_seek.front(), this->_data._in.temporary->beg);
	this->_data._in.bound_seek.erase(this->_data._in.bound_seek.begin());

	// getline first line to spring boundary line
	std::getline(*this->_data._in.temporary, this->_data._in.file_buf, '\n');
	std::cout << GREEN << "Extracted line is: " << RST << this->_data._in.file_buf << std::endl;
	std::cout << RED << "Cursor in position: " << this->_data._in.temporary->tellg() << RST << std::endl;

	// take header until \n is first element of line, means we have taked last line of header so we start on the first octet of data
	while (this->_data._in.file_buf.find("\r") != 0)
	{
		std::getline(*this->_data._in.temporary, this->_data._in.file_buf, '\n');
		// if content dispo
		// split to vector
		// find val for name and filename

/*
if (!this->_data._in.file_buf.find("Content-Disposition"))
{
	ft::split_to_vectors(_header.Content_Disposition, this->_data._in.file_buf, ";");
	this->_data._in.file_buf.clear();
}
if (!this->_data._in.file_buf.find("Content-Type"))
	this->_data._in.file_buf.clear();
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
*/
		std::cout << GREEN << "Extracted line is: " << RST << this->_data._in.file_buf << std::endl;
		std::cout << RED << "Cursor in position: " << this->_data._in.temporary->tellg() << RST << std::endl;
	}

	// if filename create file and return false
	// else means is only var.
	// so do var and 
	
	std::cout << PURPLE << "START OF DATA! " << this->_data._in.temporary->tellg() << " to " << this->_data._in.bound_seek.front() <<RST << std::endl;
	//char bb[this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg()];
	//memset(bb, 0, this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg());

	//this->_data._in.temporary->read(bb, this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg() - 2);
	//std::cout << RED << this->_data._in.temporary->gcount() << " char taked!" << RST << std::endl;
	//std::cout << RED << this->_data._in.temporary->tellg() << " + 2 should the position where start next boundary." << RST << std::endl;
	//std::cout << RED << bb << RST << std::endl;
	//this->_data._in.temporary->read(bb, this->_data._in.bound_seek.front() - this->_data._in.temporary->tellg() - 2);
	// end of first boudary and start of next boundary / if dont exist we should stay in eof.
	return (true);

}