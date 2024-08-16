## Introduction

Run a custom command.


## Options

* **cmd**: the binary to call
* **args**: the list of arguments to pass to the binary, `{var}` in either will be replaced by the variables below


### Available variables

* **path**: the file absolute path
* **dir**: the directory absolute path
* **filename**: the full file name (including the extension)
* **basename**: the file name without the extension (until the last dot)
* **ext**: the file extension (starting from the last dot)
* **/**: the native separator of your platform (`/` on Windows, `\` on UNIX)


## Examples

```json
{
    "type": "process",
    "cmd": "magick",
    "args": ["{path}", "-resize", "150x150", "{dir}/{basename}_150x150.{ext}"]
}
```
