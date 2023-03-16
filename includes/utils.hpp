/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:26:03 by stissera          #+#    #+#             */
/*   Updated: 2023/03/15 19:02:57 by stissera         ###   ########.fr       */
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

namespace ft {
	void	file_to_map(std::ifstream &, std::multimap<std::string, std::string> &);
	void	parse_map(std::multimap<std::string, std::string>::iterator,
					std::multimap<std::string, std::string>::iterator,
					std::multimap<std::string, std::multimap<std::string, std::string> > &);
	void	check_host(std::multimap<std::string, std::string> &);
	void	parse_err_page(std::map<int, std::string>&, std::multimap<std::string, std::string>&);
	bool	put_err_page(std::string&, std::map<int, std::string>&);
	bool	split_to_vectors(std::vector<std::string> &, std::string);
	bool	split_to_mapss(std::map<std::string, std::string> &, std::string);
	bool	split_to_maposs(std::map<std::string, std::string> &, std::string);
	bool	test_path(std::string &);
};
#endif