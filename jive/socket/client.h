/**
  * @file client.h
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
class Client: public Socket
{
public:
    Client(const ServiceAddress &serviceAddress)
    {
        this->Connect(serviceAddress);
    }

    template<typename T>
    void Write(const T &data)
    {
        this->Send(&data, sizeof(T), 0);
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

    void Read(void *target, size_t byteCount)
    {
        size_t receivedCount = 0;

        while (receivedCount < byteCount)
        {
            auto increment = this->ReceiveWait(
                static_cast<uint8_t *>(target) + receivedCount,
                byteCount - receivedCount);

            if (!increment)
            {
                throw SocketError(
                    std::make_error_code(std::errc::timed_out),
                    "Socket timed out");
            }

            receivedCount += increment.value();
        }
    }

    void Write(const void *source, size_t byteCount)
    {
        size_t sentCount = 0;

        while (sentCount < byteCount)
        {
            auto increment = this->SendWait(
                static_cast<const uint8_t *>(source) + sentCount,
                byteCount - sentCount);

            if (!increment)
            {
                throw SocketError(
                    std::make_error_code(std::errc::timed_out),
                    "Socket timed out");
            }

            sentCount += increment.value();
        }
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

    void PrintBuffer(std::ostream &outputStream) const
    {
        this->readBuffer_.PrintElements(outputStream);
    }

    void DiscardBuffer()
    {
        this->readBuffer_.Reset();
    }

private:
    template<size_t fillCount>
    void FillReadBuffer_()
    {
        static_assert(fillCount <= BufferSize);
        
        while (this->readBuffer_.GetSize() < fillCount)
        {
            this->DrainSocket_(fillCount - this->readBuffer_.GetSize());
        }
    }

    void DrainSocket_(size_t byteCount)
    {
        AsPointer buffer(this->readBuffer_);
        
        auto bufferSize = buffer.GetWritableSize();

        if (bufferSize == 0)
        {
            // There is no more room in the read buffer.
            throw SocketError(
                std::make_error_code(std::errc::no_buffer_space),
                "Cannot drain socket when read buffer is full.");
        }

        size_t count = std::min(bufferSize, byteCount);

        std::optional<size_t> receivedCount =
            this->ReceiveWait(buffer.Get(), count);

        if (!receivedCount)
        {
            throw SocketError(
                std::make_error_code(std::errc::timed_out),
                "Socket timed out");
        }

        buffer.SetWriteCount(receivedCount.value());
    }

private:
    CircularBuffer<uint8_t, BufferSize> readBuffer_;
};


} // end namespace jive
