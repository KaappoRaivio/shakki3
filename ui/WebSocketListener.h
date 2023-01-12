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
#include "WebSocketSession.h"


namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class WebSocketListener : public std::enable_shared_from_this<WebSocketListener> {
    net::io_context &ioc_;
    tcp::acceptor acceptor_;

public:
    WebSocketListener(net::io_context &ioc, const tcp::endpoint& endpoint);
    void run();

private:
    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};
