## Introduction

Rename the file. The new name can be in another folder. If it doesn't exist, it will be created.


## Options

* **from**: a regular expression to match the original filename
* **to**: what to replace the regular expression match with (use `\1`, `\2`... for matched groups)
* **overwrite**: set to true to will overwrite any already existing file (default: `false`)


## Examples

```json
{
    "type": "rename",
    "from": "!*(.+)",
    "to": "!!!\\1"
}
```
