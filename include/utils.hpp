#ifndef TEMPEST_UTILS_HPP
#define TEMPEST_UTILS_HPP

#include <string>
#include <iostream>

namespace tempest {

// A simple utility class to demonstrate the include structure
class Utils {
public:
    // Print a message to the console
    static void printMessage(const std::string& message) {
        std::cout << "TEMPEST: " << message << std::endl;
    }
    
    // C++11 constexpr function
    static constexpr int getVersion() {
        return 1;
    }
};

} // namespace tempest

#endif // TEMPEST_UTILS_HPP
