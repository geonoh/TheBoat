#include "pch.h"

#include "BoatServer.h"
#include "../../Shared/Defines.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context IoContext;
    BoatServer Server(IoContext, BOAT_SERVER_PORT);

    std::thread ServerThread([&IoContext]() { IoContext.run(); });

    std::cout << "Server started on port : " << BOAT_SERVER_PORT << '\n';
    ServerThread.join();

    return 0;
}
