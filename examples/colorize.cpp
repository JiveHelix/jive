#include <iostream>
#include <jive/colorize.h>


int main()
{
    auto colorize = jive::Colorize(std::cout);

    colorize(jive::color::black, "I should be black\n");
    colorize(jive::color::red, "I should be red\n");
    colorize(jive::color::green, "I should be green\n");
    colorize(jive::color::yellow, "I should be yellow\n");
    colorize(jive::color::blue, "I should be blue\n");
    colorize(jive::color::magenta, "I should be magenta\n");
    colorize(jive::color::cyan, "I should be cyan\n");
    colorize(jive::color::white, "I should be white\n");
    colorize(jive::color::brightBlack, "I should be brightBlack\n");
    colorize(jive::color::brightRed, "I should be brightRed\n");
    colorize(jive::color::brightGreen, "I should be brightGreen\n");
    colorize(jive::color::brightYellow, "I should be brightYellow\n");
    colorize(jive::color::brightBlue, "I should be brightBlue\n");
    colorize(jive::color::brightMagenta, "I should be brightMagenta\n");
    colorize(jive::color::brightCyan, "I should be brightCyan\n");
    colorize(jive::color::brightWhite, "I should be brightWhite\n");

    return 0;
}
