/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:57:51 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 13:44:12 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# define DEBUG
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstring>
# include <string>
# include <ctime>
# include <sys/types.h>
# include <map>
# include <vector>
# include <list>
# include "../includes/common.h"
# include <sys/un.h> // FOR LOCAL SOCKET
# include <sys/wait.h>
# include <signal.h>
// # include "../includes/Reponse.hpp"
# include <fstream>
# include <sstream>
# include <fcntl.h>
# include <cmath>

class Client
{
	private:
		int							_sock_fd;
		sockaddr_in					_addr;			// sockaddr_in is a internet socket
		socklen_t					_socklen;
		config&						_ref_conf;
		std::string					_response;
		std::string					_root;
		std::string					_index;
		long						_timeout;
		header						_header;
		ssize_t						_max_body;
		int							_allow;
		std::string					_redirect;
		std::map<int, std::string>	_error_page;
		std::string					_proxy;
		std::string					_download;
		std::map<std::string, std::string>	_cgi_call;
		int							_pipe_cgi[2];
		pid_t						_pid_cgi;
		bool						_working;
		bool						_chunked;
		bool						_cgi;
		bool						_multipart;
		fd_set						_readfd;
		bool						_sending;
		struct s_clt_data			_data;
		bool						_ready;
		bool						_close;

		std::map<std::string, std::string>	other;	
		void						_make_struct();

	public:
		Client(config&);
		Client(config&, sockaddr_in, socklen_t, int, header&);
		~Client();
		int							get_sockfd() const;
		void						set_sockfd(int sock_fd);
		std::string					get_method() const;
		time_t						get_time_alive() const;
		std::string					get_directory() const;
		header*						get_header();
		const config*				get_config() const;
		int							get_fd_cgi() const;
		int							get_fd_cgi_0() const;
		int							get_pid_cgi() const;
		long						get_timeout() const;
		bool						get_close() const;
		long						get_max_body() const;
		void						set_timeout(long);
		bool						continue_client(fd_set*);
		bool						execute_client(bool);
		bool						is_working() const;
		bool						is_cgi() const;
		bool						is_chunk() const;
		bool						is_sending() const;
		bool						is_ready() const;
		bool						is_multipart() const;
		bool						new_request();
		void						clear_header();
		// void						clear();
		bool						check_location();
		void						simple_location(std::vector<struct s_location>::const_iterator &);
		void						condition_location(std::vector<struct s_location>::const_iterator &);
		void						chunk(); // get chunked data
		int							take_data();
		void						cgi_prepare_to_send();
		bool						send_data(int);
		int							launch_cgi(std::string);
		void						kill_cgi();
		unsigned int				get_nbr_connected_client() const;
		void						add_nbr_client();
		void						make_error(int);
		bool						execute_get();
		bool						execute_post();
		bool						execute_delete();
		size_t						getMaxPower(std::string hex);
		ssize_t						hextol(std::string hex);
		int							is_hex_digit(char c);
		int							is_hex_line(std::string str);
		bool						multipart();
		void						send_success_status();
		bool						process_multipart();
};

#endif
