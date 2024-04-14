## Basic parameters for starting a raylib project in vscode on windows

## Installation (with MSYS2)
- Go through setup process to get mingw compiler working with VSCode
- Search for raylib package in mingw repository `pacman -Ss raylib`
- Copy package name that matches g++ version (default is ucrt64)
- Install package with `pacman -S {...}`
- (FYI: `-lraylib` flag was added to tasks.json manually)

## Release
- Run `./release.bat`
- Note: needs to have raylib sources files downloaded to default location (C:/raylib/raylib/src)
- Can get raylib source code from https://www.raylib.com/
