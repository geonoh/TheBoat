#include "pch.h"
#include "BoatServer.h"

BoatServer::BoatServer(boost::asio::io_context& InIoContext, const int32 InPort)
: Acceptor(InIoContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), InPort))
{
    StartAccept();
}

void BoatServer::StartAccept()
{
    auto Socket =
            std::make_shared<boost::asio::ip::tcp::socket>(Acceptor.get_executor());

    Acceptor.async_accept(*Socket, [this, Socket](const boost::system::error_code& error)
    {
        if (!error)
        {
            std::cout << "Client connected!" << '\n';
            StartRead(Socket);
        }
        
        StartAccept();
    });
}

void BoatServer::StartRead(const std::shared_ptr<boost::asio::ip::tcp::socket>& InSocket)
{
    auto Buffer = std::make_shared<std::vector<char>>(1024);
    InSocket->async_read_some(boost::asio::buffer(*Buffer), [this, InSocket, Buffer](const boost::system::error_code& ErrorCode, const std::size_t Length)
    {
        if (!ErrorCode)
        {
            std::cout << "Received: " << std::string(Buffer->data(), Length) << '\n';
            StartRead(InSocket);  // 계속해서 데이터를 받기 위해 재귀 호출
        }
        else
        {
            std::cout << "Error on receive: " << ErrorCode.message() << '\n';
        }
    });
}
