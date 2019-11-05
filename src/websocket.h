#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
    tcp::resolver resolver;
    websocket::stream<tcp::socket> ws;
    boost::beast::multi_buffer buffer;
    std::string host;
    std::string text;

public:
    explicit Session(boost::asio::io_context& ioc);

    void run(char const* host,
             char const* port,
             char const* text);

    void on_resolve(boost::system::error_code ec,
                    tcp::resolver::results_type results);

    void on_connect(boost::system::error_code ec);

    void on_handshake(boost::system::error_code ec);

    void on_write(boost::system::error_code ec,
                  std::size_t bytes_transferred);

    void on_read(boost::system::error_code ec,
                 std::size_t bytes_transferred);

    void on_close(boost::system::error_code ec);
};

