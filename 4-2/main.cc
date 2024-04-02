#include <iostream>
#include "Log.hpp"

int main()
{
    Log log;
    log.Enable(Classfile); 
    log.LogMessage(Debug, "this is a log message: %d, %lf\n", 123, 1.11);
    log.LogMessage(Info, "this is a log message: %d, %lf\n", 123, 1.11);
    return 0;
}