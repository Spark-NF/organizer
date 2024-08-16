## Introduction

Run multiple actions in order. It will immediately stop if one of the actions fail.


## Options 

* **actions**: a list of actions (many options such as `name`, `shortcut`, or `final` are unnecessary here)


## Examples

```json5
{
    "type": "multiple",
    "actions": [
        {
            "type": "process",
            // ...
        },
        {
            "type": "move",
            // ...
        }
    ]
}
```
