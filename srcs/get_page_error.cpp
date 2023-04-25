/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_page_error.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:06:17 by stissera          #+#    #+#             */
/*   Updated: 2023/04/12 00:19:47 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"
#include "../includes/Client.hpp"

std::string	ft::get_page_error(int nbr_error, std::string dir)
{
	if (!dir.empty())
	{
		std::ifstream	file(dir);
		if (file.is_open())
		{
			std::stringstream streamfile;
	    	streamfile << file.rdbuf(); //read the file
    		std::string data = streamfile.str();
			return ("Content-Length: " + std::to_string(data.length()) + "\r\n\r\n" + data);
		}
	}
	std::string	header;

	try {
		header = std::to_string(ft::make_header(nbr_error).substr(9).length()) + "\r\n\r\n" + ft::make_header(nbr_error).substr(9);
	} catch (std::exception &e) {
		header = "0";
		std::cerr << "The header couldn't be created: " << e.what() << std::endl;
	}
	return ("Content-Length: " + header); // Generation de lerreur.. A refaire sur une generation standard
}