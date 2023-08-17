#include "wanda/control/interface.hpp"

#include "wanda/proto/version.hpp"
#include "wanda/std_ext/optional.hpp"
#include "wanda/system/logging.hpp"

#include <spdlog/fmt/ostr.h>
#include <unistd.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <system_error>
#include <utility>

namespace wanda::control
{
  // 'socket_deleter' implementation

  socket_deleter::~socket_deleter()
  {
    if (std::filesystem::exists(path))
    {
      std::filesystem::remove(path);
    }
  }

  // 'interface' implementation

  interface::interface(interface::key key, asio::io_service & service, interface::protocol::endpoint endpoint, listener & listener)
      : keyed{key}
      , m_service{service}
      , m_endpoint{std::move(endpoint)}
      , m_socket{m_service}
      , m_acceptor{m_service}
      , m_listener{listener}
  {
  }

  std::error_code interface::start()
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

  std::error_code interface::shutdown()
  {
    for (auto & connection : m_connections)
    {
      connection->close();
    }

    auto error = std::error_code{};
    return m_acceptor.close(error);
  }

  void interface::perform_accept()
  {
    m_acceptor.async_accept(m_socket, [that = shared_from_this(), this](auto const & error) {
      if (error && error != asio::error::operation_aborted)
      {
        system::get_logger()->error("failed to accept connection because '{}'", error.message());
      }
      else
      {
        system::get_logger()->info("new incoming controller connection");
        auto [connection, inserted] = m_connections.insert(make_connection(std::move(m_socket)));
        if (inserted)
        {
          (*connection)->add(this);
          (*connection)->start();
        }
        perform_accept();
      }
    });
  }

  void interface::on_close(connection::pointer connection)
  {
    if (static_cast<char>(connection->current_state()) >= static_cast<char>(connection::state::established))
    {
      system::get_logger()->info("controller connection closed");
    }
    else
    {
      system::get_logger()->info("controller connection aborted before it could be established");
    }
    m_connections.erase(connection);
  }

  void interface::on_received(connection::pointer connection, proto::message message)
  {
    using namespace wanda::std_ext;

    if (m_connections.find(connection) == m_connections.cend())
    {
      system::get_logger()->error("received message from an unknown connection");
      return;
    }

    if (message.source != proto::message_source_controller)
    {
      system::get_logger()->error("received a deamon message");
      return;
    }

    if (auto state = connection->current_state(); message.command == proto::message_command_hello && state == connection::state::fresh)
    {
      system::get_logger()->info("controller connection established");
      if (message.argument.has_value())
      {
        system::get_logger()->info("remote controller version '{}'", *message.argument);
      }
      connection->send({proto::message_source_daemon, proto::message_command_hello, proto::version});
      connection->update(connection::state::established);
    }
    else
    {
      with(make_command(message), [&](auto const & command) {
        m_listener.on_received(*this, command);
      }) ||
          [&] { system::get_logger()->warn("ignoring unknown message '{}'", message); };
    }
  }

  interface::pointer make_interface(asio::io_service & service, std::filesystem::path socket, interface::listener & listener)
  {
    if (std::filesystem::exists(socket))
    {
      system::get_logger()->error("socket '{}' exists", socket.native());
      return {};
    }

    interface::protocol::endpoint endpoint
    {
      socket.string()
    };
    return std::make_shared<interface>(interface::key{}, service, std::move(endpoint), listener);
  }

}  // namespace wanda::control