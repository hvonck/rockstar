# Getting to Rock

To be able to rock, some sort of program needs to be able to parse the lyrics, create instructions and later execute those instructions.

I've been trying to make such a program for the rockstar language in C++. It is the firs time that I have ever done anything like this, so it's not as functional, nor pretty as I want it to be, but it is a lot of fun to make.


Currently quite a few things are supported.

You're able to dynamically create variables of different types: `Null, Boolean, Number and String`.
If a variable has not been initialized yet, it will always be of type `Mysterious`.

You can increment and decrement a Number variable using the `Build {variable} up` and `Knock {variable} down` keywords.

You can assign a value to a variable using `put {value} into {variable}` and `{variable} is {value}`.

You can output the value of a variable using the `say {variable}` syntax.

And you can read in a variable using the `listen to {variable}` syntax.

The basic arithmetic that the language supports is also already supported, and it also automatically orders the operations correctly if you try to chain multiple operations together.
For instance `x / y + z * w` will be executed as `x / y = t1; z * w = t2; t1 + t2 = t3` where t3 is the final result.

The fancy numbering system is also implemented. So, as in the example, this will result in PI.
`This is ice. A life unfulfilled; wakin' everybody up, taking booze and pills`


## What?? Generated Song and Artist names?
Whenever you 'compose' a 'song' (compile a script) a pseudo random song name and artist name will be generated using 100 popular songs and artist from the 1980s. The seed for the generation is based on the length of the file, excluding `\n` characters.

## The example:
The example that is provided with the source code is this:
It goes over all of the different types of operations that you can currently do with my implementation of rockstar.
```
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
```

And the output respectfully is:
```
playing:        Fight the Prayer
by:             Ratt Fixx

336416.125000
0.000000
false
a break
"testing cin with a string"
testing cin with a string
press enter to continue....
```









# Rockstar

Rockstar is a dynamically typed Turing-complete programming language. 

Rockstar is designed for creating computer programs that are also song lyrics, and is heavily influenced by the lyrical conventions of 1980s hard rock and power ballads.

### But why?

Mainly because if we make Rockstar a real (and completely pointless) programming language, then recruiters and hiring managers won't be able to talk about 'rockstar developers' any more.

Also 'cos it's kinda fun and any language based on the idea of compiling Meatloaf lyrics has to be worth a look, right?

Also we can make stickers. Who doesn't want a sticker on their laptop saying 'CERTIFIED ROCKSTAR DEVELOPER'?

## The Rockstar Language Specification

Rockstar is intended to give the programmer an unprecedented degree of poetic license when it comes to the composition and structure of their programs. 

### Comments :heavy_check_mark:
 
The use of comments in Rockstar programs is strongly discouraged. This is rock'n'roll; it's up to the audience to find their own meaning. If you absolutely insist on commenting your Rockstar programs, comments should be contained in parentheses (). Yes, this means you can't use brackets in arithmetic expressions and may need to decompose complex expressions into multiple evaluations and assignments. 
 
Rockstar developers are not into that whole [brevity thing](https://www.urbandictionary.com/define.php?term=Brevity%20Thing). 
 
``` 
Tommy was a lean mean wrecking machine.  (initialises Tommy with the value 14487) 
``` 
 
### Variables :part_alternation_mark:

There's two ways to declare and use variables in Rockstar. 

**Common variables** consist of one of the keywords `a`, `an`, `the`, `my` or `your` followed by a unique variable name, which must contain only lowercase ASCII letters a-z. 

**Proper variables** are proper nouns - any word that isn't a reserved keyword and starts with an uppercase letter. Proper variable names can contain spaces as long as each space is followed by an uppercase letter. Whilst some developers may use this feature to create variables with names like `Customer ID`, `Tax Rate` or `Distance In KM`, we recommend you favour idiomatic variable names such as `Tommy`, `Gina`, `Doctor Feelgood`, `Mister Crowley`, `Kayleigh`, `Tom Sawyer`, `Billie Jean` and `Janie`. 

(Although not strictly idiomatic, `Eleanor Rigby`, `Peggy Sue`, `Black Betty`, `Layla` and `Johnny B Goode` would also all be valid variable names in Rockstar.)

As in Ruby, Python and VBScript, variables are dynamically typed and you don't need to declare variables before use.

**Pronouns**

The keywords `it`, `he`, `she`, `him`, `her`, `them`, `they` always refer to the most recently named variable, determined at parse time. 

### Types :part_alternation_mark:
Note: I haven't implemented objects yet.

Rockstar uses a very similar type system to that defined by the [ECMAScript type system](http://www.ecma-international.org/ecma-262/5.1/#sec-8), except `undefined` doesn't sound very rock'n'roll so we use `mysterious` instead.

* **Mysterious** - the value of any variable that hasn't been assigned a value, denoted by the keyword `mysterious`
* **Null** - the null type. Evaluates as equal to zero and equal to false. The keywords `nothing`, `nowhere` and `nobody` are defined as aliases for `null`
* **Boolean** - a logical entity having two values `true` and `false`. *(The keywords `maybe` and `definitely maybe` are reserved for future use)*
 * `right`, `yes` and `ok` are valid aliases for `true`
 * `wrong`, `no` and `lies` are valid aliases for `false`
* **Number** - Numbers in Rockstar are stored using the [DEC64](http://www.dec64.com/) numeric type.
* **String** - Rockstar strings are sequences of 16-bit unsigned integer values representing UTF-16 code units.
* **Object** - a collection of named data properties, as in ECMAScript.
		

### Literals and Assignment :heavy_check_mark:

String literals in Rockstar use double quotes. 

* `"Hello World"`

The single quote character in Rockstar is treated as a letter of the alphabet. This seems unusual until you remember that `I ain't talkin' 'bout love` is a perfectly valid rock'n'roll sentence. 

Numeric literals in Rockstar are written as decimal numbers

* `123`
* `3.141592654`

Assignment is denoted by the `put/into` keyword combination:

* `Put 123 into X` will assign the value `123` to the variable `X`
* `Put "Hello World" into the message` will assign the value `"Hello World"` to the variable `the message`

#### Increment and Decrement :heavy_check_mark:

Increment and decrement are supported by the `Build {variable} up` and `Knock {variable} down` keywords.

* `Build my world up` will increment the value stored in `my world` by 1.
* `Knock the walls down` will decrement the value stored in `the walls` by 1
 
#### Arithmetic :heavy_check_mark:

Basic arithmetic is provided by the `plus`, `minus`, `times`, `over`, and `by` keywords.

Arithmetic expressions:

* `{a} plus {b}` - addition. Alias `with`
* `{a} minus {b}` - subtraction. Alias `without`
* `{a} times {b}` - multiplication. Alias `of`
* `{a} over {b}` - division. Alias `by`

Examples:

* `Put the whole of your heart into my hands` - multiply `your heart` by `the whole` and assign the result to `my hands`

* `My world is nothing without your love` - Initialize `my world` with the result of subtracting `your love` from 0

* `If the tears of a child is nothing` - check whether `the tears` * `a child` = 0 

* `My love by your eyes` - Returns `my love` divided by `your eyes`

#### Poetic Literals :heavy_check_mark:

Rockstar also supports a unique language feature known as **poetic literals**. Inspired by the [here-document](https://en.wikipedia.org/wiki/Here_document) syntax supported by many scripting languages, poetic literals allow the programmer to simultaneously initialize a variable and express their innermost angst.

##### Poetic Type Literals :heavy_check_mark:

For the keywords `true`, `false`, `nothing`, `nobody` and `nowhere`, a poetic assignment is a single line consisting of a variable name, the `is` keyword and the required value literal

* `My heart is true` - initialises the variable `my heart` with the Boolean value `true` 
* `Tommy is nobody` - initialises the variable `Tommy` with the value `null` using the `nobody` alias

##### Poetic String Literals  :x:

A poetic string literal assignment starts with a variable name, followed by one of the keywords `says` followed by a single space. The rest of the line up to the `\n` terminator is treated as an unquoted string literal.

* `Billy says hello world!\n` will initialise the variable `Billy` with the string literal `"hello world!"`
* `The world says hello back\n` will initialise the variable `the world` with the string literal `hello back`

#### Poetic Number Literals :heavy_check_mark:

A poetic number literal begins with a variable name, followed by the keyword `is`, or the aliases `was` or `were`. As long as the next symbol is not a reserved keyword, the rest of the line is treated as a decimal number in which the values of consecutive digits are given by the lengths of the subsequent barewords, up until the end of the line. To allow the digit zero, and to compensate for a lack of suitably rock'n'roll 1- and 2-letter words, word lengths are parsed modulo 10. A period (.) character denotes a decimal place. Other than the first period, any non-alphabetical characters are ignored.

* `Tommy was a lovestruck ladykiller` initialises `Tommy` with the value `100`
* `Sweet Lucy was a dancer` - initialises `Sweet Lucy` with the value 16
* `A killer is on the loose` - initialises `a killer` with the value 235.
* `My dreams were ice. A life unfulfilled; wakin' everybody up, taking booze and pills` - initialises `my dreams` with the value `3.1415926535`
 * Note that poetic literals **can** include reserved keywords, as with `taking` in this example.

### Comparison :x:

Similar to the single-equals operator in Visual Basic and some scripting languages, the `is` keyword in Rockstar is interepreted differently depending whether it appears as part of a statement or as part of an expression.

Comparison in Rockstar can only be done within an expression.

* `Tommy is nobody` initialises the variable `Tommy` with the value `nobody`
* `If Tommy is nobody` - will execute the following block if, and only if, the variable `Tommy` is equal to `nobody`

The modifier `not`  will invert the meaning of the comparison, similar to `IS NULL / IS NOT NULL` in SQL. The keyword `ain't` is an alias for `is not`. This usage runs contrary to idiomatic English, where "Tommy isn't anybody", "Tommy ain't nobody" and "Tommy ain't not nobody" somehow mean exactly the same thing.

Rockstar also supports the following comparison syntax:

* `is higher/greater/bigger/stronger than` to denote 'greater than'
* `is lower/less/smaller/weaker than` to denote 'less than'
* `is as high/great/big/strong as` to denote 'greater than or equal to'
* `is as low/little/small/weak as` to denote 'less than or equal to'

### Input/Output :heavy_check_mark:

Use the `Listen` keyword to read one line of input from `STDIN`. Use `Listen to` to capture the input into a named variable.

* `Listen to your heart` - read one line of input from `STDIN` and store it in `your heart`

Use the `Say` keyword to write the value of a variable to `SDTOUT`.

* `Say Tommy` - will output the value stored in `Tommy` to `STDOUT`
 
Rockstar defines `Shout`, `Whisper` and `Scream` as aliases for `Say`
 
### Flow Control and Block Syntax

#### Conditionals :x:

Conditional expressions start with the `If` keyword, followed by an expression. If the expression evaluates to `true`, then the subsequent code block is executed. Optionally, an `Else` block can be written after an `If` block. The code block following the `Else` keyword would be executed if the `If` expression evaluated to `false`.

#### Loops :x:

Similar to the `If` statement, a loop is denoted by the `While` or `Until` keyword, which will cause the subsequent code block to be executed repeatedly whilst the expression is satisfied:

```
Tommy was a dancer
While Tommy ain't nothing,
Knock Tommy down
```

That'll initialize Tommy with the value 16 (using the poetic number literal syntax) and then loop, decrementing Tommy by 1 each time until Tommy equals zero (i.e `ain't nothing` returns false).


The `break` and `continue` statements work as they do in most block-based languages. Rockstar defines `Break it down` as an alias for `break` and `Take it to the top` as an alias for `continue` 

#### Blocks :x:

A block in Rockstar starts with an `If`, `Else`, `While` or `Until` statement, and is terminated by a blank line or the end-of-file. EOF ends all open code blocks

```
Tommy was a dancer
While Tommy ain't nothing
Shout it
Knock it down

```

### Functions :x:

Functions are declared with a variable name followed by the `takes` keyword and a list of argument separated by the `and` keyword. 

* `Multiply takes X and Y`
* `Search takes Needle and Haystack`

The function body is a list of statements with no separating blank lines. A blank line denotes the end of a function body. Functions in Rockstar always have a return value, indicated by the `Give back` keyword. 

Functions are called using the 'taking' keyword:

* `Multiply taking 3, 5` is an expression returning (presumably) 15
* `Search taking "hands", "lay your hands on me"`

## Examples

Here's FizzBuzz in minimalist Rockstar, with block scope indented for clarity:

```
Modulus takes Number and Divisor
While Number is as high as Divisor
Put Number minus Divisor into Number
    (blank line ending While block)
Give back Number
    (blank line ending function declaration)
Limit is 100
Counter is 0
Fizz is 3
Buzz is 5
Until Counter is Limit
Build Counter up
If Modulus taking Counter, Fizz is 0 and Modulus taking Counter, Buzz is 0
Say "FizzBuzz!"
Continue
    (blank line ending 'If' Block)
If Modulus taking Counter and Fizz is 0
Say "Fizz!"
Continue
    (blank line ending 'If' Block)	
If Modulus taking Counter and Buzz is 0
Say "Buzz!"
Continue
    (blank line ending 'If' Block)
Say Counter
    (EOL ending Until block)
```

And here's the same thing in idiomatic Rockstar, using poetic literals and no indentation

```
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
```

## Ideas

* Work out if this is even remotely implementable. I'm not sold on the idea of continuation prefixes for block syntax - for starters it won't let you implement nested blocks.
* Explore other ideas for Turing-complete rock ballad compilers. Maybe something based on BF where we use word length or initial letters or something to compile lyrics down to BF or some other very minimalist but Turing-complete language
* Make 'Certified Rockstar Developer' stickers and give them out to anybody who can write even one line of Rockstar.
* Generate a score for the lyrics using a component called a `composer`.

# Implementations
* [rockstar-js](https://github.com/wolfgang42/rockstar-js) - Rockstar-to-JavaScript transpiler
* [rockstar-lexer](https://github.com/aitorres/rockstar-lexer) - Rockstar lexer written in Haskell with Alex
* [rockstar-java](https://github.com/nbrevu/rockstar-java) - Rockstar interpreter in Java
