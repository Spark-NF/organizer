## Introduction

Create a [symbolic link](https://en.wikipedia.org/wiki/Symbolic_link) to the file.

> [!WARNING]
>
> On Windows, you need elevated permissions to create symbolic links, unless [Developer Mode](https://learn.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development) is enabled. Otherwise, you can use the [Shortcut](shortcut.md) or [Hard link](hard-link.md) actions.


## Options

* **name**: what to name the symbolic link
* **overwrite**: set to true to will overwrite any already existing file (default: `false`)


## Examples

```json
{
    "type": "symlink",
    "name": "Link to this file"
}
```
