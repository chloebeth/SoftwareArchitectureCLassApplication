How to run the project on Windows:
<br>
Method 1: 
<br>
Use command to open
1. Syntax:
  File.OpenProject filename
2. Arguments
  The full path and file name of the project or solution to open is required.
3. Remarks
  Auto-completion tries to locate the correct path and file name as you type.
4. Example

>File.OpenProject "C:\My Projects\Test1\Test1.vbproj"
  
How to run the project on Unix: 
<br>
Method 1:
1. Select the Alt-F2 to open the Run Command box;
2. Type the name of the program you want to run

How to run Windows program in Unix:
<br>
Method 1:
1. Install Wine on Ubuntu Linux.
2. Configure Wine on Ubuntu Linux.
3. Install Windows applications on Ubuntu Linux.
4. Set up Wine as the default program to load Windows applications.
5. Install Winetricks.
6. Use Winetricks to install Windows programs.

Method 2:
1. Design the project in windows system and save it as a vcx project file;
2. Use CMake to package the vcx project file;
3. Transform the vcx project file into an appropriate file format in Unix system using the cmake converter. 
Execution flow: vcx project file -> cmake -> unix project file
