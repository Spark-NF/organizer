## Introduction

A rule is a JSON object with a few options:

* **name**: the name of the action that will be displayed on the button
* **shortcut**: the assigned keyboard shortcut (ex: `H`, `3`, `Ctrl+X`, etc.)
* **final**: if set to true, triggering this action will also go to the next item (default: `false`)
* **priority**: the priority of this rule when multiple rules match a given file, highest-priority rules will be executed (default: `0`)
* **conditions**: an array of conditions (see [Conditions](conditions/)), if all of them match the actions will be run automatically (default: empty)
* **actions**: an array of actions (see [Actions](actions/)) to all be run when executing this rule

Example:
```json5
{
    "name": "Hello",
    "shortcut": "H",
    "conditions": [{
        // ...
    }],
    "actions": [{
        // ...
    }]
}
```
