#ifndef TERMINAL_INPUT__H
#define TERMINAL_INPUT__H

#include <chrono>
#include <iostream>
#include <thread>

#if defined(_WIN32) || defined(_WIN64)
#    include <conio.h>
#    define PLATFORM_WINDOWS
#else
#    include <sys/select.h>
#    include <termios.h>
#    include <unistd.h>
#    define PLATFORM_UNIX
#endif

class TerminalInput {
   public:
    static char getCharTimeout(int timeout_ms) {
#ifdef PLATFORM_WINDOWS
        auto start = std::chrono::steady_clock::now();
        while (true) {
            if (_kbhit()) {
                return static_cast<char>(_getch());
            }

            auto now = std::chrono::steady_clock::now();
            auto elapsed =
                std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            if (elapsed >= timeout_ms)
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        return '\0';

#elif defined(PLATFORM_UNIX)
        struct termios oldt, newt;
        char ch = '\0';

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
            read(STDIN_FILENO, &ch, 1);
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
#endif
    }
};

#endif  // INPUT_HANDLER_HPP
