/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:38:09 by stissera          #+#    #+#             */
/*   Updated: 2023/02/12 22:26:08 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inlcude/Webserv.hpp"

webserv::webserv()
{
	if (this->created)
		throw err_init();
	this->created = true;
}

webserv::~webserv()
{
	this->stop_all(this->servers.begin());
	/* for (std::vector<config>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		delete it;
	} */
}

void webserv::addserv(std::vector<std::map<std::string, std::string>> const& config)
{

}

void webserv::remserv(std::vector<config>::iterator &old)
{

}

const unsigned int webserv::get_nbr_server() const
{
	return (this->nbr_server);
}

void webserv::bind_server(std::vector<config>::iterator &bind)
{

}

void webserv::stop_server(std::vector<config>::iterator &stop)
{

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
			this->stop_server(server);
		}
		catch (const err_init e)
		{
			std::cerr << e.what() << std::endl;
		}
		server++;
	}
	std::cout << "All server stopped!" << std::endl;
}

const char *webserv::err_init::what() const
{
	return ("class webserv already init!");	
}