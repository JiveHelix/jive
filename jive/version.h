/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2021 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <stdexcept>
#include <fmt/core.h>
#include "jive/create_exception.h"
#include "jive/strings.h"
#include "jive/to_integer.h"
#include "jive/binary_io.h"

#undef major
#undef minor


namespace jive
{


CREATE_EXCEPTION(VersionError, std::runtime_error);


template<typename T>
class Version
{
public:
    static_assert(std::is_integral_v<T>);
    static_assert(sizeof(T) <= 2);

    constexpr Version(): major(0), minor(0), revision(0) {}

    constexpr Version(T major_, T minor_, T revision_)
        :
        major(major_),
        minor(minor_),
        revision(revision_)
    {

    }

    Version(const std::string &versionAsString)
    {
        auto split = jive::strings::Split(versionAsString, '.');

        if (split.size() != 3)
        {
            throw VersionError(
                "Unrecognized version format: " + versionAsString);
        }

        try
        {
            this->major = jive::ToInteger<T>(split[0]);
            this->minor = jive::ToInteger<T>(split[1]);
            this->revision = jive::ToInteger<T>(split[2]);
        }
        catch (std::invalid_argument &error)
        {
            throw VersionError(
                "Unrecognized version format: "
                + versionAsString
                + ", "
                + error.what());
        }
        catch (std::out_of_range &error)
        {
            throw VersionError(error.what());
        }
    }

    std::string ToString() const
    {
        return fmt::format(
            "{}.{}.{}",
            this->major,
            this->minor,
            this->revision);
    }

    static Version FromString(const std::string &versionAsString)
    {
        return Version(versionAsString);
    }

    bool operator<(const Version &other) const
    {
        return this->GetAsTuple() < other.GetAsTuple();
    }

    bool operator>(const Version &other) const
    {
        return this->GetAsTuple() > other.GetAsTuple();
    }

    bool operator<=(const Version &other) const
    {
        return this->GetAsTuple() <= other.GetAsTuple();
    }

    bool operator>=(const Version &other) const
    {
        return this->GetAsTuple() >= other.GetAsTuple();
    }

    bool operator==(const Version &other) const
    {
        return this->GetAsTuple() == other.GetAsTuple();
    }

    bool operator!=(const Version &other) const
    {
        return !(*this == other);
    }

    std::tuple<T, T, T> GetAsTuple() const
    {
        return {this->major, this->minor, this->revision};
    }

    void Write(std::ostream &outputStream)
    {
        io::Write(outputStream, this->major);
        io::Write(outputStream, this->minor);
        io::Write(outputStream, this->revision);
    }

    static Version Read(std::istream &inputStream)
    {
        Version result;

        result.major = io::Read<T>(inputStream);
        result.minor = io::Read<T>(inputStream);
        result.revision = io::Read<T>(inputStream);

        return result;
    }

    T major;
    T minor;
    T revision;
};


} // end namespace jive
