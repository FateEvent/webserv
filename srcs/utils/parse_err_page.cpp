/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_err_page.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 13:30:38 by stissera          #+#    #+#             */
/*   Updated: 2023/03/11 15:17:19 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

bool	ft::put_err_page(std::string& value, std::map<int, std::string>& err)
{
	size_t e_nbr;
	size_t s_string;

	e_nbr = value.find_first_not_of("1234567890");
	s_string = value.find_first_not_of(" \t\v\n", e_nbr);

	if (e_nbr == value.npos || s_string == value.npos || e_nbr == s_string)
		return (false);
	err.insert(std::pair<int, std::string>(std::stoul(value.substr(0, e_nbr), NULL,10), value.substr(s_string)));
	#ifdef DEBUG
		std::cout << "Error_page implementation: " << std::stoul(value.substr(0, e_nbr), NULL,10) << " - " << value.substr(s_string) << std::endl;
	#endif
	return (true);
}

void	ft::parse_err_page(std::map<int, std::string>& err, std::multimap<std::string, std::string>& page)
{
	for (std::map<std::string, std::string>::iterator it = page.begin(); it != page.end(); it++)
	{
		if (it->first.compare("error_page") == 0 && !ft::put_err_page(it->second, err))
			throw std::invalid_argument("error page in config file not set properly!");
	}
}