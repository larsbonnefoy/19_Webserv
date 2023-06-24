#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>

#define NBR_METHODS 3
#define GET 0
#define POST 1
#define DELETE 2

class Location {
    private:
        std::string         _Path; 
        bool                _autoindex;
        bool                _autorizedMethods[NBR_METHODS];

    public:
        Location(void);
        Location(const Location &other);
        ~Location(void);
        Location &operator=(const Location &other);

        std::string getPath(void) const;
        bool        getAutoIndex(void) const;
        bool        getGetVal(void) const;
        bool        getPostVal(void) const;
        bool        getDelVal(void) const;

        void        setPath(std::string path);
        void        setAutoIndex(bool val);
        void        setAutorizedMethods(bool get, bool post, bool del);
};

std::ostream &operator<<(std::ostream &out, const Location &loc);
#endif
