/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_page_error.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:06:17 by stissera          #+#    #+#             */
/*   Updated: 2023/03/31 13:47:41 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"
#include "../includes/Client.hpp"

std::string	ft::get_page_error(int nbr_error, std::string dir)
{
	std::ifstream	file(dir);
	if (file.is_open())
	{
		std::string	data;
		file >> data;
		return ("Content-Length: " + std::to_string(data.length()) + "\r\n\r\n" + data);
	}
	return ("Content-Length: " + std::to_string(ft::make_header(nbr_error).substr(9).length()) + "\r\n\r\n" + ft::make_header(nbr_error).substr(9)); // Generation de lerreur.. A refaire sur une generation standard
}