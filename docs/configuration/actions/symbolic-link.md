## Introduction

Create a [symbolic link](https://en.wikipedia.org/wiki/Symbolic_link) to the file.

> [!WARNING]
>
> On Windows, you need elevated permissions to create symbolic links, unless [Developer Mode](https://learn.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development) is enabled. Otherwise, you can use the [Shortcut](shortcut.md) or [Hard link](hard-link.md) actions.


## Options

* **dest**: where to create the symbolic link (absolute, or relative to the source file's directory)
* **create**: whether to create the destination directory if it doesn't exist (default: `true`)
* **overwrite**: set to true to will overwrite any already existing file (default: `false`)


## Examples

```json
{
    "type": "symlink",
    "dest": "Link to this file"
}
```
