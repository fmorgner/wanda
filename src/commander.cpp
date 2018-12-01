#include "commander.hpp"
#include "message.hpp"
#include "optional.hpp"

#include <spdlog/fmt/ostr.h>

namespace wanda
{

std::optional<message> commander::command::message() const
{
    using namespace std::string_literals;
    auto const command = [this] {
        switch (id)
        {
        case command_id::change:
            return "CHANGE"s;
        default:
            return ""s;
        }
    }();

    auto argument_string = std::string{};
    for(int index = 0ul; index < arguments.size(); ++index)
    {
        argument_string += (index) ? "," + arguments[index] : arguments[index]; 
    }

    if(command.empty())
    {
        return std::nullopt;
    }

    return wanda::message{"C", command, argument_string};
}

commander::commander(asio::io_service &service, std::filesystem::path socket, listener &listener, std::shared_ptr<spdlog::logger> logger)
    : m_service{service},
      m_endpoint{socket.string()},
      m_socket{service},
      m_listener{listener},
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

void commander::send(command command)
{
    using namespace wanda::std_ext;
    
    if (!m_connection || m_connection->current_state() != control_connection::state::established)
    {
        m_logger->error("tried to send command without an established connection");
        m_listener.on_error(*this, "tried to send command without an established connection");
        return;
    }

    with(command.message(), [&](auto const & message){
        m_connection->send(message);
    }) || [&]{ m_logger->error("unknown command"); };
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
        m_listener.on_connected(*this);
    }
    else
    {
        m_logger->error("unexpected message: '{}'", message);
        m_listener.on_error(*this, "unexpected message '" + static_cast<std::string>(message) + '\'');
    }
}

} // namespace wanda