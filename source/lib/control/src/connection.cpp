#include "wanda/control/connection.hpp"

#include "wanda/proto/message.hpp"

#include <boost/asio/completion_condition.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/system/error_code.hpp>

#include <limits>

namespace wanda::control
{
  connection::pointer make_connection(connection::protocol::socket && socket)
  {
    return std::make_shared<connection>(connection::key{}, std::move(socket));
  }

  connection::connection(connection::key key, connection::protocol::socket socket)
      : keyed{key}
      , m_socket{std::move(socket)}
  {
  }

  bool connection::add(listener * listener)
  {
    auto [_, inserted] = m_listeners.insert(listener);
    return inserted;
  }

  bool connection::remove(listener * listener)
  {
    return m_listeners.erase(listener);
  }

  void connection::start()
  {
    if (m_state == state::unknown)
    {
      m_state = state::fresh;
      perform_read();
    }
  }

  void connection::send(proto::message message)
  {
    m_output << message << '\n';
    boost::asio::async_write(m_socket, m_out, boost::asio::transfer_exactly(message.size() + 1), [that = shared_from_this(), this](auto const & error, auto const length) {
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

  void connection::close()
  {
    auto error = boost::system::error_code{};

    if (m_socket.cancel(error), error)
    {
      for (auto & listener : m_listeners)
      {
        listener->on_error(shared_from_this(), error);
      }
    }

    if (m_socket.close(error), error)
    {
      for (auto & listener : m_listeners)
      {
        listener->on_error(shared_from_this(), error);
      }
    }

    for (auto & listener : m_listeners)
    {
      listener->on_close(shared_from_this());
    }
    m_listeners.clear();
  }

  void connection::update(state state)
  {
    m_state = state;
  }

  connection::state connection::current_state() const
  {
    return m_state;
  }

  void connection::perform_read()
  {
    boost::asio::async_read_until(m_socket, m_in, '\n', [that = shared_from_this(), this](auto const & error, auto const length) {
      if (error)
      {
        for (auto & listener : m_listeners)
        {
          listener->on_error(shared_from_this(), error);
        }
        close();
      }
      else
      {
        auto msg = proto::message{};
        m_input >> msg;
        if (!m_input)
        {
          m_input.ignore(std::numeric_limits<std::streamsize>::max());
          m_input.clear();
        }
        else
        {
          for (auto & listener : m_listeners)
          {
            listener->on_received(shared_from_this(), msg);
          }
        }
        perform_read();
      }
    });
  }

}  // namespace wanda::control