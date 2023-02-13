/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:06:43 by stissera          #+#    #+#             */
/*   Updated: 2023/02/13 00:18:34 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <map>
#include <vector>
#include <exception>


struct	config
{
	int							sock_fd;
	std::string					name;
	sockaddr					addr;
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
//		size					nbr_server;

	public:
		webserv();
		~webserv();
		void				addserv(std::vector<std::map<std::string, std::string> > const&);
		void 				remserv(std::vector<config>::iterator &);
		unsigned			get_nbr_server() const;
		void				bind_server(std::vector<config>::iterator &);
		void				stop_server(std::vector<config>::iterator &);
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