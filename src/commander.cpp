#include "commander.hpp"
#include "message.hpp"

#include <iostream>

namespace wanda
{
commander::commander(boost::asio::io_service &service, std::filesystem::path socket)
    : m_service{service},
      m_endpoint{socket.string()},
      m_socket{service}
{
}

void commander::start()
{
    std::clog << "[commander::start] Starting commander on socket '" << m_endpoint.path() << "'\n";
    m_socket.async_connect(m_endpoint, [&](auto const &error) {
        if (error)
        {
            std::cerr << "[commander::start] error occured: " << error.message() << '\n';
        }
        else
        {
            m_connection = wanda::make_control_connection(std::move(m_socket));
            m_connection->add(this);
            m_connection->start();
            m_connection->send({message_source_controller, message_command_hello, message_argument_hello});
        }
    });
}

void commander::on_error(wanda::control_connection::pointer connection, boost::system::error_code error)
{
    std::cerr << "[commander::on_error] error occured: " << error.message() << '\n';
}

void commander::on_received(wanda::control_connection::pointer connection, message message)
{
    if (auto state = connection->current_state(); message.command == "HELLO" && state == control_connection::state::fresh)
    {
        connection->update(control_connection::state::established);
    }
    else
    {
        std::cerr << "[commander::on_receive] unexpected message: " << message << '\n';
    }
}

} // namespace wanda