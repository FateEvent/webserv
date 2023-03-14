/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/03/14 22:15:16 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(const config &config) : _ref_conf(config)
{
	_socklen = sizeof(this->_addr);
	this->_working = false;
	FD_ZERO(&this->_readfd); // maybe don't need
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	if (this->_sock_fd == -1)
		throw std::invalid_argument("Socket error in constructor Client!");
	fcntl(this->_sock_fd, F_SETFL, O_NONBLOCK);
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
		return (this->_fd_cgi);
	return (0);
}

void	Client::clear_header()
{
	this->_header.clear();
	this->_working = false;
	this->_chunked = false;
	this->_fd_cgi = 0;
	this->_cgi = false;
}

bool	Client::new_request()
{
	char 						buffer[2];
	std::string					tmp;
	std::string					line;
	std::vector<std::string>	header;
	size_t 						s_str;
	size_t 						e_str;
	ssize_t						recept = 1;

	while (tmp.find("\r\n\r\n") == tmp.npos)
	{
		recept = recv(this->_sock_fd, &buffer, 1, 0);
		if (recept < 1 || buffer[0] == 0)
			break;
		tmp.append(buffer);
	}

	#ifdef DEBUG
		if (recept == 0) // empty fd
		{
			std::cout << "---	RECV return 0 -\n\e[10	0m" + tmp + "\e[0m" << std::endl;
			return (false);
		}
		else if (recept == -1)
			std::cout << "HEADER RECEPT RAW -1: \n\e[100m" + tmp + "\e[0m" << std::endl;
	#endif

	if (recept == -1) // end of fd
	{
		#ifdef DEBUG
			std::cout << "\n\e[94m" + tmp + "\e[0m" << std::endl;
		#endif
		return (false);
	}

	// SET HEADER IN VECTOR BY LINE
	for (std::string::iterator it = tmp.begin(); it != tmp.end() && *it != 0; it++)
	{
		for (; *it != '\n' && it != tmp.end() && *it != 0; it++)
			line.push_back(*it);
		header.push_back(line);
		line.clear();
	}
	if (header.data() == 0)
	{
		this->_header.Methode = "CLOSE";
		return (false);
	}

	std::vector<std::string>::iterator it = header.begin();
	if (it->find(" HTTP/1.1"))
	{
		if (!it->compare(0, 5, "GET /"))
			this->_header.Methode = "GET";
		else if (!it->compare(0, 6, "POST /"))
			this->_header.Methode = "POST";
		else if (!it->compare(0, 8, "DELETE /"))
			this->_header.Methode = "DELETE";
		else
		{
			this->_header.Methode = "BAD";
			return (true);
		}
		s_str = it->find_first_of("/");
		e_str = it->find_first_of(" \r\v\t\f\n", s_str);
		if (s_str == e_str || e_str == it->npos)
		{
			std::cout << "unvailable header!" << std::endl;
			return (false);
		}
		this->_header.Dir = it->substr(s_str, it->find_first_of(" \v\t\f\n\r", s_str) - s_str);
//		this->_header.Dir = this->_header.Dir.substr(0, this->_header.Dir.find_first_of(' '));
		//this->_header.Dir.append(*it, s_str, e_str - s_str);
	}
	else
	{
		this->_header.Methode = "NOT_SUPPORTED";
		return (false);
	}

	for (++it; it != header.end() && *it->data() != '\r' && *it->data() != 0; it++)
	{
		if (!it->find("Host:"))
			this->_header.Host = it->substr(it->find_last_of(' ') + 1, it->find_last_of(':') - it->find_last_of(' ') - 1);
		else if (!it->find("Accept:")) 
			ft::split_to_vectors(this->_header.Accept, it->data());
		else if (!it->find("User-Agent:"))
			this->_header.User_Agent = it->substr(it->find_last_of(' ') + 1, it->find_last_of('\r') - it->find_last_of(" ") - 1);
		else if (!it->find("Cookie:"))
		{
			if (!ft::split_to_mapss(this->_header.Cookie, it->data()))
				return (false);
		}
		else if (!it->find("Keep-Alive:"))
			this->_header.Connexion = std::stoul(it->substr(it->find_last_of(' ') + 1));
		else if (!it->find("Connection:"))
			this->_header.Connexion = it->substr(it->find_last_of(' ') + 1, it->find_last_of('\r') - it->find_last_of(" ") - 1);
		else if (it->find("Content-Length:") == 0)
			this->_header.Content_Length = std::strtol(it->substr(it->find(' ') + 1).c_str(), NULL, 10);
		else if (!it->find("Content_Type:")) 
			ft::split_to_vectors(this->_header.Content_Type, it->data());
		else if (!it->find("Content_Encoding:")) 
			ft::split_to_vectors(this->_header.Content_Encoding, it->data());
		else if (!it->find("Transfer_Encoding:")) 
			ft::split_to_vectors(this->_header.Transfer_Encoding, it->data());
		else
			ft::split_to_maposs(this->_header.other, it->data());
	}
	this->_header.split_dir();
	this->_timeout = ::time(NULL);
	this->_header.time_out = this->_timeout;
	#ifdef DEBUG
		this->_header.print_all();
	#endif
	return (true);
}

void	Client::continue_client(fd_set *fdset)
{
	// SHOULD POST METHOD OR MAYBE DELETE ONLY
	if (this->_chunked)
		this->chunk();
	else if (FD_ISSET(this->_cgi, fdset))
		this->take_data();
	else
	{
		if (!_header.Methode.compare("GET"))
		{
			std::cout << "GET METHODE" << std::endl;
			this->_working = false;
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
}

void	Client::execute_client()
{
	// THERE SEND ARE ONLY FOR PRE-TEST...
	#ifdef DEBUG
		std::cout << "\e[100m---------- HEADER CLIENT NUMBER " << this->_sock_fd << " ---------------" << std::endl;
		std::cout << _header.Methode + " " + _header.Dir << std::endl;
		std::cout << "Host: " + _header.Host + "\e[0m" << std::endl;
	#endif
	if (_header.Methode.compare("GET") == 0)
	{
		std::cout << "GET METHODE" << std::endl;
		send(this->_sock_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 52\r\n\r\n<html><head></head><body>GET ERROR 404</body></html>\0", 99, MSG_OOB); // , NULL, 0);
		this->_working = false;
		this->_header.clear();
	}
	else if (_header.Methode.compare("POST") == 0)
	{
		std::cout << "POST METHODE" << std::endl;
		send(this->_sock_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 53\r\n\r\n<html><head></head><body>POST ERROR 404</body></html>\0", 100, MSG_OOB); // , NULL, 0);
		this->_working = true;
	}
	else if (_header.Methode.compare("DELETE") == 0)
		std::cout << "DELETE METHODE" << std::endl;
	else
		std::cout << "BAD REQUEST / BAD HEADER" << std::endl;
	
}

void	Client::chunk()
{
	
}

void	Client::take_data()
{
	
}