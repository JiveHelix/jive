/**
  * @file socket.h
  * 
  * @brief A minimal C++ wrapper for sockets.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 Jan 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <cstring>
#include <optional>
#include <ctime>
#include <sys/socket.h>
#include <unistd.h>
#include "jive/strings.h"
#include "jive/range.h"
#include "jive/endian_tools.h"
#include "jive/error.h"
#include "jive/socket/error.h"
#include "jive/socket/address.h"


namespace jive
{


class Socket
{
public:
    Socket()
        :
        handle_{-1},
        connectedAddress_{}
    {
        this->handle_ = socket(AF_INET, SOCK_STREAM, 0);

        if (this->handle_ == -1)
        {
            throw ErrnoException<SocketError>(
                "Failed to create socket",
                errno);
        }
    }

    Socket(Socket &&other)
        :
        handle_{other.handle_},
        connectedAddress_{std::move(other.connectedAddress_)}
    {
        other.handle_ = -1;
    }

    Socket & operator=(Socket &&other)
    {
        this->Close(); 
        this->handle_ = other.handle_;
        this->connectedAddress_ = std::move(other.connectedAddress_);
        other.handle_ = -1;
        return *this;
    }

    ~Socket()
    {
        this->Close();
    }

    void Close() noexcept
    {
        if (this->handle_ > -1)
        {
            close(this->handle_);
        }
    }

    void Listen()
    {
        int result = listen(this->handle_, SOMAXCONN);

        if (result == -1)
        {
            throw ErrnoException<SocketError>(
                "Failed to mark socket as listening",
                errno);
        }
    }

    void Bind(const ServiceAddress &serviceAddress)
    {
        SocketAddress socketAddress = serviceAddress.GetSocketAddress();

        int result = bind(
            this->handle_,
            reinterpret_cast<struct sockaddr *>(&socketAddress),
            sizeof(SocketAddress));

        if (result == -1)
        {
            throw ErrnoException<SocketError>(
                "Failed to bind to " + serviceAddress.ToString(),
                errno);
        }

        this->connectedAddress_ = serviceAddress;
    }

    /**
     ** Blocks until a new connection is ready.
     **
     ** @return the new socket, connected to the client and ready to be used.
     **/
    Socket Accept()
    {
        SocketAddress connectedAddress;
        socklen_t length = sizeof(SocketAddress);

        int connectedHandle = accept(
            this->handle_,
            reinterpret_cast<struct sockaddr *>(&connectedAddress),
            &length);

        if (connectedHandle == -1)
        {
            throw ErrnoException<SocketError>(
                "Failed to accept connection",
                errno);
        }

        Socket result{};
        result.handle_ = connectedHandle;
        result.connectedAddress_ = ServiceAddress(connectedAddress);

        return result;
    }

    void Connect(const ServiceAddress &serviceAddress)
    {
        SocketAddress socketAddress = serviceAddress.GetSocketAddress();

        int result = connect(
            this->handle_,
            reinterpret_cast<struct sockaddr *>(&socketAddress),
            sizeof(SocketAddress));

        if (result == -1)
        {
            throw ErrnoException<SocketError>(
                "Failed to connect to " + serviceAddress.ToString(),
                errno);
        }

        this->connectedAddress_ = serviceAddress;
    }

    const ServiceAddress & GetConnectedAddress() const
    {
        return this->connectedAddress_;
    }

    ssize_t Receive(void *buffer, size_t count, int flags)
    {
        return recv(this->handle_, buffer, count, flags);
    }

    size_t ReceiveNoWait(void *buffer, size_t count)
    {
        ssize_t receivedCount =
            this->Receive(buffer, count, MSG_DONTWAIT);

        if (receivedCount < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                receivedCount = 0;
            }
            else
            {
                throw ErrnoException<SocketError>(
                    "Failed to receieve data from socket",
                    errno);
            }
        }

        return static_cast<size_t>(receivedCount);
    }

    std::optional<size_t> ReceiveWait(void *buffer, size_t count)
    {
        ssize_t receivedCount = this->Receive(buffer, count, 0);

        if (receivedCount < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return {};
            }
            else
            {
                throw ErrnoException<SocketError>(
                    "Failed to receieve data from socket",
                    errno);
            }
        }
        
        return {static_cast<size_t>(receivedCount)};
    }

    ssize_t Send(const void *buffer, size_t byteCount, int flags)
    {
        return send(this->handle_, buffer, byteCount, flags);
    }

    std::optional<size_t> SendWait(const void *buffer, size_t count)
    {
        ssize_t sentCount = this->Send(buffer, count, 0);

        if (sentCount < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return {};
            }
            else
            {
                throw ErrnoException<SocketError>(
                    "Failed to receieve data from socket",
                    errno);
            }
        }
        
        return {static_cast<size_t>(sentCount)};
    }

    template<typename T>
    void SetSocketOption(int optionName, const T &value)
    {
        int result = setsockopt(
            this->handle_,
            SOL_SOCKET,
            optionName,
            &value,
            sizeof(T));

        if (result == -1)
        {
            throw ErrnoException<SocketError>(
                "Failed to set socket option",
                errno);
        }
    }

    void SetReceiveTimeOut(long seconds, suseconds_t microseconds)
    {
        timeval timeValue;
        timeValue.tv_sec = seconds;
        timeValue.tv_usec = microseconds;

        this->template SetSocketOption(SO_RCVTIMEO, timeValue);
    }

    void SetSendTimeOut(long seconds, suseconds_t microseconds)
    {
        timeval timeValue;
        timeValue.tv_sec = seconds;
        timeValue.tv_usec = microseconds;

        this->template SetSocketOption(SO_SNDTIMEO, timeValue);
    }

    /* @return true when Accept will not block */
    bool WaitForConnection(long seconds, suseconds_t microseconds)
    {
        timeval timeOut = {.tv_sec = seconds, .tv_usec = microseconds};
        fd_set fileSet;

        FD_ZERO(&fileSet);
        FD_SET(this->handle_, &fileSet);
        int result = select(this->handle_ + 1, &fileSet, NULL, NULL, &timeOut);

        if (result == -1)
        {
            if (errno == EINTR)
            {
                // The select operation was interrupted before the timeout
                // expired.
                // There is no pending connection.
                return false;
            }

            throw ErrnoException<SocketError>(
                "Failed to wait for new connection",
                errno);
        }

        // A value greater than zero indicates that accept will not block.
        return (result != 0);

    }


private:
    int handle_;
    ServiceAddress connectedAddress_;
};


} // end namespace jive
