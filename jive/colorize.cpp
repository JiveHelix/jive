#include "jive/colorize.h"


#ifdef _WIN32
#include <windows.h>


namespace jive
{


ConsoleMode::ConsoleMode(bool isTerminal)
{
    if (!isTerminal)
    {
        return;
    }

    auto handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (0 == SetConsoleMode(
        handle,
        ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        std::cerr << "Failed to SetConsoleMode" << std::endl;
    }
}


} // end namespace jive


#endif
