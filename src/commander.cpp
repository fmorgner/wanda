#include "commander.hpp"
#include "message.hpp"

#include <spdlog/fmt/ostr.h>

namespace wanda
{
commander::commander(asio::io_service &service, std::filesystem::path socket, std::shared_ptr<spdlog::logger> logger)
    : m_service{service},
      m_endpoint{socket.string()},
      m_socket{service},
      m_logger{logger}
{
}

void commander::start()
{
    m_socket.async_connect(m_endpoint, [&](auto const &error) {
        if (error)
        {
            m_logger->error("error while connecting to control interface: '{}'", error.message());
        }
        else
        {
            m_logger->info("establishing connection to wanda deamon");
            m_connection = wanda::make_control_connection(std::move(m_socket));
            m_connection->add(this);
            m_connection->start();
            m_connection->send({message_source_controller, message_command_hello, message_argument_hello});
        }
    });
}

void commander::on_error(control_connection::pointer connection, std::error_code error)
{
    m_logger->error("control interface communication error: '{}'", error.message());
}

void commander::on_received(wanda::control_connection::pointer connection, message message)
{
    if (auto state = connection->current_state(); message.command == "HELLO" && state == control_connection::state::fresh)
    {
        m_logger->info("connection to wanda deamon successfully established");
        connection->update(control_connection::state::established);
    }
    else
    {
        m_logger->error("unexpected message: '{}'", message);
    }
}

} // namespace wanda