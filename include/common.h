/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:21:50 by stissera          #+#    #+#             */
/*   Updated: 2023/02/21 08:45:38 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
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

#endif