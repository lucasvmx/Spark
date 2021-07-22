/**
 * @file platform.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "qglobal.h"
#include "platform.h"

bool Is64BitVersion()
{
#if defined(__x86_64__) || !defined(Q_OS_WIN64)
    return true;
#else
    return false;
#endif
}
