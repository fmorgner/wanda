#include "control_interface.hpp"
#include "logging.hpp"
#include "optional.hpp"

#include <spdlog/fmt/ostr.h>

#include <unistd.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <system_error>
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

  control_interface::control_interface(control_interface::key key, asio::io_service & service, control_interface::protocol::endpoint endpoint, listener & listener)
      : keyed{key}
      , m_service{service}
      , m_endpoint{std::move(endpoint)}
      , m_socket{m_service}
      , m_acceptor{m_service}
      , m_listener{listener}
  {
  }

  std::error_code control_interface::start()
  {
    if (auto error = std::error_code{}; m_acceptor.open(m_endpoint.protocol(), error))
    {
      return error;
    }

    if (auto error = std::error_code{}; m_acceptor.bind(m_endpoint, error))
    {
      return error;
    }

    if (auto error = std::error_code{}; m_acceptor.listen(128, error))
    {
      return error;
    }
    else
    {
      perform_accept();
      return error;
    }
  }

  std::error_code control_interface::shutdown()
  {
    for (auto & connection : m_connections)
    {
      connection->close();
    }

    auto error = std::error_code{};
    return m_acceptor.close(error);
  }

  void control_interface::perform_accept()
  {
    m_acceptor.async_accept(m_socket, [that = shared_from_this(), this](auto const & error) {
      if (error && error != asio::error::operation_aborted)
      {
        get_logger()->error("failed to accept connection because '{}'", error);
      }
      else
      {
        get_logger()->info("new incoming controller connection");
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
    if (static_cast<char>(connection->current_state()) >= static_cast<char>(control_connection::state::established))
    {
      get_logger()->info("controller connection closed");
    }
    else
    {
      get_logger()->info("controller connection aborted before it could be established");
    }
    m_connections.erase(connection);
  }

  void control_interface::on_received(control_connection::pointer connection, message message)
  {
    using namespace wanda::std_ext;

    if (m_connections.find(connection) == m_connections.cend())
    {
      get_logger()->error("received message from an unknown connection");
      return;
    }

    if (message.source != message_source_controller)
    {
      get_logger()->error("received a deamon message");
      return;
    }

    if (auto state = connection->current_state(); message.command == message_command_hello && state == control_connection::state::fresh)
    {
      get_logger()->info("controller connection established");
      if (message.argument.has_value())
      {
        get_logger()->info("remote controller version '{}'", *message.argument);
      }
      connection->send({message_source_daemon, message_command_hello, message_argument_hello});
      connection->update(control_connection::state::established);
    }
    else
    {
      with(make_command(message), [&](auto const & command) {
        m_listener.on_received(*this, command);
      }) ||
          [&] { get_logger()->warn("ignoring unknown message '{}'", message); };
    }
  }

  control_interface::pointer make_interface(asio::io_service & service, std::filesystem::path socket, control_interface::listener & listener)
  {
    if (std::filesystem::exists(socket))
    {
      get_logger()->error("socket '{}' exists", socket.native());
      return {};
    }

    control_interface::protocol::endpoint endpoint{socket.string()};
    return std::make_shared<control_interface>(control_interface::key{}, service, std::move(endpoint), listener);
  }

}  // namespace wanda