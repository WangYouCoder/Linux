
#pragma once
#define CONV(addr_ptr) ((struct sockaddr*)addr_ptr)
enum
{
    Usage_Err = 1,
    Socket_Err,
    Bind_Err,
    Listen_Err
};