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

### In

Will match if the data is one of the values in the list. Comparison is case-insensitive by default.

Supports both string and numeric values.

Example:
```json5
{
    "in": ["jpg", "jpeg", "png"]
}
```

With case-sensitive matching:
```json5
{
    "in": ["image/jpeg", "image/png"],
    "case_sensitive": true
}
```

### Not

Will match if the wrapped comparator does not match.

Example:
```json5
{
    "not": {"glob": "*.tmp"}
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

#### Capture groups

Named and positional capture groups are extracted on a successful match and made available as template variables in other actions via `{captures.$data.$group}`, where:

* `$data`: the loader key of the condition, or "content" for content loaders
* `$group` the group index, or name when using named groups: `(?P<name>...)`

With `"data": "filename"` and `"regex": "(?P<title>.+)-(\\d+)\\.jpg"`, a match on `photo-42.jpg` exposes:

- `{captures.filename.title}`: `photo`
- `{captures.filename.1}`: `photo` (same group but by position)
- `{captures.filename.2}`: `42`

These values are usable in any action template in the same rule. For example, to rename `photo-2024-06-01.jpg` into `2024-06-01-photo.jpg`:

```json
{
    "name": "Rename photos",
    "conditions": [
        {
            "data": "stem",
            "regex": "(?P<name>.+)-(?P<date>\\d{4}-\\d{2}-\\d{2})"
        }
    ],
    "actions": {
        "type": "rename",
        "dest": "{captures.stem.date}-{captures.stem.name}.{extension}"
    }
}
```

#### Composite comparators

* `and` / `or`: when multiple regex comparators are composed, all of them write into the same capture map. Positional indices are shared and the last-evaluated regex wins. Prefer named groups (`(?P<name>...)`) whenever captures will be used in templates inside a composite condition.
* `not`: the inner comparator's captures are passed through even when `not` inverts the result.
