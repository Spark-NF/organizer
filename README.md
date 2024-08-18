<p align="center"><img src="src/gui/resources/images/readme.png" alt="" /></p>

<h1 align="center">Organizer</h1>

[![GitHub release](https://img.shields.io/github/release/Spark-NF/organizer.svg)](https://github.com/Spark-NF/organizer/releases/latest)
[![GitHub downloads (latest)](https://img.shields.io/github/downloads/Spark-NF/organizer/latest/total.svg)](https://github.com/Spark-NF/organizer/releases/latest)
[![GitHub downloads (total)](https://img.shields.io/github/downloads/Spark-NF/organizer/total.svg)](https://github.com/Spark-NF/organizer/releases)
[![GitHub issues](https://img.shields.io/github/issues/Spark-NF/organizer.svg)](https://github.com/Spark-NF/organizer/issues)
[![Build status](https://img.shields.io/github/actions/workflow/status/Spark-NF/organizer/build.yml)](https://github.com/Spark-NF/organizer/actions)
[![Code Coverage](https://img.shields.io/codecov/c/github/Spark-NF/organizer.svg)](https://codecov.io/gh/Spark-NF/organizer)
[![Project license](https://img.shields.io/github/license/Spark-NF/organizer.svg)](https://raw.githubusercontent.com/Spark-NF/organizer/develop/LICENSE)

## About
Organizer allows to automatically or manually apply actions on your files. It can be configured to perform actions automatically given the file's name, size, etc. to allow for automated sorting of many files at once.

For example, it can be used to sort a vacation's pictures and videos and put them in separate folders according to their location, the context, etc. or to move old unused downloaded files to the trash automatically.

It is composed of a few projects:

* A command-line interface (CLI), which takes a configuration file and a list of files, and will apply actions matching conditions on your files automatically.
* A drop window that does the same thing as the CLI, but with a user interface.
* A manual sorting interface, which allows to load a bunch of media files, show/play them one-by-one, and allowing the user to do custom actions on them on the press of a button.

### Download
You can download the latest release [here](https://github.com/Spark-NF/organizer/releases/latest), or find all previous releases [here](https://github.com/Spark-NF/organizer/releases).

For users interested, a nightly version is built automatically on every commit, and can be downloaded [here](https://github.com/Spark-NF/organizer/releases/nightly). Note that it might be less stable than official releases, so use at your own risk.

### Screenshots

![Main](src/gui/resources/images/screenshots/main.png)

### Languages
* English

### Authors
* Nicolas Faure ([Spark-NF](https://github.com/Spark-NF))

### License
The script is licensed under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

## Building
Note: you need to have CMake, a C++ compiler, and the Qt libraries installed to build the project.

```
mkdir build
cd build
cmake ../src
make -j 8
```

You'll find the built binaries in the `build/` directory, named `Organizer`.
