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
#include <cstddef>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "jive/strings.h"
#include "jive/range.h"
#include "jive/endian_tools.h"
#include "jive/error.h"
#include "jive/socket/error.h"
#include "jive/socket/address.h"


namespace jive
{


inline
bool AddFlag(int handle, int flag)
{
    int flags = fcntl(handle, F_GETFL);

    if (-1 == flags)
    {
        // Failed to get flags
        return false;
    }

    int result = fcntl(handle, F_SETFL, flags | flag);

    if (-1 == result)
    {
        // Failed to set flags
        return false;
    }

    return true;
}

inline
bool RemoveFlag(int handle, int flag)
{
    int flags = fcntl(handle, F_GETFL);

    if (-1 == flags)
    {
        // Failed to get flags
        return false;
    }

    flags &= ~flag;

    int result = fcntl(handle, F_SETFL, flags);

    if (-1 == result)
    {
        // Failed to set flags
        return false;
    }

    return true;
}


inline
bool WouldBlock(int errorCode)
{
#if EAGAIN == EWOULDBLOCK
    return errorCode == EAGAIN;
#else
    return errorCode == EAGAIN || errorCode == EWOULDBLOCK;
#endif
}


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
            throw SocketError(SystemError(errno), "Failed to create socket");
        }
    }

    Socket(Socket &&other) noexcept
        :
        handle_{other.handle_},
        connectedAddress_{other.connectedAddress_}
    {
        other.handle_ = -1;
    }

    Socket & operator=(Socket &&other) noexcept
    {
        this->Close(); 
        this->handle_ = other.handle_;
        this->connectedAddress_ = other.connectedAddress_;
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

        this->handle_ = -1;
    }

    void Listen() const
    {
        int result = listen(this->handle_, SOMAXCONN);

        if (result == -1)
        {
            throw SocketError(
                SystemError(errno),
                "Failed to mark socket as listening");
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
            throw SocketError(
                SystemError(errno),
                "Failed to bind to " + serviceAddress.ToString());
        }

        this->connectedAddress_ = serviceAddress;
    }

    /**
     ** Blocks until a new connection is ready.
     **
     ** @return the new socket, connected to the client and ready to be used.
     **/
    Socket Accept() const
    {
        SocketAddress connectedAddress;
        socklen_t length = sizeof(SocketAddress);

        int connectedHandle = accept(
            this->handle_,
            reinterpret_cast<struct sockaddr *>(&connectedAddress),
            &length);

        if (connectedHandle == -1)
        {
            throw SocketError(
                SystemError(errno),
                "Failed to accept connection");
        }

        Socket result{};
        result.handle_ = connectedHandle;
        result.connectedAddress_ = ServiceAddress(connectedAddress);

        return result;
    }

    void Connect(
        const ServiceAddress &serviceAddress,
        int timeOutMilliseconds = 2000)
    {
        SocketAddress socketAddress = serviceAddress.GetSocketAddress();

        if (timeOutMilliseconds >= 0)
        {
            if (!AddFlag(this->handle_, O_NONBLOCK))
            {
                std::cout
                    << "Failed to make socket non-blocking for connection."
                    << std::endl;
            }
        }

        int result = connect(
            this->handle_,
            reinterpret_cast<struct sockaddr *>(&socketAddress),
            sizeof(SocketAddress));

        if (result == -1)
        {
            if (errno != EINPROGRESS)
            {
                throw SocketError(
                    SystemError(errno),
                    "Failed to connect to " + serviceAddress.ToString());
            }

            // The socket is non-blocking.
            // Use poll to determine when it is ready.
            struct pollfd events{};
            events.fd = this->handle_;
            events.events = POLLOUT | POLLPRI;
            
            result = poll(&events, 1, timeOutMilliseconds);

            if (0 == result)
            {
                // time out
                throw SocketError(
                    SystemError(ETIMEDOUT),
                    "Connection timed out");
            }
            else if (-1 == result)
            {
                throw SocketError(
                    SystemError(errno),
                    "Failed to connect to " + serviceAddress.ToString());
            }
            else
            {
                // Check for connection success
                int socketError = this->GetSocketOption<int>(SO_ERROR);

                if (socketError != 0)
                {
                    // An error occurred
                    throw SocketError(
                        SystemError(errno),
                        "Failed to connect to " + serviceAddress.ToString());
                }
                // else
                // Success!
            }
        }

        if (timeOutMilliseconds >= 0)
        {
            if (!RemoveFlag(this->handle_, O_NONBLOCK))
            {
                std::cout
                    << "Failed to make socket blocking after connection."
                    << std::endl;
            }
        }

        this->connectedAddress_ = serviceAddress;
    }

    const ServiceAddress & GetConnectedAddress() const
    {
        return this->connectedAddress_;
    }

    std::ptrdiff_t Receive(void *buffer, size_t count, int flags) const
    {
        return recv(this->handle_, buffer, count, flags);
    }

    size_t Flush() const
    {
        static constexpr size_t bufferSize = 64;
        uint8_t discard[bufferSize];
        size_t readCount = 0;
        size_t discardedCount = 0;

        while (0 != (readCount = ReceiveNoWait(discard, bufferSize)))
        {
            discardedCount += readCount;
        }

        return discardedCount;
    }

    size_t ReceiveNoWait(void *buffer, size_t count) const
    {
        std::ptrdiff_t receivedCount =
            this->Receive(buffer, count, MSG_DONTWAIT);

        if (receivedCount < 0)
        {
            if (WouldBlock(errno))
            {
                receivedCount = 0;
            }
            else
            {
                throw SocketError(
                    SystemError(errno),
                    "Failed to receieve data from socket");
            }
        }

        return static_cast<size_t>(receivedCount);
    }

    std::optional<size_t> ReceiveWait(void *buffer, size_t count) const
    {
        std::ptrdiff_t receivedCount = this->Receive(buffer, count, 0);

        if (receivedCount < 0)
        {
            if (WouldBlock(errno))
            {
                return {};
            }

            throw SocketError(
                SystemError(errno),
                "Failed to receieve data from socket");
        }
        
        return {static_cast<size_t>(receivedCount)};
    }

    std::ptrdiff_t Send(const void *buffer, size_t byteCount, int flags) const
    {
        return send(this->handle_, buffer, byteCount, flags);
    }

    std::optional<size_t> SendWait(const void *buffer, size_t count) const
    {
        std::ptrdiff_t sentCount = this->Send(buffer, count, 0);

        if (sentCount < 0)
        {
            if (WouldBlock(errno))
            {
                return {};
            }

            throw SocketError(
                SystemError(errno),
                "Failed to send data to socket");
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
            throw SocketError(
                SystemError(errno),
                "Failed to set socket option");
        }
    }

    template<typename T>
    T GetSocketOption(int optionName)
    {
        T result;
        socklen_t optionSize = sizeof(T);

        int socketResult = getsockopt(
            this->handle_,
            SOL_SOCKET,
            optionName,
            &result,
            &optionSize);

        if (socketResult == -1)
        {
            throw SocketError(
                SystemError(errno),
                "Failed to get socket option");
        }

        if (optionSize != sizeof(T))
        {
            throw std::runtime_error("Result size mismatch");
        }

        return result;
    }

    void SetReceiveTimeOut(long seconds, suseconds_t microseconds)
    {
        timeval timeValue;
        timeValue.tv_sec = seconds;
        timeValue.tv_usec = microseconds;

        this->SetSocketOption(SO_RCVTIMEO, timeValue);
    }

    void SetSendTimeOut(long seconds, suseconds_t microseconds)
    {
        timeval timeValue;
        timeValue.tv_sec = seconds;
        timeValue.tv_usec = microseconds;

        this->SetSocketOption(SO_SNDTIMEO, timeValue);
    }

    /* @return true when Accept will not block */
    bool WaitForConnection(long seconds, suseconds_t microseconds)
    {
        timeval timeOut = {seconds, microseconds};
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

            throw SocketError(
                SystemError(errno),
                "Failed to wait for new connection");
        }

        // A value greater than zero indicates that accept will not block.
        return (result != 0);
    }


private:
    int handle_;
    ServiceAddress connectedAddress_;
};


} // end namespace jive
