/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_4.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 15:25:04 by averon            #+#    #+#             */
/*   Updated: 2023/02/28 16:53:07 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <string>

std::vector<std::string> str_to_vect(std::string str, std::string sep)
{
	std::vector<std::string> accept_vect;
	char* tmp = new char[str.length() +1];

	std::strcpy(tmp, str.c_str());
	char *tok = std::strtok(tmp, sep.data());

	while (tok != NULL)
	{
		std::string(tok).erase(0, std::string(tok).find_first_not_of(" \t"));
   		std::string(tok).erase(std::string(tok).find_last_not_of(" \t") + 1);
		accept_vect.push_back(std::string(tok));
		tok = std::strtok(NULL, sep.data());	
	}	

	std::cout << "AFFICHAGE APRES PARSING:"<< std::endl;
	for (std::vector<std::string>::const_iterator it = accept_vect.begin(); it != accept_vect.end(); it++) 
	{
		std::cout << *it << std::endl;
	}

	delete [] tmp;
	return (accept_vect);
}


int main()
{
	std::string str = "qwerqwre,asdfasda ,  hkjzhkjdha,    fdfffdsg    ";
	str_to_vect(str, ", ");
	return 0;
}