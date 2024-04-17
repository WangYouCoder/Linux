#include "TcpServer.hpp"
using namespace std;
#include <memory>
#include "Comm.hpp"

void Usage(string proc)
{
    cout << "Usage : \n\t" << proc << " local_ port\n" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return Usage_Err;
    }
    uint16_t port = stoi(argv[1]);

    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port);
    tsvr->Init();
    tsvr->Start();

    return 0;
}