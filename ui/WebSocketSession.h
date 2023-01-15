//
// Created by kaappo on 1/12/23.
//

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <memory>



namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

void fail(beast::error_code ec, char const *what);

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;

public:
    explicit WebSocketSession(tcp::socket &&socket) : ws_(std::move(socket)) {}


    void run() {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this WebSocketSession. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(ws_.get_executor(), beast::bind_front_handler(&WebSocketSession::on_run, shared_from_this()));
    }

    void on_run();
    void on_accept(beast::error_code ec);
    void do_read();
    void on_read(
            beast::error_code ec,
            std::size_t bytes_transferred);

    void on_write(
            beast::error_code ec,
            std::size_t bytes_transferred);
};