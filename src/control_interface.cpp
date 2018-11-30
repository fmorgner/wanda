#include "control_interface.hpp"

#include <boost/system/error_code.hpp>

#include <unistd.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>

namespace wanda
{

// 'socket_deleter' implementation

socket_deleter::~socket_deleter()
{
    if (std::filesystem::exists(path))
    {
        std::filesystem::remove(path);
    }
}

// 'control_interface' implementation

control_interface::control_interface(control_interface::key key, boost::asio::io_service &service, control_interface::protocol::endpoint endpoint)
    : keyed{key},
      m_service{service},
      m_endpoint{std::move(endpoint)},
      m_socket{m_service},
      m_acceptor{m_service}
{
}

boost::system::error_code control_interface::start()
{
    if (auto error = boost::system::error_code{}; m_acceptor.open(m_endpoint.protocol(), error))
    {
        return error;
    }

    if (auto error = boost::system::error_code{}; m_acceptor.bind(m_endpoint, error))
    {
        return error;
    }

    if (auto error = boost::system::error_code{}; m_acceptor.listen(128, error))
    {
        return error;
    }
    else
    {
        perform_accept();
        return error;
    }
}

boost::system::error_code control_interface::shutdown()
{
    for (auto &connection : m_connections)
    {
        connection->close();
    }

    auto error = boost::system::error_code{};
    return m_acceptor.close(error);
}

void control_interface::perform_accept()
{
    m_acceptor.async_accept(m_socket, [that = shared_from_this(), this](auto const &error) {
        if (error && error != boost::asio::error::operation_aborted)
        {
            // TODO: Handle error
        }
        else
        {
            auto [connection, inserted] = m_connections.insert(make_control_connection(std::move(m_socket)));
            if (inserted)
            {
                (*connection)->add(this);
                (*connection)->start();
            }
            perform_accept();
        }
    });
}

void control_interface::on_close(control_connection::pointer connection)
{
    m_connections.erase(connection);
}

void control_interface::on_received(control_connection::pointer connection, message message)
{
    if (m_connections.find(connection) == m_connections.cend())
    {
        // TODO: Handle unknown connection
        return;
    }

    switch (connection->current_state())
    {
    case control_connection::state::fresh:
        if (message.command == "HELLO")
        {
            connection->send({"D", "HELLO", "1.0.0"});
            connection->update(control_connection::state::established);
        }
    }
}

control_interface::pointer make_interface(boost::asio::io_service &service, std::filesystem::path file)
{
    if (std::filesystem::exists(file))
    {
        std::filesystem::remove(file);
    }
    control_interface::protocol::endpoint endpoint{file.string()};
    return std::make_shared<control_interface>(control_interface::key{}, service, std::move(endpoint));
}

} // namespace wanda