/**
 * @file   environment.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_CONTROL_INTERFACE_HPP
#define WANDA_CONTROL_INTERFACE_HPP

#include <wanda/command.hpp>
#include <wanda/control_connection.hpp>
#include <wanda/keyed.hpp>

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <filesystem>
#include <istream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <type_traits>

namespace wanda
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
  struct control_interface : control_connection::listener, keyed<control_interface>, std::enable_shared_from_this<control_interface>
  {
    using protocol = asio::local::stream_protocol;
    using pointer = std::shared_ptr<control_interface>;

    /**
     * @brief The interface to be implemented by the control interface listener
     */
    struct listener
    {
      virtual void on_received(control_interface & interface, command command){};
    };

    /**
     * @internal
     * @brief Construct a new control interface object
     * 
     * @note This constructor is keyed on a private key type so it can only be constructed using the #wanda::make_interface factory
     */
    control_interface(key, asio::io_service & service, protocol::endpoint endpoint, listener & listener);

    /**
     * @brief Start handling of controller connections
     */
    std::error_code start();

    /**
     * @brief Stop the control interface
     */
    std::error_code shutdown();

    void on_close(control_connection::pointer connection) override;
    void on_received(control_connection::pointer connection, message message) override;

  private:
    void perform_accept();

    friend pointer make_interface(asio::io_service & service, std::filesystem::path file, control_interface::listener & listener);

    asio::io_service & m_service;
    protocol::endpoint m_endpoint;
    protocol::socket m_socket;
    protocol::acceptor m_acceptor;
    listener & m_listener;
    socket_deleter m_deleter{m_endpoint.path()};
    std::set<control_connection::pointer> m_connections;
  };

  /**
   * @brief A factory to create new #control_interface instances
   */
  control_interface::pointer make_interface(asio::io_service & service, std::filesystem::path socket, control_interface::listener & listener);

}  // namespace wanda

#endif