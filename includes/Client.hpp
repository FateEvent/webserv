/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:57:51 by stissera          #+#    #+#             */
/*   Updated: 2023/04/03 15:11:24 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#define DEBUG
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <ctime>
#include <sys/types.h>
#include <map>
#include <vector>
#include <list>
#include "../includes/common.h"
#include <sys/un.h> // FOR LOCAL SOCKET
#include <sys/wait.h>
#include <signal.h>
// #include "../includes/Reponse.hpp"
#include <fstream>
#include <sstream>
#include <fcntl.h>

class Client
{
	private:
		int							_sock_fd;
		sockaddr_in					_addr;			// sockaddr_in is a internet socket
		socklen_t					_socklen;
//		sockaddr_un					_addr_local;	// sockaddr_un is a local socket
		const config&				_ref_conf;
		std::string					_reponse;
		std::string					_root;
		std::string					_index;
		std::time_t					_timeout;
		header						_header;
		size_t						_max_body;
		std::map<int, std::string>	_error_page;
		std::string					_proxy;
		std::map<std::string, std::string>	_cgi_call;
		int							_pipe_cgi_out[2];
		int							_pipe_cgi_in[2];
		pid_t						_pid_cgi;
		bool						_working;
		bool						_chunked;
		bool						_cgi;
		fd_set						_readfd;
		bool						_sedding;
		struct s_clt_data			_data;
		bool						_ready;
		std::map<std::string, std::string>	other;
//		fd_set						_writefd;		
		void						_make_struct();

	public:
		Client(const config&);
		Client(const config&, sockaddr_in, socklen_t, int, header&);
		~Client();
		int							get_sockfd() const;
		std::string					get_method() const;
		time_t						get_time_alive() const;
		std::string					get_directory() const;
		std::pair<std::string, std::string>	get_file() const;
		header*						get_header();
		const config*				get_config() const;
		int							get_fd_cgi() const;
		int							get_pid_cgi() const;
		bool						continue_client(fd_set*);
		bool						execute_client(bool);
		bool						is_working() const;
		bool						is_cgi() const;
		bool						is_chunk() const;
		bool						is_seeding() const;
		bool						is_ready() const;
		bool						new_request();
		void						clear_header();
		void						clear();
		bool						check_location();
		void						simple_location(std::vector<struct s_location>::const_iterator &);
		void						condition_location(std::vector<struct s_location>::const_iterator &);
		void						chunk(); // get chunked data
		int							take_data();
		void						cgi_prepare_to_send();
		bool						send_data(int);
		int							launch_cgi(std::string);
		void						kill_cgi();
		
	protected:
};

#endif
