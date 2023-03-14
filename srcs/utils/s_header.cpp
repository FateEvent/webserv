/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_header.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 20:50:29 by stissera          #+#    #+#             */
/*   Updated: 2023/03/14 16:49:32 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.h"

s_header::s_header()
{
	this->clear();
}

void	s_header::clear()
{
	Methode.clear();
	Dir.clear();
	Host.clear();
	User_Agent.clear();
	Content_Length = 0;
	Content_Type.clear();
	Content_Encoding.clear();
	Transfer_Encoding.clear();
	Accept.clear();
	Cookie.clear(); // change to a ref of cookie in instance
	Keep_Alive = 0;
	time_out = 0;
	Connexion.clear();
	// A voir MISS some directives...
	file.first.clear();
	file.second.clear();
	other.clear(); // Need change to a map<string, string>
}

void	s_header::print_all()
{
	std::cout << "Methode        : " + Methode << std::endl; // ok
	std::cout << "Directory      : " + Dir << std::endl; // ok
	std::cout << "Host           : " + Host << std::endl; // ok
	std::cout << "UserAgent      : " + User_Agent << std::endl; // ok
	std::cout << "Content Length : " << Content_Length << std::endl; // ok
	for (std::vector<std::string>::iterator it = Content_Type.begin(); it != Content_Type.end(); it++) // ok
		std::cout << "Content Type   : " << it->data() << std::endl;	
	for (std::vector<std::string>::iterator it = Content_Encoding.begin(); it != Content_Encoding.end(); it++) // ok
		std::cout << "Content Encod. : " << it->data() << std::endl;
	for (std::vector<std::string>::iterator it = Transfer_Encoding.begin(); it != Transfer_Encoding.end(); it++) // ok
		std::cout << "Transfer Encod.: " << it->data() << std::endl;	
	for (std::vector<std::string>::iterator it = Accept.begin(); it != Accept.end(); it++) //ok
		std::cout << "Accept         : " << it->data() << std::endl;	
	for (std::map<std::string, std::string>::iterator it = Cookie.begin(); it != Cookie.end(); it++) // ok
		std::cout << "Cookie         : " << it->first << " = " << it->second << std::endl;
	std::cout << "Keep_Alive     : " << Keep_Alive << std::endl; // ok
	std::cout << "Connection     : " << Connexion << std::endl; // ok
	std::cout << "Time Out       : " << time_out << std::endl;
	std::cout << "FILE FIRST     : " << file.first << std::endl;
	std::cout << "FILE SECOND    : " << file.second << std::endl << std::endl;
	for (std::map<std::string, std::string>::iterator it = other.begin(); it != other.end(); it++) // ok
		std::cout << "Other          : " << it->first << " = " << it->second << std::endl;
}