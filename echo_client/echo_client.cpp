#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

boost::asio::io_service service;
boost::asio::ip::tcp::resolver resolver(service);
boost::asio::ip::tcp::resolver::query query("localhost", "8090");
boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
boost::asio::ip::tcp::endpoint ep = *iter;
//boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 8090);

size_t handler_complete(char* buffer, const boost::system::error_code& err, size_t bytes) {
    bool found = std::find(buffer, buffer + bytes, '\n') < buffer + bytes;
    if (err || found) {
        return 0;
    }
    else
    {
        return 1;
    }
}

void send(std::string message) {

    message += "\n";
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(boost::asio::buffer(message));
    char buf[1024];
    //    int bytes = boost::asio::read(sock, boost::asio::buffer(buf), boost::bind(handler_complete, buf, _1, _2));
    int bytes = boost::asio::read(sock, boost::asio::buffer(buf), boost::bind(handler_complete, buf, _1, _2));
    std::string reply(buf, bytes - 1);
    std::cout << reply << std::endl;
    sock.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    try {
        send("Hello world!");
    }
    catch (boost::system::system_error& e) {
        std::cout << (e.what());
    }
}