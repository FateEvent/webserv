/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/21 12:16:05 by faventur         ###   ########.fr       */
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
#include <time.h>

//#include <sys/fcntl.h>
//#include <sys/ioctl.h>
//#include <arpa/inet.h>

#include "../include/common.h"
#include "../include/Client.hpp"

class Webserv
{
	private:
		Webserv() {};
		Webserv(Webserv &) {};
		int										_sock_fd;
		Webserv&								operator=(Webserv const&);
		std::multimap<std::string, std::string>	mainconfig; // principal config
		std::vector<config>						servers;
		unsigned int							nbr_server;
		std::map<int, Client>			 	 	client;
		void									check_instance(config &);
		config									_base;
		timeval									_timeout;

	public:
		Webserv(std::multimap<std::string, std::string> &);
		~Webserv();
		void				add(std::vector<std::multimap<std::string, std::string> > &);
		void				add(std::multimap<std::string, std::string>);
		void 				remove(std::vector<config>::iterator &);

		// INFO
		unsigned			get_nbr_server() const;
		std::string			get_info_server() const;
		std::string			get_info_instance() const;
		std::string			get_info_on(std::vector<config>::const_iterator &) const;
		int					get_greaterfd() const;

		// SOCKET
		void				prepare_all(std::vector<config>::iterator &);
		void				prepare(std::vector<config>::iterator &);
		void				bind_all(std::vector<config>::iterator &);
		void				bind(std::vector<config>::iterator &);
		void				close(std::vector<config>::iterator &); // close connexion qnd remove instance
		void				stop(std::vector<config>::iterator &);
		void				stop_all(std::vector<config>::iterator &);
		void				listen(config &);
		void				listen_all();
		void				fd_rst();
		fd_set&				get_writefd();
		fd_set&				get_readfd();
		timeval&			timeout();

		Client				make_client();

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
		static fd_set		readfd;
		static fd_set		writefd;
		static fd_set		errfd;
};

#endif
