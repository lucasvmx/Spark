#ifndef EXCEPTION_H
#define EXCEPTION_H

// Tamanho do buffer interno para essa classe
#define INTERNAL_BUFLEN 512

class Exception
{
public:
    Exception(const char *what);
    Exception();
    ~Exception();

    virtual void raise() const;
    virtual const char *what() const;

private:
    char exception_text[INTERNAL_BUFLEN];
};

#endif // EXCEPTION_H
