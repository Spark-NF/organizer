## Introduction

Create a Windows shortcut (LNK file) to the file.

> [!NOTE]
>
> This action is Windows-only. On other systems, see the [Symbolic link](symbolic-link.md) or [Hard link](hard-link.md) actions.


## Options

* **dest**: where to create the shortcut (the ".lnk" will automatically be added if not already present). Supports `{key}` placeholders (see [Loader](../conditions/loader.md)) resolved at execution time.
* **overwrite**: set to true to will overwrite any already existing shortcut file (default: `false`)


## Examples

```json
{
    "type": "shortcut",
    "dest": "Shortcut to this file"
}
```
