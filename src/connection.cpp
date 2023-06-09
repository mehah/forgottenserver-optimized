/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 * Copyright (C) 2019-2021  Saiyans King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "configmanager.h"
#include "connection.h"
#include "outputmessage.h"
#include "protocol.h"
#include "tasks.h"
#include "server.h"

extern ConfigManager g_config;

Connection_ptr ConnectionManager::createConnection(asio::io_service& io_service, const ServicePort_ptr&
                                                   servicePort)
{
    std::lock_guard<std::mutex> lockClass(connectionManagerLock);

    auto connection = std::make_shared<Connection>(io_service, servicePort);
    connections.insert(connection);
    return connection;
}

void ConnectionManager::releaseConnection(const Connection_ptr& connection)
{
    std::lock_guard<std::mutex> lockClass(connectionManagerLock);

    connections.erase(connection);
}

void ConnectionManager::closeAll()
{
    std::lock_guard<std::mutex> lockClass(connectionManagerLock);

    for (const auto& connection : connections) {
        try {
            std::error_code error;
            connection->socket.shutdown(asio::ip::tcp::socket::shutdown_both, error);
            connection->socket.close(error);
        } catch (std::system_error&) {
        }
    }
    connections.clear();
}

// Connection

void Connection::close(const bool force)
{
    //any thread
    ConnectionManager::getInstance().releaseConnection(shared_from_this());

    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    if (connectionState == CONNECTION_STATE_CLOSED) {
        return;
    }
    connectionState = CONNECTION_STATE_CLOSED;

    if (protocol) {
        g_dispatcher.addTask([this] { protocol->release(); });
    }

    if (messageQueue.empty() || force) {
        closeSocket();
    } else {
        //will be closed by the destructor or onWriteOperation
    }
}

void Connection::closeSocket()
{
    if (socket.is_open()) {
        try {
            readTimer.cancel();
            writeTimer.cancel();
            std::error_code error;
            socket.shutdown(asio::ip::tcp::socket::shutdown_both, error);
            socket.close(error);
        } catch (std::system_error& e) {
            std::cout << "[Network error - Connection::closeSocket] " << e.what() << std::endl;
        }
    }
}

Connection::~Connection()
{
    closeSocket();
}

void Connection::accept(const Protocol_ptr& protocol)
{
    this->connectionState = CONNECTION_STATE_IDENTIFYING;
    this->protocol = protocol;
    g_dispatcher.addTask([protocol] { protocol->onConnect(); });

    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    try {
        readTimer.expires_from_now(asio::chrono::seconds(CONNECTION_READ_TIMEOUT));
        readTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
            return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
        });

        // Read header bytes to identify if it is proxy identification
        asio::async_read(socket, asio::buffer(msg.getBuffer(), NetworkMessage::HEADER_LENGTH), std::bind(&Connection::parseProxyIdentification, shared_from_this(), std::placeholders::_1));
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::accept] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

void Connection::accept()
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    try {
        readTimer.expires_from_now(asio::chrono::seconds(CONNECTION_READ_TIMEOUT));
        readTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
            return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
        });

        // Read size of the first packet
        asio::async_read(socket, asio::buffer(msg.getBuffer(), NetworkMessage::HEADER_LENGTH), std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::accept] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

void Connection::parseProxyIdentification(const std::error_code& error)
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    readTimer.cancel();

    if (error) {
        close(FORCE_CLOSE);
        return;
    }
    if (connectionState == CONNECTION_STATE_CLOSED) {
        return;
    }

    uint8_t* msgBuffer = msg.getBuffer();
    const std::string serverName = g_config.getString(ConfigManager::SERVER_NAME) + "\n";
    if (connectionState == CONNECTION_STATE_IDENTIFYING) {
        if (msgBuffer[1] == 0x00 || strncasecmp(reinterpret_cast<char*>(msgBuffer), serverName.data(), 2) != 0) {
            //Probably not proxy identification so let's try standard parsing method
            connectionState = CONNECTION_STATE_OPEN;
            parseHeader(error);
            return;
        }
        const size_t remainder = serverName.length() - 2;
        if (remainder > 0) {
            connectionState = CONNECTION_STATE_READINGS;
            try {
                readTimer.expires_from_now(asio::chrono::seconds(CONNECTION_READ_TIMEOUT));
                readTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
                    return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
                });

                // Read the remainder of proxy identification
                asio::async_read(socket, asio::buffer(msg.getBuffer(), remainder), std::bind(&Connection::parseProxyIdentification, shared_from_this(), std::placeholders::_1));
            } catch (std::system_error& e) {
                std::cout << "[Network error - Connection::parseProxyIdentification] " << e.what() << std::endl;
                close(FORCE_CLOSE);
            }
            return;
        }
        connectionState = CONNECTION_STATE_OPEN;
    } else if (connectionState == CONNECTION_STATE_READINGS) {
        const size_t remainder = serverName.length() - 2;
        if (strncasecmp(reinterpret_cast<char*>(msgBuffer), &serverName[2], remainder) == 0) {
            connectionState = CONNECTION_STATE_OPEN;
        } else {
            close(FORCE_CLOSE);
            return;
        }
    }

    try {
        readTimer.expires_from_now(asio::chrono::seconds(CONNECTION_READ_TIMEOUT));
        readTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
            return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
        });

        // Wait to the next packet
        asio::async_read(socket, asio::buffer(msg.getBuffer(), NetworkMessage::HEADER_LENGTH), std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::parseProxyIdentification] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

void Connection::parseHeader(const std::error_code& error)
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    readTimer.cancel();

    if (error) {
        close(FORCE_CLOSE);
        return;
    }
    if (connectionState == CONNECTION_STATE_CLOSED) {
        return;
    }

    const uint32_t timePassed = std::max<uint32_t>(1, time(nullptr) - timeConnected + 1);
    if (++packetsSent / timePassed > static_cast<uint32_t>(g_config.getNumber(ConfigManager::MAX_PACKETS_PER_SECOND))) {
        std::cout << convertIPToString(getIP()) << " disconnected for exceeding packet per second limit." << std::endl;
        close();
        return;
    }

    if (timePassed > 2) {
        timeConnected = time(nullptr);
        packetsSent = 0;
    }

    const uint16_t size = msg.getLengthHeader();
    if (size == 0 || size > INPUTMESSAGE_MAXSIZE) {
        close(FORCE_CLOSE);
        return;
    }

    try {
        readTimer.expires_from_now(asio::chrono::seconds(CONNECTION_READ_TIMEOUT));
        readTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
            return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
        });

        // Read packet content
        msg.setLength(size + NetworkMessage::HEADER_LENGTH);
        asio::async_read(socket, asio::buffer(msg.getBodyBuffer(), size), std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::parseHeader] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

void Connection::parsePacket(const std::error_code& error)
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    readTimer.cancel();

    if (error) {
        close(FORCE_CLOSE);
        return;
    }
    if (connectionState == CONNECTION_STATE_CLOSED) {
        return;
    }

    bool skipReadingNextPacket = false;
    if (!receivedFirst) {
        // First message received
        receivedFirst = true;

        if (!protocol) {
            //Check packet checksum
            uint32_t checksum;
            const int32_t len = msg.getLength() - msg.getBufferPosition() - NetworkMessage::CHECKSUM_LENGTH;
            if (len > 0) {
                checksum = adlerChecksum(msg.getBuffer() + msg.getBufferPosition() + NetworkMessage::CHECKSUM_LENGTH, len);
            } else {
                checksum = 0;
            }

            const auto recvChecksum = msg.get<uint32_t>();
            if (recvChecksum != checksum) {
                // it might not have been the checksum, step back
                msg.skipBytes(-NetworkMessage::CHECKSUM_LENGTH);
            }

            // Game protocol has already been created at this point
            protocol = service_port->make_protocol(recvChecksum == checksum, msg, shared_from_this());
            if (!protocol) {
                close(FORCE_CLOSE);
                return;
            }
        } else {
            // It is rather hard to detect if we have checksum or sequence method here so let's skip checksum check
            // it doesn't generate any problem because olders protocol don't use 'server sends first' feature
            msg.get<uint32_t>();
            msg.skipBytes(1);    // Skip protocol ID
        }

        protocol->onRecvFirstMessage(msg);
    } else {
        skipReadingNextPacket = protocol->onRecvMessage(msg); // Send the packet to the current protocol
    }

    try {
        readTimer.expires_from_now(asio::chrono::seconds(CONNECTION_READ_TIMEOUT));
        readTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
            return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
        });

        if (!skipReadingNextPacket) {
            // Wait to the next packet
            asio::async_read(socket, asio::buffer(msg.getBuffer(), NetworkMessage::HEADER_LENGTH), std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
        }
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::parsePacket] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

void Connection::resumeWork()
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);

    try {
        // Wait to the next packet
        asio::async_read(socket, asio::buffer(msg.getBuffer(), NetworkMessage::HEADER_LENGTH), std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::parsePacket] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

void Connection::send(const OutputMessage_ptr& msg)
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
    if (connectionState == CONNECTION_STATE_CLOSED) {
        return;
    }

    const bool noPendingWrite = messageQueue.empty();
    messageQueue.emplace_back(msg);
    if (noPendingWrite) {
        // Make asio thread handle xtea encryption instead of dispatcher
        try {
            asio::post([capture0 = shared_from_this()] { capture0->internalWorker(); });
        } catch (std::system_error& e) {
            std::cout << "[Network error - Connection::send] " << e.what() << std::endl;
            messageQueue.clear();
            close(FORCE_CLOSE);
        }
    }
}

void Connection::internalWorker()
{
    std::unique_lock<std::recursive_mutex> lockClass(connectionLock);
    if (!messageQueue.empty()) {
        const OutputMessage_ptr& msg = messageQueue.front();
        lockClass.unlock();
        protocol->onSendMessage(msg);
        lockClass.lock();
        internalSend(msg);
    } else if (connectionState == CONNECTION_STATE_CLOSED) {
        closeSocket();
    }
}

void Connection::internalSend(const OutputMessage_ptr& msg)
{
    try {
        writeTimer.expires_from_now(asio::chrono::seconds(CONNECTION_WRITE_TIMEOUT));
        writeTimer.async_wait([capture0 = std::weak_ptr<Connection>(shared_from_this())](auto&& PH1) {
            return Connection::handleTimeout(capture0, std::forward<decltype(PH1)>(PH1));
        });

        asio::async_write(socket, asio::buffer(msg->getOutputBuffer(), msg->getLength()), std::bind(&Connection::onWriteOperation, shared_from_this(), std::placeholders::_1));
    } catch (std::system_error& e) {
        std::cout << "[Network error - Connection::internalSend] " << e.what() << std::endl;
        close(FORCE_CLOSE);
    }
}

uint32_t Connection::getIP()
{
    std::lock_guard<std::recursive_mutex> lockClass(connectionLock);

    // IP-address is expressed in network byte order
    std::error_code error;
    const asio::ip::tcp::endpoint endpoint = socket.remote_endpoint(error);
    if (error) {
        return 0;
    }

    return htonl(endpoint.address().to_v4().to_ulong());
}

void Connection::onWriteOperation(const std::error_code& error)
{
    std::unique_lock<std::recursive_mutex> lockClass(connectionLock);
    writeTimer.cancel();
    messageQueue.pop_front();

    if (error) {
        messageQueue.clear();
        close(FORCE_CLOSE);
        return;
    }

    if (!messageQueue.empty()) {
        const OutputMessage_ptr& msg = messageQueue.front();
        lockClass.unlock();
        protocol->onSendMessage(msg);
        lockClass.lock();
        internalSend(msg);
    } else if (connectionState == CONNECTION_STATE_CLOSED) {
        closeSocket();
    }
}

void Connection::handleTimeout(const ConnectionWeak_ptr& connectionWeak, const std::error_code& error)
{
    if (error == asio::error::operation_aborted) {
        //The timer has been manually cancelled
        return;
    }

    if (const auto connection = connectionWeak.lock()) {
        connection->close(FORCE_CLOSE);
    }
}