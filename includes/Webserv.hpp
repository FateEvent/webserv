/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/04/04 15:29:34 by stissera         ###   ########.fr       */
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
#include <ctime>
#include <fcntl.h>

//#include <sys/fcntl.h>
//#include <sys/ioctl.h>
//#include <arpa/inet.h>

#include "../includes/common.h"
#include "../includes/utils.hpp"
#include "../includes/Client.hpp"

class Webserv
{
	private:
		Webserv() {};
		Webserv(Webserv &) {};
		Webserv&										operator=(Webserv const&);
		config											_base;
		std::map<std::string, config>					_servers;
		unsigned int									_nbr_server;
		std::map<int, Client>			 	 			_client;
		timeval											_timeout;
		void											_check_instance(config &);

	public:
		Webserv(std::multimap<std::string, std::multimap<std::string, std::string> > &);
		~Webserv();
		void			add(std::multimap<std::string, std::multimap<std::string, std::string> > &);
		void			add(std::multimap<std::string, std::string> &);
		void 			remove(std::map<std::string, config>::iterator &);

		// INFO
		unsigned		get_nbr_server() const;
		std::string		get_info_server() const;
		std::string		get_info_instance() const;
		std::string		get_info_on(const config &) const;
		int				get_greaterfd() const;

		// SOCKET
		void			prepare_all();
		void			prepare(config &);
		void			bind_all();
		void			bind(config &);
		void			close(std::map<std::string, config>::iterator &); // close connexion qnd remove instance
		void			stop(config &);
		void			stop_all();
		void			listen(config &);
		void			listen_all();
		void			fd_rst();
		fd_set&			get_writefd();
		fd_set&			get_readfd();
		void			timeout(int);
		timeval&		timeout();
		void			check_server();
		void			check_client();
		void			exec_client();


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
		static time_t		time;
};

#endif
