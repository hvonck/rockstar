#include "interpreter.h"

#define FIBONACCI
int main(int argc, char** argv)
{
  lambda::Interpreter interpreter;

  std::string old;
#ifdef MAIN_EXAMPLE
    old = R"(
put 0 into A
put 10 into B
until A is B
  build A up
  if A is not B
  say A
                                              (END_SCOPE_IF)
  else if A is B
    say "Equal!"
                                              (END_SCOPE_ELSE_IF)
                                              (END_SCOPE_WHILE)
Alpha takes Beta, Gamma
  say Beta
  say Gamma
  say "returning gamma"
  give back Gamma
                                              (END_SCOPE_ALPHA)

Number takes
  Ret is 10
  give back Ret
                                              (END_SCOPE_NUMBER)
Dennis is 11.11
put "twenty one" into Sander
say Dennis
shout Sander
if Dennis is greater than Sander
  say "hello there"
                                              (END_SCOPE_IF)
else
  say "Or not.."
                                              (END_SCOPE_ELSE)
)";
#elif defined MY_EXAMPLE
    old = R"(
put a lot into Timmy
Tommy is nothing

while Tommy is as weak as Timmy
  build Tommy up
  if Tommy is not Timmy
    say Tommy

  else if Tommy is Timmy
    shout "they are equal!"


put all the things that I desire. I want and I require. into The Raging Fire  (create a variable, and assign a number to it generated from a string)
My Last Hope is The Raging Fire                                               (create a variable, and store a previously created variable into it)
build My Hope up                                                              (increment a number variable)
knock My Hope down                                                            (decrement a number variable)
Trust is what's required. No matter when, no matter how.                      (create a variabbe, and store a number to it generated from a string, like in the first line)
A Soothing Sound is My Last Hope over Trust with My Hope of nobody            (arithmetic using proper ordering. it says a/b+c*d which will be executed as a/b=t1, c*d=t2, t1+t2=t3)
put lies into These Thoughts                                                  (assigning a boolean to a newly created variable)
What's Required is "a break"                                                  (assigning a string to a newly created variable)
whisper My Last Hope                                                          (this will output one of the values so you can see the proper result, number)
shout A Soothing Sound                                                        (this will output one of the values so you can see the proper result, number)
scream These Thoughts                                                         (this will output one of the values so you can see the proper result, boolean)
say What's Required                                                           (this will output one of the values so you can see the proper result, string)
listen to These Words                                                         (read a line of input. Will be evaluated the same way that put and is evaluate their variables. Without the possibility of maths operators or variables since it is done at 'runtime', not 'compile time')
say These Words                                                               (this will output the variable that got created in the previous line)
(please ignore these next few lines. It's just to keep the window from closing.)
Enter is "press enter to continue..."
say Enter
listen to Temporary Variables
)";
#elif defined FIZZ_BUZZ
    old = R"(
(FizzBuzz example)
Midnight takes Your Heart and Your Soul
  While Your Heart is as high as Your Soul
    Put Your Heart without Your Soul into Your Heart
  (EOS)
  Give back Your Heart
(EOS)

Desire is a lovestruck ladykiller
My World is nothing 
Fire is ice
Hate is water
Until My World is Desire
  Build My World up
  If Midnight taking My World, Fire is nothing and Midnight taking My World, Hate is nothing
    Shout "FizzBuzz!"
    Take it to the top
  (EOS)
  If Midnight taking My World, Fire is nothing
    Shout "Fizz!"
    Take it to the top
  (EOS)
  If Midnight taking My World, Hate is nothing
    Say "Buzz!"
    Take it to the top
  (EOS)
  Whisper My World
(EOS)
)";
#elif defined FIBONACCI
    old = R"(
Tommy was a kindle
Jane was gasoline
Put Tommy over Jane into the fire

Put the fire into the daydream
Knock the daydream down

Away takes Time
If Time is nothing
Give back Time

The nightmare was over
Build the nightmare up
If Time is the nightmare
Give back Time

Put Time without the nightmare into my world
Put Time without the fire into Yours
Put Away taking my world into the daylight
Put Away taking Yours into the night
Give back the daylight with the night

Put the daydream into my love
Ever is so rock

Until my love is stronger than Ever
Put Away taking my love into the night
Whisper the night
Build my love up
)";
#endif
    lambda::ScopeManager scope_manager = interpreter.interpret(old.c_str());
    scope_manager.exec();
    scope_manager.free();

  return 0;
}