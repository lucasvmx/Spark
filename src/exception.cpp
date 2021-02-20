#include "exception.h"
#include "init.h"
#include <QString>

using namespace std;

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
    return this->exception_text.toStdString().c_str();
}
