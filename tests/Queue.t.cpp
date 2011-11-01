#include <tut/tut.hpp>

#include "Queue.hpp"

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Queue");
} // unnamed namespace


namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{
}

} // namespace tut
