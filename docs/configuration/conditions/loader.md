# Loader

## Introduction

A loader is a string that identifies what data to extract from a file. It is used in conditions to specify what to compare against, and in action parameters as `{key}` placeholders for template substitution (e.g. `"dest": "D:\\Photos\\{extension}"`).

## Possible values

* `created`: the creation time of the file
* `directory`: the canonical absolute path of the directory containing the file
* `empty`: if the file is zero bytes, or if the directory has no contents
* `directory_name`: the name of the directory containing the file (last path component only)
* `exif`: a metadata tag from the file's EXIF data (e.g. `DateTimeOriginal`, `Make`, `Model`...), requires `exiftool` to be installed
* `extension`: the file extension, without the leading dot (e.g. `gz`)
* `filename`: the filename including extension
* `filesize`: the size of the file on disk, in bytes
* `id3`: a metadata tag from the file's ID3 tags (e.g. `Artist`, `Album`, `Title`...), requires `exiftool` to be installed
* `kind`: the broad category of the file: `image`, `audio`, `video`, `font`, `document`, `archive`, `application`, or `other`
* `last_modified`: the last modified time of the file
* `mime_type`: the MIME type of the file (e.g. `image/jpeg`), detected using the file extension first, then the file content
* `path`: the canonical absolute path of the file
* `stem`: the filename without its last extension (e.g. `archive.tar` from `archive.tar.gz`)

## Template filters

When used as `{key}` placeholders in action parameters, a pipe-separated filter chain can be appended to transform the value before substitution.

A few examples:

* `{extension|upper}`: "JPG"
* `{stem|lower}`: "photo"
* `{stem|trim}`: strips leading/trailing whitespace
* `{created|year}/{created|month}`: "2024/06"
* `{stem|trim|upper}`: filters are applied left to right

### String filters

* `upper`: convert to uppercase
* `lower`: convert to lowercase
* `trim`: strip leading and trailing whitespace

### Date filters

* `year`: four-digit year (e.g. `2024`)
* `month`: zero-padded month (e.g. `06`)
* `day`: zero-padded day of month (e.g. `03`)
* `hour`: zero-padded hour, 24-hour clock (e.g. `14`)
* `minute`: zero-padded minute (e.g. `05`)
* `format:<pattern>`: custom format using [Qt date/time format strings](https://doc.qt.io/qt-6/qdatetime.html#toString) (e.g. `format:yyyy-MM-dd`, `format:MMM yyyy`)

### Size filters

* `kb`: convert bytes to kilobytes
* `mb`: convert bytes to megabytes
* `gb`: convert bytes to gigabytes
* `filesize`: human-readable size string using system locale (e.g. `1.5 MB`, `300 KB`)

### Fallback filter

`default:<value>` will substitute `<value>` if the key is missing or resolves to an empty string. Filters placed after `default` are applied to the fallback value as well.

A few examples:

* `{unknown_data|default:Unknown}`: "Unknown"
* `{stem|trim|default:file}`: "file" if the stem is blank after trimming
* `{unknown_data|default:Unknown|upper}`: "UNKNOWN"

## Options

### exif

* **tag**: the exiftool tag name to read (e.g. `DateTimeOriginal`, `Make`, `Model`...)

Example:
```json5
{
    "data": "exif",
    "tag": "DateTimeOriginal",
    "glob": "2024*"
}
```

Template variable: `{exif.DateTimeOriginal}`

### id3

* **tag**: the exiftool tag name to read (e.g. `Artist`, `Album`, `Title`...)

Example:
```json5
{
    "data": "id3",
    "tag": "Artist",
    "glob": "Beatles"
}
```

Template variable: `{id3.Artist}`

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
