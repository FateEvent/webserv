/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/03/31 15:07:35 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(const config &config) : _ref_conf(config)
{
	this->clear_header();
	_socklen = sizeof(this->_addr);
	//FD_ZERO(&this->_readfd); // maybe don't need
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	if (this->_sock_fd == -1)
	{
		std::cout << strerror(errno) << std::endl;
		throw std::invalid_argument("Socket error in constructor Client!");
	}
	fcntl(this->_sock_fd, F_SETFL, O_NONBLOCK);
}

Client::Client(const config &config, sockaddr_in sock, socklen_t len, int fd, header& head) : _ref_conf(config), _header(head)
{
	this->_working = false;
	this->_chunked = false;
	this->_fd_cgi[0] = 0;
	this->_fd_cgi[1] = 1;
	this->_cgi = false;
	this->_sedding = false;
	this->_reponse.clear();
	this->_root.clear();
	this->_index.clear();
	this->_error_page.clear();
	this->_proxy.clear();
	this->_cgi_call.clear();
	this->_max_body = 0;
	this->other.clear();
	this->_data.data_sended = 1;
	this->_data.data_size = 0;
	this->_data.header.clear();
	this->_data.file = NULL;
	
	this->_sock_fd = fd;
	this->_addr = sock;
	this->_socklen = len;
	this->_timeout = std::time(nullptr);
	this->_header.time_out = this->_timeout;
	this->_ready = true;
}

Client::~Client() {}
int	Client::get_sockfd() const 				{ return (this->_sock_fd);}
time_t	Client::get_time_alive() const		{ return (this->_timeout);}
std::string Client::get_methode() const		{ return (this->_header.Methode);}
std::string	Client::get_directory() const	{ return (this->_header.Dir);}
bool	Client::is_working() const			{ return (this->_working);}
bool	Client::is_chunk() const			{ return(this->_chunked);}
header*	Client::get_header()				{ return (&this->_header);}
const config*	Client::get_config() const	{ return (&this->_ref_conf);}
bool	Client::is_cgi() const				{ return (this->_cgi?true:false);}

std::pair<std::string, std::string>	Client::get_file() const
{
	if (this->_header.file.first.empty())
		throw;
	return (this->_header.file);
}

int	Client::get_fd_cgi() const
{
	if (this->is_cgi())
		return (this->_fd_cgi[1]); // SHOULD CORRECT, if not put _fd_cgi[1]
	return (0);
}

void	Client::clear_header()
{
	this->_header.clear();
	this->_working = false;
	this->_chunked = false;
	this->_fd_cgi[0] = 0;
	this->_fd_cgi[1] = 0;
	this->_pid_cgi = 0;
	this->_cgi = false;
	this->_sedding = false;
	this->_reponse.clear();
	this->_root.clear();
	this->_index.clear();
	this->_error_page.clear();
	this->_proxy.clear();
	this->_cgi_call.clear();
	this->_max_body = 0;
	this->other.clear();
	this->_data.data_sended = 1;
	this->_data.data_size = 0;
	this->_data.header.clear();
	if (this->_data.file != 0)
		delete this->_data.file;
	this->_data.file = NULL;
	this->_ready = false;
}

bool	Client::new_request()
{
	if (!ft::parse_header(this->_sock_fd, this->_header))
			return (false);
	this->_timeout = std::time(nullptr);
	this->_header.time_out = this->_timeout;
	this->_ready = true;
	return (true);
}

bool	Client::is_seeding() const	{ return (this->_sedding?true:false); }
bool	Client::is_ready() const	{ return (this->_ready?true:false); }

bool	Client::send_data(int fd)
{
	int size;
	socklen_t len = sizeof(size);
	if (getsockopt(this->_sock_fd, SOL_SOCKET, SO_SNDBUF, &size, &len) == -1)
	{
		std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		return (false);
	}
	this->_data.file->seekg(this->_data.data_sended - 1, this->_data.file->beg);
	char buff[size + 1];
	std::memset(buff, 0, size + 1);
	this->_data.file->read(buff, size);

	ssize_t s = send(fd, buff,  this->_data.file->gcount(), 0);
	if (s == -1 || s == 0)
	{
		return (false);
	}
	this->_data.data_sended += s;
	if (this->_data.data_sended >= this->_data.data_size)
		return (true);
	return (false);
}

bool	Client::continue_client(fd_set *fdset)
{
	(void)fdset;
	if (this->_sedding)
	{
		if (this->send_data(this->_sock_fd))
		{
			this->clear_header();
			this->_index.clear();
			this->_root.clear();
			std::cout << YELLOW << "Sending finish for socket " << this->_sock_fd << RST << std::endl;
			return (true);
		}
		return (false);
	}
	else if (this->_chunked)
		this->chunk(); // need put data of chunked in s_clt_data::body_in
	else if (this->is_cgi())
	{
		if (waitpid(this->_pid_cgi, &this->_data.wsatus, WNOHANG) == this->_pid_cgi) // Should means cgi ad finish.
		{
			this->take_data();
			// end if cgi
		}
		
	}
	else
	{
		if (!_header.Methode.compare("GET"))
		{
			std::cout << "GET METHODE" << std::endl;
		}
		else if (!_header.Methode.compare("POST"))
		{
			std::cout << "POST METHODE" << std::endl;
		}
		else if (!_header.Methode.compare("DELETE"))
		{
			std::cout << "DELETE METHODE" << std::endl;
		}
		else
			std::cout << "PROBLEME EXIST IF ON SCREEN!!!!!continue_client" << std::endl;
	}
	return (false);
}

bool	Client::execute_client(bool path)
{
	if (!path && !this->is_working())
	{
		#ifdef DEBUG
			std::cout << PURPLE << "File not found or can't open!" << RST << std::endl;
		#endif
		std::string header;
		this->_data.file = new std::stringstream();
		header = ft::make_header(404);
		send(this->_sock_fd, header.c_str(), header.length(), 0);
		*static_cast<std::stringstream*>(_data.file) << ft::get_page_error(404, this->_error_page[404].empty() ?
					(this->_ref_conf.error_page.find(404)->second.empty() ?
					this->_ref_conf._base->error_page.find(404)->second : this->_ref_conf.error_page.find(404)->second) :
					this->_error_page[404]);
		this->_data.file->seekg(0, this->_data.file->end);
		this->_data.data_size = this->_data.file->tellg();
		this->_data.file->seekg(0, this->_data.file->beg);
		this->_sedding = true;
	}
	else if (_header.Methode.compare("GET") == 0)
	{
		std::cout << "GET METHODE" << std::endl;
		if ((!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end()) ||
			(!this->_ref_conf.cgi.empty() && _ref_conf.cgi.find(_index.substr(_index.find_last_of("."))) != _ref_conf.cgi.end()))
		{
			if (_ref_conf.cgi.find(this->_header.file.second) != this->_ref_conf.cgi.end()) // Tester en premier les gci dans location... switch if and else..
			{
				// WARNING IF CGI VAR AS ONLY A KEY THAT MEANS THE KEY IS A PATH! TODO THAT!
				this->launch_cgi(this->_ref_conf.cgi.find(_index.substr(_index.find_last_of(".")))->second);
				std::cout << "CGI on base" << std::endl;
			}
			else
			{
				// WARNING IF CGI VAR AS ONLY A KEY THAT MEANS THE KEY IS A PATH! TODO THAT!
				this->launch_cgi(this->_cgi_call.find(_index.substr(_index.find_last_of(".")))->second);
				std::cout << "CGI on location" << std::endl;
			}
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
			{
				// ERROR 500
			}
			if (check == 0)
			{
				// ERROR 501
			}
			std::cout << BLUE << this->_data.header << RST << std::endl;
			this->_sedding = true;
			//this->_working = true;
		}
	}
	else if (_header.Methode.compare("POST") == 0)
	{
		std::cout << "POST METHODE" << std::endl;
		char buff[2];
		memset(buff, 0, 2);
		int recept = 0;
		recept = recv(this->_sock_fd, &buff, 1, 0);
		while (recept > 0)
		{
			std::cout << buff;
			recept = recv(this->_sock_fd, &buff, 1, 0);
		}
		std::cout << std::endl;
		std::cout << "Send: " << send(this->_sock_fd, "HTTP/1.1 405 Method Not Allowed\r\n\r\n\0", 36, MSG_OOB) << std::endl; // , NULL, 0);
		//close(this->_sock_fd);
		this->clear_header();
		this->_index.clear();
		this->_root.clear();
		this->_working = true;
		return (true);
	}
	else if (_header.Methode.compare("DELETE") == 0)
		std::cout << "DELETE METHODE" << std::endl;
	else
		std::cout << "BAD REQUEST / BAD HEADER" << std::endl; // Should not goto inside.
	return (false);
}

void	Client::chunk()
{}

void	Client::take_data()
{}