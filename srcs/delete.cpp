/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:56:13 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 11:58:20 by stissera         ###   ########.fr       */
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
		// check file and delete it
		// return 202 if ok
		// otherwise return 404
		make_error(202);
		return (true);
	}
	return (false);
}