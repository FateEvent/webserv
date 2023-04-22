/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:56:13 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 19:09:57 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::execute_delete()
{
	std::cout << "DELETE METHOD" << std::endl;
	if (!(this->_allow >> 2 & 1) && this->_allow != 0)
	{
		this->make_error(401);
		return (false);
	}
	else
	{
		static_cast<std::fstream *>(this->_data.file)->close();
		this->_data.file = NULL;
		std::string path = this->_root + "/" + this->_index;
		std::cout << GREEN << "PATH: " << RST << path << std::endl;
		if (remove(path.c_str()) == 0)
			this->make_error(202);
		else
			this->make_error(403);
		return (false);
	}
	return (false);
}