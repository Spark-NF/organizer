## Introduction

Matches a file using its size on disk.


## Options

* **min**: the minimum size in bytes, inclusive (`-1` for no minimum, default: `-1`)
* **max**: the maximum size in bytes, exclusive (`-1` for no maximum, default: `-1`)


## Examples

```json5
{
    "type": "filesize",
    "min": 5242880 // 5 MB
}
```
