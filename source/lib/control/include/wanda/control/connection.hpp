#ifndef WANDA_CONTROL_CONNECTION_HPP
#define WANDA_CONTROL_CONNECTION_HPP

#include "wanda/meta/keyed.hpp"
#include "wanda/proto/message.hpp"

#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/streambuf.hpp>

#include <istream>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <system_error>

namespace wanda::control
{
  /**
   * @brief A connection to a remote control endpoint
   */
  struct connection : meta::keyed<connection>, std::enable_shared_from_this<connection>
  {
    using protocol = boost::asio::local::stream_protocol;
    using pointer = std::shared_ptr<connection>;

    /**
     * @brief The interface to be implemented by the control interface listener
     */
    struct listener
    {
      virtual void on_close(pointer connection) {}
      virtual void on_received(pointer connection, proto::message message) {}
      virtual void on_error(pointer connection, std::error_code) {}
    };

    /**
     * @brief A enum to describe different connection states
     */
    enum struct state : std::underlying_type_t<std::byte>
    {
      unknown,  //< Connection is in an unknown state
      fresh,  //< Connection is freshly created but not established
      established,  //< Connection has been established
    };

    /**
     * @internal
     * @brief Construct a new control connection object
     *
     * @note This constructor is keyed on a private key type so it can only be constructed using the
     * #wanda::make_connection factory
     */
    connection(key, protocol::socket socket);

    /**
     * @brief Add the given listener to this control connection's listener set
     *
     * @returns <code>true</code> iff. the listener was not already in the listener set
     */
    bool add(listener * listener);

    /**
     * @brief Remove the given listener from this control connection's listener set
     *
     * @return <code>true</code> iff. the listener was previously registered with this control connection
     */
    bool remove(listener * listener);

    /**
     * @brief Start I/O processing for this control connection
     */
    void start();

    /**
     * @brief Close this control connection
     */
    void close();

    /**
     * @brief Send the given message to the remote endpoint
     */
    void send(proto::message message);

    /**
     * @brief Set the connection state to the provided state
     */
    void update(state state);

    /**
     * @brief Get the current connection state
     */
    state current_state() const;

  private:
    friend pointer make_connection(protocol::socket && socket);

    void perform_read();

    protocol::socket m_socket;
    boost::asio::streambuf m_in{};
    boost::asio::streambuf m_out{};
    std::istream m_input{&m_in};
    std::ostream m_output{&m_out};
    std::set<listener *> m_listeners{};
    state m_state{};
  };

  /**
   * @brief Create a new control connection
   */
  connection::pointer make_connection(connection::protocol::socket && socket);

}  // namespace wanda::control

#endif