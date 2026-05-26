## Introduction

Create a [hard link](https://en.wikipedia.org/wiki/Hard_link) to the file.


## Options

* **dest**: where to create the hard link (absolute, or relative to the source file's directory)
* **create**: whether to create the destination directory if it doesn't exist (default: `true`)
* **overwrite**: set to true to will overwrite any already existing file (default: `false`)


## Examples

```json
{
    "type": "hardlink",
    "dest": "Link to this file"
}
```
