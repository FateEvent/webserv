/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_config.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:23:24 by stissera          #+#    #+#             */
/*   Updated: 2023/03/13 11:31:43 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.h"

s_config::s_config()
{
	set_zero();
}

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
			int max = std::stol(it->second);
			if (!(max > 0 && max < static_cast<int>(0x7FFFFFFF)))
				throw ("Max client incorrect in instance!");
			this->max_client = max;
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
		else if (!it->first.compare("error_page"))
		{
			if (!ft::put_err_page(it->second, this->error_page))
				throw std::invalid_argument("error page in config file not set properly!");
		}
		else if (!it->first.compare("location"))
			do_location(it->second);
		#ifdef DEBUG
		else
		{
			std::cout << "Unknown key " << it->first << " in config file" << std::endl;
			//throw ("Unknow parameter in config file!");
		}
		std::cout << "\033[0;33m" + it->first << " | " << it->second + "\033[0m" << std::endl;
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
	this->allow.clear();
	this->max_client = 0;
	this->max_body = 0;
	this->active = false;
	this->prepare = false;
	this->location.clear();
	this->error_page.clear();
	this->time_out = 0;
	this->other.clear();
	this->sock_fd = 0;
	this->domain = 2;
	this->type = 0;	
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
	std::string line;
	size_t		pos;
	while ((pos = loc.find(";")) != loc.npos)
	{
		line = loc.substr(0, pos);
		loc.erase(0, pos + 1);
		st->to.insert(std::make_pair(line.substr(0, line.find_first_of(" \v\f\r\n\t")),
									line.substr(line.find_last_of(" \v\f\r\n\t"))));
		#ifdef DEBUG
			std::cout << "Location line: " + line << std::endl;
		#endif
	}
}