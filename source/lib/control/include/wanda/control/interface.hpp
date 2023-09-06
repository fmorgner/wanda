/**
 * @file   interface.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_CONTROL_INTERFACE_HPP
#define WANDA_CONTROL_INTERFACE_HPP

#include "wanda/control/connection.hpp"
#include "wanda/meta/keyed.hpp"
#include "wanda/proto/command.hpp"

#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/io_context.hpp>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <filesystem>
#include <istream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <type_traits>

namespace wanda::control
{
  /**
   * @brief An RAII type to delete a socket file upon destruction
   */
  struct socket_deleter
  {
    ~socket_deleter();

    std::filesystem::path path;
  };

  /**
   * @brief The daemon control interface
   */
  struct interface : connection::listener, meta::keyed<interface>, std::enable_shared_from_this<interface>
  {
    using protocol = boost::asio::local::stream_protocol;
    using pointer = std::shared_ptr<interface>;

    /**
     * @brief The interface to be implemented by the control interface listener
     */
    struct listener
    {
      virtual void on_received(interface & interface, proto::command command){};
    };

    /**
     * @internal
     * @brief Construct a new control interface object
     *
     * @note This constructor is keyed on a private key type so it can only be constructed using the #wanda::make_interface factory
     */
    interface(key, boost::asio::io_context & service, protocol::endpoint endpoint, listener & listener);

    /**
     * @brief Start handling of controller connections
     */
    std::error_code start();

    /**
     * @brief Stop the control interface
     */
    std::error_code shutdown();

    void on_close(connection::pointer connection) override;
    void on_received(connection::pointer connection, proto::message message) override;

  private:
    void perform_accept();

    friend pointer make_interface(boost::asio::io_context & service, std::filesystem::path file, interface::listener & listener);

    boost::asio::io_context & m_service;
    protocol::endpoint m_endpoint;
    protocol::socket m_socket;
    protocol::acceptor m_acceptor;
    listener & m_listener;
    socket_deleter m_deleter{m_endpoint.path()};
    std::set<connection::pointer> m_connections;
  };

  /**
   * @brief A factory to create new #interface instances
   */
  interface::pointer make_interface(boost::asio::io_context & service, std::filesystem::path socket, interface::listener & listener);

}  // namespace wanda::control

#endif