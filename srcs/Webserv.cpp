/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/03/25 20:09:47 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

Webserv::Webserv(std::multimap<std::string, std::multimap<std::string, std::string> >& config) : _nbr_server(0)
{
	if (this->created)
		throw err_init(); // Classe webserv already exist
	if (config.find("http") == config.end())
		throw std::invalid_argument("No http in config file!"); //("No default configuration set in config file.");
	std::multimap<std::string, std::multimap<std::string, std::string> >::iterator itconfig = config.find("http");
	std::multimap<std::string, std::string> it = itconfig->second;
	this->_base.name.assign("Default");
	this->_base.root.assign(it.find("root")->second);
	this->_base.index.assign(it.find("index_page")->second);;
	this->_base.port = std::stoul(it.find("listen")->second.data(), NULL, 10);
	this->_base.addr.sin_addr.s_addr = INADDR_ANY;
	this->_base.addr.sin_family = AF_INET;
	this->_base.addr.sin_port = htons(this->_base.port);
	this->_base.domain = AF_INET;
	this->_base.type = SOCK_STREAM;
	this->_base.max_client = std::strtoul(it.find("max_client")->second.data(), NULL, 10);
	if (it.find("error_page") != it.end())
		ft::parse_err_page(this->_base.error_page, itconfig->second);
	// Do socket, bind and listen on general port (usualy on port 80 given in config file)
	this->_base.sock_fd = socket(this->_base.addr.sin_family, this->_base.type, 0);
	fcntl(this->_base.sock_fd, F_SETFL, O_NONBLOCK);
	if (::bind(this->_base.sock_fd, reinterpret_cast<sockaddr *>(&this->_base.addr), sizeof(this->_base.addr)) != 0)
		throw std::invalid_argument("Cannot bind port! Usualy already used."); // ("Server bind error.");
	if (::listen(this->_base.sock_fd, this->_base.max_client) != 0)
		throw std::invalid_argument("Cannot listen port! Usualy already used."); // ("Server listen error.");
	// Set prepare and active on true
	this->_base.prepare = true;
	this->_base.active = false;
	this->created = true;
}

Webserv::~Webserv()
{
	//this->stop_all(this->servers.begin());
	this->created = false;
}

std::map<std::string, config>::iterator	Webserv::begin()	{ return (this->_servers.begin());}
std::map<std::string, config>::iterator	Webserv::end()		{ return (this->_servers.end());}

void	Webserv::close(std::map<std::string, config>::iterator &instance)
{
	try
	{
		this->stop(instance->second);
		this->remove(instance);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void	Webserv::prepare_all()
{
	for (std::map<std::string, config>::iterator instance = this->_servers.begin(); instance != this->_servers.end(); instance++)
		prepare(instance->second);
}

/**
 * @brief Create a socket for the instance passed by iterator
 * 
 * @param instance Iterator of the instance to create a socket
 */
void	Webserv::prepare(config &instance)
{
	if (instance.prepare == false && instance.port != this->_base.port)
	{
		if ((instance.sock_fd = socket(instance.addr.sin_family, instance.type, 0)) != -1)
		{
			//socket(instance.addr.sin_family, instance.type, instance.addr.sin_addr.s_addr)))
			instance.prepare = true;
			fcntl(instance.sock_fd, F_SETFL, O_NONBLOCK);
		}
	}
		else
		std::cout << "Info: " + instance.name + " have already a socket!" << std::endl;
}

/**
 * @brief Add multiple instance by en vector of multimap<string, string>
 * 
 * @param server Iterator of vector of multimap<string, string>
 */
void	Webserv::add(std::multimap<std::string, std::multimap<std::string, std::string> > &server)
{
	for (std::multimap<std::string, std::multimap<std::string, std::string> >::iterator it = server.begin(); it != server.end(); it++)
	{
		if (it->first.compare("server") == 0)
		{
			try
			{
				this->add(it->second);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
}

/**
 * @brief Stop and unbind a selected server given by iterator
 * 
 * @param stop The instance iterator to shutdown.
 */
void	Webserv::stop(config &server)
{
	if (server.active == true && server.port != 80)
	{
		if (!::close(server.sock_fd))
			return; //throw ("intern problem.");
		server.sock_fd = -1;
		std::cout << "Closed..." << std::endl;
	}
	else
		std::cout << "Not active, not need to close..." << std::endl;
//		throw ("Server was already shutdown.");
}

/**
 * @brief Stop all instance in a vector given by vector iterator
 * 
 * @param server The iterator of vector stocked all instance
 */
void	Webserv::stop_all()
{
	for (std::map<std::string, config>::iterator it = this->_servers.begin() ; it != this->_servers.end(); it++)
	{
		try
		{
			std::cout << "Stopping server " << it->second.name << "...." << std::endl;
			this->stop(it->second);
		}
		catch (std::exception &e)
		{
			std::cout << "\033[0;31mServer " << it->second.name << "was stopped not correctly!\033[0m" << std::endl;
			std::cerr << e.what() << std::endl;
		}
	}
	std::cout << "All server stopped!" << std::endl;
}

/**
 * @brief Remove a selected instance passing by iterator
 * 
 * @param old The iterator of select instance to remove
 */
void	Webserv::remove(std::map<std::string, config>::iterator &old)
{
	if (old->second.active == true)
		stop(old->second);
	this->_servers.erase(old);
}

/**
 * @brief Bind the select instance by iterator to a port
 * 
 * @param bind The iterator of instance to bind
 */
void	Webserv::bind(config &bind)
{
	int	res = 0;

	if (bind.sock_fd)
	{
		//bind.addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // OK ON MY SERVER (MULTI NETWORK CARD) NOT OK WITH MY LABTOP (MAYBE ONLY WIFI??!)
		//htonl(INADDR_LOOPBACK);
		//inet_addr("127.0.0.1");
		//bind.addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind.port != this->_base.port)
		{
			if (bind.port == 0)
			{
				bind.active = true;
				bind.port = this->_base.port;
				std::cout << "Instance \033[0;33m" + bind.name + "\033[0m is vhost of main port \033[0;33m" + std::to_string(bind.port)  + "\033[0m."<< std::endl;
				return ;
			}
			else
			{
				res = ::bind(bind.sock_fd, reinterpret_cast<sockaddr *>(&bind.addr), sizeof(bind.addr));
				fcntl(bind.sock_fd, F_SETFL, O_NONBLOCK);
			}
		}
		if (res)
		{
			bind.active = false;
			if (errno == 48 || errno == 98)
				std::cout << "\033[0;31mAddress and protocol already used. Can't bind " << bind.name << "\033[0m" << std::endl;
			else if (errno == 13)
				std::cout << "\033[0;31mAcces denied. Can't bind " << bind.name << "\033[0m" << std::endl;
			else
				std::cout << "\033[0;31mError: instance " + bind.name + " not initialized!\033[0m" << std::endl;
		}
		else
		{
			bind.active = true;
			//FD_SET(bind.sock_fd, &this.readfd);
			std::cout << "Instance \033[0;33m" + bind.name + "\033[0m on port \033[0;33m" + std::to_string(bind.port)  + "\033[0m is now bound."<< std::endl;
		}
	}
	else
	{
		bind.active = false;
		throw ("Error: instance " + bind.name + " not initialized!");
	}
}

/**
 * @brief Bind all server to a port
 * 
 * @param server  The iterator of vector stocked all instance
 */
void	Webserv::bind_all()
{
	for (std::map<std::string, config>::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
	{
		try
		{
			this->bind(server->second);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

const char	*Webserv::err_init::what() const throw()	{ return ("\033[0;31mInstance webserv already init!\033[0m");}

void	Webserv::add(std::multimap<std::string, std::string> &server)
{
	config	ret(server);
	_check_instance(ret);
	ret._base = &this->_base;
	this->_servers.insert(std::make_pair(ret.name, ret));
	this->_nbr_server++;
}

/**
 * @brief Check if the instance have the right parameter
 * if some non importante param is not present, then set
 * as delfault value in mainconfig
 * 
 * @param conf Struct config to check
 */
void	Webserv::_check_instance(config &conf)
{
	std::multimap<std::string, std::string>::iterator main;
	if (conf.name.empty())
		throw std::invalid_argument("Error in config file, miss server name.");
	if (conf.root.empty())
		throw std::invalid_argument("Error in config file, miss root directory.");
	if (conf.index.empty())
		conf.index = this->_base.index;
	if (conf.ip.empty())
		conf.ip = this->_base.ip;
	if (!conf.domain)
	{
		conf.domain = AF_INET;
		conf.addr.sin_family = conf.domain;
	}
	if (!conf.type)
		conf.type = SOCK_STREAM;
	if (conf.max_client)
		conf.max_client = this->_base.max_client;
	if (!conf.port)
		std::cout << conf.name << " is a virtual host (no listen in config file)" << std::endl;
		//throw std::invalid_argument("Error in config file, miss miss port on instance.");
}

void	Webserv::listen_all()
{
	for (std::map<std::string, config>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		if (it->second.active && it->second.prepare)
		{
			try
			{
				listen(it->second);
			}
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
			std::cout << "Instance " + it->second.name + " ignored. Not active or prepared." << std::endl;
	}
}

void	Webserv::listen(config &instance)
{
	if (::listen(instance.sock_fd, instance.max_client) != 0)
		throw ("Error on listen for " + instance.name + " with error " + std::to_string(errno));
}



unsigned	Webserv::get_nbr_server() const		{ return (this->_nbr_server);}

/**
 * @brief Return the info about server
 * 
 * @return std::string Returned information in a string
 */
std::string	Webserv::get_info_server() const
{
	std::string info;

	info.append("\033[0;36mNom de domaine: " + _base.name + "\033[0m\n");
	info.append("\033[0;36mAdresse IP: " + _base.ip + "\033[0m\n");
	info.append("\033[0;36mPort: " + std::to_string(_base.port) + "\033[0m\n");
	info.append("\033[0;36mRoot: " + _base.root + "\033[0m\n");
	info.append("\033[0;36mIndex: " + _base.index + "\033[0m\n");
	return (info);
}

/**
 * @brief Return info about instance given by iterator
 * 
 * @param other The iterator of selected instance
 * @return std::string Returned information in en string.
 */
std::string	Webserv::get_info_on(const config &other) const
{
	std::string	info;
	info.append("FD:     " + std::to_string(other.sock_fd) + "\n" +
				"Name:   " + other.name + "\n" +
				"Address:" + other.ip + "\n" +
				"Port:   " + std::to_string(other.port) + "\n" +
				"Root:   " + other.root + "\n" +
				"Index:  " + other.index + "\n" +
				"Prepare:" + std::to_string(other.prepare) + "\n" +
				"Active: " + std::to_string(other.active) + "\n\n");
	return (info);
}

std::string	Webserv::get_info_instance() const
{
	std::string	info;
	for (std::map<std::string, config>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
		info.append(get_info_on(it->second));
	return (info);
}

void	Webserv::fd_rst()
{
	FD_ZERO(&this->writefd);
	FD_ZERO(&this->readfd);
	FD_ZERO(&this->errfd);

	for (std::map<std::string, config>::const_iterator it = this->_servers.begin(); 
			it != this->_servers.end(); it++)
		if (it->second.active && it->second.sock_fd > 0)
			FD_SET(it->second.sock_fd, &this->readfd);
	// Set FD on client if exist
	for (std::map<int, Client>::iterator it = this->_client.begin();
			it != this->_client.end(); it++)
	{
		FD_SET(it->first, &this->readfd);
		if (it->second.is_cgi())
			FD_SET(it->second.get_fd_cgi(), &this->readfd);
	}
	// Set FD on Server default port
	FD_SET(this->_base.sock_fd, &this->readfd);
}

int	Webserv::get_greaterfd() const
{
	int	nbr = this->_base.sock_fd + 1;
	
	for (std::map<std::string, config>::const_iterator it = this->_servers.begin();
			it != this->_servers.end(); it++)
		if (it->second.active && it->second.sock_fd > 0 && it->second.sock_fd >= nbr)
			nbr = it->second.sock_fd + 1;
 	for (std::map<int, Client>::const_iterator it = this->_client.begin();
			it != this->_client.end(); it++)
	{
		if (it->first >= nbr)
			nbr = it->first + 1;
		if (it->second.get_fd_cgi() >= nbr)
			nbr = it->second.get_fd_cgi() + 1;
	}
	return (nbr);
}

fd_set&	Webserv::get_writefd()		{ return (this->writefd);}
fd_set&	Webserv::get_readfd()		{ return (this->readfd);}
void	Webserv::timeout(int sec)	{ this->_timeout = (timeval){sec,0};}
timeval&	Webserv::timeout()		{ return (this->_timeout); }

// Check if is a new connexion on main config or on vhost
// If it's a new client create the client and set right variables.
void	Webserv::check_server()
{
	if (FD_ISSET(this->_base.sock_fd, &this->readfd))
	{
		int				sock_fd;
		sockaddr_in		addr;
		socklen_t		socklen;
		header			head;

		std::cout << "Ask of new client on vhost." << std::endl;
		socklen = sizeof(addr);
		sock_fd = accept(_base.sock_fd, reinterpret_cast<sockaddr *>(&addr), reinterpret_cast<socklen_t *>(&socklen));
		if (sock_fd == -1)
		{
			std::cout << strerror(errno) << std::endl;
			throw std::invalid_argument("Socket error in vhost!");
		}
		fcntl(sock_fd, F_SETFL, O_NONBLOCK);
		if (!ft::parse_header(sock_fd, head))
			::close(sock_fd);
		std::map<std::string, config>::iterator serv = this->_servers.begin();
		for (; serv != this->_servers.end(); serv++)
			if (serv->first.compare(head.Host) == 0)
				break;
		if (serv == this->_servers.end())
		{
			std::cout << PURPLE << "Vhost don't exist, connexion close." << RST << std::endl;
			::close(sock_fd);
		}
		else
		{
			try
			{
				Client *ret = new Client(serv->second, addr, socklen, sock_fd, head);
				this->_client.insert(std::make_pair(sock_fd, *ret));
				std::cout << GREEN << "New vhost client accepted on connexion number " << ret->get_sockfd() << "." << RST << std::endl;
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	for (std::map<std::string, config>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		if (it->second.active && FD_ISSET(it->second.sock_fd, &this->readfd))
		{
			std::cout << "Ask of new client." << std::endl;
			try
			{
				Client *ret = new Client(it->second);
				this->_client.insert(std::make_pair(ret->get_sockfd(), *ret));
				std::cout << GREEN << "New client accepted on connexion number " << ret->get_sockfd() << "." << RST << std::endl;
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			//FD_CLR(it->second.sock_fd, &this->readfd);
		}
}

// Check client if exist and if working to call the right function
void	Webserv::check_client()
{
	std::vector<int>	to_close;
	for (std::map<int, Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++)
	{
/* 		if ((FD_ISSET(it->second.get_sockfd(), &this->readfd) && it->second.is_working()) ||
			(it->second.get_fd_cgi() > 0 && FD_ISSET(it->second.get_fd_cgi(), &this->readfd)))
		{
			it->second.continue_client(&this->readfd);
			continue;
		} */
		if (FD_ISSET(it->second.get_sockfd(), &this->readfd) &&
				!it->second.is_ready())
		{
			if (it->second.new_request())
			{
				//std::cout << GREEN << "Valid Header." << RST << std::endl;
				FD_CLR(it->second.get_sockfd(), &this->readfd);
			}
			else
			{
				// send error 400 to client.
				to_close.push_back(it->second.get_sockfd());
			}
			continue;
		}
		else if (FD_ISSET(it->second.get_sockfd(), &this->readfd) &&
					it->second.is_ready())// && it->second.is_ready())// ELSE ONLY FOR TEST// AFTER WHEN NEW REQUETE IS OK is_working SHOULD RETURN TRUE! 
		{
			to_close.push_back(it->second.get_sockfd());
			std::cout << YELLOW << "CLOSE CONNEXION" << RST << std::endl;
			FD_CLR(it->second.get_sockfd(), &this->readfd);
		}
	}
	if (!to_close.empty())
	{
		for (std::vector<int>::iterator it = to_close.begin(); it != to_close.end(); ++it)
		{
			std::cout << GREEN << "Connexion number: " << *it << " close" << RST << std::endl;
			::close(*it);
			this->_client.erase(*it);
		}
	}
}

void	Webserv::exec_client()
{
	std::list<int> toclose;
	for (std::map<int, Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++)
	{
		if (it->second.get_methode().empty())
			continue;
		if ((it->second.get_methode().compare("BAD") == 0 || it->second.get_methode().compare("CLOSE") == 0))
		{
			send(it->second.get_sockfd(), "HTTP/1.1 400 Bad Request\r\nContent-Length: 60\r\n\r\n<html><head></head><body>BAD REQUEST ERROR 400</body></html>\0", 109, MSG_OOB); // , NULL, 0);
			it->second.clear_header();
			toclose.push_back(it->second.get_sockfd());
		}
		else if (!it->second.is_seeding() && it->second.is_ready()) // else if (!it->second.get_methode().empty() && !it->second.is_working() && !it->second.is_seeding() && it->second.is_ready())
			it->second.execute_client(it->second.check_location());
		else if (it->second.is_seeding() && it->second.is_ready())
		{
			if (it->second.continue_client(&this->readfd))
				toclose.push_back(it->first);	//check keep alive...
			else
				this->timeout(0);
		}
	}
	for (std::list<int>::iterator it = toclose.begin(); it != toclose.end(); it++)
	{
		::close(*it);
		this->_client.erase(*it);
		std::cout << BLUE << "Socket " << *it << " closed." << RST << std::endl;
	}
}
