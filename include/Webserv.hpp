/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/16 13:39:39 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <map>
#include <utility>
#include <vector>
#include <exception>
#include <list>

struct	config
{
	int							sock_fd;	// retour of socket()
	std::string					name;
	sockaddr_in					addr;
	sockaddr_in					from;
	int							domain;		//Type AF_INET, AF_LOCAL, AF_LINUX....
	int							type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
	std::string					ip;
	int							max_client;
	uint16_t					port;
	std::string					root;
	std::string					index;
	std::map<int, std::string>	error_page;
	bool						active;
};

class webserv
{
	private:
		webserv() {};
		webserv(webserv &) {};
		webserv&							operator=(webserv const&);
		std::multimap<std::string, std::string>	mainconfig; // principal config
		std::vector<config>					servers;
		unsigned int						nbr_server;

	public:
		webserv(std::multimap<std::string, std::string> &);
		~webserv();
		void				add(std::vector<std::multimap<std::string, std::string> > &);
		void				add(std::multimap<std::string, std::string>);
		void 				remove(std::vector<config>::iterator &);
		void				bind(std::vector<config>::iterator &);
		void				stop(std::vector<config>::iterator &);
		unsigned			get_nbr_server() const;
		std::string			get_info_server() const;
		std::string			get_info_instance() const;
		std::string			get_info_on(std::vector<config>::iterator &) const;
		void				bind_all(std::vector<config>::iterator);
		void				stop_all(std::vector<config>::iterator);
		void				close(std::vector<config>::iterator &); // close connexion qnd remove instance

		class err_init : public std::exception
		{
			const char* what() const throw();
		};

		// Operator
		std::vector<config>::iterator begin();
		std::vector<config>::iterator end();
		//std::vector<config>::iterator operator++(int);
		config	*operator[](size_t &);

	protected:
		static bool			created;
};

#endif
