# greenbot
[![Coverity Scan Build Status](https://scan.coverity.com/projects/5556/badge.svg)](https://scan.coverity.com/projects/5556)

A configurable chat based bot written in C++.

# Building
Prerequisites:
- A C++11-compliant compiler (It's 2015, people)
- V8 (the javascript engine) for the JS module

```
./configure
make
./greenbot
```
# Adding modules
All that is required for a module is a header file in `modules/inc` that contains a class that implements the `Callback` class (defined in [`src/inc/bot.h`](https://github.com/GreenBagels/greenbot/blob/master/src/inc/bot.h#L7-L20)).  The implementation of the module can go in `modules/` as a `.cpp` file and it will be automatically compiled.  The `configure` script will scan the header files in `modules/inc` to generate a new header file `src/inc/modules.h`, which will allow the `src/core.cpp` to call the function `LoadModules`.  **This means you do not need to modify any existing code to add a module, simply drop in the new files.**

In order for the files to be recognized they will need a header of the form
```
//MODULE:[ClassName]
//LIBS:-l[libname1] -l[libname2]
```
where `[ClassName]` is the name of the class that implements the `Callback` class and `[libname]` are any library dependencies the module has.  If there are no library dependencies, you may omit the `//LIBS` line.  Check out the modules currently added for examples.

# Contact
If you need to directly contact someone you can join #greenbot on irc.rizon.net (someone is almost always there)

#TODO
- Change modules to based on a shared library scheme
- Add logging functionality
