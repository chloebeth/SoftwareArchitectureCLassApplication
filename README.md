# SoftwareArchitectureCLassApplication

## Task 2: Build on Linux and Windows

### Introduction
Final project for Software Architecture. The goal is to make the application cross-compatible on Unix and Windows. 

### Requirements:


    (REQUIRED) Run on Unix and Windows (build and compile)
    (REQUIRED) Implement 2 unit tests that use Googletest, and ensure they can run on windows as well as Unix
    (DESIRED) Do not lose functionality for the other tasks 
    (DESIRED) Get successful header file exports
    (DESIRED) Be available to build and debug on Linux as well on Windows using project configuration files (cmake, sln, vcxproj, etc)
    (DESIRED) It is still necessary to use Windows to run the product.



[Design Document](DesignDocument.md)

[Unit Tests for AutomationBinding API](AutomationAPI-Test)

### Implementation
[CMakeSettings.json (Proof of the Linux-Debug and x64-Debug configurations)](SoftwareArchitectureCLassApplication\packages\boost.1.78.0\lib\native\include\boost\safe_numerics\CMakeSettings.json)
#### Unix
To run on a Unix machine, select the "Linux-Debug" configuration and then follow the prompts to remote into a Unix machine.

#### Windows
To run on a Windows machine, select the "x64-Debug" configuration and then run.

### Conclusions
Our group successfully completed all requirements. The program successfully builds and compiles on Unix and Windows and the Unit Tests run on Windows and Unix. We give our permission for you to add our work to the GitHub project. A potential extension for the functionality is to implement the desired requirements. Specifically the header file exports.
