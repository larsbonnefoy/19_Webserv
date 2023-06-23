/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:38:54 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/23 18:54:50 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>

#include "socket/Socket.hpp"
#include "log/Log.hpp"
#define MAX_EVENT 10
int main()
{
	const char *response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: 617\r\n"
                          "\r\n"
                          "<!DOCTYPE html>\r\n"
                          "<html lang=\"en\">\r\n"
                          "<head>\r\n"
                          "  <meta charset=\"UTF-8\" />\r\n"
                          "  <title>Hello, world!</title>\r\n"
                          "  <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\" />\r\n"
                          "  <meta name=\"description\" content=\"\" />\r\n"
                          "  <link rel=\"icon\" href=\"favicon.png\">\r\n"
                          "</head>\r\n"
                          "<body>\r\n"
                          "    <h1>Hello</h1>\r\n"
                          "    <form method=\"POST\">\r\n"
                          "        <label for=\"fname\">First name:</label><br>\r\n"
                          "        <input type=\"text\" id=\"fname\" name=\"fname\"><br>\r\n"
                          "        <label for=\"lname\">Last name:</label><br>\r\n"
                          "        <input type=\"text\" id=\"lname\" name=\"lname\"><br><br>\r\n"
                          "        <input type=\"submit\" value=\"Submit\">\r\n"
                          "    </form>\r\n"
                          "</body>\r\n"
                          "</html>";

	// const char *response1 = "HTTP/1.1 302 Found\r\n"
    //                       "Location: https://http.cat/status/418\r\n"
    //                       "\r\n";
	struct pollfd	openSockets[2];
	Socket	listener(8282);
	Socket	listener1(8181);
	// Socket	listener2(80);
		
	Socket *Sockets[2];
	Sockets[0] = &listener;	
	Sockets[1] = &listener1;
	// Sockets[2] = &listener2;	
	openSockets[0].fd = Sockets[0]->getServerSocket();
	openSockets[0].events = POLLIN | POLLPRI;
	openSockets[1].fd = Sockets[1]->getServerSocket();
	openSockets[1].events = POLLIN | POLLPRI;
	// openSockets[2].fd = Sockets[2]->getServerSocket();
	// openSockets[2].events = POLLIN | POLLPRI;
	
	while (1)
	{
		int returnPoll = poll(openSockets, 2, -1);
		if (returnPoll < 0)
			return (404);
		else if (returnPoll > 0)
		{
			for (int i = 0; i < 2; ++i)
			{
				if (openSockets[i].revents & POLLIN)
				{
					Sockets[i]->connectClient();
					const char *buffer = Sockets[i]->receiveRequest();
					logFile(buffer);
					logFile("DONE");
					Sockets[i]->sendResponse(response);			
					logFile(response);
					openSockets[i].revents = 0;
				}
			}			
		}
	}
	// const char *buffer = listener.receiveRequest();
	// // const char *buffer1 = listener1.receiveRequest();
	// logFile(buffer);
	// logErr("DONE");
	// // std::cout << buffer << std::endl;
	// // std::cout << "DONE" << std::endl;
	// listener.sendResponse(response);
	// logFile(response);
	// // listener1.sendResponse(response1);
	// Socket	listener1(listener);
	// const char *buffer1 = listener1.receiveRequest();
	// log(buffer1);
	// listener1.sendResponse(response1);
	// logFile(response1);
}
