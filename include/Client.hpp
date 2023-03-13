/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 00:57:51 by stissera          #+#    #+#             */
/*   Updated: 2023/03/13 13:44:36 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <cstring>
# include <string>
# include <ctime>
# include <sys/types.h>
# include <map>
# include <vector>
# include <list>
# include <sys/un.h>
# include "../include/common.h"
# include "../include/search.hpp"

class Client
{
	private:
		int				_sock_fd;
		sockaddr_in		_addr;			// sockaddr_in is a internet socket
//		sockaddr_un		_addr_local;	// sockaddr_un is a local socket
		socklen_t		_socklen;
		const config&	_ref_conf;
//		time_t			_timeout; time(NULL) dans tous les cas si keep-alive, chercher si temps existe puis add, sinon add valeur par defaut config 
		fd_set			_readfd;
//		fd_set			_writefd;
		header			_header;
			
		void			_make_struct();
		
		
	public:
		Client(const config&);
		~Client();
		int				get_sockfd() const;
		void			test_client();
		std::string		find_path();

	protected:
};

#endif
