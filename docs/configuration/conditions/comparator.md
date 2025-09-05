# Comparator

## Introduction

A comparator is a JSON object that tells the program how to compare the data fetched from the loader. 

## Possible values

### And

Will match if all the comparators contained match.

Example:
```json5
{
    "and": [
        {"regex": "^start_"},
        {"regex": "_end$"},
    ]
}
```

### Glob

Will perform a glob match on the data. Multiple patterns can be used with the ";" separator.

Example:
```json5
{
    "glob": "*.jpg; *.png"
}
```

### Or

Will match if one of the comparators contained match. Comparators are evaluated in order.

Example:
```json5
{
    "or": [
        {"regex": "^start_"},
        {"regex": "_end$"},
    ]
}
```

### Range

Will perform a range check on the data. One or both of `min` and `max` can be set. `min` is inclusive, `max` is exclusive.

Supports both numbers and dates. Dates are parsed with the [ISO 8601 format](https://en.wikipedia.org/wiki/ISO_8601).

Example:
```json5
{
    "min": 100,
    "max": 200
}
```
```json5
{
    "min": "2017-07-24T15:46:29"
}
```

### Regex

Will perform a regex match on the data.

Example:
```json5
{
    "regex": "^start_with_"
}
```
