#ifndef WANDA_CONTROL_INTERFACE_HPP
#define WANDA_CONTROL_INTERFACE_HPP

#include "control_connection.hpp"
#include "keyed.hpp"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

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
    using protocol = boost::asio::local::stream_protocol;
    using pointer = std::shared_ptr<control_interface>;

    control_interface(key, boost::asio::io_service &service, protocol::endpoint endpoint);

    boost::system::error_code start();
    boost::system::error_code shutdown();

    void on_close(control_connection::pointer connection) override;
    void on_received(control_connection::pointer connection, message message) override;

  private:
    enum struct state : std::underlying_type_t<std::byte>
    {
        fresh,
        greeted,
    };

    void perform_accept();

    friend pointer make_interface(boost::asio::io_service &service, std::filesystem::path file);

    boost::asio::io_service &m_service;
    protocol::endpoint m_endpoint;
    protocol::socket m_socket;
    protocol::acceptor m_acceptor;
    socket_deleter m_deleter{m_endpoint.path()};
    std::set<control_connection::pointer> m_connections;
    std::map<control_connection::pointer, state> m_states;
};

control_interface::pointer make_interface(boost::asio::io_service &service, std::filesystem::path file);

} // namespace wanda

#endif