/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/17 11:29:46 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <unistd.h> // for close()
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <map>
#include <utility>
#include <vector>
#include <exception>
#include <list>

//#include <sys/fcntl.h>
//#include <sys/ioctl.h>
//#include <arpa/inet.h>

struct	config
{
	std::string					name;
	std::string					root;
	std::string					index;
	std::string					ip;
	sockaddr_in					addr;
	sockaddr_in					from;
	int							sock_fd;	// retour of socket()
	int							domain;		//Type AF_INET, AF_LOCAL, AF_LINUX....
	int							type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
	int							max_client;
	uint16_t					port;
	bool						active;
	bool						prepare;
	std::map<int, std::string>	error_page;
};

class webserv
{
	private:
		webserv() {};
		webserv(webserv &) {};
		webserv&								operator=(webserv const&);
		std::multimap<std::string, std::string>	mainconfig; // principal config
		std::vector<config>						servers;
		unsigned int							nbr_server;
		void									check_instance(config &);

	public:
		webserv(std::multimap<std::string, std::string> &);
		~webserv();
		void				add(std::vector<std::multimap<std::string, std::string> > &);
		void				add(std::multimap<std::string, std::string>);
		void 				remove(std::vector<config>::iterator &);

		// INFO
		unsigned			get_nbr_server() const;
		std::string			get_info_server() const;
		std::string			get_info_instance() const;
		std::string			get_info_on(std::vector<config>::const_iterator &) const;

		// SOCKET
		void				prepare_all(std::vector<config>::iterator &);
		void				prepare(std::vector<config>::iterator &);
		void				bind_all(std::vector<config>::iterator &);
		void				bind(std::vector<config>::iterator &);
		void				close(std::vector<config>::iterator &); // close connexion qnd remove instance
		void				stop(std::vector<config>::iterator &);
		void				stop_all(std::vector<config>::iterator &);

		// TRHOW
		class err_init : public std::exception
		{
			const char* what() const throw();
		};

		// Operator
		std::vector<config>::iterator begin();
		std::vector<config>::iterator end();
		config	operator[](size_t);

	protected:
		static bool			created;
};

#endif
