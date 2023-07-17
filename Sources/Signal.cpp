/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 14:06:24 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/17 15:16:28 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "../Includes/Signal.hpp"

static void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{	
		extern bool g_isGood;
		g_isGood = false;
		std::cout << std::endl;
		ws_log("Quitting...");
	}
}

int	ws_signal(void)
{
	struct sigaction	sc;

	sc.sa_handler = &handle_signal;
	sigaction(SIGINT, &sc, NULL);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}