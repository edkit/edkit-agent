#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char **argv)
{
   CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
   CppUnit::TextUi::TestRunner runner;

   runner.addTest( suite );

   runner.run();
   return 0;
}
