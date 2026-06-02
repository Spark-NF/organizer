## Introduction

Move the file to a different directory. The destination can be relative or absolute.


## Options

* **dest**: the destination directory, either absolute or relative to the current browsed directory. Supports `{key}` placeholders (see [Loader](../conditions/loader.md)) resolved at execution time.
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

Template destination (sorts files by year and month of creation):
```json
{
    "type": "move",
    "dest": "D:\\Photos\\{created|year}\\{created|month}"
}
```

Filters can also transform string values (see [Loader](../conditions/loader.md#template-filters)):
```json
{
    "type": "move",
    "dest": "D:\\Photos\\{extension|upper}"
}
```
