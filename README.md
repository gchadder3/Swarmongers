==========================================
   README  George Chadderdon, 5/22/2009
==========================================

[This file needs to be updated, 10/22/2016]

CONTENTS AND DIRECTIONS FOR COMPILING

This folder contains C++ source and executable code for 
a retro 2D video-game developed using DirectX.  The 
entire directory should work as a project directory 
under the Visual C++ 2008 Express Edition.  Note that
to compile and link the code, you need to have the 
DirectX SDK installed.  (I used the March 2009 version.)
In case of linking problems, know that the following 
libs need to be included in the build:
    ddraw.lib
    dinput8.lib
    dsound.lib
    winmm.lib

Docs/ contains (profuse) development notes.
Debug/ contains the Debug executable version of the game.
Release/ contains the Release version of the game.
Swarmongers/ contains the source code.
Swarmongers.sln is the solution file.

SETTING UP AND PLAYING THE GAME

For those interested only in playing the game, the 
folders and files in Release/ could be moved into a 
folder elsewhere named Swarmongers/ and the game may 
be run by double-clicking Swarmongers.exe.

ACKNOWLEDGEMENTS

Special thanks go to Andre Lamothe and his wonderful 
tutorial book "Windows Game Programming for Dummies" 
from which wrappers for the DirectX routines were taken, 
and Duncan McPherson for the sounds and music.
