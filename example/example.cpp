#include <arba/rand/rand.hpp>
#include <arba/rand/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << arba::rand::project_name() << std::endl;
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
