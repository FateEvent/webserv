/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:39:20 by stissera          #+#    #+#             */
/*   Updated: 2023/02/18 22:02:07 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"
bool Webserv::created = false;
fd_set		Webserv::readfd;
fd_set		Webserv::writefd;
fd_set		Webserv::errfd;

int	main(int ac, char **av)
{
	// Config webserb test - Not server instance!!
	std::multimap<std::string, std::string> config;
	config.insert(std::pair<std::string, std::string>("BLOCK", "http"));
	config.insert(std::pair<std::string, std::string>("max_connect", "1000"));
	config.insert(std::pair<std::string, std::string>("max_buff", "1000000"));
	config.insert(std::pair<std::string, std::string>("time_out", "120"));
	config.insert(std::pair<std::string, std::string>("default", "127.0.0.1"));
	config.insert(std::pair<std::string, std::string>("index", "index.html"));

	// Config instance test
	std::vector<std::multimap<std::string, std::string> > instance;
	std::multimap<std::string, std::string> server1;
	server1.insert(std::pair<std::string, std::string>("BLOCK", "server"));
	server1.insert(std::pair<std::string, std::string>("name", "test server"));
	server1.insert(std::pair<std::string, std::string>("host", "127.0.0.1"));
	server1.insert(std::pair<std::string, std::string>("protocol", "AF_INET"));
	server1.insert(std::pair<std::string, std::string>("listen", "800"));
	server1.insert(std::pair<std::string, std::string>("type", "tcp"));
	server1.insert(std::pair<std::string, std::string>("root", "/web1/"));

	std::multimap<std::string, std::string> server2;
	server2.insert(std::pair<std::string, std::string>("BLOCK", "server"));
	server2.insert(std::pair<std::string, std::string>("name", "test server2"));
	server2.insert(std::pair<std::string, std::string>("protocol", "AF_INET"));
	server2.insert(std::pair<std::string, std::string>("host", "192.168.1.99"));
	server2.insert(std::pair<std::string, std::string>("listen", "8080"));
	server2.insert(std::pair<std::string, std::string>("type", "tcp"));
	server2.insert(std::pair<std::string, std::string>("root", "/web2/"));

	instance.push_back(server1);
	instance.push_back(server2);

	std::cout << "Start" << std::endl;
	std::cout << "Creating class webserv." << std::endl;
	Webserv test(config);
/* 	try {
		std::cout << "Try create second class webserb." << std::endl;
		webserv test1(config);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	} */
//	std::cout << test.get_info_server() << std::endl;

/* 	std::cout << "Test add instance by map in vector (multi instance)" << std::endl;
	for (std::vector<std::multimap<std::string, std::string> >::iterator it = instance.begin();
			it != instance.end(); it++)
	{
		try
		{
			test.add(*it);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}		
	}

	std::cout << "Number of instances: " << test.get_nbr_server() << std::endl;
	
 */
	std::cout << "Add vector of instance" << std::endl;
	test.add(instance);

	std::cout << "Number of instances: " << test.get_nbr_server() << std::endl;
	
/* 	std::cout << "Get instance info." << std::endl;
	for (std::vector<struct config>::const_iterator ginfo = test.begin(); ginfo != test.end(); ginfo++)
	{
		std::cout << test.get_info_on(ginfo) << std::endl;
	} */

	std::cout << "Prepare instance." << std::endl;
//	for (std::vector<struct config>::iterator ginfo = test.begin(); ginfo != test.end(); ginfo++)
//	{
		std::vector<struct config>::iterator ginfo = test.begin();
		test.prepare_all(ginfo);
//	}

/* 	std::cout << "Get all instances info." << std::endl;
	std::cout << test.get_info_instance() << std::endl; */

	ginfo = test.begin();
	test.bind_all(ginfo);

	std::cout << "Get all instances info." << std::endl;
	std::cout << test.get_info_instance() << std::endl;
	
/* 	std::cout << "Name of instance 1 passed by operator[]: ";
	try
	{
		std::cout << test[0].name << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	} */

	test.listen_all();


	
	while (1)
	{	
		test.fd_rst();
		std::cout << "waitting..." << std::endl;
		int recept = select(test.get_greaterfd(), &test.get_readfd(), &test.get_writefd(), NULL, &test.timeout());
		std::cout << std::to_string(test.get_greaterfd()) << std::endl;
		if (recept)
		{
			std::cout << "Data recept: " << std::endl;
		}
	}
	ginfo = test.begin();
	test.stop_all(ginfo);
	
	(void) ac;
	(void) av;
	return (0);
}
