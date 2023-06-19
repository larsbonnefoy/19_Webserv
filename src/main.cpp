/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 12:38:54 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/19 19:20:51 by hdelmas          ###   ########.fr       */
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
#define PORT 8080
#define BUFF_SIZE 1024
int main(int ac, char **av)
{
	int 				socket_fd;
	int					opt;
	int					addr_len;
	struct sockaddr_in	addr;
	int 				new_socket;
	char				buffer[BUFF_SIZE];
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

	// const char *response = "HTTP/1.1 302 Found\r\n"
    //                       "Location: https://http.cat/status/418\r\n"
    //                       "\r\n";
	opt = 1;
	addr_len = sizeof(addr);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		return (1); // error
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		return (2);
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(PORT);
	if (bind(socket_fd, (struct sockaddr *)&addr, (socklen_t)addr_len) == -1)
		return (3);
	if (listen(socket_fd, 1) == -1)
		return (4);
	int i = 0;
	while (1)
	{
		new_socket = accept(socket_fd, (struct sockaddr *)&addr, (socklen_t *)&addr_len);
		if (new_socket == -1)
			return (5);
		read(new_socket, buffer, BUFF_SIZE);
		std::cout << buffer << std::endl;
		std::cout << "DONE " << i++ << std::endl;
		// std::cout << response << std::endl;
		write(new_socket, response, strlen(response));
		close(new_socket);
		close(new_socket);
	}
}