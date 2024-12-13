#include <arba/rand/rand.hpp>

inline namespace arba
{
namespace rand
{
namespace private_
{

rand_int_engine_type_& rand_int_engine_()
{
    static thread_local rand_int_engine_type_ instance( std::random_device{}() );
    return instance;
}

}
}
}
