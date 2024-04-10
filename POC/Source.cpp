#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <nlohmann/json.hpp>

namespace asio = boost::asio;
namespace ip = asio::ip;
using namespace nlohmann;

class CONNECTION_HANDLER : public boost::enable_shared_from_this<CONNECTION_HANDLER>
{
private:
    ip::tcp::socket socket;
    enum { max_length = 1024 };
    std::string returnData;
    char receivedData[max_length];

public:
    typedef boost::shared_ptr<CONNECTION_HANDLER> pointer;
    CONNECTION_HANDLER(boost::asio::io_context& io_context) : socket(io_context) {}
    // creating the pointer
    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new CONNECTION_HANDLER(io_context));
    }
    //socket creation
    ip::tcp::socket& getSocket()
    {
        return socket;
    }

    void start()
    {
        /*socket.async_read_some(
            asio::buffer(receivedData, max_length),
            boost::bind(&CONNECTION_HANDLER::handle_read,
                shared_from_this(),
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));

        socket.async_write_some(
            asio::buffer(returnData, max_length),
            boost::bind(&CONNECTION_HANDLER::handle_write,
                shared_from_this(),
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));*/
        auto self(shared_from_this());

        socket.async_read_some(
            asio::buffer(receivedData, max_length),
            [this, self](const boost::system::error_code & error, size_t bytesRead) {
                if (!error) {

                    std::string request(receivedData, bytesRead);
                    if (request.find("/socket.io") != std::string::npos) {
 
                        //std::cout << "Ignoring Socket.IO request: "<< std::endl;
                        return;
                    }

                    // Handle read completion
                    handle_read(error, bytesRead);
                    // Start async write operation only after read operation is completed
                    std::cout << "\nReturn Data : "<< returnData << std::endl;
                    size_t bytes_written = socket.write_some(asio::buffer(returnData, max_length));
                    std::cout << "\nBytes Written : " << bytes_written << std::endl;
                }
                else {
                    // Handle read error
                }
            });
    }

    void handle_read(const boost::system::error_code& err, size_t bytes_transferred)
    {
        if (!err) {
            std::cout << "Received Data : " << receivedData;
            strcat_s(receivedData, sizeof receivedData, " | extended in the c++ server");
            returnData = receivedData;

        }
        else {
            std::cerr << "error: " << err.message() << std::endl;
            socket.close();
        }
    }
    /*void handle_write(const boost::system::error_code& err, size_t bytes_transferred)
    {
        if (!err) {
            
            std::cout << "Return Data : " << returnData;
        }
        else {
            std::cerr << "error: " << err.message() << std::endl;
            socket.close();
        }
    }*/
};

class Server
{
private:
    ip::tcp::acceptor acceptor_;
    void start_accept()
    {
        // socket
        CONNECTION_HANDLER::pointer connection = CONNECTION_HANDLER::create((asio::io_context&)acceptor_.get_executor().context());

        // asynchronous accept operation and wait for a new connection.
        acceptor_.async_accept(connection->getSocket(),
            boost::bind(&Server::handle_accept, this, connection,
                boost::asio::placeholders::error));
    }
public:
    //constructor for accepting connection from client
    Server(boost::asio::io_service& io_service) : acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), 3000))
    {
        start_accept();
    }
    void handle_accept(CONNECTION_HANDLER::pointer connection, const boost::system::error_code& err)
    {
        if (!err) {
            //std::cout << "connected" << std::endl;
            connection->start();
        }
        start_accept();
    }

};

int main(int argc, char* argv[])
{
    try
    {
        asio::io_context io_context;
        Server server(io_context);
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}


//#include<iostream>
//#include<nlohmann/json.hpp>
//
//using namespace nlohmann;
////example structure of data
//struct userMove
//{
//    int id;
//    std::string chessPiece;
//    std::pair<int, int> newPosition;
//};
//
//int main()
//{
//    // 1)converting our struct/class etc. to json
//    userMove move = { 0,"Queen",std::make_pair(4,6) };
//    json json_data = {
//        {"id",move.id},
//        {"chessPiece",move.chessPiece},
//        {"newPosition",move.newPosition}
//    };
//    std::cout << json_data.dump(1);
//    /*
//    above cout prints the following-->
//        {
//         "chessPiece": "Queen",
//         "id": 0,
//         "newPosition": [
//          4,
//          6
//         ]
//        }
//    */
//
//    std::cout << std::endl << std::endl;
//
//    // 2) reading json string back to struct.
//
//    std::string jsonString = json_data.dump(1);
//    std::cout << jsonString << std::endl;
//    json readJson = json::parse(jsonString);
//
//    userMove readMove;
//    readMove.id = readJson["id"];
//    readMove.chessPiece = readJson["chessPiece"];
//    readMove.newPosition = readJson["newPosition"];
//
//    std::cout << readMove.id << std::endl;
//    std::cout << readMove.chessPiece << std::endl;
//    std::cout << readMove.newPosition.first << std::endl;
//    std::cout << readMove.newPosition.second << std::endl;
//    return 0;
//}