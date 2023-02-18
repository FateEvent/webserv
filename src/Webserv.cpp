/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/02/18 22:01:50 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

Webserv::Webserv(std::multimap<std::string, std::string>& config) : nbr_server(0)
{
	if (this->created)
		throw err_init();
	this->created = true;
	std::multimap<std::string, std::string>::iterator it = config.begin();
	if (!it->first.compare("BLOCK") && !it->second.compare("http"))
		this->mainconfig = config;
	else
		throw err_init();
}

config	Webserv::operator[](size_t index)
{
	return (this->servers.at(index));
}

void	Webserv::close(std::vector<config>::iterator &instance)
{
	try
	{
		this->stop(instance);
		this->remove(instance);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void	Webserv::prepare_all(std::vector<config>::iterator &instance)
{
	for (; instance != this->servers.end(); instance++)
		prepare(instance);
}

/**
 * @brief Create a socket for the instance passed by iterator
 * 
 * @param instance Iterator of the instance to create a socket
 */
void	Webserv::prepare(std::vector<config>::iterator &instance)
{
	if (instance->prepare == false && (instance->sock_fd = socket(instance->addr.sin_family, instance->type, 0)))
	//socket(instance->addr.sin_family, instance->type, instance->addr.sin_addr.s_addr)))
		instance->prepare = true;
	else
		std::cout << "Info: " + instance->name + " have already a socket!" << std::endl;
}

/**
 * @brief Add multiple instance by en vector of multimap<string, string>
 * 
 * @param server Iterator of vector of multimap<string, string>
 */
void	Webserv::add(std::vector<std::multimap<std::string, std::string> > &server)
{
	for (std::vector<std::multimap<std::string, std::string> >::iterator it = server.begin(); it != server.end(); it++)
	{
		try
		{
			this->add(*it);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

/**
 * @brief Stop and unbind a selected server given by iterator
 * 
 * @param stop The instance iterator to shutdown.
 */
void	Webserv::stop(std::vector<config>::iterator &server)
{
	if (server->active == true)
	{
		if (!::close(server->sock_fd))
			return; //throw ("intern problem.");
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
void	Webserv::stop_all(std::vector<config>::iterator &server)
{
	for (;server != this->servers.end(); server++)
	{
		try
		{
			std::cout << "Stopping server " << server->name << "...." << std::endl;
			this->stop(server);
		}
		catch (std::exception &e)
		{
			std::cout << "\033[0;31mServer " << server->name << "was stopped not correctly!\033[0m" << std::endl;
			std::cerr << e.what() << std::endl;
		}
	}
	std::cout << "All server stopped!" << std::endl;
}

/**
 * @brief Bind the select instance by iterator to a port
 * 
 * @param bind The iterator of instance to bind
 */
void	Webserv::bind(std::vector<config>::iterator &bind)
{
	int	res;

	if (bind->sock_fd)
	{
		//bind->addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // OK ON MY SERVER (MULTI NETWORK CARD) NOT OK WITH MY LABTOP (MAYBE ONLY WIFI??!)
		//htonl(INADDR_LOOPBACK);
		//inet_addr("127.0.0.1");
		//bind->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		res = ::bind(bind->sock_fd, reinterpret_cast<sockaddr *>(&bind->addr), sizeof(bind->addr));
		if (res)
		{
			bind->active = false;
			if (errno == 48 || errno == 98)
				std::cout << "\033[0;31mAddress and protocol already used. Can't bind " << bind->name << "\033[0m" << std::endl;
			else if (errno == 13)
				std::cout << "\033[0;31mAcces denied. Can't bind " << bind->name << "\033[0m" << std::endl;
			else
				std::cout << "\033[0;31mError: instance " + bind->name + " not initialized!\033[0m" << std::endl;
		}
		else
		{
			bind->active = true;
			//FD_SET(bind->sock_fd, &this->readfd);
			std::cout << "Instance \033[0;33m" + bind->name + "\033[0m on port \033[0;33m" + std::to_string(bind->port)  + "\033[0m is now bound."<< std::endl;
		}
	}
	else
	{
		bind->active = false;
		throw ("Error: instance " + bind->name + " not initialized!");
	}
}

/**
 * @brief Bind all server to a port
 * 
 * @param server  The iterator of vector stocked all instance
 */
void	Webserv::bind_all(std::vector<config>::iterator &server)
{
	for (; server != this->servers.end(); server++)
	{
		try
		{
			this->bind(server);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

/**
 * @brief Remove a selected instance passing by iterator
 * 
 * @param old The iterator of select instance to remove
 */
void	Webserv::remove(std::vector<config>::iterator &old)
{
	if (old->active == true)
		stop(old);
	this->servers.erase(old);
}

/**
 * @brief Return number of instance
 * 
 * @return unsigned 
 */
unsigned	Webserv::get_nbr_server() const
{
	return (this->nbr_server);
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

	for (std::multimap<std::string, std::string>::const_iterator it = mainconfig.begin(); it != mainconfig.end(); it++)
	{
		info.append("\033[0;36m" + it->first + " | " + it->second + "\033[0m\n");
	}
	return (info);
}

/**
 * @brief Return info about instance given by iterator
 * 
 * @param other The iterator of selected instance
 * @return std::string Returned information in en string.
 */
std::string	Webserv::get_info_on(std::vector<config>::const_iterator &other) const
{
	std::string	info;
	info.append("FD:     " + std::to_string(other->sock_fd) + "\n" +
				"Name:   " + other->name + "\n" +
				"Address:" + other->ip + "\n" +
				"Port:   " + std::to_string(other->port) + "\n" +
				"Root:   " + other->root + "\n" +
				"Index:  " + other->index + "\n" +
				"Prepare:" + std::to_string(other->prepare) + "\n" +
				"Active: " + std::to_string(other->active) + "\n\n");
	return (info);
}

std::string	Webserv::get_info_instance() const
{
	std::string	info;
	for (std::vector<config>::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		info.append(get_info_on(it));
	}
	return (info);
}

Webserv::~Webserv()
{
	//this->stop_all(this->servers.begin());
	this->created = false;
}

std::vector<config>::iterator	Webserv::begin()
{
	return (this->servers.begin());
}

std::vector<config>::iterator	Webserv::end()
{
	return (this->servers.end());
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

void	Webserv::add(std::multimap<std::string, std::string> server)
{
	config	ret = {"","","","",{},0,0,0,0,0,0,0,{}}; // Last bracket for map<> work on c++11. Need to fix this for c++98
	for (std::multimap<std::string, std::string>::iterator it = server.begin(); it != server.end(); it++)
	{
		if (!it->first.compare("BLOCK") && !it->second.compare("server"))
			continue;
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
				throw ("Socket protocol invalid!");
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
				throw ("Type incorrect in config file!");
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
			throw ("Unknow parameter in config file!");
		}
		std::cout << "\033[0;33m" + it->first << " | " << it->second + "\033[0m" << std::endl;
	}
	check_instance(ret);
	this->servers.push_back(ret);
	this->nbr_server++;
}

/**
 * @brief Check if the instance have the right parameter
 * if some non importante param is not present, then set
 * as delfault value in mainconfig
 * 
 * @param conf Struct config to check
 */
void	Webserv::check_instance(config &conf)
{
	std::multimap<std::string, std::string>::iterator main;
	if (conf.name.empty())
		throw ("Error in config file, miss server name.");
	if (conf.root.empty())
		throw ("Error in config file, miss root directory.");
	if (conf.index.empty())
	{
		main = this->mainconfig.find("index");
		conf.index = main->second;
	}
	if (conf.ip.empty())
	{
		main = this->mainconfig.find("default");
		conf.ip = main->second;
	}
	if (!conf.domain)
	{
		conf.domain = AF_INET;
		conf.addr.sin_family = conf.domain;
	}
	if (!conf.type)
		conf.type = SOCK_STREAM;
	if (conf.max_client)
	{
		main = this->mainconfig.find("max_client");
		conf.max_client = std::stoul(main->second);
	}
	if (!conf.port)
		throw ("Error in config file, miss miss port on instance.");
}


void	Webserv::listen_all()
{
	for (std::vector<config>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		if (it->active && it->prepare)
		{
			try
			{
				listen(*it);
			}
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
			std::cout << "Instance " + it->name + " ignored. Not active or prepared." << std::endl;
	}
}

void	Webserv::listen(config &instance)
{
	if (::listen(instance.sock_fd, instance.max_client) != 0)
		throw ("Error on listen for " + instance.name + " with error " + std::to_string(errno));
}

int	Webserv::get_greaterfd() const
{
	int	nbr = 0;
	std::vector<config>::const_iterator it = this->servers.begin();
	for (; it != this->servers.end(); it++)
		if (it->active && it->sock_fd > 0 && it->sock_fd >= nbr)
			nbr = it->sock_fd + 1;
	return (nbr);
}

void	Webserv::fd_rst()
{
	FD_ZERO(&this->writefd);
	FD_ZERO(&this->readfd);
	FD_ZERO(&this->errfd);
	std::vector<config>::const_iterator it = this->servers.begin();
	for (; it != this->servers.end(); it++)
		if (it->active && it->sock_fd > 0)
			FD_SET(it->sock_fd, &this->readfd);
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