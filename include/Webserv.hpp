/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/28 13:05:56 by faventur         ###   ########.fr       */
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
# include "../include/search.hpp"

class Webserv
{
	private:
		Webserv() {};
		Webserv(Webserv &) {};
		Webserv&										operator=(Webserv const&);
		std::map<std::string, config>					_servers;
		unsigned int									_nbr_server;
		std::map<int, Client>			 	 			_client;
		void											_check_instance(config &);
		config											_base;
		timeval											_timeout;

	public:
		Webserv(std::multimap<std::string, std::multimap<std::string, std::string> > &);
		~Webserv();
		void				add(std::multimap<std::string, std::multimap<std::string, std::string> > &);
		void				add(std::multimap<std::string, std::string> &);
		void 				remove(std::map<std::string, config>::iterator &);

		// INFO
		unsigned			get_nbr_server() const;
		std::string			get_info_server() const;
		std::string			get_info_instance() const;
		std::string			get_info_on(const config &) const;
		int					get_greaterfd() const;

		// SOCKET
		void				prepare_all();
		void				prepare(config &);
		void				bind_all();
		void				bind(config &);
		void				close(std::map<std::string, config>::iterator &); // close connexion qnd remove instance
		void				stop(config &);
		void				stop_all();
		void				listen(config &);
		void				listen_all();
		void				fd_rst();
		fd_set&				get_writefd();
		fd_set&				get_readfd();
		timeval&			timeout();

		std::map<int, Client>::iterator	make_client();

		// TRHOW
		class err_init : public std::exception
		{
			const char* what() const throw();
		};

		// Operator
		std::map<std::string, config>::iterator begin();
		std::map<std::string, config>::iterator end();

	protected:
		static bool			created;
		static fd_set		readfd;
		static fd_set		writefd;
		static fd_set		errfd;
};

#endif
