/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipart.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:53:24 by stissera          #+#    #+#             */
/*   Updated: 2023/04/17 15:19:39 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::multipart()
{
	if (this->_data._in.receipt != 0)
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
	
	else if (this->_data._in.receipt == 0 && this->_data._in.pos_seek != -1)
	{
		char	buff[BUFFER_SIZE_MB];
		memset(&buff, 0, BUFFER_SIZE_MB);
		this->_data._in.temporary->seekg(this->_data._in.pos_seek, this->_data._in.temporary->beg);
		this->_data._in.temporary->read(buff, BUFFER_SIZE_MB);

		// Check if boundary if end of search.
		if (this->_data._in.temporary->eof())
			this->_data._in.pos_seek = -1;	
		else
			this->_data._in.pos_seek = this->_data._in.temporary->gcount() - this->_header.Boundary.length() + 1;

		std::string search_b(buff, this->_data._in.temporary->gcount() - this->_data._in.pos_seek);
		// Check if boundary if end of search.
		int	pos_str = 0;
		while (pos_str = search_b.find(this->_header.Boundary, pos_str) != search_b.npos)
		{
			std::cout << RED << "Before push back: " << pos_str << RST << std::endl;
			this->_data._in.bound_seek.push_back(pos_str++);
			std::cout << RED << "After push back: " << pos_str << RST << std::endl;
			std::cout << RED << "Push back: " << this->_data._in.bound_seek.back() << RST << std::endl;
		}


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

	// Use bool in if for finish?!
	if (this->_header.Content_Length == this->_data._in.size)
	{
		 return (true);
		// make_error(201);
		//remove(this->_data._in.tmpfile);
	}
	return (false);
}