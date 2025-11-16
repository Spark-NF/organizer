## Introduction

Move the file to a different directory. The destination can be relative or absolute.


## Options

* **dest**: the destination directory, either absolute or relative to the current browsed directory
* **create**: whether to create the destination directory if it doesn't exist (default: `true`)
* **overwrite**: set to true to will overwrite any already existing file (default: `false`)


## Examples

Absolute destination:
```json
{
    "type": "move",
    "dest": "D:\\Test\\Hello"
}
```

Relative destination:
```json
{
    "type": "move",
    "dest": "../Hello"
}
```
