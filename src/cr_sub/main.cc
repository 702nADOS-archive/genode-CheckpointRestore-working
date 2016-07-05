#include <base/printf.h>
#include <timer_session/connection.h>

int fib(int n)
{
  int first = 0;
  int second = 1;
  int result;

  if (n == 1)
  {
    result = 0;
  }
  else if (n == 2)
  {
    result = 1;
  }
  else
  {
    for (int i = 2; i < n; n++)
    {
      result = first + second;
      first = second;
      second = result;
      Genode::printf("INTERMEDIATE RESULT: %i\n", result);
    }
    
    return result;
  }
}

void test_print()
{
  while(1)
  {
    Genode::printf("TEST");
  }
}

int main(int argc, char const *argv[]) {
  Genode::printf("Hello world from cr_sub\n");
  Genode::printf("it actually works\n");

//  Timer::Connection timer;
//  int counter = 0;

  // int fib_result = fib(10);
  // Genode::printf("Fib(10): %i\n", fib_result);

  test_print();

//  while(1)
//  {
//
//    Genode::printf("Hey i am a child at: %i\n", counter);
//    counter++;
//    timer.msleep(200);
//  }
  return 0;
}
