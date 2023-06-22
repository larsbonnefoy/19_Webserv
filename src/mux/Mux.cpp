#include "Mux.hpp"

// Constructors
Mux::Mux()
{
	std::cout << "\e[0;33mDefault Constructor called of Mux\e[0m" << std::endl;
}

Mux::Mux(const Mux &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of Mux\e[0m" << std::endl;
}


// Destructor
Mux::~Mux()
{
	std::cout << "\e[0;31mDestructor called of Mux\e[0m" << std::endl;
}


// Operators
Mux & Mux::operator=(const Mux &assign)
{
	(void) assign;
	return *this;
}

