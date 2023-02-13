/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:39:20 by stissera          #+#    #+#             */
/*   Updated: 2023/02/13 00:20:43 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"
bool webserv::created = false;
int	main(int ac, char **av)
{
	std::cout << "Start" << std::endl;
	webserv test;
	try {
		webserv test1;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	(void)ac;
	(void)av;
	return (0);
}
