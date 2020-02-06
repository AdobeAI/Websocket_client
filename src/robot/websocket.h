#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <queue>

class WebSocket;

enum OperatorError {
    RESOLVE_ERROR,
    HANDSHARK_ERROR,
    CONNECT_ERROR,
    READ_ERROR,
    WRITE_ERROR,
    TIMEOUT_ERROR
};

const std::string OperatorErrorNames[] = {
    "RESOLVE_ERROR",
    "HANDSHARK_ERROR",
    "CONNECT_ERROR",
    "READ_ERROR",
    "WRITE_ERROR",
    "TIMEOUT_ERROR"
};

namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;

using ConnectedCallback = std::function<void(std::shared_ptr<WebSocket>)>;
using SendCallback = std::function<void(std::shared_ptr<WebSocket>)>;
using ReceiveCallback = std::function<void(std::shared_ptr<WebSocket>, const std::string&)>;
using DisconnectCallback = std::function<void(std::shared_ptr<WebSocket>)>;
using ErrorCallback = std::function<void(std::shared_ptr<WebSocket>, OperatorError, boost::system::error_code)>;
using TimeoutCallback = std::function<void(std::shared_ptr<WebSocket>)>;

class Robot;

class WebSocket : public std::enable_shared_from_this<WebSocket> {
    static const int MAX_SEND_QUEUE_SIZE;
    tcp::resolver resolver;
    websocket::stream<tcp::socket> ws;
    boost::beast::multi_buffer buffer;
    std::string host;
    //std::queue<std::string> send_queue;
    ConnectedCallback connected_callback;
    SendCallback send_callback;
    ReceiveCallback receive_callback;
    DisconnectCallback close_callback;
    ErrorCallback error_callback;
    TimeoutCallback timeout_callback;

    boost::circular_buffer<std::string> send_queue;
    boost::asio::io_context::strand strand;

    void onResolve(boost::system::error_code ec,
                   tcp::resolver::results_type results);

    void onConnect(boost::system::error_code ec);

    void onHandshake(boost::system::error_code ec);

    void onWrite(boost::system::error_code ec,
                 std::size_t bytes_transferred);

    void onRead(boost::system::error_code ec,
                std::size_t bytes_transferred);

    void onClosed(boost::system::error_code ec);

    //void write(const std::string& message);

    void doHandshake();

    void doConnect(tcp::resolver::results_type& results);

    void disconnect();

public:
    explicit WebSocket(boost::asio::io_context& ioc);

    void run(char const* host,
             char const* port);

    void receicve();

    std::string getErrorType(OperatorError oe);

    void send(const std::string& send_message);

    void doWrite(const std::string & send_message);

    void write();

    template <typename T>
    void onConnected(T &&callback) {
        connected_callback = std::forward<ConnectedCallback>(callback);
    }

    template <typename T>
    void onReceive(T &&callback) {
        receive_callback = std::forward<ReceiveCallback>(callback);
    }

    template <typename T>
    void onSend(T &&callback) {
        send_callback = std::forward<SendCallback>(callback);
    }

    template <typename T>
    void onClose(T &&callback) {
        close_callback = std::forward<DisconnectCallback>(callback);
    }

    template <typename T>
    void onError(T &&callback) {
        error_callback = std::forward<ErrorCallback>(callback);
    }

    template <typename T>
    void onTimeout(T &&callback) {
        timeout_callback = std::forward<TimeoutCallback>(callback);
    }
};

