/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:26:03 by stissera          #+#    #+#             */
/*   Updated: 2023/03/12 11:04:36 by stissera         ###   ########.fr       */
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

namespace ft {
	void	file_to_map(std::ifstream &, std::multimap<std::string, std::string> &);
	void	parse_map(std::multimap<std::string, std::string>::iterator,
					std::multimap<std::string, std::string>::iterator,
					std::multimap<std::string, std::multimap<std::string, std::string> > &);
	void	check_host(std::multimap<std::string, std::string> &);
	void	parse_err_page(std::map<int, std::string>&, std::multimap<std::string, std::string>&);
	bool	put_err_page(std::string&, std::map<int, std::string>&);
};
#endif