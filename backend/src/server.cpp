#include "server.h"

#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "engine.h"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

int difficulty;
// keeps track of the depth of the eval function

std::string get_fen(std::string input) {
    // splits the message to the server into difficulty and FEN of the position
    difficulty = std::stoi(input.substr(0, 1));
    return input.substr(2, -1);
}

void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg) {
    // function that handles incoming messages
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    try {
        // tries to send back the best move using the calculate_move function
        s->send(hdl, calculate_move(get_fen(msg->get_payload()), difficulty), msg->get_opcode());
    } catch (websocketpp::exception const &e) {
        // in case the sending process fails
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}

void serve() {
    // Create a server endpoint
    server echo_server;

    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(
            websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(
            bind(&on_message, &echo_server, ::_1, ::_2));

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        std::cout << "server up" << std::endl;

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
