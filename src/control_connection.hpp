#ifndef WANDA_CONTROL_CONNECTION_HPP
#define WANDA_CONTROL_CONNECTION_HPP

#include "keyed.hpp"

#include <boost/asio.hpp>

#include <memory>
#include <set>
#include <string>

namespace wanda
{

struct control_connection : keyed<control_connection>, std::enable_shared_from_this<control_connection>
{
    using protocol = boost::asio::local::stream_protocol;
    using pointer = std::shared_ptr<control_connection>;

    struct listener
    {
        virtual void on_close(pointer connection) {}
        virtual void on_received(pointer connection, std::string message) {}
        virtual void on_error(pointer connection, boost::system::error_code) {}
    };

    /**
     * @internal
     * @brief Construct a new control connection object
     * 
     * @param socket The socket connected to the remote control endpoint
     */
    control_connection(key, protocol::socket socket);

    /**
     * @brief Add the given listener to this control connection's listener set
     * 
     * @returns <code>true</code> iff. the listener was not already in the listener set
     */
    bool add(std::shared_ptr<listener> listener);

    /**
     * @brief Remove the given listener from this control connection's listener set
     * 
     * @return <code>true</code> iff. the listener was previously registered with this control connection
     */
    bool remove(std::shared_ptr<listener> listener);

    /**
     * @brief Start I/O processing for this control connection
     */
    void start();

    /**
     * @brief Close this control connection
     */
    void close();

  private:
    friend pointer make_control_connection(protocol::socket &&socket);

    void perform_read();

    protocol::socket m_socket;
    boost::asio::streambuf m_in{};
    boost::asio::streambuf m_out{};
    std::istream m_input{&m_in};
    std::set<std::shared_ptr<listener>> m_listeners{};
    bool m_running{};
};

control_connection::pointer make_control_connection(control_connection::protocol::socket &&socket);

} // namespace wanda

#endif