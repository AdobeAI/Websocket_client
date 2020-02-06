#include "websocket.h"


const int WebSocket::MAX_SEND_QUEUE_SIZE = 128;

void fail(boost::system::error_code ec,
          char const* what) {
    std::cerr << what << " :  " << ec.message() << "\n";
}

WebSocket::WebSocket(boost::asio::io_context& ioc)  // Resolver and socket require an io_context
    : resolver(ioc)
    , ws(ioc)
    , strand(ioc) {
    ws.binary(true);
}

void WebSocket::run(
        char const* _host,
        char const* _port) {
    // Save these for later
    host = _host;

    // Look up the domain name
    resolver.async_resolve(_host,
                           _port,
                           std::bind(&WebSocket::onResolve,
                                     shared_from_this(),
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

void WebSocket::onResolve(boost::system::error_code ec,
                          tcp::resolver::results_type results) {
    if (ec) {
        error_callback(shared_from_this(), OperatorError::RESOLVE_ERROR, ec);
        return;
    }
    doConnect(results);
}

void WebSocket::onConnect(boost::system::error_code ec) {
    if (ec) {
        error_callback(shared_from_this(), OperatorError::CONNECT_ERROR, ec);
        return;
    }
    doHandshake();
}

void WebSocket::onHandshake(boost::system::error_code ec) {
    if (ec) {
        error_callback(shared_from_this(), OperatorError::HANDSHARK_ERROR, ec);
        return;
    }

    connected_callback(shared_from_this());
}

void WebSocket::doHandshake() {
    // Perform the websocket handshake
    ws.async_handshake(host, "/",
                       std::bind(&WebSocket::onHandshake,
                                 shared_from_this(),
                                 std::placeholders::_1));
}

void WebSocket::doConnect(tcp::resolver::results_type& results) {
    // Make the connection on the IP address we get from a lookup
    boost::asio::async_connect(ws.next_layer(),
                               results.begin(),
                               results.end(),
                               std::bind(&WebSocket::onConnect,
                                         shared_from_this(),
                                         std::placeholders::_1));
}

void WebSocket::doWrite(const std::string & send_message) {
    //    auto size = send_queue.size();
    //    auto write_in_progress = !send_queue.empty();
    //    send_queue.push_back(send_message);

    //    if (!write_in_progress) {
    //        write();
    //    }

    ws.async_write(
                boost::asio::buffer(send_message),
                strand.wrap(std::bind(
                                &WebSocket::onWrite,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2)));
}

void WebSocket::write() {
    ws.async_write(
                boost::asio::buffer(&send_queue.front()[0], send_queue.front().size()),
            strand.wrap(std::bind(
                            &WebSocket::onWrite,
                            shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2)));
}

void WebSocket::onWrite(boost::system::error_code ec,
                        std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) {
        error_callback(shared_from_this(), OperatorError::WRITE_ERROR, ec);
        return;
    }

    send_callback(shared_from_this());
}

void WebSocket::send(const std::string& send_message) {
    //    if (send_queue.size() < MAX_SEND_QUEUE_SIZE) {
    //        send_queue.push(send_message);
    //    }
    //    write(send_message);

    auto buffer(send_message);
    strand.post(std::bind(&WebSocket::doWrite, this, buffer));
}

void WebSocket::onRead(boost::system::error_code ec,
                       std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) {
        error_callback(shared_from_this(), OperatorError::READ_ERROR, ec);
        return;
    }

    std::string result = boost::beast::buffers_to_string(buffer.data());
    buffer.consume(buffer.size());

    receive_callback(shared_from_this(), result);
}

void WebSocket::onClosed(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "close");

    close_callback(shared_from_this());
}

//void WebSocket::write(const std::string& message) {
//    ws.async_write(boost::asio::buffer(message),
//                   std::bind(&WebSocket::onWrite,
//                             shared_from_this(),
//                             std::placeholders::_1,
//                             std::placeholders::_2));
//}

void WebSocket::disconnect() {
    // Close the WebSocket connection
    ws.async_close(websocket::close_code::normal,
                   std::bind(&WebSocket::onClosed,
                             shared_from_this(),
                             std::placeholders::_1));
}

void WebSocket::receicve() {
    // Read a message into our buffer
    ws.async_read(buffer,
                  std::bind(&WebSocket::onRead,
                            shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2));
}

std::string WebSocket::getErrorType(OperatorError oe) {
    return OperatorErrorNames[oe];
}
