#include "control_connection.hpp"

#include <boost/asio.hpp>

#include <filesystem>
#include <iostream>
#include <memory>

namespace
{
struct commander : wanda::control_connection::listener, std::enable_shared_from_this<commander>
{
    commander(boost::asio::io_service &service, std::filesystem::path socket)
        : m_endpoint{socket.string()},
          m_socket{service}
    {
    }

    void start()
    {
        m_socket.async_connect(m_endpoint, [&](auto const &error) {
            if (error)
            {
                std::cerr << "[commander::start] error occured: " << error.message() << '\n';
            }
            else
            {
                m_connection = wanda::make_control_connection(std::move(m_socket));
                m_connection->start();
                send("C:HELLO:1.0.0\n");
                m_connection->close();
            }
        });
    }

    void send(std::string message)
    {
        if (m_connection)
        {
            std::clog << "[commander::send] sending message: " << message;
            m_connection->send(std::move(message));
        }
    }

    void on_error(wanda::control_connection::pointer connection, boost::system::error_code error) override
    {
        std::cerr << "[commander::on_error] error occured: " << error.message() << '\n';
    }

  private:
    wanda::control_connection::protocol::endpoint m_endpoint;
    wanda::control_connection::protocol::socket m_socket;
    wanda::control_connection::pointer m_connection;
};
} // namespace

int main()
{
    auto service = boost::asio::io_service{};
    auto command_processor = commander{service, std::filesystem::path{".wanda_interface"}};

    command_processor.start();

    service.run();
}