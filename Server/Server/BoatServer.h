#pragma once

class BoatServer
{
public:
    BoatServer(boost::asio::io_context& InIoContext, int32 InPort);

private:
    void StartAccept();
    void StartRead(const std::shared_ptr<boost::asio::ip::tcp::socket>& InSocket);
    
    boost::asio::ip::tcp::acceptor Acceptor;
};
