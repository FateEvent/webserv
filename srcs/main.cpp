/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 08:32:08 by stissera          #+#    #+#             */
/*   Updated: 2023/03/26 01:29:34 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define DEBUG
#include "../includes/utils.hpp"
#include "../includes/Webserv.hpp"

bool Webserv::created = false;
fd_set          Webserv::readfd;
fd_set          Webserv::writefd;
fd_set          Webserv::errfd;


int main(int ac, char **av) //, char** ev)
{
	if (ac != 2)
	{
		std::cout << "Argument error.\nNeed " << av[0] << " [CONFIG FILE]" << std::endl;
		return (1);
	}

	ft::make_header(0);
	ft::make_content_type("");

	// FIRST PARSE IN MAP STRING STRING
	std::ifstream file;
	file.open(av[1]);
	if (!file.is_open())
	{
		std::cout << "Can't open config file." << std::endl;
		return (1);
	}
	std::multimap<std::string, std::string> block;
	std::multimap<std::string, std::multimap<std::string, std::string> >	pre_conf;
	try {
		ft::file_to_map(file, block); // file to map -> block name / value
		bool config = false;
		for (std::multimap<std::string, std::string>::iterator it = block.find("http");
				it != block.end(); it++)
		{
			if (config == true && !it->first.compare("http"))
			{
				std::cout << "Multi http in config file." << std::endl;
				return (1);
			}
			if (it->first.compare("http") == 0)
				config = true;
		}
		if (!config)
		{
			std::cout << "No http in config file." << std::endl;
			return (1);
		}
		ft::parse_map(block.begin(), block.end(), pre_conf); // map to config -> title / map< name / value >
	}
	catch (std::invalid_argument &e) {
		std::cout << "Config file error: " << e.what() << std::endl;
		return (1);
	}
	file.close();

/* #ifdef DEBUG
	for (std::multimap<std::string, std::multimap<std::string, std::string> >::iterator it = pre_conf.begin(); it != pre_conf.end(); it++)
	{
		std::cout << "---> " << RED << it->first << RST << " <----\n";
		for (std::multimap<std::string, std::string>::iterator tt = it->second.begin(); tt != it->second.end(); tt++)
			std::cout << GREEN << tt->first << RST << " : " << GREEN << tt->second << RST << std::endl;
	}
#endif */

	Webserv server(pre_conf);
	server.add(pre_conf);

	server.prepare_all();
	server.bind_all();
	server.listen_all();
	server.timeout(1);
	std::cout << server.get_info_server() << std::endl;
	std::cout << server.get_info_instance() << std::endl;

	char			wait[] = {'/', '-', '\\', '|', '/', '-', '\\', '|'};
	unsigned int	nbr = 0;
	while (1)
	{	
		server.fd_rst();
		std::cout << "\b\rwait... " << wait[nbr++ % 8]  << std::flush;
		int recept = select(server.get_greaterfd(), &server.get_readfd(), &server.get_writefd(), NULL, &server.timeout());
		if (recept)
		{
			std::cout << std::endl;
			server.check_client(); // TO IMPLEMENTE HEADER OR CONTINUE WORKING
			server.check_server(); // TO CREATE CLIENT AND ACCEPT SOCKET
		}
		server.timeout(1);	
		server.exec_client(); // TO LAUNCH CLIENT IF WORKING FALSE, USUALY GOES ONE TIME/REQUEST
		recept = 0;
	}
	server.stop_all();

	return (0);
}