/**
  * @file describe_type_detail.h
  *
  * @brief Optionally wrap stream output in color codes if it is going to a
  * terminal.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 12 May 2020
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  *
  */

#pragma once

#include <iostream>
#include <cstdio>


#ifdef _WIN32

    #include <io.h>

    inline bool IsTerminal()
    {
        return (1 == _isatty(_fileno(stdout)));
    }

#else // *NIX

    #include <unistd.h>

    inline bool IsTerminal()
    {
        return (1 == isatty(STDOUT_FILENO));
    }

#endif


namespace jive
{

namespace color
{

inline constexpr auto black = "\u001b[30m";
inline constexpr auto red = "\u001b[31m";
inline constexpr auto green = "\u001b[32m";
inline constexpr auto yellow = "\u001b[33m";
inline constexpr auto blue = "\u001b[34m";
inline constexpr auto magenta = "\u001b[35m";
inline constexpr auto cyan = "\u001b[36m";
inline constexpr auto white = "\u001b[37m";

inline constexpr auto brightBlack = "\u001b[30;1m";
inline constexpr auto brightRed = "\u001b[31;1m";
inline constexpr auto brightGreen = "\u001b[32;1m";
inline constexpr auto brightYellow = "\u001b[33;1m";
inline constexpr auto brightBlue = "\u001b[34;1m";
inline constexpr auto brightMagenta = "\u001b[35;1m";
inline constexpr auto brightCyan = "\u001b[36;1m";
inline constexpr auto brightWhite = "\u001b[37;1m";
inline constexpr auto reset = "\u001b[0m";

inline constexpr auto bold = "\u001b[1m";
inline constexpr auto underline = "\u001b[4m";
inline constexpr auto reversed = "\u001b[7m";

} // end namespace color


/** Only colorizes output to stdout **/
class Colorize
{
public:
    Colorize(std::ostream &outputStream)
        :
        outputStream_(outputStream),
        isTerminal_(false)
    {
        if (outputStream.rdbuf() == std::cout.rdbuf())
        {
            // The output stream is writing to stdout
            this->isTerminal_ = IsTerminal();
        }
    }

public:
    template<typename ...Args>
    void operator()(const std::string_view &color, Args &&...args)
    {
        if (this->isTerminal_)
        {
            this->outputStream_ << color;
        }

        (this->outputStream_ << ... << args);

        if (this->isTerminal_)
        {
            this->outputStream_ << color::reset;
        }
    }

private:
    std::ostream &outputStream_;
    bool isTerminal_;
};

} // end namespace jive
