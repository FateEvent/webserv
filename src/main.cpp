/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 21:39:20 by stissera          #+#    #+#             */
/*   Updated: 2023/02/24 12:28:38 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../include/common.h"
#include "../include/Webserv.hpp"

bool Webserv::created = false;
fd_set		Webserv::readfd;
fd_set		Webserv::writefd;
fd_set		Webserv::errfd;

int	main(int ac, char **av)
{
	std::multimap<std::string, std::string>	block_map;
	std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator>	ret;

	ft::cut_multiple_blocks('{', '}', block_map);
	ret = block_map.equal_range("server");

	std::multimap<std::string, std::multimap<std::string, std::string> > config;
	// Config webserb test - Not server instance!!
	std::multimap<std::string, std::string> conf(ft::split_block(block_map.find("http"), '{', '}'));
/*	
	conf.insert(std::pair<std::string, std::string>("max_client", "100"));
	conf.insert(std::pair<std::string, std::string>("max_buff", "1000000"));
	conf.insert(std::pair<std::string, std::string>("time_out", "120"));
	conf.insert(std::pair<std::string, std::string>("default", "10.12.3.16"));
	// IF DONT WORK LIKE THIS MAYBE NEED ROOT USER TO LISTEN ON PORT 80
	conf.insert(std::pair<std::string, std::string>("listen", "80")); // PORT 80 WITH A LOCALHOST ADDRESS (127.0.0.1) DONT WORK.
	conf.insert(std::pair<std::string, std::string>("index", "index.html"));
*/
	std::multimap<std::string, std::string> server1(ft::split_block(ret.first, '{', '}'));
/*
	server1.insert(std::pair<std::string, std::string>("name", "test server"));
	server1.insert(std::pair<std::string, std::string>("host", "10.12.3.16"));
	server1.insert(std::pair<std::string, std::string>("protocol", "AF_INET"));
	server1.insert(std::pair<std::string, std::string>("listen", "1234"));
	server1.insert(std::pair<std::string, std::string>("type", "tcp"));
	server1.insert(std::pair<std::string, std::string>("root", "/web1/"));
*/
	std::multimap<std::string, std::string> server2(ft::split_block(++ret.first, '{', '}'));
/*
	server2.insert(std::pair<std::string, std::string>("name", "test server2"));
	server2.insert(std::pair<std::string, std::string>("protocol", "AF_INET"));
	server2.insert(std::pair<std::string, std::string>("host", "127.0.0.1"));
	server2.insert(std::pair<std::string, std::string>("listen", "1024"));
	server2.insert(std::pair<std::string, std::string>("type", "tcp"));
	server2.insert(std::pair<std::string, std::string>("root", "/web2/"));
*/
	config.insert(std::make_pair("http", conf));
	config.insert(std::make_pair("server", server1));
	config.insert(std::make_pair("server", server2));

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
	try {
		test.add(config);	
	}
	catch (std::exception &e) {
		std::cerr <<  e.what() << std::endl;
	}

	std::cout << "Number of instances: " << test.get_nbr_server() << std::endl;
	
/* 	std::cout << "Get instance info." << std::endl;
	for (std::vector<struct config>::const_iterator ginfo = test.begin(); ginfo != test.end(); ginfo++)
	{
		std::cout << test.get_info_on(ginfo) << std::endl;
	} */

	std::cout << "Prepare instance." << std::endl;
//	for (std::vector<struct config>::iterator ginfo = test.begin(); ginfo != test.end(); ginfo++)
//	{
		test.prepare_all();
//	}

/* 	std::cout << "Get all instances info." << std::endl;
	std::cout << test.get_info_instance() << std::endl; */
	//std::map<std::string, config>::iterator ginfo = test.begin();
	test.bind_all();

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
			//nsocket = accept(fd,&addr, len);
			/* In Webserv class
				Search the fd as receipt data with FD_ISSET,
				create a new class Client with the config of the instance with accept,
				put the new fd in Client class,
				Refer in this Client class the data I/O.		
			*/

			//if FD_ISSET(config.sock_fd iterator, &test.get_readfd()))
			// Client->fd = accept(...)
			std::cout << "Data recept: " << std::endl;
		}
	}
//	ginfo = test.begin();
	test.stop_all();
	
	(void) ac;
	(void) av;
	return (0);
}
