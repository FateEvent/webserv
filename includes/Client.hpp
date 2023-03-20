/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:57:51 by stissera          #+#    #+#             */
/*   Updated: 2023/03/20 14:44:55 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#define DEBUG
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <time.h>
#include <sys/types.h>
#include <map>
#include <vector>
#include <list>
#include "../includes/common.h"
#include <sys/un.h> // FOR LOCAL SOCKET
// #include "../includes/Reponse.hpp"
#include <fstream>
#include <fcntl.h>

class Client
{
	private:
		int							_sock_fd;
		sockaddr_in					_addr;			// sockaddr_in is a internet socket
		socklen_t					_socklen;
//		sockaddr_un					_addr_local;	// sockaddr_un is a local socket
		const config&				_ref_conf;
		std::string					_root;
		std::string					_index;
		time_t						_timeout;
		header						_header;
		size_t						_max_body;
		std::map<int, std::string>	_error_page;
		int							_fd_cgi;
		bool						_working;
		bool						_chunked;
		bool						_cgi;
		fd_set						_readfd;
		std::map<std::string, std::string>	other;
//		fd_set			_writefd;		
		void			_make_struct();

	public:
		Client(const config&);
		~Client();
		int				get_sockfd() const;
		std::string		get_methode() const;
		time_t			get_time_alive() const;
		std::string		get_directory() const;
		std::pair<std::string, std::string>	get_file() const;
		header*			get_header();
		const config*	get_config() const;
		int				get_fd_cgi() const;
		void			continue_client(fd_set*);
		void			execute_client(bool);
		bool			is_working() const;
		bool			is_cgi() const;
		bool			is_chunk() const;
		bool			new_request();
		void			clear_header();
	
		bool			check_location();
		void			simple_location(std::vector<struct s_location>::const_iterator &);
		void			condition_location(std::vector<struct s_location>::const_iterator &);
		void			chunk(); // get chunked data
		void			take_data(); // Get or search body data. count and put total in length variable
		
	protected:
};

#endif
