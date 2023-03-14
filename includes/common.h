/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:21:50 by stissera          #+#    #+#             */
/*   Updated: 2023/03/14 16:27:29 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
#define COMMON_H
#define DEBUG
#include <iostream>
#include <string>
#include <ctime>
#include <map>
#include <vector>
#include <netinet/in.h>
#include "./utils.hpp"

typedef struct	s_config
{
	s_config();
	s_config(std::multimap<std::string, std::string>&);
	void	set_zero();
	void	do_location(std::string);
	void	get_all_loc(std::string, struct s_location*);
	std::string							name;
	std::string							root;
	std::string							index;
	std::string							ip;
	uint16_t							port;
	std::string							error_log;
	std::map<std::string, std::string>	cgi;
	std::vector<std::string>			allow;
	unsigned int						max_client;
	size_t								max_body;
	bool								active;
	bool								prepare;
	std::vector<struct s_location>		location; // can have multiple location
	std::map<int, std::string>			error_page;
	unsigned int						time_out;
	std::map<std::string, std::string>	other;
	// Bind and socket
	sockaddr_in							addr;
	int									sock_fd;	// socket() return
	int									domain;		// Type AF_INET, AF_LOCAL, AF_LINUX.... # We use only AF_INET
	int									type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
}	config;

typedef struct s_header
{
	s_header();
	void								clear();
	void								print_all();
	std::string							Methode;
	std::string							Dir;
	std::string							Host;
	std::string							User_Agent;
	size_t								Content_Length;
	std::vector<std::string>			Content_Type;
	std::vector<std::string>			Content_Encoding;
	std::vector<std::string>			Transfer_Encoding;
	std::vector<std::string>			Accept;
	std::map<std::string, std::string>	Cookie; // change to a ref of cookie in instance
	ssize_t								Keep_Alive;
	size_t								time_out;
	std::string							Connexion;
	// A voir MISS some directives...
	std::pair<std::string, std::string>	file;
	std::map<std::string, std::string>	other; // Need change to a map<string, string>
}	header;

typedef struct s_location
{
	std::string							base; 	// first argument after location
	std::string							search;	// second before open bracket if exist"
	std::map<std::string, std::string>	to; 	// what we do (name, directive).
}	t_location;

#endif