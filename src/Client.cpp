/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/02/22 23:49:17 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(const config &config) : _ref_conf(config)
{
	_socklen = sizeof(this->_addr);
	FD_ZERO(&this->_readfd);
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	if (this->_sock_fd == -1)
		throw ("Socket error in constructor Client!");
}

Client::~Client()
{}

int	Client::get_sockfd() const
{
	return (this->_sock_fd);
}

void	Client::test_client()
{
	char buffer[4096];
	std::string	header;
	int i = -1;

	while (i == -1 || i == 4095)
	{
		i = recv(this->_sock_fd, &buffer, 4095, 0);
		header.append(buffer);
		memset(buffer, 0, 4096);
	}
	std::cout << header << std::endl;
	sendto(this->_sock_fd, "SALUT", 6, MSG_OOB, NULL, 0);
}