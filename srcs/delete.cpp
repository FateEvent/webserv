/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:56:13 by stissera          #+#    #+#             */
/*   Updated: 2023/04/06 19:56:59 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::execute_delete()
{
	std::cout << "DELETE METHOD" << std::endl;
	if (!(this->_allow >> 2 & 1) && this->_allow != 0)
	{
		this->make_error(405);
		return (false);
	}
	return (false);
}