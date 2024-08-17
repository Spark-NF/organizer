## Introduction

Matches a file using its creation time.


## Options

* **min**: the minimum creation time in [ISO 8601 format](https://en.wikipedia.org/wiki/ISO_8601), inclusive (empty for no minimum, default: `""`)
* **max**: the maximum creation time in [ISO 8601 format](https://en.wikipedia.org/wiki/ISO_8601), exclusive (empty for no minimum, default: `""`)


## Examples

```json5
{
    "type": "created",
    "min": "2017-07-24T15:46:29"
}
```
