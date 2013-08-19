////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scenerytests.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cassert>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/results_reporter.hpp>

#include "results.h"
#include "vector3.h"
#include "matrix.h"

namespace SceneryTests
{
    BOOST_AUTO_TEST_CASE(vector_tests)
    {
        Float3 vec1(1.0f, 2.0f, 3.0f);
        Float3 vec2(4.0f, 5.0f, 6.0f);  
        Float3 vec3(0.0f, 3.0f, 4.0f);

        const float vec3Length = 25.0f;
        Float3 vec1Normalised = vec1;
        vec1Normalised.Normalize();

        BOOST_CHECK(vec1 != vec2);
        BOOST_CHECK(vec1 == vec1);
        BOOST_CHECK(vec3Length == vec3.Length());
        BOOST_CHECK(vec1.GetNormalized().Length() == 1.0f);
        BOOST_CHECK(vec1Normalised == vec1.GetNormalized());
        BOOST_CHECK(vec1Normalised.Length() == 1.0f);
    }

    BOOST_AUTO_TEST_CASE(matrix_tests)
    {


    }

    BOOST_AUTO_TEST_CASE(shader_tests)
    {






    }
}