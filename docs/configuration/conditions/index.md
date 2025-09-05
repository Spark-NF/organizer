# Condition

## Introduction

A condition is a JSON object with a few common options:

* **data**: what data loader to use (such as `filename` or `filesize`), see [Loader](loader.md)

In addition, each condition can have one [Comparator](comparator.md) set on it.

Example:
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

