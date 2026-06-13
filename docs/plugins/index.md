# Plugins

## Introduction

Plugins extend Organizer with new loaders and actions without modifying the core application.

A plugin is a folder containing a `plugin.json` manifest and whatever executable it needs, be it a Python script, a Node.js file, a compiled binary, or pretty much anything that can read from stdin and write to stdout.

## Installing a plugin

Place the plugin folder in any of the directories below. All paths are scanned and all plugins
loaded; if two plugins provide the same key, the first path in the list wins.

=== "Windows"

    1. `%ORGANIZER_PLUGINS%` (if set)
    2. `%APPDATA%\Organizer\plugins\`
    3. `<install directory>\plugins\`

=== "Linux"

    1. `$ORGANIZER_PLUGINS` (if set)
    2. `~/.local/share/organizer/plugins/`
    3. `<install directory>/plugins/`

=== "macOS"

    1. `$ORGANIZER_PLUGINS` (if set)
    2. `~/Library/Application Support/organizer/plugins/`
    3. `<install directory>/plugins/`

The folder name is arbitrary. The only required file is `plugin.json`.

```
plugins/
  my-plugin/
    plugin.json
    script.py  # and any other files the plugin needs
```

## Using a plugin

Once installed, a plugin's loaders and actions are used exactly like built-in ones.

### Plugin loaders

Use the plugin's loader key as the `data` field:

```json5
{
    "data": "my_loader",
    "some_option": "value",
    "glob": "*expected_value*"
}
```

Template variable: `{my_loader}`, or `{my_loader.subfield}` if the plugin exposes sub-fields.

### Plugin actions

Use the plugin's action type as the `type` field. All other fields are forwarded to the plugin as-is:

```json5
{
    "type": "my_action",
    "key": "value"
}
```
