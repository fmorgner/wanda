#include "control_interface.hpp"

#include <boost/system/error_code.hpp>

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>

#include <iostream>

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

    if (auto error = boost::system::error_code{}; m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), error))
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

void control_interface::perform_accept()
{
    m_acceptor.async_accept(m_socket, [that = shared_from_this(), this](auto const &error) {
        if (error)
        {
            // TODO: Handle error
        }
        else
        {
            auto [connection, inserted] = m_connections.insert(make_control_connection(std::move(m_socket)));
            if(inserted)
            {
                std::cout << "Accepted a new connection\n";
                (*connection)->add(shared_from_this());
                (*connection)->start();
            }
            perform_accept();
        }
    });
}

void control_interface::on_received(control_connection::pointer, std::string message)
{
    std::cout << "Received '" << message << "'\n";
}

void control_interface::on_close(control_connection::pointer connection)
{
    std::cout << "Connection closed\n";
    m_connections.erase(connection);
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