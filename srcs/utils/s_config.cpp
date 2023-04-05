/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_config.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:23:24 by stissera          #+#    #+#             */
/*   Updated: 2023/04/05 22:48:50 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.h"

s_config::s_config()	{ set_zero(); }

s_config::s_config(std::multimap<std::string, std::string>& server)
{
	set_zero();
	for (std::multimap<std::string, std::string>::iterator it = server.begin(); it != server.end(); it++)
	{
		if (!it->first.compare("name"))
		{
			if (it->second.empty())
				throw ("No instance name!");
			this->name = it->second;
		}
		else if (!it->first.compare("protocol"))
		{
			if (it->second.empty())
				this->domain = AF_INET;
			else if (it->second.compare("IPV4") || it->second.compare("INET") || it->second.compare("AF_INET"))
				this->domain = AF_INET;
			else if (it->second.compare("IPV6") || it->second.compare("INET6") || it->second.compare("AF_INET6"))
				this->domain = AF_INET6;
			else if (it->second.compare("local") || it->second.compare("LOCAL"))
				this->domain = AF_LOCAL;
			else
				throw ("Socket protocol invalid!");
			this->addr.sin_family = this->domain;
		}
		else if (!it->first.compare("host"))
		{
			unsigned int ip[4];
			if (!sscanf(it->second.data(), "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]))
				throw ("IP bad host in config file");
			this->ip = it->second;
			this->addr.sin_addr.s_addr =  (ip[0] % 256 << 0 | 0) |\
										(ip[1] % 256 << 8 | 0) |\
										(ip[2] % 256 << 16 | 0) |\
										(ip[3] % 256 << 24 | 0);
		}
		else if (!it->first.compare("listen"))
		{
			if ((std::stoul(it->second) < 1) && (std::stoul(it->second) >= 0xFFFFFF))
				throw ("Invalid port");
			this->port = std::stoul(it->second);
			this->addr.sin_port = htons(this->port);
		}
		else if (!it->first.compare("type"))
		{
			if (!it->second.compare("tcp"))
				this->type = SOCK_STREAM;
			else if (!it->second.compare("udp"))
				this->type = SOCK_DGRAM;
			else
				this->type = SOCK_STREAM;;
		}
		else if (!it->first.compare("max_client"))
		{
			ssize_t max = std::stol(it->second);
			if (!(max > 0 && max < static_cast<ssize_t>(0x7FFFFFFF)))
				throw ("Max client incorrect in instance!");
			this->max_client = max;
		}
		else if (!it->first.compare("max_body_size"))
		{
			ssize_t max = std::stol(it->second);
			if (max < 0)
				throw ("Max body size incorrect in instance!");
			this->max_body = max;
		}
		else if (!it->first.compare("root"))
		{
			if (it->second.empty())
				throw ("root destination empty!");
			this->root = it->second;
		}
		else if (!it->first.compare("index_page"))
		{
			if (it->second.empty())
				throw ("no index referenced!");
			this->index = it->second;
		}
		else if (!it->first.compare("allow"))
		{
			if (it->second.find("GET") != it->second.npos)
				this->allow |= 0x001;
			if (it->second.find("POST") != it->second.npos)
				this->allow |= 0x002;
			if (it->second.find("DELETE") != it->second.npos)
				this->allow |= 0x004;
		}
		else if (!it->first.compare("error_page"))
		{
			if (!ft::put_err_page(it->second, this->error_page))
				throw std::invalid_argument("error page in config file not set properly!");
		}
		else if (!it->first.compare("return"))
			this->redirect = it->second;
		else if (!it->first.find("location"))
			do_location(it->second);
		else if (!it->first.find("cgi"))
			this->cgi.insert(std::make_pair(it->second.substr(0, it->second.find_first_of(" \t\v\f")),
											it->second.substr(it->second.find_last_of(" \t\v\f") + 1)));
		#ifdef DEBUG
		else
		{
			std::cout << "Unknown key " << it->first << " in config file - s_config.cpp" << std::endl;
			//throw ("Unknow parameter in config file!");
		}
		//std::cout << "\033[0;33m" + it->first << " | " << it->second + "\033[0m" << std::endl;
		#endif
	}
}

void s_config::set_zero()
{
	this->name.clear();
	this->root.clear();
	this->index.clear();
	this->ip.clear();
	this->port = 0;
	this->error_log.clear();
	this->cgi.clear();
	this->allow = 0;
	this->nbr_client = 0;
	this->max_client = -1;
	this->max_body = 0;
	this->redirect.clear();
	this->active = false;
	this->prepare = false;
	this->location.clear();
	this->error_page.clear();
	this->time_out = 0;
	this->other.clear();
	this->sock_fd = 0;
	this->domain = 2;
	this->type = 0;	
	this->root.clear();
	this->index.clear();
}

void s_config::do_location(std::string loc)
{
	t_location st;
	size_t start , end;
	
	start = loc.find_first_not_of(" \v\f\r\n\t");
	end = loc.find_first_of(" \v\f\r\n\t{", start);

	if (start != loc.npos)
		st.base = loc.substr(start, end);
	else
		throw std::invalid_argument("location error!");
	start = loc.find_first_not_of(" \v\f\r\n\t", end);

	if (start == loc.npos)
		throw std::invalid_argument("location error!");
	if (loc[start] == '{')
		st.search.clear();
	else
	{
		end = loc.find_first_of(" \v\f\r\n\t{", start);
		st.search = loc.substr(start, end);
	}
	start = loc.find_first_of("{", end) + 1;
	end = loc.find_last_of("}") - 1;
	if (start == loc.npos || end == loc.npos)
		throw std::invalid_argument("location error!");
	get_all_loc(loc.substr(start, end), &st);
	this->location.push_back(st);
}

void	s_config::get_all_loc(std::string loc, struct s_location *st)
{
	std::string	line;
	std::string	key;
	size_t		pos;

	while ((pos = loc.find(";")) != loc.npos)
	{
		line = loc.substr(0, pos);
		key = line.substr(0, line.find_first_of(" \v\f\r\n\t"));
		line = line.substr(line.find_first_of(" \v\t\r\n\t"));
		line = line.substr(line.find_first_not_of(" \v\t\r\n\t"));
		loc.erase(0, pos + 1);
		st->to.insert(std::make_pair(key, line.substr(line.find_first_not_of(" \v\f\r\n\t"))));
	}
}

bool	s_config::if_max_client() const
{
	if (this->nbr_client >= this->max_client)
		std::cout << YELLOW << "Client in qeue." << RST << std::endl;
	return (this->nbr_client < this->max_client ? false : true);
}