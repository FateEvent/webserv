/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clt1.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:47:46 by averon            #+#    #+#             */
/*   Updated: 2023/02/28 10:28:18 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream> // ***
#include <sstream> // ***

#define PORT 8080

int main(int argc, char const *argv[])
{
    
	
	int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

	// *** essai d'envoi d'une requete http  avec un flux sstring

	std::string hostname = "www.example.com"; 
  	std::string path = "/path-teque"; 
	std::ostringstream request;

	request << "GET " << path << " HTTP/1.1\r\n";
	request << "Host: " << hostname << "\r\n";
	request << "Accept: everything\r\n";
	request << "Connection: close\r\n";
	request << "\r\n";

	std::string request_str = request.str();
	ssize_t sent = send(sock, request_str.c_str(), request_str.size(), 0);
	if (sent == -1) 
	{
    	std::cerr << "send: " << std::strerror(errno) << std::endl;
		close(sock);
    	return 1;
  	}
	std::cout << "\nHEADER ENVOYE AU SERVEUR:\n" << std::endl; 
	std::cout << request.str() << std::endl; 
	//-------------------------------------

    //send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}