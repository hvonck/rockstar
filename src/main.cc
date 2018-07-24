#include "composer.h"

int main(int argc, char** argv)
{
  String lyrics = R"(
put all the things that I desire. I want and I require. into the raging fire  (create a variable, and assign a number to it generated from a string)
my last hope is the raging fire                                               (create a variable, and store a previously created variable into it)
build my hope up                                                              (increment a number variable)
knock my hope down                                                            (decrement a number variable)
trust is what's required. No matter when, no matter how.                      (create a variabbe, and store a number to it generated from a string, like in the first line)
a soothing sound is nothing over trust with my hope of nobody                 (arithmetic using proper ordering. it says a/b+c*d which will be executed as a/b=t1, c*d=t2, t1+t2=t3)
put lies into these thoughts                                                  (assigning a boolean to a newly created variable)
what's required is "a break"                                                  (assigning a string to a newly created variable)
whisper my last hope                                                          (this will output one of the values so you can see the proper result, number)
shout a soothing sound                                                        (this will output one of the values so you can see the proper result, number)
scream these thoughts                                                         (this will output one of the values so you can see the proper result, boolean)
say what's required                                                           (this will output one of the values so you can see the proper result, string)
listen to these words                                                         (read a line of input. Will be evaluated the same way that put and is evaluate their variables. Without the possibility of maths operators or variables since it is done at 'runtime', not 'compile time')
say these words                                                               (this will output the variable that got created in the previous line)
(please ignore these next few lines. It's just to keep the window from closing.)
enter is "press enter to continue...."
say enter
listen to temporary variables
)";

  lambda::Composer parser;
  lambda::Song song = parser.compose(lyrics);

  song.play();

  return 0;
}