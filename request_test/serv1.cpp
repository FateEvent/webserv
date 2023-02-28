/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:47:09 by averon            #+#    #+#             */
/*   Updated: 2023/02/28 10:10:25 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h> //for socket programmong
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include "request.hpp"

#define PORT 8080
int main(int argc, char const *argv[])
{
	/*	In summary, our server will have a socket that:

	listens for incoming network connections and puts them on a Queue
	accepts a network connection from the Queue one at a time
	reads the data (Request) sent from the client over the network connection
	sends data (Response) to the client over the network connection.
	*/
	
	int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "Hello from server";
	

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
  		std::cout << "AFFICHAGE DU BUFFER RECU PAR LE SERVEUR:\n"<< std::endl;      
		printf("%s",buffer );
        write(new_socket , hello , strlen(hello));   

// *** essai de parsing d'un header http

		request requ_1(buffer);

		std::cout << "AFFICHAGE AVEC ITERATEUR APRES PARSING:\n"<< std::endl;
		for (std::map<std::string, std::string>::const_iterator it = requ_1.headers.begin(); it != requ_1.headers.end(); it++) 
		{
			std::cout << it->first << " => " << it->second<< std::endl;
		}
		std::cout << std::endl;
	
		std::cout << "AFFICHAGE SANS ITERATEUR APRES PARSING:\n"<< std::endl;
		std::cout << "Key Method: 		value = " << requ_1.headers["Method"] << std::endl;
		std::cout << "Key Path: 		value = " << requ_1.headers["Path"] << std::endl;
		std::cout << "Key Protocol:		value = " << requ_1.headers["Protocol"] << std::endl;
		std::cout << "Key Host: 		value = " << requ_1.headers["Host"] << std::endl;
		std::cout << "Key Accept: 		value = " << requ_1.headers["Accept"] << std::endl;
		std::cout << "Key Connection: 	value = " << requ_1.headers["Connection"] << std::endl;	

// ****

	    printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}
