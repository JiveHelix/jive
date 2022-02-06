/**
  * @file address.h
  * 
  * @brief Work with network addresses.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 Jan 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <netinet/in.h>
#include <array>
#include <ostream>
#include "jive/socket/error.h"
#include "jive/strings.h"
#include "jive/range.h"
#include "jive/to_integer.h"
#include "jive/endian_tools.h"


namespace jive
{


typedef struct in_addr InternetAddress;
typedef struct sockaddr_in SocketAddress;


static_assert(sizeof(InternetAddress) == 4);


struct Address
{
    std::array<uint8_t, 4> values;

    Address(): values() {}

    Address(InternetAddress internetAddress)
    {
        memcpy(&this->values, &internetAddress, sizeof(InternetAddress));
    }

    Address(const char *address)
        :
        values()
    {
        auto parts = strings::Split(address, '.');

        if (parts.size() != 4)
        {
            throw SocketError(
                std::make_error_code(std::errc::bad_address),
                "Expected 4 octets");
        }

        for (auto index: Range(0U, 4U))
        {
            this->values[index] = ToInteger<uint8_t>(parts[index]);
        }
    }

    std::string GetAsString() const
    {
        return strings::Join(
            {
                std::to_string(std::get<0>(this->values)),
                std::to_string(std::get<1>(this->values)),
                std::to_string(std::get<2>(this->values)),
                std::to_string(std::get<3>(this->values))},
            '.');
    }

    InternetAddress Get() const
    {
        return {*reinterpret_cast<const uint32_t *>(&this->values)};
    }
};


struct ServiceAddress
{
    Address address;
    uint16_t port;

    ServiceAddress()
        :
        address{},
        port{}
    {

    }

    ServiceAddress(Address inAddress, uint16_t inPort)
        :
        address(inAddress),
        port(inPort)
    { 

    }

    ServiceAddress(SocketAddress socketAddress)
        :
        address{socketAddress.sin_addr},
        port{BigEndianToHost(socketAddress.sin_port)}
    {

    }

    ServiceAddress(const char *socketAddress, uint16_t inPort)
        :
        address{socketAddress},
        port{inPort}
    {

    }

    SocketAddress GetSocketAddress() const
    {
        SocketAddress result;

        result.sin_family = AF_INET;
        result.sin_port = HostToBigEndian(this->port);
        result.sin_addr = this->address.Get();

        return result;
    }

    std::string ToString() const
    {
        return this->address.GetAsString() + ":" + std::to_string(this->port);
    }
};


inline
std::ostream & operator<<(
    std::ostream &outputStream,
    const ServiceAddress &serviceAddress)
{
    return outputStream << serviceAddress.ToString();
}


} // end namespace jive
