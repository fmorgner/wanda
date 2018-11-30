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
            std::clog << "[commander::start] Control connection established\n";
            m_connection = wanda::make_control_connection(std::move(m_socket));
            m_connection->add(this);
            m_connection->start();
            send({"C", "HELLO", "1.0.0"});
        }
    });
}

void commander::send(message message)
{
    if (m_connection)
    {
        std::clog << "[commander::send] sending message: " << message << '\n';
        m_connection->send(std::move(message));
    }
}

void commander::on_error(wanda::control_connection::pointer connection, boost::system::error_code error)
{
    std::cerr << "[commander::on_error] error occured: " << error.message() << '\n';
}

void commander::on_received(wanda::control_connection::pointer connection, message message)
{
    std::clog << "[commander::on_receive] Received message: " << message << '\n';
}

} // namespace wanda