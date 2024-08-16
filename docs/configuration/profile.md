## Introduction

A profile file is a JSON file that contains a single profile as the root object.

A profile is a JSON object with a few options:

* **name**: the name of the profile to show in the interface
* **rules**: an array or an array of arrays of rules (see [Rules](rule.md))

The first array represents the columns of actions, and the second each row of those columns. When using a single depth of arrays, there will be a single column. This is mainly useful for manual rules.

Example:
```json5
{
    "name": "My profile",
    "rules": [
        {
            // rule 1
        },
        {
            // rule 2
        }
    ]
}
```

With multiple columns:
```json5
{
    "name": "My profile",
    "rules": [
        [
            {
                // column 1, rule 1
            },
            {
                // column 1, rule 2
            }
        ],
        [
            {
                // column 2, rule 1
            },
            {
                // column 2, rule 2
            }
        ]
    ]
}
```