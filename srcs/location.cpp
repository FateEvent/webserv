/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 10:49:12 by stissera          #+#    #+#             */
/*   Updated: 2023/04/21 11:51:03 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::check_location()
{
	static struct stat path_stat;
	std::string	path;

	if (_header.other.find("Expect")->second.find("100") != std::string::npos)
		send(this->_sock_fd, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
	this->_max_body = this->_ref_conf.max_body == 0 ? this->_ref_conf._base->max_body : this->_ref_conf.max_body;
	if (this->_download.empty())
		this->_download = this->_ref_conf.download.empty() ? (this->_ref_conf._base != 0 && !this->_ref_conf._base->download.empty() ? this->_ref_conf._base->download : this->_ref_conf.root) : this->_ref_conf.download;
	this->_allow = this->_ref_conf.allow;
	this->_redirect = this->_ref_conf.redirect;
	for (std::vector<struct s_location>::const_iterator it = this->_ref_conf.location.begin();
				it != this->_ref_conf.location.end(); it++)
	{
		if (it->search.empty())
			this->simple_location(it);
		else
			this->condition_location(it);
	}

	if (this->_timeout == 0)
	{
		if (this->_ref_conf.time_out == 0)
			this->_timeout = this->_ref_conf._base->time_out + this->_header.time_out;
		else
			this->_timeout = this->_ref_conf.time_out + this->_header.time_out;
	}

	// CHECK IF DOWNLOAD REPERTORY EXIST ELSE CREATE.
	if (stat(this->_download.c_str(), &path_stat) == -1)
	{
		if (mkdir(this->_download.c_str(), S_IRWXU | S_IRGRP | S_IROTH))
			this->_download = this->_ref_conf.root;
	}
	else if (S_ISREG(path_stat.st_mode) || S_ISLNK(path_stat.st_mode))
			this->_download = this->_ref_conf.root;

	// CHECK DIR IS A FILE AND CHECK THE ROOT
	if (this->_root.empty())
	{
		if (!this->_header.Dir.empty() && this->_header.file.first.empty())
		{
			std::string	path_dir(this->_ref_conf.root + this->_header.Dir);
			stat(path_dir.c_str(), &path_stat);
			if (S_ISREG(path_stat.st_mode))
			{
				this->_root = this->_ref_conf.root + this->_header.Dir.substr(0, this->_header.Dir.find_last_of("/"));
				this->_index = this->_header.Dir.substr(this->_header.Dir.find_last_of("/") + 1);
			}
			else
				this->_root = this->_ref_conf.root + this->_header.Dir;
		}
		else
			this->_root = this->_ref_conf.root + this->_header.Dir;
	}
	if (this->_index.empty())
	{
		if (this->_header.file.second.empty() && this->_ref_conf.index.empty())
		{
			this->_data.file = ft::listing_creator(this->_root.c_str(), this->_header.Dir);
			if (!this->_data.file->good())
				return (false);
			this->_data.file->seekg(0, this->_data.file->end);
			this->_data.data_size = this->_data.file->tellg();
			this->_data.file->seekg(0, this->_data.file->beg);
			this->_index = "index.html";
			return true;
		}
		else if (this->_header.file.second.empty())
			this->_index = this->_ref_conf.index;
		else
			this->_index = this->_header.file.first + "." + this->_header.file.second; // ????
	}

	path = this->_root + "/" + this->_index;

/*   	#ifdef DEBUG
		std::cout << "Path of file is: \"" + path + "\"" << std::endl;
	#endif */

	this->_data.file = new std::fstream(path, std::ios::in | std::ios::binary);
	if (!this->_data.file->good())
		return (false);
	this->_data.file->seekg(0, this->_data.file->end);
	this->_data.data_size = this->_data.file->tellg();
	this->_data.file->seekg(0, this->_data.file->beg);
	return (true);
}

void	Client::simple_location(std::vector<struct s_location>::const_iterator &location)
{
	if (location->base.length() > this->_header.Dir.length())
		return ;
	if (std::strncmp((location->base + "/").c_str(), this->_header.Dir.c_str(), location->base.length()) == 0)
		for (std::multimap<std::string, std::string>::const_iterator it = location->to.begin(); it != location->to.end(); it++)
		{
			if (!it->first.find("root"))
			{
				this->_root = it->second;
				this->_root.append(this->_header.Dir, location->base.length());
			}
			else if (!it->first.find("index_page"))
			{
				if (this->_header.file.first.empty())
					this->_index = it->second;
				else
					this->_index = this->_header.file.first + "." + this->_header.file.second;
			}
			else if (!it->first.find("error_page"))
			{
				std::string err = it->second;
				ft::put_err_page(err, this->_error_page);
			}
			else if (!it->first.find("max_body_size"))
				this->_max_body = std::stol(it->second);
 			else if (!it->first.find("proxy_pass"))
				this->_proxy = it->second;
			else if (!it->first.find("allow"))
			{
				this->_allow = 0;
				if (it->second.find("GET") != it->second.npos)
					this->_allow |= 0x001;
				if (it->second.find("POST") != it->second.npos)
					this->_allow |= 0x002;
				if (it->second.find("DELETE") != it->second.npos)
					this->_allow |= 0x004;
			}
			else if (!it->first.find("return"))
				this->_redirect = it->second;
			else if (!it->first.find("download"))
				this->_download = it->second;
			else if (!it->first.find("cgi"))
				this->_cgi_call.insert(std::make_pair(it->second.substr(0, it->second.find_first_of(" \t\v\f")),
													it->second.substr(it->second.find_last_of(" \t\v\f") + 1)));
			else if (!it->first.find("time_out"))
				this->_timeout = std::stol(it->second) + this->_header.time_out;
		}
}

void	Client::condition_location(std::vector<struct s_location>::const_iterator &location)
{
	if (std::strncmp((location->base + "/").c_str(), this->_header.Dir.c_str(), location->base.length()) == 0)
		std::cout << "Location with double conditions doesn't work! Please fix this in your config file" << std::endl;
//	for (std::map<std::string, std::string>::const_iterator it = location->to.begin(); it != location->to.end(); it++)
//	{
//	}	
}
