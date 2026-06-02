# Loader

## Introduction

A loader is a string that identifies what data to extract from a file. It is used in conditions to specify what to compare against, and in action parameters as `{key}` placeholders for template substitution (e.g. `"dest": "D:\\Photos\\{extension}"`).

## Possible values

* `created`: the creation time of the file
* `directory`: the canonical absolute path of the directory containing the file
* `directory_name`: the name of the directory containing the file (last path component only)
* `extension`: the file extension, without the leading dot (e.g. `gz`)
* `filename`: the filename including extension
* `filesize`: the size of the file on disk, in bytes
* `last_modified`: the last modified time of the file
* `mime_type`: the MIME type of the file (e.g. `image/jpeg`), detected using the file extension first, then the file content
* `path`: the canonical absolute path of the file
* `stem`: the filename without its last extension (e.g. `archive.tar` from `archive.tar.gz`)

## Options

### extension

* **complete**: set to `true` to return the full compound extension (e.g. `tar.gz` instead of `gz`). Default: `false`

Example:
```json5
{
    "data": "extension",
    "complete": true,
    "glob": "tar.gz"
}
```

### stem

* **base**: set to `true` to strip all extensions (e.g. `archive` instead of `archive.tar`). Default: `false`

Example:
```json5
{
    "data": "stem",
    "base": true,
    "glob": "archive"
}
```
