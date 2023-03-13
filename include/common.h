/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:21:50 by stissera          #+#    #+#             */
/*   Updated: 2023/03/13 12:29:58 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <ctime>
#include <map>
#include <netinet/in.h>

typedef struct	s_config
{
	std::string													name;
	std::string													root;
	std::string													index;
	std::string													ip;
	sockaddr_in													addr;
	int															sock_fd;	// retour of socket()
	int															domain;		//Type AF_INET, AF_LOCAL, AF_LINUX....
	int															type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
	int															max_client;
	uint16_t													port;
	bool														active;
	bool														prepare;
	std::map<int, std::string>									error_page;
	std::map<std::string, std::map<std::string, std::string> >	location;
}	config;

typedef struct s_header
{
	std::string							method;
	std::string							directory;
	std::pair<std::string, std::string>	file; //pour file et extension
	std::string							host;
	std::string							connection;
	std::string							cache_control;
	std::string							user_agent;
	std::string							listen;
	
	std::vector<std::string>			accept;
	std::vector<std::string>			language;
	std::vector<std::string>			encoding;
	std::map<std::string, std::string>	cookie; // change to a ref of cookie in instance
	unsigned long						length;
	std::string							content_type;
	std::vector<std::string>			content;
	std::map<std::string, std::string>	data; // Need change to a map<string, string> => OK
}	header;

typedef struct s_cookie
{
//	timeval_t							created;
//	timeval_t							alive;
	std::map<std::string, std::string>	data;
}	cookie;

#endif