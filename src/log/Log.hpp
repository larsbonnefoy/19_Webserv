/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:08:39 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/21 16:41:26 by hdelmas          ###   ########.fr       */
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
void	logFile(T input)
{
	
	std::ofstream	filestream(LOGFILE, std::ios::app);
	if (!filestream.good())
		throw std::exception();
	timestamp(filestream);
 	filestream << input << std::endl;
	filestream.close();
}

template<typename T>
void	log(T input)
{
	timestamp(LOGSTREAM);
	logFile(input);
	LOGSTREAM << input << std::endl;

}


template<typename T>
void	logErr(T input)
{
	timestamp(LOGERRSTREAM);
	std::stringstream	ss;
	ss << "Error : " << input;
	logFile(ss.str());
	LOGERRSTREAM << ss.str() << std::endl;
}

#endif