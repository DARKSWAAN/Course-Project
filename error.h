#ifndef ERROR_H
#define ERROR_H


class Error
{
public:
    Error();
    virtual bool open(const char *filename) = 0;
       virtual bool close() = 0;

       virtual bool writeError(const char *errorMessage) = 0;

       virtual ~Error() {};
};

#endif // ERROR_H
