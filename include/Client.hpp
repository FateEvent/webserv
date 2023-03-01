/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:57:51 by stissera          #+#    #+#             */
/*   Updated: 2023/03/01 14:03:46 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <cstring>
# include <string>
# include <time.h>
# include <sys/types.h>
# include <map>
# include <vector>
# include <list>
# include "../include/common.h"
# include "../include/search.hpp"
# include <sys/un.h>

class Client
{
	private:
		int				_sock_fd;
		sockaddr_in		_addr;			// sockaddr_in is a internet socket
//		sockaddr_un		_addr_local;	// sockaddr_un is a local socket
		socklen_t		_socklen;
		const config&	_ref_conf;
//		timeval			_timeout;
		fd_set			_readfd;
//		fd_set			_writefd;
		header			_header;
			
		void			_make_struct();
		
		
	public:
		Client(const config&);
		~Client();
		int				get_sockfd() const;
		void			test_client();

	protected:
};

#endif
