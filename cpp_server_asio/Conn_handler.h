#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <nlohmann/json.hpp>

namespace asio = boost::asio;
namespace ip = asio::ip;
using namespace nlohmann;

class Conn_handler : public boost::enable_shared_from_this<Conn_handler>
{
private:
    ip::tcp::socket socket;
    enum { max_length = 1024 };
    std::string returnData;
    char receivedData[max_length];

public:
    typedef boost::shared_ptr<Conn_handler> pointer;
    Conn_handler(boost::asio::io_context& io_context) : socket(io_context) {}

    // creating the pointer
    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new Conn_handler(io_context));
    }
    //socket creation
    ip::tcp::socket& getSocket();

    void start();
    void ProcessData(const boost::system::error_code& , size_t );

};

