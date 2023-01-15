//
// Created by kaappo on 1/12/23.
//

#include "WebSocketSession.h"

void WebSocketSession::on_run() {
    ws_.set_option(
            websocket::stream_base::timeout::suggested(
                    beast::role_type::server));

    ws_.set_option(websocket::stream_base::decorator(
            [](websocket::response_type &res) {
                res.set(http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-server-async");
            }));

    ws_.async_accept(
            beast::bind_front_handler(
                    &WebSocketSession::on_accept,
                    shared_from_this()));
}

void WebSocketSession::on_accept(beast::error_code ec) {
    if (ec)
        return fail(ec, "accept");

    do_read();
}

void WebSocketSession::do_read() {
    ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                    &WebSocketSession::on_read,
                    shared_from_this()));
}

void WebSocketSession::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This indicates that the WebSocketSession was closed
    if (ec == websocket::error::closed)
        return;

    if (ec)
        return fail(ec, "read");

    // Echo the message
    ws_.text(ws_.got_text());
//        ws_.text(ws_.got_text());
    const auto &data = buffer_.data();
    std::cout << "Incoming message: " << beast::buffers_to_string(data) << std::endl;

    ws_.async_write(
            data,
            beast::bind_front_handler(
                    &WebSocketSession::on_write,
                    shared_from_this()));
}

void WebSocketSession::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write");

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    do_read();
}

void fail(beast::error_code ec, const char *what) {
    std::cerr << what << ": " << ec.message() << "\n";
}
