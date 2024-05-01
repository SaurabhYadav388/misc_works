#include "Conn_handler.h"

ip::tcp::socket& Conn_handler::getSocket()
{
    return this->socket;
}

void Conn_handler::start()
{
    auto self(shared_from_this());
    socket.async_read_some(
        asio::buffer(receivedData, max_length),
        [this, self](const boost::system::error_code& error, size_t bytesRead) {
            if (!error) {

                /*std::string request(receivedData, bytesRead);
                if (request.find("/socket.io") != std::string::npos) {
                    return;
                }*/

                ProcessData(error, bytesRead);
                size_t bytes_written = socket.write_some(asio::buffer(returnData, max_length));
                std::cout << "\nBytes Written : " << bytes_written << std::endl;
            }
            else {
                // Handle read error
            }
        });
}

void Conn_handler::ProcessData(const boost::system::error_code& err, size_t bytes_transferred)
{
    if (!err) {
        std::cout << "Received Data : " << receivedData << " " << bytes_transferred;
        strcat_s(receivedData, sizeof receivedData, " | extended in the c++ server");
        returnData = receivedData;

    }
    else {
        std::cerr << "error: " << err.message() << std::endl;
        socket.close();
    }
}