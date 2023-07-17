#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iterator>
#include <string>
#include <vector>

#define NBR_METHODS 3
#define GET 0
#define POST 1
#define DELETE 2
#define CGI 3

#define FALSE 0
#define TRUE 1
#define UNDEFINED 2

class Location {
    private:
        std::string                         _path;
        std::string                         _root; 
        std::string                         _index; 
        size_t                              _autoindex;
        std::pair<size_t, std::string>      _redirect;
        bool                                _autorizedMethods[NBR_METHODS];
        std::string                         _cgiPath;
        std::string                         _uploadDir;

    public:
        Location(void);
        Location(const Location &other);
        ~Location(void);
        Location &operator=(const Location &other);

        std::string                     getPath(void) const;
        std::string                     getRoot(void) const;
        std::string                     getIndex(void) const;
        size_t                          getAutoIndex(void) const;
        std::pair<size_t, std::string>  getRedirect(void) const;
        bool                            getGetVal(void) const;
        bool                            getPostVal(void) const;
        bool                            getDelVal(void) const;
        std::string                     getCGIPath(void) const;
        std::string                     getUploadDir(void) const;

        void                            setPath(std::string root);
        void                            setRoot(std::string root);
        void                            setIndex(std::string index);
        void                            setAutoIndex(size_t val);
        void                            setRedirect(size_t val, std::string path);
        void                            setAutorizedMethods(bool get, bool post, bool del);
        void                            setGet(bool val);
        void                            setPost(bool val);
        void                            setDel(bool val);
        void                            setCGIPath(std::string &cgi);
        void                            setUploadDir(std::string &uploadDir);
};

std::ostream &operator<<(std::ostream &out, const Location &loc);
#endif
