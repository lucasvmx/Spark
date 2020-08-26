#include "exception.h"
#include "hack.h"
#include <QString>

using namespace std;

Exception::Exception(const char *what)
{
    strncpy(exception_text, what, arraySize(exception_text));
}

Exception::Exception()
{
    memset(exception_text, 0, sizeof(exception_text));
}

Exception::~Exception()
{

}

void Exception::raise() const
{
    throw *this;
}

const char *Exception::what() const
{
    return QString(this->exception_text).toStdString().c_str();
}
