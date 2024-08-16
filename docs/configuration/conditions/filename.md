## Introduction

Matches a file using its filename.


## Options

* **filename**: the filename condition to use, including the extension. Can use multiple patterns with the ";" separator (disabled when using regular expressions)
* **regex**: set to true to use regular expressions for matching (default: `false`)


## Examples

```json
{
  "type": "filename",
  "filename": "*.jpg; *.png",
  "regex": false
}
```
