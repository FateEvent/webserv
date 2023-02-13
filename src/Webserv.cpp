/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/02/14 00:09:34 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

webserv::webserv(std::map<std::string, std::string>& config)
{
	if (this->created)
		throw err_init();
	this->created = true;
	std::cout << "Server created" << std::endl;
	for ( std::map<std::string,std::string>::iterator it = config.begin(); it != config.end(); it++)
	{
		std::cout << "\033[0;36m" + it->first << " | " << it->second + "\033[0m" << std::endl;
	}
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

void webserv::remove(std::vector<config>::iterator &old)
{
	(void)old;
}

unsigned webserv::get_nbr_server() const
{
	return (static_cast<unsigned int>(*this->nbr_server));
}

void webserv::bind(std::vector<config>::iterator &bind)
{
	(void)bind;
}

void webserv::stop(std::vector<config>::iterator &stop)
{
	(void)stop;
}

void webserv::bind_all(std::vector<config>::iterator server)
{
	std::cout << "Server name: " << server->name << " binded." << std::endl;
}

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
			std::cerr << e.what() << std::endl;
		}
		server++;
	}
	std::cout << "All server stopped!" << std::endl;
}

std::string webserv::get_info_on(std::vector<config>::iterator &other) const
{
	(void)other;
	return ("return test");
}

const char *webserv::err_init::what() const throw()
{
	return ("\033[0;31mInstance webserv already init!\033[0m");
}