# Loader

## Introduction

A loader is a string that tells the condition what data to use.

## Possible values

* `created` the creation time of the file
* `directory`: the canonical absolute path of the directory containing the file
* `extension`: the file extension, without the leading dot (e.g. `jpg`)
* `filename`: the filename
* `filesize`: the size of the file on disk, in bytes
* `last_modified`: the last modified time of the file
* `mime_type`: the MIME type of the file (e.g. `image/jpeg`), detected using the file extension first, then the file content
* `path`: the canonical absolute path of the file

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
