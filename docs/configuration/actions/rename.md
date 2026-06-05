## Introduction

Rename the file. The new name can be in another folder. If it doesn't exist, it will be created.


## Options

* **dest**: the new filename. Supports `{key}` placeholders (see [Loader](../conditions/loader.md)) resolved at execution time.
* **overwrite**: set to `true` to overwrite any already existing file (default: `false`)


## Examples

Rename using loader placeholders:
```json
{
    "type": "rename",
    "dest": "{created|year}-{created|month}-{created|day}_{filename}"
}
```

Rename based on a capture group from a condition regex (see [Comparator](../conditions/comparator.md)):
```json
{
    "type": "rename",
    "dest": "{captures.stem.date}-{captures.stem.name}.{extension}"
}
```
