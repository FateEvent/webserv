/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 08:32:08 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 13:46:50 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define DEBUG
#include "../includes/utils.hpp"
#include "../includes/Webserv.hpp"

bool Webserv::created = false;
fd_set          Webserv::readfd;
fd_set          Webserv::writefd;
fd_set          Webserv::errfd;
bool _SIGNAL_END = true;
bool _BROKEN_PIPE = false;

void	Webserv::show_client_list()
{
	std::map<int, Client>::iterator first = this->_client.begin(), last = this->_client.end();
	//std::cout << PURPLE << "The socket of the following client has caused a SIG_PIPE and has to be closed: ";
	
	for (; first != last; ++first)
	{
		if (waitpid(first->second.get_pid_cgi(), NULL, 0) != 0)
		{
			//std::cout << first->first << RST << std::endl;
			first->second.clear_header();
			::close(first->second.get_fd_cgi_0());
			::close(first->second.get_sockfd());
			this->_client.erase(first);
			std::cout << GREEN << "Socket has been closed." << RST << std::endl;
			return ;
		}
	}
}

void	sigpipe_solving(int signal)
{
	std::cout << YELLOW << "Reception of a signal " << signal << RST << std::endl;
	_BROKEN_PIPE = true;
}

void	false_while(int type)
{
	std::cout << std::flush << YELLOW << "Stopping server by signal " << type << RST << std::endl;
	_SIGNAL_END = false;
}

int main(int ac, char **av) //, char** ev)
{
	if (ac > 2)
	{
		std::cout << RED << "Argument error.\nNeed " << av[0] << " [CONFIG FILE] or nothing." << RST << std::endl;
		return (1);
	}
	try {
		ft::make_header(0);
		ft::make_content_type("");
	} catch (std::exception &e) {
		std::cerr << "The header couldn't be created: " << e.what() << std::endl;
	}

	// FIRST PARSE IN MAP STRING STRING
	std::ifstream file;
	av[1] ? file.open(av[1]) : file.open("./config/config.conf");
	if (!file.is_open())
	{
		std::cout << "Can't open config file." << std::endl;
		return (1);
	}
	std::multimap<std::string, std::string> 								block;
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
	catch (std::exception &e) {
		std::cerr << "Config file error: " << e.what() << std::endl;
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

	try {
		Webserv server(pre_conf);
		server.add(pre_conf);

		server.prepare_all();
		server.bind_all();
		server.listen_all();
		server.timeout(1);
		std::cout << server.get_info_server() << std::endl;
		std::cout << server.get_info_instance() << std::endl;

		signal(SIGINT, false_while);
		signal(SIGPIPE, sigpipe_solving);

		char			wait[] = {'/', '-', '\\', '|', '/', '-', '\\', '|'};
		unsigned int	nbr = 0;
		while (_SIGNAL_END)
		{	
			server.fd_rst();
			std::cout << "wait... " << wait[nbr++ % 8] << "\r" << std::flush;
			int recept = select(server.get_greaterfd(), &server.get_readfd(), &server.get_writefd(), NULL, &server.timeout());
			if (recept)
			{
				server.check_server(); // TO CREATE CLIENT AND ACCEPT SOCKET
				server.check_client(); // TO IMPLEMENTE HEADER OR CONTINUE WORKING
			}
			server.timeout(1);	
			server.exec_client(); // TO LAUNCH CLIENT IF WORKING FALSE, USUALY GOES ONE TIME/REQUEST
			recept = 0;
			if (_BROKEN_PIPE)
			{
				server.show_client_list();
				_BROKEN_PIPE = false;
			}
		}
		std::cout << RED << "EXIT PROGRAM" << RST << std::endl;
		server.stop_all();
	}
	catch (std::exception &e) {
		std::cerr << "Webserv encountered a problem: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
