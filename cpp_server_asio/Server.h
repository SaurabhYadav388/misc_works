#pragma once
#include "Conn_handler.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace asio = boost::asio;
namespace ip = asio::ip;

class Server
{
private:
    ip::tcp::acceptor acceptor_;
    void start_accept();
public:
    //constructor for accepting connection from client
    Server(asio::io_context& io_context, int port) : acceptor_(io_context, ip::tcp::endpoint(ip::tcp::v4(), port))
    {
        start_accept();
    }
    void handle_accept(Conn_handler::pointer , const boost::system::error_code& );
};
