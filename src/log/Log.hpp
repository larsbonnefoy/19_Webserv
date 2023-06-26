/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:08:39 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/26 11:15:24 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include <iostream>
# include <string>
# include <ctime>
# include <fstream>
# include <sstream>
# include <exception>

# define LOGSTREAM 		std::cout
# define LOGERRSTREAM 	std::cerr
# define LOGFILE		"webserv.log"

void	timestamp(std::ostream &stream);

template<typename T>
void	ws_logFile(T input)
{
	
	std::ofstream	filestream(LOGFILE, std::ios::app);
	if (!filestream.good())
		throw std::exception();
	timestamp(filestream);
 	filestream << input << std::endl;
	filestream.close();
}

template<typename T>
void	ws_log(T input)
{
	timestamp(LOGSTREAM);
	ws_logFile(input);
	LOGSTREAM << input << std::endl;

}


template<typename T>
void	ws_logErr(T input)
{
	timestamp(LOGERRSTREAM);
	std::stringstream	ss;
	ss << "Error : " << input;
	ws_logFile(ss.str());
	LOGERRSTREAM << ss.str() << std::endl;
}

#endif