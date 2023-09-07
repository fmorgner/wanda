#ifndef WANDA_APP_WANDAC_LISTENER_HPP
#define WANDA_APP_WANDAC_LISTENER_HPP

#include "wandac/cli.hpp"

#include <wanda/control/commander.hpp>

#include <boost/asio/io_context.hpp>

namespace wandac
{

  struct listener : wanda::control::commander::listener
  {
    listener(wandac::cli const & cli, boost::asio::io_context & service);

    auto on_connected(wanda::control::commander & commander) -> void override ;

  private:
    wandac::cli const & m_cli;
    boost::asio::io_context & m_service;
  };

}  // namespace wandac

#endif