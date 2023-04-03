/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:26:03 by stissera          #+#    #+#             */
/*   Updated: 2023/04/03 16:43:47 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

//#define DEBUG
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>
#include "../includes/common.h"
//#include "../includes/Client.hpp"

namespace ft {
	void			file_to_map(std::ifstream &, std::multimap<std::string, std::string> &);
	void			parse_map(std::multimap<std::string, std::string>::iterator,
						std::multimap<std::string, std::string>::iterator,
						std::multimap<std::string, std::multimap<std::string, std::string> > &);
	void			check_host(std::multimap<std::string, std::string> &);
	void			parse_err_page(std::map<int, std::string>&, std::multimap<std::string, std::string>&);
	bool			put_err_page(std::string&, std::map<int, std::string>&);
	bool			split_to_vectors(std::vector<std::string> &, std::string, char sep = ',');
	bool			split_to_vectors(std::vector<std::string> &svector, std::string &Boundary, std::string line);
	bool			split_to_mapss(std::map<std::string, std::string> &, std::string, char sep = ';');
	bool			split_to_maposs(std::map<std::string, std::string> &, std::string);

	std::ifstream	*test_path(std::string &);
	std::string		make_header(int = 0);
	std::string 	make_content_type(std::string);
	std::string		get_page_error(int nbr_error, std::string dir);
	void			do_cgi();
	std::string		return_content_type(char *);
	char			**vector_to_tab(std::vector<std::string>&);
	//void			free_tab(char **);
};
#endif