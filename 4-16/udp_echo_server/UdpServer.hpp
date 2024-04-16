#pragma once

#include <iostream>
#include "nocopy.hpp"
#include "Log.hpp" 
#include <string>
#include <sys/types.h>
//#include <sys/scoket.h>

static const std::string defaultip = "0.0.0.0";
static const uint16_t defaultport = 8888;

class UdpServer : public nocopy
{
public:
    UdpServer(const std::string& ip = defaultip, uint16_t port = defaultport)
    :_ip(ip), _port(port)
    {}
    void Init()
    {}
    void Start()
    {}
    ~UdpServer()
    {}
private:
    std::string _ip;
    uint16_t _port;
};