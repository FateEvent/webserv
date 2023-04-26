/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:53:10 by stissera          #+#    #+#             */
/*   Updated: 2023/04/22 02:27:20 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

int	Client::launch_cgi(std::string path)
{
	std::string STR;
	std::vector<std::string> env;
	char hostName[NI_MAXHOST];

	env.push_back("REQUEST_METHOD=" + this->get_method()); // : La méthode HTTP utilisée dans la requête (GET, POST, PUT, DELETE, etc.).
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	if (this->_header.Dir.empty())
	{
		env.push_back("PATH_INFO=/" + this->_index); // NOT REALY GOOD... SHOULD BE AFTER THIS CALLING FILE
		env.push_back("REQUEST_URI=/" + this->_index + (!this->_header.get_var.empty()?("?" + this->_header.get_var):""));
	}
	else
	{
		env.push_back("PATH_INFO=" + this->_header.Dir + "/" + this->_index); // NOT REALY GOOD... SHOULD BE AFTER THIS CALLING FILE
		env.push_back("REQUEST_URI=" + this->_header.Dir + "/" + this->_index + (!this->_header.get_var.empty()?("?" + this->_header.get_var):"")); // SHOULD BY cgi_tester
	}
	env.push_back("SCRIPT_NAME=" + this->_root + "/" + this->_index); // Le chemin d'accès relatif du script CGI à partir de la racine du serveur web.
	env.push_back("SERVER_NAME=Webserv"); // Le nom du serveur web.
	env.push_back("SERVER_PORT=" + std::to_string(this->_ref_conf.port)); // Le port sur lequel le serveur web écoute les requêtes.
	env.push_back("SERVER_SOFTWARE=0.2"); // Le nom et la version du serveur web utilisé.
	env.push_back("SCRIPT_FILENAME=" + this->_root + "/" + this->_index); // Le chemin d'accès absolu du script CGI sur le serveur.
	env.push_back("DOCUMENT_ROOT=" + this->_root); // Le chemin d'accès absolu du répertoire racine du site web.
	env.push_back("QUERY_STRING=" + this->_header.get_var); // La chaîne de requête (paramètres de la requête) envoyée avec la requête HTTP.
	env.push_back("CONTENT_LENGTH=" + std::to_string(this->_header.Content_Length)); // La longueur (en octets) du corps de la requête HTTP.
	env.push_back("HTTP_USER_AGENT=" + this->_header.User_Agent); // Le nom et la version du navigateur ou du client HTTP utilisé pour envoyer la requête.
	env.push_back("DOCUMENT_ROOT=" + this->_root); // 	Racine des documents Web sur le serveur
	env.push_back("GATEWAY_INTERFACE=CGI/1.1"); // 	Version des spécifications CGI utilisées par le serveur
	env.push_back("HTTP_HOST=" + this->_header.Host); // 	Nom de domaine du serveur
	env.push_back("SERVER_ADMIN=noname@nomail.com"); // 	Adresse électronique de l'administrateur du serveur
	env.push_back("SERVER_SOFTWARE=Webserv/0.2"); // 	Type (logiciel) du serveur web
	if (this->_header.other.find("Referer") != this->_header.other.end())
		env.push_back("HTTP_REFERER=" + this->_header.other.find("Referer")->second); // L'URL de la page précédente qui a conduit à la requête actuelle.
	env.push_back("REMOTE_USER=" + STR); // Le nom d'utilisateur fourni par l'utilisateur dans le cadre d'une authentification HTTP.
	
	if (!this->_header.Content_Type.empty())
		env.push_back("CONTENT_TYPE=" + this->_header.Content_Type.front()); // Le type MIME du corps de la requête HTTP (par exemple, application/json).
	if (!this->_header.Cookie.empty())
	{
		std::string cookie;
		for (std::map<std::string, std::string>::iterator it = this->_header.Cookie.begin();
				it != this->_header.Cookie.end(); ++it)
			if (it->first != "")
				cookie.append(it->first + "=" + it->second + "; ");
		cookie.resize(cookie.size() - 2);
		env.push_back("HTTP_COOKIE=" + cookie); // Les cookies HTTP envoyés avec la requête.
	}
	std::string remote = "REMOTE_ADDR=";
	remote += inet_ntoa(this->_addr.sin_addr);
	env.push_back(remote); // L'adresse IP de l'utilisateur qui a envoyé la requête.
    if (!getnameinfo((struct sockaddr*)&this->_addr, sizeof(this->_addr), hostName, sizeof(hostName), NULL, 0, 0))
		env.push_back(std::string("REMOTE_HOST=") + hostName); // Le nom d'hôte de l'utilisateur qui a envoyé la requête.
	//env.push_back("DATE_GMT="); // 	Date actuelle au format GMT
	//env.push_back("DATE_LOCAL="); // 	Date actuelle au format local
	
	std::string file(this->_root + "/" + this->_index);
	char **ENVP = ft::vector_to_tab(env);
	char *ARGV[] = {const_cast<char*>(path.c_str()), const_cast<char*>(file.c_str()), NULL};


	if (pipe(this->_pipe_cgi) == -1)
		return (503);
	this->_pid_cgi = fork();
	if (this->_pid_cgi > 0)
	{
		// MAIN
		close(this->_pipe_cgi[1]);
		fcntl(this->_pipe_cgi[0], F_SETFL, O_NONBLOCK);
		//usleep(50000);
		// a fermer ou rediriger dans le cgi....
 	//	if (this->_header.Method.find("GET") != this->_header.Method.npos ||
	//		this->_header.Content_Length == 0)
		//	close(this->_pipe_cgi[1]);
		this->_cgi = true;
	}
	else if (this->_pid_cgi == -1)
	{
		std::cout << RED << "Fork error: can't create a fork!" << RST << std::endl;
		this->_pid_cgi = 0;
		// CLOSE PIPE
		return (503);
	}
	else
	{
		// CHILD
		close(this->_pipe_cgi[0]);
//		fcntl(this->_pipe_cgi_out[0], F_SETFL, ~O_NONBLOCK);
		if (this->_header.Method.find("GET") != this->_header.Method.npos ||
			this->_header.Content_Length == 0)
			close(STDIN_FILENO);
		else if (this->_header.Method.find("POST") != this->_header.Method.npos)
		{
			if (!this->is_chunk())
			{
				//fcntl(this->_sock_fd, F_SETFL, ~O_NONBLOCK);
				dup2(this->_sock_fd, STDIN_FILENO);
			}
			else
			{
				dup2(this->_sock_fd, STDIN_FILENO); // FOR TEST
				//DO CHUNK MODE -- WE ARE IN CHILD WE CAN TAKE ALL THE DATA IN SOCKET AND WORK AFTER. CAN'T BLOCK IN MAIN PROGRAM ///
			}
		}
		dup2(this->_pipe_cgi[1], STDOUT_FILENO);
		dup2(this->_pipe_cgi[1], STDERR_FILENO); // RECEIPT ERROR IN PIPE, TO SEE WHAT THE PROBLEM IF EXIST.
		if (execve(path.c_str(), ARGV, ENVP) < 0)
		{
			try {
				std::string header(ft::make_header(500));
				header.append(ft::get_page_error(500, this->_error_page[500].empty() ?
						(this->_ref_conf.error_page.find(500)->second.empty() ?
						this->_ref_conf._base->error_page.find(500)->second : this->_ref_conf.error_page.find(500)->second) :
						this->_error_page[500]));
				std::cout << header;
			} catch (std::exception &e) {
				std::string header = "0 No Response";
				std::cerr << "The header couldn't be created: " << e.what() << std::endl;
			}
			delete[] ENVP; // not good lot of new inside ENVP
			std::exit(EXIT_FAILURE);
      	}
	}
	int i = 0;
	while (ENVP[i])
		delete[] ENVP[i++];
	delete[] ENVP;
	return (0);
}
