
#include <new>
#include <stdio.h>
#include <stdlib.h>

class Foo
{
   public:
      Foo(void) {Bar = 10; };
      virtual ~Foo(void) {};
      int Bar;
};

int main(int argc, char *argv[])
{
   Foo *p_Data = new Foo;
   if(p_Data == NULL)
   {
      perror("new returns NULL\n");
      return(-1);
   }

   if(p_Data->Bar != 10)
   {
      perror("bad value\n");
      return(-1);
   }
   delete p_Data;
   return(0);
}
