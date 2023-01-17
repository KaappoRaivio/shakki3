//
// Created by kaappo on 1/12/23.
//

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "WebSocketSession.h"
#include "WebSocketListener.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;




int main() {
    auto const address = net::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(8081);

    net::io_context context{1};

    std::make_shared<WebSocketListener>(context, tcp::endpoint{address, port})->run();

    std::cout << "Running now!" << std::endl;

    std::thread v{[&context] {
        context.run();
    }};
    v.join();
    return 0;
}

