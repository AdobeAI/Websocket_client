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

class WebSocket : public std::enable_shared_from_this<WebSocket> {
    tcp::resolver resolver;
    websocket::stream<tcp::socket> ws;
    boost::beast::multi_buffer buffer;
    std::string host;
    std::string text;

public:
    explicit WebSocket(boost::asio::io_context& ioc);

    void run(char const* host,
             char const* port,
             char const* text);

    void onResolve(boost::system::error_code ec,
                    tcp::resolver::results_type results);

    void onConnect(boost::system::error_code ec);

    void onHandshake(boost::system::error_code ec);

    void onWrite(boost::system::error_code ec,
                  std::size_t bytes_transferred);

    void onRead(boost::system::error_code ec,
                 std::size_t bytes_transferred);

    void onClose(boost::system::error_code ec);
};

