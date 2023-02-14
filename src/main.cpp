/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:39:20 by stissera          #+#    #+#             */
/*   Updated: 2023/02/14 13:25:40 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"
bool webserv::created = false;
int	main(int ac, char **av)
{
	// Config webserb test - Not server instance!!
	std::map<std::string, std::string> config;
	config.insert(std::pair<std::string, std::string>("BLOCK", "http"));
	config.insert(std::pair<std::string, std::string>("max_connect", "1000"));
	config.insert(std::pair<std::string, std::string>("max_buff", "1000000"));
	config.insert(std::pair<std::string, std::string>("time_out", "120"));
	config.insert(std::pair<std::string, std::string>("default", "127.0.0.1"));

	// Config instance test
	std::vector<std::map<std::string, std::string> > instance;
	std::map<std::string, std::string> server1;
	server1.insert(std::pair<std::string, std::string>("BLOCK", "server"));
	server1.insert(std::pair<std::string, std::string>("name", "test server"));
	server1.insert(std::pair<std::string, std::string>("host", "127.0.0.1"));
	server1.insert(std::pair<std::string, std::string>("listen", "80"));
	server1.insert(std::pair<std::string, std::string>("type", "tcp"));
	std::map<std::string, std::string> server2;
	server2.insert(std::pair<std::string, std::string>("BLOCK", "server"));
	server2.insert(std::pair<std::string, std::string>("name", "test server"));
	server2.insert(std::pair<std::string, std::string>("host", "127.0.0.1"));
	server2.insert(std::pair<std::string, std::string>("listen", "80"));
	server2.insert(std::pair<std::string, std::string>("type", "tcp"));
	instance.push_back(server1);
	instance.push_back(server2);

	std::cout << "Start" << std::endl;
	webserv test(config);
	try {
		webserv test1(config);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	test.get_info_server();
	for (std::vector<std::map<std::string, std::string> >::iterator it = instance.begin();
			it != instance.end(); it++)
	{
		test.add(it);
	}

	config.clear();
	
	std::cout << test.get_info_server() << std::endl;
	(void)ac;
	(void)av;
	return (0);
}
