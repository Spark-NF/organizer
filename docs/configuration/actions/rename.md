## Introduction

Rename the file. The new name can be in another folder. If it doesn't exist, it will be created.


## Options

* **from**: a regular expression to match the original filename
* **to**: what to replace the regular expression match with. Supports regex back-references (`\1`, `\2`...) and `{key}` placeholders (see [Loader](../conditions/loader.md)) resolved at execution time.
* **overwrite**: set to true to will overwrite any already existing file (default: `false`)


## Examples

Using a regex back-reference:
```json
{
    "type": "rename",
    "from": "!*(.+)",
    "to": "!!!\\1"
}
```

Prepending the file extension using a loader placeholder:
```json
{
    "type": "rename",
    "from": "(.+)",
    "to": "{extension}_\\1"
}
```
