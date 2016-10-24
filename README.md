# Swarmongers Repository

Swarmongers is a retro vector graphics 2D video-game I developed as a demo in 2000 at Magic Lantern Playware using DirectX and base code from Andre Lamothe's tutorial book *Windows Game Programming for Dummies*.  It is similar to Asteroids, but with a (rudimentary) story and levels geared towards defeating alien ship opponents.

## Setting Up and Playing the Game

After cloning the repo, you run the game by double-clicking `Swarmongers.exe` and clicking the Run button when the dialog box comes up.  This executable has been verified to run on Windows 7 Professional on a 64-bit Intel i3-4005U machine.

## Repo Contents

* `Docs\` game design and development notes
* `Music\` music files used in the game (courtesy of Duncan McPherson (one of my coworkers at Magic Lantern), Franz Liszt, and Dmitri Shostakovich)
* `New Sounds\` sound files used in the game (developed by Duncan)
* `src\` source code for the game (including my code and Andre Lamothe's GPDUMB engine)
* `README.md` this file
* `DPAINT.PAL` palette file for the splash screen
* `Swarmongers 1C.bmp` splash screen bitmap
* `Swarmongers.exe` the executable file

## Building an Executable in Microsoft Visual Studio

The version of the code I had [here](http://gchadder3.com/SoftwareDev/Swarmongers.zip) was built using Visual C++ 2008 Express Edition.

Currently the builds do not work reliably under Visual Studio Community 15, so the next stage would be 
to get this working.

-- George Chadderdon, 10/24/2016