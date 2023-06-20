/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:38:54 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/20 15:33:21 by hdelmas          ###   ########.fr       */
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
                        //   "Location: https://http.cat/status/418\r\n"
                        //   "\r\n";
	Socket	listener(PORT);
	const char *buffer = listener.receiveRequest();
	std::cout << buffer << std::endl;
	std::cout << "DONE" << std::endl;
	listener.sendResponse(response);
}