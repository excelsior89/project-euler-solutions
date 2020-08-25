# project-euler-solutions
Solutions to a selection of the problems in [Project Euler](https://projecteuler.net/).

The intention behind this repository is primarily to have a cloud backup of my personal solutions and to provide an easy way to share my solutions with other interested parties. Please bear in mind that you should refrain from browsing the solutions to any problems you wish to solve yourself (as mentioned on the Project Euler site).

## Dependencies
This project is developed in C++ using [Visual Studio 2017](https://visualstudio.microsoft.com/vs/older-downloads/). I've tried to avoid using any third party libraries so far, so the code should be able to build using any C++ compiler that supports C++14.

I've used [CMake](https://cmake.org/) to generate and build the solution files. The minimum version of CMake is set to 3.15 (the version I had installed when I created the files), it might work with earlier versions, although I'm reasonably sure you'll need at least 3.8.

## Building
Once you have CMake installed on your system path and have Visual Studio 2017, building should be as simple as running the build script "build_windows.cmd". This should build everything under a directory called "build_windows", and also place a copy of the compiled executable in a "bin" directory. I've included "build_mac.sh" (XCode) and "build_linux.sh" (Make) as examples for building on other systems, however these are totally untested.

## Running
The project builds as a command line application and I've tried to make it self-explanatory. When prompted, enter the problem number you're interested in. You can then choose to just run the problem, or do profiling. You can run multiple problems – the application will keep prompting for a problem number until you enter 0 to exit.

Just running the problem displays the problem statement (from the Project Euler website), the solution and an explanation of the method, or methods, that have been implemented to solve the problem. 

The profiling system is fairly simplistic: the general problem is run repeatedly with a sample input (typically the same as the specific case in the problem statement, but sometimes different). These runs are timed and an average time-per-run is determined. Where multiple solution methods have been implemented for the problem, each method is profiled separately.

### Example
```
Pick a problem (1-700, enter 0 to exit): 1
Do profiling? Y/N: N
Profiling will not be used.

-------- Problem 1 --------

If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.

-------- Solution 1 --------
<etc.>
```
