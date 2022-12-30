# The `src` directory

This directory contains all the actual source code for Organizer, to be built using [CMake](https://cmake.org/). Each sub-directory contains a different project.

Main projects:
* `lib`: the library containing Organizer's logic
* `gui`: the GUI code for the main Organizer executable

Other directories:
* `dist`: files to be packaged together with the binaries
* `vendor`: submodules Organizer depends on, such as Catch2 for testing