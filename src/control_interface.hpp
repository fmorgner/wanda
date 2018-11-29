#ifndef WANDA_CONTROL_INTERFACE_HPP
#define WANDA_CONTROL_INTERFACE_HPP

#include "control_connection.hpp"
#include "keyed.hpp"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <filesystem>
#include <istream>
#include <memory>
#include <set>
#include <string>

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

    void on_close(control_connection::pointer) override;

  private:
    void perform_accept();

    friend pointer make_interface(boost::asio::io_service &service, std::filesystem::path file);

    boost::asio::io_service &m_service;
    protocol::endpoint m_endpoint;
    protocol::socket m_socket;
    protocol::acceptor m_acceptor;
    socket_deleter m_deleter{m_endpoint.path()};
    std::set<control_connection::pointer> m_connections;
};

control_interface::pointer make_interface(boost::asio::io_service &service, std::filesystem::path file);

} // namespace wanda

#endif