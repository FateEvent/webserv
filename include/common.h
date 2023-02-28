/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:21:50 by stissera          #+#    #+#             */
/*   Updated: 2023/02/26 18:41:41 by stissera         ###   ########.fr       */
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
	std::string						name;
	std::string						root;
	std::string						index;
	std::string						ip;
	sockaddr_in						addr;
	int								sock_fd;	// retour of socket()
	int								domain;		//Type AF_INET, AF_LOCAL, AF_LINUX....
	int								type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
	int								max_client;
	uint16_t						port;
	bool							active;
	bool							prepare;
	std::map<int, std::string>		error_page;
}	config;

typedef struct s_header
{
	std::string							methode;
	std::string							directory;
	std::string							host;
	std::string							connexion;
	std::string							cache_control;
	std::string							user_agent;
	std::vector<std::string>			accept;
	std::map<std::string, std::string>	cookie; // change to a ref of cookie in instance
	unsigned long						lenght;
	std::vector<std::string>			content_type;
	std::string							data; // Need change to a map<string, string>
}	header;

typedef struct s_cookie
{
//	timeval_t							created;
//	timeval_t							alive;
	std::map<std::string, std::string>	data;
}	cookie;

#endif