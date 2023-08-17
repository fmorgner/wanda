#ifndef WANDA_CONTROL_COMMANDER_HPP
#define WANDA_CONTROL_COMMANDER_HPP

#include "wanda/control/connection.hpp"
#include "wanda/proto/command.hpp"
#include "wanda/proto/message.hpp"

#include <asio.hpp>

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace wanda::control
{
  /**
   * @brief The remote control client
   *
   */
  struct commander : connection::listener
  {
    /**
     * @brief The interface to be implemented by remote control listeners
     */
    struct listener
    {
      virtual void on_connected(commander & commander){};
      virtual void on_response(commander & commander, std::string response){};
      virtual void on_error(commander & commander, std::string error){};
    };

    /**
     * @brief Construct a new commander
     */
    commander(asio::io_service & service, std::filesystem::path socket, listener & listener);

    /**
     * @brief Start communication with the remote daemon endpoint
     */
    void start();

    /**
     * @brief Stop communication with the remote daemon endpoint
     */
    void stop();

    /**
     * @brief Send a command to the remote daemon endpoint
     */
    void send(proto::command command);

    void on_error(connection::pointer connection, std::error_code error) override;
    void on_received(connection::pointer connection, proto::message message) override;

  private:
    asio::io_service & m_service;
    wanda::control::connection::protocol::endpoint m_endpoint;
    wanda::control::connection::protocol::socket m_socket;
    wanda::control::connection::pointer m_connection;
    listener & m_listener;
  };

}  // namespace wanda::control
#endif