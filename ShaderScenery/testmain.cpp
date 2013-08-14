#include "vector3.h"

#define BOOST_TEST_MODULE ShaderSceneryTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(shader_scenery_test)
{
    //Test the vector3 library
    Float3 vec;
    BOOST_CHECK(vec.x == 2.0f && vec.y == 0.0f && vec.z == 0.0f);
}