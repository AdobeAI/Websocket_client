#include "websocket.h"

void fail(boost::system::error_code ec,
          char const* what) {
    std::cerr << what << " :  " << ec.message() << "\n";
}

Session::Session(boost::asio::io_context& ioc)  // Resolver and socket require an io_context
    : resolver(ioc)
    , ws(ioc) {

}

void Session::run(
        char const* _host,
        char const* _port,
        char const* _text) {
    // Save these for later
    host = _host;
    text = _text;

    // Look up the domain name
    resolver.async_resolve(_host,
                           _port,
                           std::bind(&Session::on_resolve,
                                     shared_from_this(),
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

void Session::on_resolve(boost::system::error_code ec,
                         tcp::resolver::results_type results) {
    if (ec)
        return fail(ec, "resolve");

    std::cout << "resolve success!" << std::endl;

    // Make the connection on the IP address we get from a lookup
    boost::asio::async_connect(ws.next_layer(),
                               results.begin(),
                               results.end(),
                               std::bind(&Session::on_connect,
                                         shared_from_this(),
                                         std::placeholders::_1));
}

void Session::on_connect(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "connect");

    std::cout << "connect success!" << std::endl;

    // Perform the websocket handshake
    ws.async_handshake(host, "/",
                        std::bind(&Session::on_handshake,
                                  shared_from_this(),
                                  std::placeholders::_1));
}

void Session::on_handshake(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "handshake");

    std::cout << "handshake success!" << std::endl;

    // Send the message
    ws.async_write(boost::asio::buffer(text),
                    std::bind(&Session::on_write,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));
}

void Session::on_write(boost::system::error_code ec,
                       std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec)
        return fail(ec, "write");

    std::cout << "write success!" << std::endl;

    // Read a message into our buffer
    ws.async_read(buffer,
                   std::bind(&Session::on_read,
                             shared_from_this(),
                             std::placeholders::_1,
                             std::placeholders::_2));
}

void Session::on_read(boost::system::error_code ec,
                      std::size_t bytes_transferred) {

    boost::ignore_unused(bytes_transferred);
    if (ec)
        return fail(ec, "read");

    std::cout << "read success!" << std::endl;

    // Close the WebSocket connection
    ws.async_close(websocket::close_code::normal,
                    std::bind(&Session::on_close,
                              shared_from_this(),
                              std::placeholders::_1));

}

void Session::on_close(boost::system::error_code ec) {
    if (ec)
        return fail(ec, "close");

    std::cout << "close success!" << std::endl;

    // If we get here then the connection is closed gracefully
    // The buffers() function helps print a ConstBufferSequence
    std::cout << boost::beast::buffers(buffer.data()) << std::endl;
}
