/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:24:08 by averon            #+#    #+#             */
/*   Updated: 2023/02/28 10:10:26 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <stdio.h>
# include <sys/socket.h> //for socket programmong
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <string.h>
# include <iostream>
# include <cstdlib>
# include <string>
# include <map>

class request
{
	private :
		request() {}; 
	
	public :
		request(const std::string & header);
		~request() {};

		void parse_first_line(std::string request_line);

		std::map<std::string, std::string> headers;
		std::string Key_Method;
		std::string Key_Path;
		std::string Key_Protocol;
		
};

#endif