/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/05/04 15:06:41 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(config &config) : _ref_conf(config)
{
	this->clear_header();
	_socklen = sizeof(this->_addr);
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	//const int set = 1;
	//setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEPORT, &set, sizeof(set));
	if (this->_sock_fd <= 0)
		throw std::invalid_argument("Socket error in constructor Client!");
	fcntl(this->_sock_fd, F_SETFL, O_NONBLOCK);
}

Client::Client(config &config, sockaddr_in sock, socklen_t len, int fd, header& head) : _ref_conf(config), _header(head)
{
	this->_working = false;
	this->_chunked = false;
	this->_multipart = false;
	this->_pipe_cgi[0] = 0;
	this->_pipe_cgi[1] = 0;
	this->_pid_cgi = 0;
	this->_cgi = false;
	this->_sending = false;
	this->_response.clear();
	this->_root.clear();
	this->_index.clear();
	this->_error_page.clear();
	this->_proxy.clear();
	this->_cgi_call.clear();
	this->_max_body = 0;
	this->_redirect.clear();
	this->other.clear();
	this->_data.data_sent = 1;
	this->_data.data_size = 0;
	this->_data.header.clear();
	this->_data.file = NULL;
	this->_sock_fd = fd;
	this->_addr = sock;
	this->_socklen = len;
	this->_ready = true;
	this->_close = false;
	this->_multipart = false;
}

void	Client::clear_header()
{
	this->_header.clear();
	this->_working = false;
	this->_chunked = false;
	this->_multipart = false;
	this->_pipe_cgi[0] = 0;
	this->_pipe_cgi[1] = 0;
	this->_pid_cgi = 0;
	this->_allow = 0x000;
	this->_cgi = false;
	this->_sending = false;
	this->_response.clear();
	this->_root.clear();
	this->_index.clear();
	this->_redirect.clear();
	this->_error_page.clear();
	this->_proxy.clear();
	this->_cgi_call.clear();
	this->_max_body = 0;
	this->other.clear();
	this->_data.data_sent = 1;
	this->_data.data_size = 0;
	this->_data.header.clear();
	if (this->_data.file != 0)
		delete this->_data.file;
	this->_data.file = NULL;
	this->_ready = false;
	this->_close = false;
	this->_multipart = false;
	this->_timeout = 0;
}

Client::~Client()
{
	if (this->_pid_cgi > 0)
		::kill(this->_pid_cgi, 2);
	if (this->_pipe_cgi[0] > 0)
		::close(this->_pipe_cgi[0]);
	// if (this->_sock_fd != -1)
	// {
	// 	std::cout << "-1 here and there" << std::endl;
	// 	::close(_sock_fd);
	// }
	if (this->_data.file != 0)
		delete this->_data.file;
	this->_ref_conf.nbr_client--;
}

int				Client::get_sockfd() const 					{ return (this->_sock_fd); }
void			Client::set_sockfd(int sock_fd)				{ this->_sock_fd = sock_fd; }
time_t			Client::get_time_alive() const				{ return (this->_timeout); }
std::string 	Client::get_method() const					{ return (this->_header.Method); }
std::string		Client::get_directory() const				{ return (this->_header.Dir); }
int				Client::get_pid_cgi() const				 	{ return (this->_pid_cgi); }
bool			Client::is_working() const					{ return (this->_working); }
bool			Client::is_chunk() const					{ return(this->_chunked); }
header*			Client::get_header()						{ return (&this->_header); }
const config*	Client::get_config() const					{ return (&this->_ref_conf); }
bool			Client::is_cgi() const						{ return (this->_cgi?true:false); }
bool			Client::is_multipart() const				{ return (this->_multipart); }
bool			Client::is_sending() const					{ return (this->_sending?true:false); }
bool			Client::is_ready() const					{ return (this->_ready?true:false); }
unsigned int	Client::get_nbr_connected_client() const	{ return (this->_ref_conf.nbr_client); }
void			Client::add_nbr_client()					{ this->_ref_conf.nbr_client++; }
bool			Client::get_close() const					{ return (this->_close); }
long			Client::get_max_body() const				{ return (this->_max_body); }
long			Client::get_timeout() const					{ return (this->_timeout); }
void			Client::set_timeout(long timeout)			{ this->_timeout = std::time(nullptr) + timeout; }

int	Client::get_fd_cgi_0() const
{
	if (this->is_cgi())
		return (this->_pipe_cgi[0]);
	return (0);
}

int	Client::get_fd_cgi() const
{
	if (this->is_cgi())
		return (this->_pipe_cgi[1]);
	return (0);
}

bool	Client::new_request()
{
	if (!ft::parse_header(this->_sock_fd, this->_header))
			return (false);
	this->add_nbr_client();
	this->_header.time_out = std::time(nullptr);
	this->_ready = true;
	std::cout << GREEN << "Connexion accepted to socket " << this->_sock_fd << RST << std::endl;
	return (true);
}

bool	Client::send_data(int fd)
{
	int size;
	socklen_t len = sizeof(size);
	if (getsockopt(this->_sock_fd, SOL_SOCKET, SO_SNDBUF, &size, &len) == -1)
		return (false);
	this->_data.file->clear();
	this->_data.file->seekg(this->_data.data_sent - 1, this->_data.file->beg);
	char buff[size + 1];
	std::memset(buff, 0, size + 1);
	this->_data.file->read(buff, size);
	ssize_t s = send(fd, buff, this->_data.file->gcount(), 0);
	if (s > 0)
		this->_data.data_sent += s;
	if (this->_data.data_sent >= this->_data.data_size)
		return (true);
	//if (s <= 0)
	//	return (false);
	return (false);
}

bool	Client::continue_client(fd_set *fdset)
{
	(void)fdset;
	if (this->_sending)
	{
		if (this->send_data(this->_sock_fd))
		{
			//this->clear_header();
			this->_sending = false;
			this->_close = true;
			delete this->_data.file;
			this->_data.file = 0;
			::shutdown(this->_sock_fd, SHUT_WR);
			return (true);
		}
	}
	else if (this->is_cgi())
	{
		if (this->is_chunk())
		{
			int			recept = 1;
			char 		buff[2];
			std::string tmp;
			int			size = 0;
			std::memset(buff, 0, 2);
			while (tmp.find("/r/n") == tmp.npos)
			{
				// ATTENTION BLOQUANT EN CAS DE SOCKET NON PRET OU LONGUE RECUPERATION! PASSER PAR UN STREAMSTRING??
				// NORMALEMENT NON CAR LES CHUNK DEVRAIENT ETRE DE PETITE TAILLE.
				recept = recv(this->_sock_fd, &buff, 1, 0);
 				if (recept == -1)
					continue;
				if (recept == 0 || buff[0] == 0) // Usually client close connection...
					break;
				tmp.push_back(buff[0]);
			}
			size = std::strtoul(tmp.c_str(), NULL, 16);
			char tosend[size + 1];
			std::memset(tosend, 0 ,size + 1);
		}
		else
		{
			int taking = this->take_data();
			if (this->_data.wpid == this->_pid_cgi)
			{
				// check return value wsatus, if != 0 do free already receipt and send a error 500
				if (taking == 0)
				{
					cgi_prepare_to_send();
					close(this->_pipe_cgi[0]);
					this->_sending = true;
				}
				//else if (taking == -1)
				//	std::cout << RED << "Read error -1 for cgi" << RST << std::endl;
			}
			this->_data.wpid = waitpid(this->_pid_cgi, &this->_data.wsatus, WNOHANG);
		}
	}
	else if (this->is_chunk())
		this->chunk(); 
	else if (this->is_multipart())
	{
		if (this->multipart() && this->process_multipart())
		{
			this->_data._in.temporary->close();
			remove(this->_data._in.tmpfile);
			delete this->_data._in.temporary;
			this->_data.header = ft::make_header(201);
			this->_data.header.append("Content-Length: " + std::to_string(this->_data.data_size) + "\r\n");
			this->_data.header.append(ft::make_content_type(this->_index.substr(this->_index.find_last_of(".") + 1)));
			int check = 0;
			check = send(this->_sock_fd, this->_data.header.c_str(), this->_data.header.length(), 0);
			if (check == -1)
				this->make_error(500);
			if (check == 0)
				this->make_error(501);
			this->_sending = true;
		}
	}
	else
	{
		std::cout << "THE PROBLEM EXISTS IF ON SCREEN!!!!! continue_client" << std::endl;
		return (true);
	}
	return (false);
}

bool	Client::execute_client(bool path)
{
	if (!this->_redirect.empty())
	{
		if (shutdown(this->_sock_fd, SHUT_RD) == -1)
			std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		this->make_error(301);
	}
	else if (!path && !this->is_working())
	{
		#ifdef DEBUG
			std::cout << PURPLE << "The file hasn't been found or can't be opened!" << RST << std::endl;
		#endif
		shutdown(this->_sock_fd, SHUT_RD);
		this->make_error(404);
	}
	// MAY BE TEST CGI HERE AND NOT IN GET,POST OR DELETE METHODE
	else if (_header.Method.compare("GET") == 0)
	{
		shutdown(this->_sock_fd, SHUT_RD);
		return (this->execute_get());
	}
	else if (_header.Method.compare("POST") == 0)
		return (this->execute_post());
	else if (_header.Method.compare("DELETE") == 0)
		return (this->execute_delete());	
	else
	{
		std::cout << "BAD REQUEST / BAD HEADER: " << _header.Method << std::endl; // Should not goto inside.
	}
	return (false);
}

int	Client::take_data() // the data should stay in pipe_in
{
	int size = BUFFER_SIZE_MB * 4;
	char buff[size];
	std::memset(buff, 0, size);

	int reading = read(this->_pipe_cgi[0], &buff, size);
	if (reading > 0)
	{
		static_cast<std::stringstream*>(_data.file)->write(buff, reading); //<< buff;
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
	int		err_header = 200;
	char	buff[BUFFER_SIZE_KB * 8 + 1];
	
	std::memset(buff, 0, BUFFER_SIZE_KB * 8 + 1);
	this->_data.file->seekg(0, this->_data.file->beg);
	this->_data.file->read(buff, BUFFER_SIZE_KB * 8);
	std::string header_test = buff;
	if (header_test.find("HTTP/1.1") == 0) // Means complete header stay stringstream
		return;
	this->_data.minus_header = header_test.find("\r\n\r\n") == header_test.npos ? 0 : header_test.find("\r\n\r\n");
	if (header_test.find("Status: ") < this->_data.minus_header)
		err_header = std::strtoul(header_test.substr(header_test.find("Status: ") + 8, 3).c_str(), NULL, 10);
	this->_data.header = ft::make_header(err_header);
	
	if (header_test.find("Content-Length:") > this->_data.minus_header && header_test.find("content-length:") > this->_data.minus_header)
		this->_data.header.append("Content-Length: " + std::to_string(this->_data.data_size - (this->_data.minus_header != header_test.npos ? this->_data.minus_header + 4 : 0)) + "\r\n");
	if (header_test.find("Content-Type:") > this->_data.minus_header && header_test.find("content-type:") > this->_data.minus_header)
		this->_data.header.append("Content-Type: text/html\r\n");
	if (header_test.find("\r\n\r\n") == header_test.npos)
		this->_data.header.append("\r\n");
	send(this->_sock_fd, this->_data.header.c_str(), this->_data.header.length(), 0);
	return ;
}

void	Client::kill_cgi()
{
	if (this->_pipe_cgi[0] > 0 && _data.wpid > 0)
	{
		kill(this->_pid_cgi, 2);
		close(this->_pipe_cgi[0]);
		close(this->_pipe_cgi[1]);
	}
}

void	Client::make_error(int i)
{
	if (this->_data.file != 0)
	{
		// check if fstream, close and delete it
		// if stringstream just delete it
		delete this->_data.file;
		this->_data.file = NULL;
	}
	this->_data.file = new std::stringstream();
	std::string	header;
		header = ft::make_header(i);
	send(this->_sock_fd, header.c_str(), header.length(), 0);
	if (!this->_redirect.empty())
		*static_cast<std::stringstream*>(_data.file) << ("Location: " + this->_redirect + "\r\n\r\n");
	*static_cast<std::stringstream*>(_data.file) << ft::get_page_error(i,
		this->_error_page[i].empty() ?
			(this->_ref_conf.error_page[i].empty() ?
				(this->_ref_conf._base->error_page[i].empty() ? "\n" : this->_ref_conf._base->error_page.find(i)->second) :
			this->_ref_conf.error_page.find(i)->second) :
		this->_error_page[i]);
	this->_data.file->seekg(0, this->_data.file->end);
	this->_data.data_size = this->_data.file->tellg();
	this->_data.file->clear();
	this->_data.file->seekg(0, this->_data.file->beg);
	this->_sending = true;
}

void	Client::chunk()
{
	char			buff[2];
	int				len(0);

	std::memset(buff, 0, 2);
	// READ SOCKET JJUSQUA \R\N
	//COVERTI EN INT
	//
	while (this->_data._in.receipt > 0)
	{
		this->_data._in.receipt = recv(this->_sock_fd, &buff, 1, 0);
		this->_data._in.file_buf += buff[0];
		std::cout << GREEN << buff[0];
		if (buff[0] == '\n')
			break;
	}
	if (!this->_data._in.file_buf.find("\r\n"))
		this->_data._in.file_buf.clear();
	if (is_hex_line(this->_data._in.file_buf))
	{
		len = hextol(this->_data._in.file_buf);
		if (!len)
		{
			this->_data._in.file_buf.clear();
			send_success_status();
			return ;
		}
		else
		{
			this->_data._in.file_buf.clear();
			while (this->_data._in.receipt > 0 && len > 0)
			{
				this->_data._in.receipt = recv(this->_sock_fd, &buff, 1, 0);
				this->_data._in.file_buf.append(&buff[0]);
				std::cout << PURPLE << buff[0];
				--len;
			}
			*(this->_data._in.temporary) << this->_data._in.file_buf;
			this->_data._in.temporary->close();
			this->_data._in.file_buf.clear();
		}
	}
}

int	Client::is_hex_line(std::string str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (Client::is_hex_digit(str[i]))
			i++;
		else
			break ;
	}
	if (str[i] == '\r' && str[i + 1] == '\n')
		return (1);
	return (0);
}

int	Client::is_hex_digit(char c)
{
	if ((c >= 48 && c <= 57)
		|| (c >= 65 && c <= 70)
		|| (c >= 97 && c <= 102))
		return (1);
	return (0);
}

size_t	Client::getMaxPower(std::string hex)
{
	size_t	i;

	i = 0;
	while (hex[i])
	{
		if (Client::is_hex_digit(hex[i]))
			i++;
		else
			break ;
	}
	--i;
	return (i);
}

ssize_t	Client::hextol(std::string hex)
{
	size_t  i;
	ssize_t  power;
	ssize_t count;

	i = 0;
	count = 0;
	power = getMaxPower(hex);
	while (power >= 0)
	{
		if (hex[i] >= 97 && hex[i] <= 102)
			hex[i] -= 32;
		if (hex[i] >= 48 && hex[i] <= 57)
			count += (hex[i] - '0') * pow(16, power);
		else if (hex[i] >= 65 && hex[i] <= 70)
			count += (10 + hex[i] - 'A') * pow(16, power);
		else
			break ;
		power--;
		i++;
	}
	return (count);
}

void	Client::send_success_status()
{
	this->_data.header = ft::make_header(200);
	if (this->_data.data_size > 0)
	{
		this->_data.header.append("Content-Length: " + std::to_string(this->_data.data_size) + "\r\n");
		this->_data.header.append(ft::make_content_type(this->_index.substr(this->_index.find_last_of(".") + 1)));
	}
	else
		this->_data.header.append("\r\n");
	int check = 0;
	check = send(this->_sock_fd, this->_data.header.c_str(), this->_data.header.length(), 0);
	if (check == -1)
		this->make_error(500);
	if (check == 0)
		this->make_error(501);
	if (this->_data.data_size == 0)
		this->_close = true;
	this->_sending = true;
}