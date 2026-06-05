# Condition

## Loader condition

The most common condition type. Loads a value from the file using a [Loader](loader.md) and checks it against a [Comparator](comparator.md).

* **data**: what data loader to use (such as `filename` or `filesize`), see [Loader](loader.md)

Examples:
```json5
{
    "data": "filename",
    "glob": "*.jpg; *.png"
}
```
```json5
{
    "data": "created",
    "min": "2017-07-24T15:46:29"
}
```

## Process condition

Runs a command and check if it exits with code 0.

### Options

* **cmd**: the binary to call
* **args**: the list of arguments to pass to the binary, `{var}` in either will be replaced by the variables below
* **timeout**: maximum time in milliseconds to wait for the process to finish (default: `30000`); use `null` to wait indefinitely

### Available variables

* **path**: the file absolute path
* **dir**: the directory absolute path
* **filename**: the full file name (including the extension)
* **basename**: the file name without the extension (until the last dot)
* **ext**: the file extension (starting from the last dot)
* **/**: the native separator of your platform (`\` on Windows, `/` on UNIX)

### Examples

```json5
{
    "data": "process",
    "cmd": "my-validator",
    "args": ["{path}"]
}
```
