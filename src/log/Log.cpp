/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:08:36 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/21 16:15:03 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

void	timestamp(std::ostream &stream)
{
		std::time_t now = time(0);
		tm	*ltm = localtime(&now);
		stream << "["<< 1900 + ltm->tm_year << "/" << 1 + ltm->tm_mon
					<< "/" << ltm->tm_mday << " ";
		if (ltm->tm_hour > 9)
			stream << ltm->tm_hour << ":";
		else
			stream << 0 << ltm->tm_hour << ":";
		if (ltm->tm_min > 9)
			stream << ltm->tm_min << ":";
		else
			stream << 0 << ltm->tm_min << ":";
		if (ltm->tm_sec > 9)
			stream << ltm->tm_sec << "] ";
		else
			stream << 0 << ltm->tm_sec << "] ";
		
}

