#include "../Includes/Http.hpp"

// Constructors
Http::Http()
{
	std::cout << "\e[0;33mDefault Constructor called of Http\e[0m" << std::endl;
}

Http::Http(const Http &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of Http\e[0m" << std::endl;
}


// Destructor
Http::~Http()
{
	std::cout << "\e[0;31mDestructor called of Http\e[0m" << std::endl;
}


// Operators
Http & Http::operator=(const Http &assign)
{
	(void) assign;
	return *this;
}

