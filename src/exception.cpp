#include "exception.h"
#include "init.h"
#include <QString>

using namespace std;

char exceptionText[BUFLEN];

Exception::Exception(const char *what)
{
    exception_text = QString(what);
}

Exception::Exception()
{

}

Exception::~Exception()
{

}

void Exception::raise() const
{
    throw *this;
}

const char *Exception::what()
{
    strncpy(exceptionText, exception_text.toStdString().c_str(), sizeof(exceptionText));
    return const_cast<const char*>(&exceptionText[0]);
}
