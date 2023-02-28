/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:06:43 by averon            #+#    #+#             */
/*   Updated: 2023/02/28 15:31:08 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request(const std::string & header) : Key_Method("Method"), Key_Path("Path"), Key_Protocol("Protocol")
{
	std::string::size_type start = 0;
	std::string::size_type end = header.find("\r\n");
	std::string line = header.substr(start, end - start);

	// *** traitement de la premiere ligne pour recupérer les 3 parametres Method, Path et Protocol 
	parse_first_line(line);
	
	start = end + 2;
	end = header.find("\r\n", start);	
	
	while (end != std::string::npos)
	{
		std::string line = header.substr(start, end - start);

	//	*** recherche de la position de ":" dans la ligne + identification key et value
		std::string::size_type colon = line.find(":");
 		if (colon != std::string::npos) 
		{
			std::string key = line.substr(0, colon);
 		   	std::string value = line.substr(colon + 1);

	// *** effacement des espaces avant et apres les ":"
 		   	key.erase(key.find_last_not_of(" \t") + 1);
  		  	value.erase(0, value.find_first_not_of(" \t"));
   			value.erase(value.find_last_not_of(" \t") + 1);

	// *** enregistrement de la valeur dans la map
	    	request::headers[key] = value;	
		}

	// *** Passage à la ligne suivante (idem que pour premiere ligne)
 		start = end + 2;
		end = header.find("\r\n", start);
	}
}

void request::parse_first_line(std::string request_line)
{
	char *c_line = new char [request_line.length() +1];
	std::strcpy(c_line, request_line.c_str());
	char *tok = std::strtok(c_line, " ");
	request::headers[Key_Method] = tok;
	tok = std::strtok(NULL, " ");
	request::headers[Key_Path] = tok;	
	tok = std::strtok(NULL, " ");
	request::headers[Key_Protocol] = tok;		
	delete [] c_line;
}


