/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location .cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 10:49:12 by stissera          #+#    #+#             */
/*   Updated: 2023/03/31 10:49:31 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

bool	Client::check_location()
{
	std::string	path;
	for (std::vector<struct s_location>::const_iterator it = this->_ref_conf.location.begin();
				it != this->_ref_conf.location.end(); it++)
	{
		if (it->search.empty())
			this->simple_location(it);
		else
			this->condition_location(it);
	}
	if (this->_root.empty())
		this->_root = this->_ref_conf.root + this->_header.Dir;
	if (this->_index.empty())
	{
		if (this->_header.file.second.empty())
			this->_index = this->_ref_conf.index;
		else
			this->_index = this->_header.file.first + "." + this->_header.file.second; // ????
	}
	path = this->_root + "/" + this->_index;
	#ifdef DEBUG
		std::cout << "Path of file is: " + path << std::endl;
	#endif
	this->_data.file = new std::ifstream(path, std::ios::binary);
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
 			else if (!it->first.find("proxy_pass"))
			{
				this->_proxy = it->second;
			}
			else if (!it->first.find("cgi"))
			{
				this->_cgi_call.insert(std::make_pair(it->second.substr(0, it->second.find_first_of(" \t\v\f")),
													it->second.substr(it->second.find_last_of(" \t\v\f") + 1)));
			}
		}
}

void	Client::condition_location(std::vector<struct s_location>::const_iterator &location)
{
	if (std::strncmp((location->base + "/").c_str(), this->_header.Dir.c_str(), location->base.length()) == 0)
		std::cout << "Location double condition can't work! Please fix this in your config file" << std::endl;
//	for (std::map<std::string, std::string>::const_iterator it = location->to.begin(); it != location->to.end(); it++)
//	{
//	}	
}
