// I spent way too much time on this 
// And it is useless
// But please enjoy

#include <cstring>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <sstream>

// https://cplusplus.com/doc/tutorial/preprocessor/
// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
// TBH I don't think this is the best approach, it's better to define in the build process
// But this is not meant to be serious
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
    #define PLATFORM_UNIX
    #include <unistd.h>
#else
    #error "What are you even using? TempleOS?"
#endif

#ifdef PLATFORM_WINDOWS
// Windows my enemy
enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    BRIGHT_GREEN = 10,
    BRIGHT_CYAN = 11,
    BRIGHT_RED = 12,
    BRIGHT_YELLOW = 14
};

void setColor(ConsoleColor color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#elif defined(PLATFORM_UNIX)
// ANSI
const char* BLACK = "\033[0;30m";
const char* RED = "\033[0;31m";
const char* GREEN = "\033[0;32m";
const char* YELLOW = "\033[0;33m";
const char* BLUE = "\033[0;34m";
const char* MAGENTA = "\033[0;35m";
const char* CYAN = "\033[0;36m";
const char* WHITE = "\033[0;37m";
const char* BRIGHT_RED = "\033[1;31m";
const char* BRIGHT_GREEN = "\033[1;32m";
const char* BRIGHT_YELLOW = "\033[1;33m";
const char* BRIGHT_CYAN = "\033[1;36m";

void setColor(const char* colorCode) {
    write(STDOUT_FILENO, colorCode, strlen(colorCode));
}
#endif

// Moved due to compiler issue
void resetColor() {
#ifdef PLATFORM_WINDOWS
    setColor(WHITE);
#elif defined(PLATFORM_UNIX)
    write(STDOUT_FILENO, "\033[0m", 4);
#endif
}

void print(const char *str, const char* color = nullptr) {
#ifdef PLATFORM_WINDOWS
    if (color != nullptr) {
        setColor(static_cast<ConsoleColor>(std::stoi(color)));
    }
    DWORD written;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteFile(hConsole, str, strlen(str), &written, NULL);
    resetColor();  
#elif defined(PLATFORM_UNIX)
    if (color != nullptr) {
        setColor(color); 
    }
    write(STDOUT_FILENO, str, strlen(str));
    resetColor();
#endif
}

// Using stream for these to keep it more consistent
// Since I need it for some but for others it technically could be skipped
void print(int num, const char* color = nullptr) {
    std::ostringstream stream;
    stream << num;
    print(stream.str().c_str(), color);
}

void print(float num, int precision = 2, const char* color = nullptr) {
    std::ostringstream stream;
    stream.precision(precision);
    stream << std::fixed << num;
    print(stream.str().c_str(), color);
}

template <typename T>
void print(T arr[], size_t size, const char* color = nullptr) {
    std::ostringstream stream;
    stream << "[ ";
    for (size_t i = 0; i < size; ++i) {
        stream << arr[i];
        if (i < size - 1) {
            stream << ", ";
        }
    }
    stream << " ]\n";
    print(stream.str().c_str(), color);
}

int main() {
    print("Hello, World!... in a stupid, stupid way.\n");
    print("And in color, if you want that.\n", BRIGHT_GREEN);
    print("It can print integers too.\n");

    print(12345, BRIGHT_YELLOW);
    print("\n");

    print("And floats.\n", BRIGHT_CYAN);
    print(3.14159f); 

    print("I don't like iterating over arrays, because I am lazy, so it can also do that.\n ");

    int intArray[] = {1, 2, 3, 4, 5};
    float floatArray[] = {3.14f, 2.71f, 1.41f};

    print(intArray, 5, BRIGHT_RED);
    print(floatArray, 3, BRIGHT_GREEN);

    print("But that's more than five messages :p\n", BRIGHT_YELLOW);
    
    return 0;
}
