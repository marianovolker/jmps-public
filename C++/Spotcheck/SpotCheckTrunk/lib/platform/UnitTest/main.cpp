#include <gtest/gtest.h>

int main(int argc, char **argv)
{
   // Initialize the testing framework
   testing::InitGoogleTest(&argc, argv);

   // Run the tests
   return RUN_ALL_TESTS();
}
