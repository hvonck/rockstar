#include "composer.h"

//#define ROCK_OLD_EXAMPLE

int main(int argc, char** argv)
{
#ifdef ROCK_OLD_EXAMPLE
  String lyrics = R"(
put a lot into timmy
tommy is nothing

while tommy is as weak as timmy
build tommy up
if tommy is not timmy
say tommy
else if tommy is timmy
shout "they are equal!"


put all the things that I desire. I want and I require. into the raging fire  (create a variable, and assign a number to it generated from a string)
my last hope is the raging fire                                               (create a variable, and store a previously created variable into it)
build my hope up                                                              (increment a number variable)
knock my hope down                                                            (decrement a number variable)
trust is what's required. No matter when, no matter how.                      (create a variabbe, and store a number to it generated from a string, like in the first line)
a soothing sound is my last hope over trust with my hope of nobody            (arithmetic using proper ordering. it says a/b+c*d which will be executed as a/b=t1, c*d=t2, t1+t2=t3)
put lies into these thoughts                                                  (assigning a boolean to a newly created variable)
what's required is "a break"                                                  (assigning a string to a newly created variable)
whisper my last hope                                                          (this will output one of the values so you can see the proper result, number)
shout a soothing sound                                                        (this will output one of the values so you can see the proper result, number)
scream these thoughts                                                         (this will output one of the values so you can see the proper result, boolean)
say what's required                                                           (this will output one of the values so you can see the proper result, string)
listen to these words                                                         (read a line of input. Will be evaluated the same way that put and is evaluate their variables. Without the possibility of maths operators or variables since it is done at 'runtime', not 'compile time')
say these words                                                               (this will output the variable that got created in the previous line)
(please ignore these next few lines. It's just to keep the window from closing.)
enter is "press enter to continue..."
say enter
listen to temporary variables
)";
#else
  String lyrics = R"(
(FizzBuzz example)
Midnight takes your heart and your soul
While your heart is as high as your soul
Put your heart without your soul into your heart

Give back your heart


Desire is a lovestruck ladykiller
My world is nothing 
Fire is ice
Hate is water
Until my world is Desire,
Build my world up
If Midnight taking my world, Fire is nothing and Midnight taking my world, Hate is nothing
Shout "FizzBuzz!"
Take it to the top

If Midnight taking my world, Fire is nothing
Shout "Fizz!"
Take it to the top

If Midnight taking my world, Hate is nothing
Say "Buzz!"
Take it to the top

Whisper my world
)";
#endif

  lambda::Composer parser;
  lambda::Song song = parser.compose(lyrics);

  song.play();

  return 0;
}