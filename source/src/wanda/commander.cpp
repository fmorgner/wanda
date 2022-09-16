#include <wanda/commander.hpp>
#include <wanda/logging.hpp>
#include <wanda/message.hpp>
#include <wanda/optional.hpp>

#include <spdlog/fmt/ostr.h>

namespace wanda
{
  commander::commander(asio::io_service & service, std::filesystem::path socket, listener & listener)
      : m_service{service}
      , m_endpoint{socket.string()}
      , m_socket{service}
      , m_listener{listener}
  {
  }

  void commander::start()
  {
    m_socket.async_connect(m_endpoint, [&](auto const & error) {
      if (error)
      {
        get_logger()->error("error while connecting to control interface: '{}'", error.message());
      }
      else
      {
        get_logger()->info("establishing connection to wanda deamon");
        m_connection = wanda::make_control_connection(std::move(m_socket));
        m_connection->add(this);
        m_connection->start();
        m_connection->send({message_source_controller, message_command_hello, message_argument_hello});
      }
    });
  }

  void commander::stop()
  {
    get_logger()->info("closing control connection");
    m_connection->close();
  }

  void commander::send(command command)
  {
    using namespace wanda::std_ext;

    if (!m_connection || m_connection->current_state() != control_connection::state::established)
    {
      get_logger()->error("tried to send command without an established connection");
      m_listener.on_error(*this, "tried to send command without an established connection");
      return;
    }

    with(command.message(), [&](auto const & message) { m_connection->send(message); }) ||
        [&] { get_logger()->error("unknown command"); };
  }

  void commander::on_error(control_connection::pointer connection, std::error_code error)
  {
    get_logger()->error("control interface communication error: '{}'", error.message());
  }

  void commander::on_received(wanda::control_connection::pointer connection, message message)
  {
    if (auto state = connection->current_state(); message.command == "HELLO" && state == control_connection::state::fresh)
    {
      get_logger()->info("connection to wanda deamon successfully established");
      connection->update(control_connection::state::established);
      m_listener.on_connected(*this);
    }
    else
    {
      get_logger()->error("unexpected message: '{}'", message);
      m_listener.on_error(*this, "unexpected message '" + static_cast<std::string>(message) + '\'');
    }
  }

}  // namespace wanda