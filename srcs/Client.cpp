/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 23:20:41 by stissera          #+#    #+#             */
/*   Updated: 2023/03/28 11:19:21 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(const config &config) : _ref_conf(config)
{
	this->clear_header();
	_socklen = sizeof(this->_addr);
	//FD_ZERO(&this->_readfd); // maybe don't need
	this->_sock_fd = accept(_ref_conf.sock_fd, reinterpret_cast<sockaddr *>(&this->_addr), reinterpret_cast<socklen_t *>(&this->_socklen));
	if (this->_sock_fd == -1)
	{
		std::cout << strerror(errno) << std::endl;
		throw std::invalid_argument("Socket error in constructor Client!");
	}
	fcntl(this->_sock_fd, F_SETFL, O_NONBLOCK);
}

Client::Client(const config &config, sockaddr_in sock, socklen_t len, int fd, header& head) : _ref_conf(config), _header(head)
{
	this->_working = false;
	this->_chunked = false;
	this->_fd_cgi = 0;
	this->_cgi = false;
	this->_sedding = false;
	this->_reponse.clear();
	this->_root.clear();
	this->_index.clear();
	this->_error_page.clear();
	this->_proxy.clear();
	this->_cgi_call.clear();
	this->_max_body = 0;
	this->other.clear();
	this->_data.data_sended = 1;
	this->_data.data_size = 0;
	this->_data.fd = 0;
	this->_data.header.clear();
	this->_data.file = NULL;
	
	this->_sock_fd = fd;
	this->_addr = sock;
	this->_socklen = len;
	this->_timeout = std::time(nullptr);
	this->_header.time_out = this->_timeout;
	this->_ready = true;
}

Client::~Client() {}
int	Client::get_sockfd() const 				{ return (this->_sock_fd);}
time_t	Client::get_time_alive() const		{ return (this->_timeout);}
std::string Client::get_methode() const		{ return (this->_header.Methode);}
std::string	Client::get_directory() const	{ return (this->_header.Dir);}
bool	Client::is_working() const			{ return (this->_working);}
bool	Client::is_chunk() const			{ return(this->_chunked);}
header*	Client::get_header()				{ return (&this->_header);}
const config*	Client::get_config() const	{ return (&this->_ref_conf);}
bool	Client::is_cgi() const				{ return (this->_cgi?true:false);}

std::pair<std::string, std::string>	Client::get_file() const
{
	if (this->_header.file.first.empty())
		throw;
	return (this->_header.file);
}

int	Client::get_fd_cgi() const
{
	if (this->is_cgi())
		return (this->_fd_cgi);
	return (0);
}

void	Client::clear_header()
{
	this->_header.clear();
	this->_working = false;
	this->_chunked = false;
	this->_fd_cgi = 0;
	this->_cgi = false;
	this->_sedding = false;
	this->_reponse.clear();
	this->_root.clear();
	this->_index.clear();
	this->_error_page.clear();
	this->_proxy.clear();
	this->_cgi_call.clear();
	this->_max_body = 0;
	this->other.clear();
	this->_data.data_sended = 1;
	this->_data.data_size = 0;
	this->_data.fd = 0;
	this->_data.header.clear();
	if (this->_data.file != 0)
		delete this->_data.file;
	this->_data.file = NULL;
	this->_ready = false;
}

bool	Client::new_request()
{
	if (!ft::parse_header(this->_sock_fd, this->_header))
			return (false);
	this->_timeout = std::time(nullptr);
	this->_header.time_out = this->_timeout;
	this->_ready = true;
	return (true);
}

bool	Client::is_seeding() const	{ return (this->_sedding?true:false); }
bool	Client::is_ready() const	{ return (this->_ready?true:false); }

bool	Client::send_data(int fd)
{
	int size;
	socklen_t len = sizeof(size);
	if (getsockopt(this->_sock_fd, SOL_SOCKET, SO_SNDBUF, &size, &len) == -1)
	{
		std::cout << RED << "SOCKET PROBLEM!" << RST << std::endl;
		return (false);
	}
	this->_data.file->seekg(this->_data.data_sended - 1, this->_data.file->beg);
	char buff[size + 1];
	std::memset(buff, 0, size + 1);
	this->_data.file->read(buff, size);

	ssize_t s = send(fd, buff,  this->_data.file->gcount(), 0);
	if (s == -1 || s == 0)
	{
		return (false);
	}
	this->_data.data_sended += s;
	if (this->_data.data_sended >= this->_data.data_size)
		return (true);
	return (false);
}

bool	Client::continue_client(fd_set *fdset)
{
	if (this->_sedding)
	{
		if (this->send_data(this->_sock_fd))
		{
			this->clear_header();
			this->_index.clear();
			this->_root.clear();
			std::cout << YELLOW << "Sending finish for socket " << this->_sock_fd << RST << std::endl;
			return (true);
		}
		return (false);
	}
	else if (this->_chunked)
		this->chunk();
	else if (FD_ISSET(this->_cgi, fdset))
		this->take_data();
	else
	{
		if (!_header.Methode.compare("GET"))
		{
			std::cout << "GET METHODE" << std::endl;
		}
		else if (!_header.Methode.compare("POST"))
		{
			std::cout << "POST METHODE" << std::endl;
		}
		else if (!_header.Methode.compare("DELETE"))
		{
			std::cout << "DELETE METHODE" << std::endl;
		}
		else
			std::cout << "PROBLEME EXIST IF ON SCREEN!!!!!continue_client" << std::endl;
	}
	return (false);
}

void	Client::execute_client(bool path)
{
	if (!path && !this->is_working())
	{
		std::cout << PURPLE << "File not found or can't open!" << RST << std::endl;
		std::string body, header;
		header = ft::make_header(404);
		body = ft::get_page_error(404, this->_error_page[404].empty() ?
					(this->_ref_conf.error_page.find(404)->second.empty() ?
					this->_ref_conf._base->error_page.find(404)->second : this->_ref_conf.error_page.find(404)->second) :
					this->_error_page[404]);
		header.append(body);
		header.append("\r\n\r\n");
		send(this->_sock_fd, header.c_str(), header.length(), 0);
	}
	else if (_header.Methode.compare("GET") == 0)
	{
		std::cout << "GET METHODE" << std::endl;
		if (!this->_ref_conf.cgi.empty() &&
				(_ref_conf.cgi.find(_header.file.second) != _ref_conf.cgi.end() ||
				_cgi_call.find(_header.file.second) != _cgi_call.end()))
		{
			if (_ref_conf.cgi.find(this->_header.file.second) != this->_ref_conf.cgi.end())
			{
				launch_cgi(this->_ref_conf.cgi.find(this->_header.file.second)->second);
				std::cout << "CGI on base" << std::endl;
			}
			else
			{
				std::cout << "CGI on location" << std::endl;
			}
		}
		else
		{
			std::string	header;
			this->_data.header = ft::make_header(200);
			this->_data.header.append("Content-Length: " + std::to_string(this->_data.data_size) + "\r\n");
			this->_data.header.append(ft::make_content_type(this->_index.substr(this->_index.find_last_of(".") + 1)));
			int check = 0;
			check = send(this->_sock_fd, this->_data.header.c_str(), this->_data.header.length(), 0);
			if (check == -1)
			{
				// ERROR 500
			}
			if (check == 0)
			{
				// ERROR 501
			}
			std::cout << BLUE << this->_data.header << RST << std::endl;
			this->_sedding = true;
			//this->_working = true;
		}
	}
	else if (_header.Methode.compare("POST") == 0)
	{
		std::cout << "POST METHODE" << std::endl;
		send(this->_sock_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 53\r\n\r\n<html><head></head><body>POST ERROR 404</body></html>\0", 100, MSG_OOB); // , NULL, 0);
		this->_working = true;
	}
	else if (_header.Methode.compare("DELETE") == 0)
		std::cout << "DELETE METHODE" << std::endl;
	else
		std::cout << "BAD REQUEST / BAD HEADER" << std::endl;
}

void	Client::launch_cgi(std::string path)
{
	std::string STR = 0;
	std::vector<std::string> env;
	env.push_back("REQUEST_METHOD=" + this->get_methode()); // : La méthode HTTP utilisée dans la requête (GET, POST, PUT, DELETE, etc.).
	env.push_back("SERVER_PROTOCOL=HTTP1.1");
	env.push_back("PATH_INFO=");
	
	env.push_back("SERVER_NAME=" + this->_header.Host); // Le nom du serveur web.
	env.push_back("SERVER_PORT=" + std::to_string(this->_ref_conf.port)); // Le port sur lequel le serveur web écoute les requêtes.
	env.push_back("SERVER_SOFTWARE=" + STR); // Le nom et la version du serveur web utilisé.
	env.push_back("SCRIPT_NAME=" + STR); // Le chemin d'accès relatif du script CGI à partir de la racine du serveur web.
	env.push_back("SCRIPT_FILENAME=" + path); // Le chemin d'accès absolu du script CGI sur le serveur.
	env.push_back("DOCUMENT_ROOT=" + this->_root); // Le chemin d'accès absolu du répertoire racine du site web.
	env.push_back("QUERY_STRING=" + STR); // La chaîne de requête (paramètres de la requête) envoyée avec la requête HTTP.
	env.push_back("HTTP_COOKIE=" + STR); // Les cookies HTTP envoyés avec la requête.
	env.push_back("HTTP_USER_AGENT=" + this->_header.User_Agent); // Le nom et la version du navigateur ou du client HTTP utilisé pour envoyer la requête.
	env.push_back("HTTP_REFERER=" + STR); // L'URL de la page précédente qui a conduit à la requête actuelle.
	env.push_back("REMOTE_ADDR=" + STR); // L'adresse IP de l'utilisateur qui a envoyé la requête.
	env.push_back("REMOTE_HOST=" + STR); // Le nom d'hôte de l'utilisateur qui a envoyé la requête.
	env.push_back("REMOTE_USER=" + STR); // Le nom d'utilisateur fourni par l'utilisateur dans le cadre d'une authentification HTTP.
	//env.push_back("CONTENT_TYPE=" + this->_header.Content_Type.begin()->data()); // Le type MIME du corps de la requête HTTP (par exemple, application/json).
	env.push_back("CONTENT_LENGTH=" + std::to_string(this->_header.Content_Length)); // La longueur (en octets) du corps de la requête HTTP.
	env.push_back("DATE_GMT=" + STR); // 	Date actuelle au format GMT
	env.push_back("DATE_LOCAL=" + STR); // 	Date actuelle au format local
	env.push_back("DOCUMENT_ROOT=" + this->_root); // 	Racine des documents Web sur le serveur
	env.push_back("GATEWAY_INTERFACE=" + STR); // 	Version des spécifications CGI utilisées par le serveur
	env.push_back("HTTP_HOST=" + this->_header.Host); // 	Nom de domaine du serveur
	env.push_back("SERVER_ADMIN=" + STR); // 	Adresse électronique de l'administrateur du serveur
	env.push_back("SERVER_SOFTWARE=" + STR); // 	Type (logiciel) du serveur web
	env.push_back(0);
}

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

void	Client::chunk()
{
	
}

void	Client::take_data()
{
	
}