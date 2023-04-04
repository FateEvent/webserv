/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/04/04 09:17:55 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(config &config) : _ref_conf(config)
{
	this->clear_header();
	_socklen = sizeof(this->_addr);
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	if (this->_sock_fd == -1)
	{
		std::cout << strerror(errno) << std::endl;
		throw std::invalid_argument("Socket error in constructor Client!");
	}
	fcntl(this->_sock_fd, F_SETFL, O_NONBLOCK);
}

Client::Client(config &config, sockaddr_in sock, socklen_t len, int fd, header& head) : _ref_conf(config), _header(head)
{
	this->_working = false;
	this->_chunked = false;
	this->_pipe_cgi_in[0] = 0;
	this->_pipe_cgi_in[1] = 0;
	this->_pipe_cgi_out[0] = 0;
	this->_pipe_cgi_out[1] = 0;
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

void	Client::clear_header()
{
	this->_header.clear();
	this->_working = false;
	this->_chunked = false;
	this->_pipe_cgi_in[0] = 0;
	this->_pipe_cgi_in[1] = 0;
	this->_pipe_cgi_out[0] = 0;
	this->_pipe_cgi_out[1] = 0;
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

Client::~Client() {}
int				Client::get_sockfd() const 		{ return (this->_sock_fd);}
time_t			Client::get_time_alive() const	{ return (this->_timeout);}
std::string 	Client::get_method() const		{ return (this->_header.Method);}
std::string		Client::get_directory() const	{ return (this->_header.Dir);}
int				Client::get_pid_cgi() const	 	{ return (this->_pid_cgi);}
bool			Client::is_working() const		{ return (this->_working);}
bool			Client::is_chunk() const		{ return(this->_chunked);}
header*			Client::get_header()			{ return (&this->_header);}
const config*	Client::get_config() const		{ return (&this->_ref_conf);}
bool			Client::is_cgi() const			{ return (this->_cgi?true:false);}
bool			Client::is_seeding() const		{ return (this->_sedding?true:false); }
bool			Client::is_ready() const		{ return (this->_ready?true:false); }

std::pair<std::string, std::string>	Client::get_file() const
{
	if (this->_header.file.first.empty())
		throw;
	return (this->_header.file);
}

int	Client::get_fd_cgi() const
{
	if (this->is_cgi())
		return (this->_pipe_cgi_in[1]);
	return (0);
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
	this->_data.file->clear();
	ssize_t s = send(fd, buff, this->_data.file->gcount(), 0);
	if (s > 0)
		this->_data.data_sended += s;
	if (this->_data.data_sended >= this->_data.data_size)
		return (true);
	//if (s <= 0)
	//	return (false);
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
	}
	else if (this->is_cgi())
	{
		if (this->_chunked)
		{
			// chunk by paquet
		}
		else
		{
			if (this->_data.wpid == this->_pid_cgi)
			{
				int taking = this->take_data();
				if (taking == 0)
				{
					cgi_prepare_to_send();
					close(this->_pipe_cgi_out[1]);
					close(this->_pipe_cgi_in[0]);
					this->_sedding = true;
				}
				else if (taking == -1)
					std::cout << RED << "Read error for cgi" << RST << std::endl;
			}
			else
			{
				this->take_data();
				this->_data.wpid = waitpid(this->_pid_cgi, &this->_data.wsatus, WNOHANG);
			}
		}
	}
	else if (this->_chunked)
		this->chunk(); // need put data of chunked in s_clt_data::body_in
	else
	{
		std::cout << "PROBLEME EXIST IF ON SCREEN!!!!!continue_client" << std::endl;
		return (true);
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
		*static_cast<std::stringstream*>(_data.file) << ft::get_page_error(404,
				this->_error_page[404].empty() ?
					(this->_ref_conf.error_page[404].empty() ?
						(this->_ref_conf._base->error_page[404].empty() ? "\n" : this->_ref_conf._base->error_page.find(404)->second) :
					this->_ref_conf.error_page.find(404)->second) :
				this->_error_page[404]);
		this->_data.file->seekg(0, this->_data.file->end);
		this->_data.data_size = this->_data.file->tellg();
		this->_data.file->seekg(0, this->_data.file->beg);
		this->_sedding = true;
	}
	// MAY BE TEST CGI HERE AND NOT IN GET,POST OR DELETE METHODE
	else if (_header.Method.compare("GET") == 0)
	{
		std::cout << "GET METHOD" << std::endl;
		if ((!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end()) ||
			(!this->_ref_conf.cgi.empty() && _ref_conf.cgi.find(_index.substr(_index.find_last_of("."))) != _ref_conf.cgi.end()))
		{
			(*static_cast<std::ifstream*>(this->_data.file)).close();
			this->_data.file = 0;
			this->_data.file = new std::stringstream();
			if (_ref_conf.cgi.find(this->_header.file.second) != this->_ref_conf.cgi.end()) // Tester en premier les gci dans location... switch if and else..
			{
				// WARNING IF CGI VAR AS ONLY A KEY THAT MEANS THE KEY IS A PATH! TODO THAT!
				//this->launch_cgi(this->_ref_conf.cgi.find(_index.substr(_index.find_last_of(".")))->second);
				std::cout << "CGI on base" << std::endl;
			}
			else
			{
				std::cout << "CGI on location" << std::endl;
				if (!this->_cgi_call.empty() && _cgi_call.find(_index.substr(_index.find_last_of("."))) != _cgi_call.end())
					this->launch_cgi(this->_cgi_call.find(_index.substr(_index.find_last_of(".")))->second);
				else
					this->launch_cgi(this->_ref_conf.cgi.find(_index.substr(_index.find_last_of(".")))->second);
				
				// WARNING IF CGI VAR AS ONLY A KEY THAT MEANS THE KEY IS A PATH! TODO THAT!
				
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
	else if (_header.Method.compare("POST") == 0)
	{
		std::cout << "POST METHOD" << std::endl;
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
	else if (_header.Method.compare("DELETE") == 0)
		std::cout << "DELETE METHOD" << std::endl;
	else
		std::cout << "BAD REQUEST / BAD HEADER" << std::endl; // Should not goto inside.
	return (false);
}

void	Client::chunk()
{}

int	Client::take_data() // the data should stay in pipe_in
{
	int size = BUFFER_SIZE_MB * 4;
	char buff[size + 1];
	std::memset(buff, 0, size + 1);

	int reading = read(this->_pipe_cgi_in[0], &buff, size);
	if (reading > 0)
	{
		*static_cast<std::stringstream*>(_data.file) << buff;
		this->_data.file->seekg(0, this->_data.file->end);
		this->_data.data_size = this->_data.file->tellg();
		this->_data.file->seekg(0, this->_data.file->beg);
	}
//	else if (reading < 0)
		//std::cout << RED << "Read cgi pipe error!" << RST << std::endl;
		//std::cout << PURPLE << "Receipt again.." << RST << std::endl;
	return (reading);
}

void	Client::cgi_prepare_to_send()
{
	char	buff[BUFFER_SIZE_KB * 8 + 1];
	std::memset(buff, 0, BUFFER_SIZE_KB * 8 + 1);
	this->_data.file->seekg(0, this->_data.file->beg);
	this->_data.file->read(buff, BUFFER_SIZE_KB * 8);
	std::string header_test = buff;
	if (header_test.find("HTTP/1.1") == 0) // Means complete header stay stringstream
		return;
	this->_data.minus_header = header_test.find("\r\n\r\n");
	this->_data.header = ft::make_header(200);
	if (header_test.find("Content-Length:") == header_test.npos)
		this->_data.header.append("Content-Length: " + std::to_string(this->_data.data_size - (this->_data.minus_header != header_test.npos ? this->_data.minus_header + 4 : 0)) + "\r\n");
	if (header_test.find("Content-Type:") == header_test.npos)
		this->_data.header.append("Content-Type: text/html\r\n");
	if (this->_data.minus_header == header_test.npos)
		this->_data.header.append("\r\n");

	send(this->_sock_fd, this->_data.header.c_str(), this->_data.header.length(), 0);
	this->_sedding = true;
	return ;
}

void	Client::kill_cgi()
{
	if (this->_pipe_cgi_in[0] > 0)
	{
		kill(this->_pid_cgi, 2);
		close(this->_pipe_cgi_in[0]);
		close(this->_pipe_cgi_in[1]);
		close(this->_pipe_cgi_out[0]);
		close(this->_pipe_cgi_out[1]);
	}
}