/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/13 16:09:39 by stissera         ###   ########.fr       */
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
//	typedef typename std::vector<config>::size	size;

	private:
		webserv(webserv &other);
		void	operator=(const webserv &other);
		std::vector<config>		servers;
		unsigned int			*nbr_server;
		void					close(); // close connexion
//		size					nbr_server;

	public:
		webserv();
		~webserv();
		void				add(std::vector<std::map<std::string, std::string> > const&);
		void 				remove(std::vector<config>::iterator &);
		void				bind(std::vector<config>::iterator &);
		void				stop(std::vector<config>::iterator &);
		unsigned			get_nbr_server() const;
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
