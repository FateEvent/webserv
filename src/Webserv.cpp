/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/02/14 19:52:17 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

webserv::webserv(std::map<std::string, std::string>& config)
{
	if (this->created)
		throw err_init();
	this->created = true;
	this->mainconfig = config;
}

webserv::~webserv()
{
	this->stop_all(this->servers.begin());
	this->created = false;
}

void webserv::add(std::vector<std::map<std::string, std::string> >::iterator const& server)
{
	for (std::map<std::string,std::string>::iterator it = server->begin(); it != server->end(); it++)
	{
		std::cout << "\033[0;33m" + it->first << " | " << it->second + "\033[0m" << std::endl;
	}
}

















/**
 * @brief Stop and unbind a selected server given by iterator
 * 
 * @param stop The instance iterator to shutdown.
 */
void webserv::stop(std::vector<config>::iterator &server)
{
	if (server->active == true)
	{
		if (!::close(server->sock_fd))
			throw ("intern problem.");
	}
	else
		throw ("Server was already shutdown.");
}

/**
 * @brief Stop all instance in a vector given by vector iterator
 * 
 * @param server The iterator of vector stocked all instance
 */
void webserv::stop_all(std::vector<config>::iterator server)
{
	while (server != this->servers.end())
	{
		try
		{
			this->stop(server);
		}
		catch (std::exception &e)
		{
			std::cout << "\033[0;31mServer " << server->name << "was stopped not correctly!\033[0m" << std::endl;
			std::cerr << e.what() << std::endl;
		}
		server++;
	}
	std::cout << "All server stopped!" << std::endl;
}

/**
 * @brief Bind the select instance by iterator to a port
 * 
 * @param bind The iterator of instance to bind
 */
void webserv::bind(std::vector<config>::iterator &bind)
{
	int	res;

	if (bind->sock_fd)
	{
		res = ::bind(bind->sock_fd, reinterpret_cast<sockaddr *>(&bind->addr), sizeof(bind->addr));
		if (res)
		{
			bind->active = false;
			throw ("Error: instance " + bind->name + " not initialized!");
		}
		bind->active = true;
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
void webserv::bind_all(std::vector<config>::iterator server)
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
	std::cout << "Server name: " << server->name << " binded." << std::endl;
}

/**
 * @brief Remove a selected instance passing by iterator
 * 
 * @param old The iterator of select instance to remove
 */
void webserv::remove(std::vector<config>::iterator &old)
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
unsigned webserv::get_nbr_server() const
{
	return (static_cast<unsigned int>(*this->nbr_server));
}

const char *webserv::err_init::what() const throw()
{
	return ("\033[0;31mInstance webserv already init!\033[0m");
}

/**
 * @brief Return the info about server
 * 
 * @return std::string Returned information in a string
 */
std::string webserv::get_info_server() const
{
	std::string info;

	for (std::map<std::string, std::string>::const_iterator it = mainconfig.begin(); it != mainconfig.end(); it++)
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
std::string webserv::get_info_on(std::vector<config>::iterator &other) const
{
	std::string	info;
	info.append("FD:     " + std::to_string(other->sock_fd) +
				"Name:   " + other->name +
				"Address:" + std::to_string(other->addr.sin_addr.s_addr) +
				"Port:   " + std::to_string(other->addr.sin_port) +
				"Root:   " + other->root +
				"Index:  " + other->index +
				"Active: " + std::to_string(other->active));
	return (info);
}