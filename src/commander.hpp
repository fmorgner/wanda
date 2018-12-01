#ifndef WANDA_COMMANDER_HPP
#define WANDA_COMMANDER_HPP

#include "command.hpp"
#include "control_connection.hpp"
#include "message.hpp"

#include <asio.hpp>
#include <spdlog/logger.h>

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace wanda
{

struct commander : wanda::control_connection::listener
{
  struct listener
  {
    virtual void on_connected(commander & commander) { };
    virtual void on_response(commander & commander, std::string response) { };
    virtual void on_error(commander & commander, std::string error) { };
  };

  commander(asio::io_service &service, std::filesystem::path socket, listener & listener, std::shared_ptr<spdlog::logger> logger);

  void start();
  void send(command command);

  void on_error(control_connection::pointer connection, std::error_code error) override;
  void on_received(control_connection::pointer connection, message message) override;

private:
  asio::io_service & m_service;
  wanda::control_connection::protocol::endpoint m_endpoint;
  wanda::control_connection::protocol::socket m_socket;
  wanda::control_connection::pointer m_connection;
  listener & m_listener;
  std::shared_ptr<spdlog::logger> m_logger;
};

} // namespace wanda
#endif