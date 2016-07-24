// author: jgoenetxea@vicomtech.org

#define CATCH_CONFIG_MAIN   // This definition generates a standard main function in compilation time, so no main function is needed.
#include <catch.hpp>		// This is the testing framework. No other library or dependencies are needed.
#include <testUtils.hpp>	// This is a custom utility set, to help with OpendCV matrix testing and more.

//------------------------------------------------------------------------
//--------------- test Object registration and Factory functionality --------------------------
//
SCENARIO("A test scenario to see the structure of a testing file")
{
    GIVEN("A example variable")
	{
		// This block values is mantained inside the 'THEN' blocks, so all the blocks will have the same initial values for those elements
		int i = 0; // <-- example variable
		THEN("The variable value can be changed")
		{
		    // You can change the value of any of the variables inside the block
		    i = 1;
		    REQUIRE( i == 1 );
		}

		THEN("The variable value is again equal to the value in the initialization")
		{
		    CHECK( i == 0 );  // OK             // <-- The 'CHECK' macro checks the expression but does not stop the testing block sequence (the 'THEN' block)
		    REQUIRE( i == 1 ); // Should fail  // <-- The 'REQUIRED' macro checks the expression AND breacks the testing block (the 'THEN' block)
		    REQUIRE( 23 == 42 ); // If previous 'REQUIRED' fails this case is not checked
		}
	}
}


