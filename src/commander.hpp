#ifndef WANDA_COMMANDER_HPP
#define WANDA_COMMANDER_HPP

#include "control_connection.hpp"

#include <boost/system/error_code.hpp>

#include <filesystem>
#include <string>

namespace wanda
{

struct commander : wanda::control_connection::listener
{
  commander(boost::asio::io_service &service, std::filesystem::path socket);

  void start();
  void send(std::string message);

  void on_error(wanda::control_connection::pointer connection, boost::system::error_code error) override;

private:
  wanda::control_connection::protocol::endpoint m_endpoint;
  wanda::control_connection::protocol::socket m_socket;
  wanda::control_connection::pointer m_connection;
};

} // namespace wanda
#endif