#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>

// Tamanho do buffer interno para essa classe
#define INTERNAL_BUFLEN 512

class Exception : QException
{
public:
    Exception(const char *what);
    Exception();
    ~Exception();

    void raise() const override;
    const char *what();

private:
    QString exception_text;
};

#endif // EXCEPTION_H
