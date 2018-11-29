#include "control_connection.hpp"

namespace wanda
{

control_connection::pointer make_control_connection(control_connection::protocol::socket &&socket)
{
    return std::make_shared<control_connection>(control_connection::key{}, std::move(socket));
}

control_connection::control_connection(control_connection::key key, control_connection::protocol::socket socket)
    : keyed{key},
      m_socket{std::move(socket)}
{
}

bool control_connection::add(std::shared_ptr<control_connection::listener> listener)
{
    auto [_, inserted] = m_listeners.insert(listener);
    return inserted;
}

bool control_connection::remove(std::shared_ptr<control_connection::listener> listener)
{
    return m_listeners.erase(listener);
}

void control_connection::start()
{
    if (!m_running)
    {
        m_running = true;
        perform_read();
    }
}

void control_connection::send(std::string message)
{
    m_output << message;
    boost::asio::async_write(m_socket, m_out, boost::asio::transfer_exactly(message.size()), [that = shared_from_this(), this](auto const &error, auto const length) {
        if (error)
        {
            // TODO: Handle error
        }
        else
        {
            m_out.consume(length);
        }
    });
}

void control_connection::close()
{
    if (auto error = boost::system::error_code{}; m_socket.cancel(error))
    {
        for (auto &listener : m_listeners)
        {
            listener->on_error(shared_from_this(), error);
        }
    }

    if (auto error = boost::system::error_code{}; m_socket.close(error))
    {
        for (auto &listener : m_listeners)
        {
            listener->on_error(shared_from_this(), error);
        }
    }

    for (auto &listener : m_listeners)
    {
        listener->on_close(shared_from_this());
    }
    m_listeners.clear();
}

void control_connection::perform_read()
{
    boost::asio::async_read_until(m_socket, m_in, '\n', [that = shared_from_this(), this](auto const &error, auto const length) {
        if (error)
        {
            for (auto &listener : m_listeners)
            {
                listener->on_error(shared_from_this(), error);
            }
            close();
        }
        else
        {
            std::string message{};
            std::getline(m_input, message);
            for (auto &listener : m_listeners)
            {
                listener->on_received(shared_from_this(), message);
            }
            perform_read();
        }
    });
}

} // namespace wanda