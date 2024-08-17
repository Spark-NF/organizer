## Introduction

Matches a file using its last modified time.


## Options

* **min**: the minimum last modified time in [ISO 8601 format](https://en.wikipedia.org/wiki/ISO_8601), inclusive (empty for no minimum, default: `""`)
* **max**: the maximum last modified time in [ISO 8601 format](https://en.wikipedia.org/wiki/ISO_8601), exclusive (empty for no minimum, default: `""`)


## Examples

```json5
{
    "type": "last_modified",
    "min": "2017-07-24T15:46:29"
}
```
