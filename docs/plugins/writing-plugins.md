# Writing a plugin

## Introduction

A plugin is a folder containing a `plugin.json` manifest and an executable. The executable reads requests from stdin and writes responses to stdout using JSONL (one JSON object per line).

## Manifest (`plugin.json`)

```json5
{
    "name": "my-plugin", // shown in logs and error messages
    "version": "1.0.0", // informational only

    "cmd": "python", // executable (looked up in PATH, or absolute path)
    "args": ["script.py"], // arguments (paths are relative to the plugin folder)
    "timeout": 10000, // milliseconds to wait for a response (default: 10000)

    "provides": {
        "loaders": ["my_loader"], // loader keys this plugin registers
        "actions": ["my_action"] // action types this plugin registers
    }
}
```

Either `loaders` or `actions` may be omitted if the plugin only provides one kind. The process is started with its working directory set to the plugin folder, so relative paths in `args` are possible.

## Protocol

Communication with the plugin uses JSONL (one JSON object per line, terminated by `\n`). Requests are sent to stdin and responses are read from stdout (one at a time, sequentially).

The plugin process is started on first use and kept alive for the session, and terminated when Organizer exits.

Errors and messages written by the plugin to stderr are forwarded as-is to Organizer's log, prefixed with the plugin name.

### Load request

Sent when a loader value is needed:

```json
{"op": "load", "key": "my_loader", "path": "/path/to/file.jpg", "fields": ["subfield"]}
```

Note: `fields` is omitted when no sub-field was requested (i.e. plain `{my_loader}`).

### Load response

Possible responses:

```json
{"value": "some string"}
{"value": 42}
{"value": null}
{"error": "could not read file"}
```

`null` or a missing `value` means the loader has no data for this file, so the file will usually be skipped. An `error` is logged as a warning.

String values that look like ISO 8601 dates (`YYYY-MM-DDTHH:MM:SS`) are automatically converted to date values, and as such support date comparators and date template filters.

### Action request

Sent when an action is executed:

```json
{"op": "action", "type": "my_action", "path": "/path/to/file.jpg", "params": {"key": "value"}}
```

Note: `params` is the full action JSON object from the profile (i.e. all fields except `type`).

### Action response

Possible responses:

```json
{}
{"path": "/new/path/file.jpg"}
{"error": "permission denied"}
```

Absence of `error` means success. The optional `path` field updates the file path in Organizer, for example if the action moves or renames the file.

## Examples

=== "Python"

    ```python
    import sys
    import json

    for line in sys.stdin:
        req = json.loads(line)
        if req["op"] == "load":
            value = do_load(req["path"], req.get("fields"))
            print(json.dumps({"value": value}), flush=True)
        elif req["op"] == "action":
            do_action(req["path"], req["params"])
            print(json.dumps({}), flush=True)
    ```

=== "Node.js"

    ```js
    const readline = require('readline');
    const rl = readline.createInterface({ input: process.stdin });

    rl.on('line', line => {
        const req = JSON.parse(line);
        if (req.op === 'load') {
            const value = doLoad(req.path, req.fields);
            process.stdout.write(JSON.stringify({ value }) + '\n');
        } else if (req.op === 'action') {
            doAction(req.path, req.params);
            process.stdout.write(JSON.stringify({}) + '\n');
        }
    });
    ```
