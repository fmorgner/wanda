#ifndef WANDA_COMMANDER_HPP
#define WANDA_COMMANDER_HPP

#include <wanda/command.hpp>
#include <wanda/control_connection.hpp>
#include <wanda/message.hpp>

#include <asio.hpp>

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace wanda
{
  /**
   * @brief The remote control client
   * 
   */
  struct commander : wanda::control_connection::listener
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
    void send(command command);

    void on_error(control_connection::pointer connection, std::error_code error) override;
    void on_received(control_connection::pointer connection, message message) override;

  private:
    asio::io_service & m_service;
    wanda::control_connection::protocol::endpoint m_endpoint;
    wanda::control_connection::protocol::socket m_socket;
    wanda::control_connection::pointer m_connection;
    listener & m_listener;
  };

}  // namespace wanda
#endif