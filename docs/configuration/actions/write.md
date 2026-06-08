## Introduction

Write text to a file.


## Options

* **file**: path of the file to write to. Supports `{key}` placeholders.
* **text**: the content to write. Supports `{key}` placeholders.
* **mode**: how to write the content
  * `append`: add text at the end of the file
  * `overwrite`: replace the entire file contents
  * `prepend`: insert text before the existing file contents


## Examples

Append a line to a log file each time a rule fires:
```json
{
    "type": "write",
    "mode": "append",
    "file": "D:\\logs\\processed.txt",
    "text": "{filename}\n"
}
```

Write the file's creation year into a sidecar file:
```json
{
    "type": "write",
    "mode": "overwrite",
    "file": "{path}.meta",
    "text": "{created|year}"
}
```
