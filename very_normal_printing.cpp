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
#elif defined(__unix__) || defined(__APPLE__) // Excluded some possible identifiers (__unix, unix, __MACH__)
    #define PLATFORM_UNIX
    #include <unistd.h>
#else
    #error "What are you even using? TempleOS?"
#endif

void print(const char *str) {
#ifdef PLATFORM_WINDOWS
    // test
    DWORD written; // https://stackoverflow.com/questions/2995251/why-in-c-do-we-use-dword-rather-than-unsigned-int - Windows being Windows moment
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteFile(hConsole, str, strlen(str), &written, NULL); // From windows.h : https://stackoverflow.com/questions/9174947/writefile-function-c
#elif defined(PLATFORM_UNIX)
    write(STDOUT_FILENO, str, strlen(str)); // From unistd.h : https://pubs.opengroup.org/onlinepubs/007908799/xsh/write.html
#endif
}

// Also probably a better way to do this?
void print(int num) {
    std::ostringstream stream;
    stream << num;
    print(stream.str().c_str());
}

void print(float num, int precision = 2) {
    std::ostringstream stream;
    stream.precision(precision);
    stream << std::fixed << num;
    print(stream.str().c_str());
}

template <typename T>
void print(T arr[], size_t size) {
    std::ostringstream stream;
    stream << "[ ";
    for (size_t i = 0; i < size; ++i) {
        stream << arr[i];
        if (i < size - 1) {
            stream << ", ";
        }
    }
    stream << " ]\n";
    print(stream.str().c_str());
}

int main() {
    print("Hello, World!... in a weird way.\n");
    print("It can print integers too.\n");

    print(12345);
    print("\n");
    print(-67890);
    print("\n");

    print("And floats.\n");
    print(3.14159f); 
    print("\n");
    print(2.71828f, 5);
    print("\n");

    print("I don't like iterating over arrays, because I am lazy, so it can also do that.\n ");

    int intArray[] = {1, 2, 3, 4, 5};
    float floatArray[] = {3.14f, 2.71f, 1.41f};

    print(intArray, 5);
    print(floatArray, 3);
    
    return 0;
}
