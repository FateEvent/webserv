/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipart.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:53:24 by stissera          #+#    #+#             */
/*   Updated: 2023/04/17 22:14:01 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::multipart()
{
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
		char	buff[BUFFER_SIZE_MB];
		memset(&buff, 0, BUFFER_SIZE_MB);
		if (this->_data._in.temporary->eof())
		{
			this->_data._in.pos_seek = -1;
			std::cout << "Boundary found at position:" << std::endl;
			for (std::vector<int>::iterator it = this->_data._in.bound_seek.begin(); it != this->_data._in.bound_seek.end(); it++)
				std::cout << GREEN << *it << RST << std::endl;
			make_error(201);
			return (true);
		}
		this->_data._in.temporary->seekg(this->_data._in.pos_seek, this->_data._in.temporary->beg);
		this->_data._in.temporary->read(buff, BUFFER_SIZE_MB);

		// Check if boundary if end of search.
			
		//else
		//	this->_data._in.pos_seek = this->_data._in.temporary->gcount() - this->_header.Boundary.length() + 1;

		std::string search_b(buff, this->_data._in.temporary->gcount());
		// Check if boundary if end of search.
		size_t	pos_str = 0;
		while ((pos_str = search_b.find("--" + this->_header.Boundary, pos_str)) != search_b.npos)
		{
			//std::cout << RED << "Found in position: " << pos_str << RST << std::endl;
			this->_data._in.bound_seek.push_back(this->_data._in.pos_seek + pos_str);
			pos_str++;
		}
		if (this->_data._in.temporary->gcount() - pos_str == this->_data._in.temporary->gcount() - this->_header.Boundary.length())
			this->_data._in.pos_seek = this->_data._in.pos_seek + this->_data._in.temporary->gcount() - this->_header.Boundary.length() - 2;
		else
			this->_data._in.pos_seek = this->_data._in.pos_seek + this->_data._in.temporary->gcount();

//		std::cout << GREEN << "-----------------" << this->_data._in.pos_seek << "--------------------------\n" << search_b << "\n---------------------" << this->_data._in.temporary->tellg() << "---------------------" << RST << std::endl;
		//sleep(5);
		/* for (int i = 0; i < this->_data._in.receipt; i++)
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
		}*/
	}
//	else if (this->_data._in.pos_seek == -1)
//	{
//
//	}

	// Use bool in if for finish?!
	//if (this->_header.Content_Length == this->_data._in.size)
	//{
	//	 return (true);
		// make_error(201);
		//remove(this->_data._in.tmpfile);
	//}
	return (false);
}