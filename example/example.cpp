#include <arba/rand/rand.hpp>
#include <arba/rand/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << rand::rand_i64() << std::endl;
    std::cout << "EXAMPLE SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
