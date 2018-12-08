#ifndef WANDA_CONTROL_INTERFACE_HPP
#define WANDA_CONTROL_INTERFACE_HPP

#include "command.hpp"
#include "control_connection.hpp"
#include "keyed.hpp"

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
  struct socket_deleter
  {
    ~socket_deleter();

    std::filesystem::path path;
  };

  struct control_interface : control_connection::listener, keyed<control_interface>, std::enable_shared_from_this<control_interface>
  {
    using protocol = asio::local::stream_protocol;
    using pointer = std::shared_ptr<control_interface>;

    struct listener
    {
      virtual void on_received(control_interface & interface, command command){};
    };

    control_interface(key, asio::io_service & service, protocol::endpoint endpoint, listener & listener, std::shared_ptr<spdlog::logger> logger);

    std::error_code start();
    std::error_code shutdown();

    void on_close(control_connection::pointer connection) override;
    void on_received(control_connection::pointer connection, message message) override;

  private:
    void perform_accept();

    friend pointer make_interface(asio::io_service & service, std::filesystem::path file, control_interface::listener & listener, std::shared_ptr<spdlog::logger> logger);

    asio::io_service & m_service;
    protocol::endpoint m_endpoint;
    protocol::socket m_socket;
    protocol::acceptor m_acceptor;
    listener & m_listener;
    socket_deleter m_deleter{m_endpoint.path()};
    std::set<control_connection::pointer> m_connections;
    std::shared_ptr<spdlog::logger> m_logger;
  };

  control_interface::pointer make_interface(asio::io_service & service, std::filesystem::path file, control_interface::listener & listener, std::shared_ptr<spdlog::logger> logger);

}  // namespace wanda

#endif