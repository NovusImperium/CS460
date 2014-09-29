#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    std::ifstream in("");

    if (in.is_open()) {
        std::cout << "wat\n";
    }

    std::cout << in.peek() << std::endl;
    return 0;
}