/**
  * @file object_client.h
  * 
  * @brief A socket that reads and writes data structured as binary objects.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 Jan 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include "jive/socket/socket.h"
#include "jive/circular_buffer.h"


namespace jive
{


template<size_t BufferSize>
class ObjectClient
{
public:
    ObjectClient(const ServiceAddress &serviceAddress)
        :
        socket_()
    {
        this->socket_.Connect(serviceAddress);
    }

    template<typename T>
    void Write(const T &data)
    {
        this->socket_.Send(&data, sizeof(T), 0);
    }

    template<typename T>
    T Read()
    {
        static_assert(
            sizeof(T) <= BufferSize,
            "Increase the buffer size to receive larger objects.");

        T result;

        // Wait until the complete object has arrived.
        this->FillReadBuffer_<sizeof(T)>();

        [[maybe_unused]] bool success =
            this->readBuffer_.Read(
                reinterpret_cast<uint8_t *>(&result),
                sizeof(T));

        assert(success);

        return result;
    }

    template<typename T>
    T Peek()
    {
        static_assert(
            sizeof(T) <= BufferSize,
            "Increase the buffer size to receive larger objects.");

        T result;

        // Wait until the complete object has arrived.
        this->FillReadBuffer_<sizeof(T)>();

        [[maybe_unused]] bool success =
            this->readBuffer_.Peek(
                reinterpret_cast<uint8_t *>(&result),
                sizeof(T));

        assert(success);

        return result;
    }

private:
    template<size_t fillCount>
    void FillReadBuffer_()
    {
        static_assert(fillCount <= BufferSize);
        
        while (this->readBuffer_.GetSize() < fillCount)
        {
            this->DrainSocket_();
        }
    }

    void DrainSocket_()
    {
        AsPointer buffer(this->readBuffer_);
        
        auto count = buffer.GetWritableSize();

        if (count == 0)
        {
            // There is no more room in the read buffer.
            throw SocketError(
                std::make_error_code(std::errc::no_buffer_space),
                "Cannot drain socket when read buffer is full.");
        }

        std::optional<size_t> receivedCount =
            this->socket_.ReceiveWait(buffer.Get(), count);

        if (!receivedCount)
        {
            throw SocketError(
                std::make_error_code(std::errc::timed_out),
                "Socket timed out");
        }

        buffer.SetWriteCount(receivedCount.value());
    }

private:
    Socket socket_;
    CircularBuffer<uint8_t, BufferSize> readBuffer_;
};


} // end namespace jive
