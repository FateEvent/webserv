/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/14 13:08:08 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <map>
#include <vector>
#include <exception>
#include <list>

struct	config
{
	int							sock_fd;	// retour of socket()
	std::string					name;
	sockaddr_in					sin;
	sockaddr_in					from;
	int							domain;		//Type AF_INET, AF_LOCAL, AF_LINUX....
	int							type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
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
		std::map<std::string, std::string>	mainconfig; // principal config
		std::vector<config>					servers;
		unsigned int						*nbr_server;
		void								close(std::vector<config>::iterator &); // close connexion

	public:
		webserv(std::map<std::string, std::string> &);
		~webserv();
		void				add(std::vector<std::map<std::string, std::string> >::iterator const&);
		void 				remove(std::vector<config>::iterator &);
		void				bind(std::vector<config>::iterator &);
		void				stop(std::vector<config>::iterator &);
		unsigned			get_nbr_server() const;
		std::string			get_info_server() const;
		std::string			get_info_on(std::vector<config>::iterator &) const;
		void				bind_all(std::vector<config>::iterator);
		void				stop_all(std::vector<config>::iterator);
		class err_init : public std::exception
		{
			const char* what() const throw();
		};
	protected:
		static bool			created;
};

#endif
