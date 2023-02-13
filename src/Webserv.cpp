/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/02/13 16:09:38 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

webserv::webserv()
{
	if (this->created)
		throw err_init();
	this->created = true;
	std::cout << "Server created" << std::endl;
}

webserv::~webserv()
{
	this->stop_all(this->servers.begin());
	/* for (std::vector<config>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		delete it;
	} */
	this->created = false;
}

void webserv::add(std::vector<std::map<std::string, std::string> > const& config)
{
	(void)config;
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

const char *webserv::err_init::what() const throw()
{
	return ("Instance webserv already init!");
}