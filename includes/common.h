/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:21:50 by stissera          #+#    #+#             */
/*   Updated: 2023/04/21 11:31:20 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef COMMON_H
# define COMMON_H
# define DEBUG
# include <iostream>
# include <string>
# include <ctime>
# include <map>
# include <vector>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <sstream>
# include "./utils.hpp"

# define RED "\033[0;31m"
# define BLUE "\033[0;34m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define PURPLE "\033[0;35m"
# define BLACK "\033[0;30m"
# define WHITE "\033[0;37m"
# define RST "\033[0m"

# define BUFFER_SIZE_KB 1024
# define BUFFER_SIZE_MB 1024 * 1024

typedef struct	s_config
{
	s_config();
	s_config(std::multimap<std::string, std::string>&);
	void	set_zero();
	void	do_location(std::string);
	void	get_all_loc(std::string, struct s_location*);
	bool	if_max_client() const;
	std::string							name;
	std::string							root;
	std::string							index;
	std::string							ip;
	uint16_t							port;
	std::string							error_log;
	std::map<std::string, std::string>	cgi;
	std::string							proxy;
	int									allow;
	std::string							redirect;
	ssize_t								max_client;
	ssize_t								nbr_client;
	ssize_t								max_body;
	std::string							download;
	bool								active;
	bool								prepare;
	std::vector<struct s_location>		location;
	std::map<int, std::string>			error_page;
	long								time_out;
	std::map<std::string, std::string>	other;
	// Bind and socket
	sockaddr_in							addr;
	int									sock_fd;	// socket() return
	int									domain;		// Type AF_INET, AF_LOCAL, AF_LINUX.... # We use only AF_INET
	int									type;		// type TCP,UDP... SOCK_STREAM, SOCK_DGRAM
	struct s_config						*_base;
}	config;

typedef struct s_header
{
	s_header();
	void								clear();
	void								print_all();
	bool								split_dir();
	std::string							Method;
	std::string							Dir;
	std::string							Host;
	std::string							User_Agent;
	ssize_t								Content_Length;
	std::vector<std::string>			Content_Type;
	std::vector<std::string>			Content_Encoding;
	std::vector<std::string>			Transfer_Encoding;
	std::vector<std::string>			Accept;
	std::map<std::string, std::string>	Cookie; // change to a ref of cookie in instance
	ssize_t								Keep_Alive;
	std::time_t							time_out;
	std::string							Connexion;
	std::string							get_var;
	std::map<std::string, std::string>	body_var;
	std::string							header_clt;
	// A voir MISS some directives...
	std::pair<std::string, std::string>	file;
	std::map<std::string, std::string>	other; // Need change to a map<string, string>
	// FABIO BOUNDARY
	std::string							Boundary;
	std::vector<std::string>			Content_Disposition;
	std::string							entry_name;
	std::string							filename;
}	header;

typedef struct s_location
{
	std::string								base; 	// first argument after location
	std::string								search;	// second before open bracket if exist"
	std::multimap<std::string, std::string>	to; 	// what we do (name, directive).
}	t_location;

typedef struct s_clt_data_in
{
	std::fstream		*temporary;
	int					receipt; // Only for take socket to file don't change the value; 
	ssize_t				size;
	std::string			file_buf;
	char				tmpfile[11];
	std::fstream		*filename;
	ssize_t				pos_seek;
	std::vector<size_t>	bound_seek; // seek position of boundary starting/end
	size_t				in_bound; // Maybe don t need .... not used actualy
}	clt_data_in;

typedef struct s_clt_data
{
	s_clt_data();
	std::iostream			*file;
	size_t					minus_header; // substract this number on data_size before make header (for content-length)
	std::string				header;
	ssize_t					data_size;
	ssize_t					data_sent;
	int						wsatus; // status du fork
	int						wpid; // status de la reception
	//std::FILE				*cgi_return;
	struct s_clt_data_in	_in;
}	clt_data;



namespace ft {
	bool				parse_header(int, s_header&);
	std::stringstream*	listing_creator(char const *directory, std::string here);
};

#endif