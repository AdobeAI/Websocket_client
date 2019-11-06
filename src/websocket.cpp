#include "websocket.h"

void fail(boost::system::error_code ec,
          char const* what) {
    std::cerr << what << " :  " << ec.message() << "\n";
}

WebSocket::WebSocket(boost::asio::io_context& ioc)  // Resolver and socket require an io_context
    : resolver(ioc)
    , ws(ioc) {

}

void WebSocket::run(
        char const* _host,
        char const* _port,
        char const* _text) {
    // Save these for later
    host = _host;
    text = _text;

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
    if (ec)
        return fail(ec, "resolve");

    // Make the connection on the IP address we get from a lookup
    boost::asio::async_connect(ws.next_layer(),
                               results.begin(),
                               results.end(),
                               std::bind(&WebSocket::onConnect,
                                         shared_from_this(),
                                         std::placeholders::_1));
}

void WebSocket::onConnect(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "connect");

    // Perform the websocket handshake
    ws.async_handshake(host, "/",
                        std::bind(&WebSocket::onHandshake,
                                  shared_from_this(),
                                  std::placeholders::_1));
}

void WebSocket::onHandshake(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "handshake");

    // Send the message
    ws.async_write(boost::asio::buffer(text),
                    std::bind(&WebSocket::onWrite,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));
}

void WebSocket::onWrite(boost::system::error_code ec,
                       std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec)
        return fail(ec, "write");

    // Read a message into our buffer
    ws.async_read(buffer,
                   std::bind(&WebSocket::onRead,
                             shared_from_this(),
                             std::placeholders::_1,
                             std::placeholders::_2));
}

void WebSocket::onRead(boost::system::error_code ec,
                      std::size_t bytes_transferred) {

    boost::ignore_unused(bytes_transferred);
    if (ec)
        return fail(ec, "read");

    // Close the WebSocket connection
    ws.async_close(websocket::close_code::normal,
                    std::bind(&WebSocket::onClose,
                              shared_from_this(),
                              std::placeholders::_1));

}

void WebSocket::onClose(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "close");

    // If we get here then the connection is closed gracefully
    // The buffers() function helps print a ConstBufferSequence
    std::cout << boost::beast::buffers(buffer.data()) << std::endl;
}
