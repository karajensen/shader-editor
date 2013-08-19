////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - results.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace SceneryTests
{
    std::ofstream results; ///< stream to output results

    /**
    * Helps redirect results to a file
    */
    struct ResultOutput
    {
        /**
        * Constructor
        */
        ResultOutput()
        {
            results.open("results.txt");
            assert(results.is_open());
            boost::unit_test::results_reporter::set_stream(results);
        }
    };
    BOOST_GLOBAL_FIXTURE(ResultOutput)
}