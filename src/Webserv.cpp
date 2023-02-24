/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/02/24 09:53:30 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

Webserv::Webserv(std::multimap<std::string, std::multimap<std::string, std::string> >& config) : _nbr_server(0)
{
	if (this->created)
		throw err_init();
	if (config.find("http") == config.end())
		throw err_init(); //("No default configuration set in config file.");
	std::multimap<std::string, std::multimap<std::string, std::string> >::iterator itconfig = config.find("http");
	std::multimap<std::string, std::string> it = itconfig->second;
	//this->_mainconfig = config;
		// Convert IP to uint32 for sockaddr
	this->_base.name.assign("Default");
	this->_base.root.assign(it.find("root")->second);
	this->_base.index.assign(it.find("index_page")->second);
	this->_base.ip.assign(it.find("default")->second);
	this->_base.port = std::stoul(it.find("listen")->second.data(), NULL, 10);
	{
	unsigned int ip[4];
	if (!sscanf(this->_base.ip.data(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]))
		throw ("IP bad host in it file");
	//Do sockaddr_in
	this->_base.addr.sin_addr.s_addr =  (ip[0] % 256 << 0 | 0) |\
								(ip[1] % 256 << 8 | 0) |\
								(ip[2] % 256 << 16 | 0) |\
								(ip[3] % 256 << 24 | 0);
	}
	this->_base.addr.sin_family = AF_INET;
	this->_base.addr.sin_port = htons(this->_base.port);
	this->_base.domain = AF_INET;
	this->_base.type = SOCK_STREAM;
	this->_base.max_client = std::strtoul(it.find("max_client")->second.data(), NULL, 10);
	if (it.find("error_page") != it.end())
		this->_base.error_page = {}; // parsse_error(config.find("error_page")); // Give iterator of error_page to parse end return a map<int, string>
	// Do socket, bind and listen on general port (usualy on port 80 given in config file)
	this->_base.sock_fd = socket(this->_base.addr.sin_family, this->_base.type, 0);
	if (::bind(this->_base.sock_fd, reinterpret_cast<sockaddr *>(&this->_base.addr), sizeof(this->_base.addr)) != 0)
		std::cout << "BIND PROBLEM " + std::to_string(errno) << std::endl;
	if (::listen(this->_base.sock_fd, this->_base.max_client) != 0)
		std::cout << "LISTEN PROBLEM" << std::endl;
	// Set prepare and active on true
	this->_base.prepare = true;
	this->_base.active = false;
	this->created = true;


//std::cout << _base.domain + " \n";
std::cout << std::to_string(_base.sock_fd) + " \n";
std::cout << _base.ip + " \n";
std::cout << std::to_string(_base.port) + " \n";
//std::cout << _base.domain + " \n";
//std::cout << _base.domain + " \n";
//std::cout << _base.domain + " \n";


}

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
	if (instance.prepare == false && instance.port != 80)
	{
		if ((instance.sock_fd = socket(instance.addr.sin_family, instance.type, 0)) != -1)
			//socket(instance.addr.sin_family, instance.type, instance.addr.sin_addr.s_addr)))
			instance.prepare = true;
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
		if (bind.port != 80)
			res = ::bind(bind.sock_fd, reinterpret_cast<sockaddr *>(&bind.addr), sizeof(bind.addr));
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

/**
 * @brief Return number of instance
 * 
 * @return unsigned 
 */
unsigned	Webserv::get_nbr_server() const
{
	return (this->_nbr_server);
}

const char	*Webserv::err_init::what() const throw()
{
	return ("\033[0;31mInstance webserv already init!\033[0m");
}

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

Webserv::~Webserv()
{
	//this->stop_all(this->servers.begin());
	this->created = false;
}

std::map<std::string, config>::iterator	Webserv::begin()
{
	return (this->_servers.begin());
}

std::map<std::string, config>::iterator	Webserv::end()
{
	return (this->_servers.end());
}








/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADD INSTANCE                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   PART OF SHIT                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*                                                    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	Webserv::add(std::multimap<std::string, std::string> &server)
{
	config	ret = {"","","","",{},-1,0,0,0,0,0,0,{}}; // Last bracket for map<> work on c++11. Need to fix this for c++98
	for (std::map<std::string, std::string>::iterator it = server.begin(); it != server.end(); it++)
	{
		if (!it->first.compare("name"))
		{
			if (it->second.empty())
				throw ("No instance name!");
			ret.name = it->second;
		}
		else if (!it->first.compare("protocol"))
		{
			if (it->second.empty())
				throw ("Socket protocol invalid!");
			else if (it->second.compare("IPV4") || it->second.compare("INET") || it->second.compare("AF_INET"))
				ret.domain = AF_INET;
			else if (it->second.compare("IPV6") || it->second.compare("INET6") || it->second.compare("AF_INET6"))
				ret.domain = AF_INET6;
			else if (it->second.compare("local") || it->second.compare("LOCAL"))
				ret.domain = AF_LOCAL;
			else
				ret.domain = AF_INET;
			ret.addr.sin_family = ret.domain;
		}
		else if (!it->first.compare("host"))
		{
			unsigned int ip[4];
			if (!sscanf(it->second.data(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]))
				throw ("IP bad host in config file");
			ret.ip = it->second;
			ret.addr.sin_addr.s_addr =  (ip[0] % 256 << 0 | 0) |\
										(ip[1] % 256 << 8 | 0) |\
										(ip[2] % 256 << 16 | 0) |\
										(ip[3] % 256 << 24 | 0);
		}
		else if (!it->first.compare("listen"))
		{
			if ((std::stoul(it->second) < 1) && (std::stoul(it->second) >= 0xFFFFFF))
				throw ("Invalid port");
			ret.port = std::stoul(it->second);
			ret.addr.sin_port = htons(ret.port);
		}
		else if (!it->first.compare("type"))
		{
			if (!it->second.compare("tcp"))
				ret.type = SOCK_STREAM;
			else if (!it->second.compare("udp"))
				ret.type = SOCK_DGRAM;
			else
				ret.type = SOCK_STREAM;
		}
		else if (!it->first.compare("max_client"))
		{
			int max = std::stol(it->second);
			if (!(max > 0 && max < static_cast<int>(0x7FFFFFFF)))
				throw ("Max client incorrect in instance!");
			ret.max_client = max;
		}
		else if (!it->first.compare("root"))
		{
			if (it->second.empty())
				throw ("root destination empty!");
			ret.root = it->second;
		}
		else if (!it->first.compare("index_page"))
		{
			if (it->second.empty())
				throw ("no index referenced!");
			ret.index = it->second;
		}
		else if (!it->first.compare("error_page"))
		{
			std::map<int, std::string> errnbr; // = ft::parse_err(it->second);
			std::map<int, std::string>::iterator iterr = errnbr.begin();
			for (; iterr != errnbr.end(); iterr++)
			{
				try
				{
					ret.error_page.insert(std::make_pair(iterr->first, iterr->second));
				}
				catch (std::exception &e)
				{
					throw ("Invalid error page in config file!");
				}
			}
		}
		else
		{
			std::cout << it->first << " | " << it->second << std::endl;
			//throw ("Unknown parameter in config file!");
		}
		std::cout << "\033[0;33m" + it->first << " | " << it->second + "\033[0m" << std::endl;
	}
	_check_instance(ret);
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
		throw ("Error in config file, miss server name.");
	if (conf.root.empty())
		throw ("Error in config file, miss root directory.");
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
		throw ("Error in config file, miss miss port on instance.");
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

int	Webserv::get_greaterfd() const
{
	int	nbr = this->_base.sock_fd + 1;
	std::map<std::string, config>::const_iterator it = this->_servers.begin();
	for (; it != this->_servers.end(); it++)
		if (it->second.active && it->second.sock_fd > 0 && it->second.sock_fd >= nbr)
			nbr = it->second.sock_fd + 1;
	return (nbr);
}

void	Webserv::fd_rst()
{
	FD_ZERO(&this->writefd);
	FD_ZERO(&this->readfd);
	FD_ZERO(&this->errfd);
	
	std::map<std::string, config>::const_iterator it = this->_servers.begin();
	for (; it != this->_servers.end(); it++)
		if (it->second.active && it->second.sock_fd > 0)
			FD_SET(it->second.sock_fd, &this->readfd);
	FD_SET(this->_base.sock_fd, &this->readfd);
}

fd_set&	Webserv::get_writefd()
{
	return (this->writefd);
}

fd_set&	Webserv::get_readfd()
{
	return (this->readfd);
}

timeval&	Webserv::timeout()
{
	this->_timeout.tv_sec = 1;
	this->_timeout.tv_usec = 0;
	return (this->_timeout);
}

/* Client	Webserv::make_client()
{
	for (std::vector<config>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		if (FD_ISSET(it->sock_fd, &this->readfd))
		{
			Client *ret = new Client(*it);
			ret->set_sock(it->sock_fd);
		}
	}
} */