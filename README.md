<p align="center"><img src="resources/images/readme.png" alt="" /></p>

<h1 align="center">Organizer</h1>

[![GitHub release](https://img.shields.io/github/release/Spark-NF/organizer.svg)](https://github.com/Spark-NF/organizer/releases/latest)
[![GitHub downloads](https://img.shields.io/github/downloads/Spark-NF/organizer/latest/total.svg)](https://github.com/Spark-NF/organizer/releases/latest)
[![GitHub downloads](https://img.shields.io/github/downloads/Spark-NF/organizer/total.svg)](https://github.com/Spark-NF/organizer/releases)
[![GitHub issues](https://img.shields.io/github/issues/Spark-NF/organizer.svg)](https://github.com/Spark-NF/organizer/issues)
[![Build Status](https://travis-ci.org/Spark-NF/organizer.svg?branch=master)](https://travis-ci.org/Spark-NF/organizer)
[![Build Status](https://ci.appveyor.com/api/projects/status/lm08r4q0kuui7a5y/branch/master?svg=true)](https://ci.appveyor.com/project/Spark-NF/organizer)
[![Project license](https://img.shields.io/github/license/Spark-NF/organizer.svg)](https://raw.githubusercontent.com/Spark-NF/organizer/develop/LICENSE)

## About
Organizer allows to load a bunch of media files, show/play them, and allowing the user to do custom actions on them on the press of a button.

For example, it can be used to sort a vacation's pictures and videos and put them in separate folders according to their location, the context, etc. The images/videos will be shown one by one, then the user can use shortcuts to move them quickly to separate directories.

### Download
You can download the latest release [here](https://github.com/Spark-NF/organizer/releases/latest), or find all previous releases [here](https://github.com/Spark-NF/organizer/releases).

For users interested, a nightly version is built automatically on every commit, and can be downloaded [here](https://github.com/Spark-NF/organizer/releases/nightly). Note that it might be less stable than official releases, so use at your own risk.

### Screenshots

![Main](resources/images/screenshots/main.png)

### Actions file
You have to create a `actions.json` file to put in the same folder as the executable file.

It is a JSON file that contains an array of arrays of actions. The first array represents the columns of actions, and the second each row of those columns.

Example:
```js
[
    [
        {
            // column 1, action 1
        },
        {
            // column 1, action 2
        }
    ],
    [
        {
            // column 2, action 1
        },
        {
            // column 2, action 2
        }
    ]
]
```

An action is a JSON object with a few common options:
* **type**: the type of the action (either `rename` or `move`)
* **name**: the name of the action that will be displayed on the button
* **shortcut**: the assignated keyboard shortcut (ex: `H`, `3`, `Ctrl+X`, etc.)
* **final**: if set to true, triggering this action will also go to the next item (default: `false`)

#### Rename
Rename the file. The new name can be in another folder. If it doesn't exist, it will be created.

Additional options:
* **from**: a regular expression to match the original filename
* **to**: what to replace the regular expression match with (use `\1`, `\2`... for matched groups)

Example:
```json
{
    "type": "rename",
    "name": "!!!",
    "shortcut": "3",
    "from": "!*(.+)",
    "to": "!!!\\1"
}
```

#### Move
Move the file to a different directory. The destination can be relative or absolute.

Additional options:
* **dest**: the destination directory, either absolute or relative to the current browsed directory
* **create**: whether to create the destination directory if it doesn't exist (default: `true`)

Example:
```json
{
    "type": "move",
    "final": true,
    "name": "Hello",
    "shortcut": "H",
    "dest": "C:\\Users\\Nicolas\\Documents\\Test\\Hello"
}
```

#### Process
Run a custom command.

Additional options:
* **cmd**: the binary to call
* **args**: the list of arguments to pass to the binary, `{var}` in either will be replaced by the variables below

Variables:
* **path**: the file absolute path
* **dir**: the directory absolute path
* **filename**: the full file name (including the extension)
* **basename**: the file name without the extension (until the last dot)
* **ext**: the file extension (starting from the last dot)
* **/**: the native separator of your platform (`/` on Windows, `\` on UNIX)

Example:
```json
{
    "type": "process",
    "final": false,
    "name": "Resize image",
    "shortcut": "R",
    "cmd": "magick",
    "args": ["{path}", "-resize", "150x150", "{dir}/{basename}_150x150.{ext}"]
}
```

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
cmake ..
make -j 8
```

You'll find the built binary in the `build/` directory, named `Organizer`.