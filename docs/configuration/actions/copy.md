## Introduction

Copy the file to a different directory. The destination can be relative or absolute. Unlike move, the source file is not removed.


## Options

* **dest**: the destination directory, either absolute or relative to the current browsed directory. Supports `{key}` placeholders (see [Loader](../conditions/loader.md)) resolved at execution time.
* **create**: whether to create the destination directory if it doesn't exist (default: `true`)
* **overwrite**: set to true to overwrite any already existing file (default: `false`)


## Examples

Absolute destination:
```json
{
    "type": "copy",
    "dest": "D:\\Test\\Hello"
}
```

Relative destination:
```json
{
    "type": "copy",
    "dest": "../Hello"
}
```
