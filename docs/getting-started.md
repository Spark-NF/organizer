# Getting started

## Download

Download the latest release from the [GitHub releases page](https://github.com/Spark-NF/organizer/releases/latest) and install it.

## Create a profile

A profile is a JSON file containing a list of rules. Each rule has conditions and actions.

Create a file called `sort.json` with the following content (this profile sorts a folder by extension):

```json
{
    "name": "Sort by type",
    "rules": [
        {
            "name": "Images",
            "conditions": [{ "data": "extension", "in": ["jpg", "jpeg", "png", "gif", "webp", "bmp", "tiff", "heic"] }],
            "actions": [{ "type": "move", "dest": "Pictures" }]
        },
        {
            "name": "Videos",
            "conditions": [{ "data": "extension", "in": ["mp4", "mov", "avi", "mkv", "wmv", "flv", "webm", "m4v"] }],
            "actions": [{ "type": "move", "dest": "Videos" }]
        },
        {
            "name": "Music",
            "conditions": [{ "data": "extension", "in": ["mp3", "flac", "m4a", "aac", "ogg", "wav"] }],
            "actions": [{ "type": "move", "dest": "Music" }]
        },
        {
            "name": "Documents",
            "conditions": [{ "data": "extension", "in": ["pdf", "docx", "doc", "xlsx", "xls", "pptx", "ppt", "txt"] }],
            "actions": [{ "type": "move", "dest": "Documents" }]
        },
        {
            "name": "Archives",
            "conditions": [{ "data": "extension", "in": ["zip", "rar", "7z", "tar", "gz", "bz2"] }],
            "actions": [{ "type": "move", "dest": "Archives" }]
        }
    ]
}
```

The `dest` path is relative to the file's current directory, so running this on `~/Downloads` creates `~/Downloads/Pictures`, `~/Downloads/Videos`, and so on.

!!! tip "Validate your profile"
    Run `Organizer-cli --profile sort.json --check` to validate the JSON before processing any files.

## Run

### Using the CLI

Preview what would happen without touching any files using the `--dry-run` flag:

```shell
Organizer-cli --profile sort.json --dry-run ~/Downloads
```

When you're happy with the output, apply the rules:

```shell
Organizer-cli --profile sort.json ~/Downloads
```

Add `--recurse` to descend into subdirectories, or pipe filenames from another command for more complex operations:

```shell
find ~/Downloads -maxdepth 1 -type f | Organizer-cli --profile sort.json -
```

### Using the manual sorting window

1. Open Organizer
2. Load `sort.json` via "File > Open profile"
3. A button appears for each rule in the profile
4. Drag your Downloads folder (or individual files) onto the window

## Going further

The profile example above is just a starting point. Here are a few other examples of things you can do next:

Sort photos by date taken using EXIF metadata (requires [exiftool](https://exiftool.org/)):

```json
{
    "name": "Sort photos by year",
    "conditions": [{
        "data": "extension",
        "in": ["jpg", "jpeg", "heic", "raw", "cr2", "nef"]
    }],
    "actions": [{
        "type": "move",
        "dest": "Photos/{exif.DateTimeOriginal|year}"
    }]
}
```

Sort music by artist and album using ID3 tags (requires [exiftool](https://exiftool.org/)):

```json
{
    "name": "Sort music",
    "conditions": [{
        "data": "extension",
        "in": ["mp3", "flac", "m4a"] }
    ],
    "actions": [{
        "type": "move",
        "dest": "Music/{id3.Artist|default:Unknown}/{id3.Album|default:Unknown}"
    }]
}
```

Move old temporary files to the trash by combining extension and age conditions:
    
```json
{
    "name": "Delete old temp files",
    "conditions": [
        { "data": "extension", "glob": "tmp" },
        { "data": "last_modified", "max": "2024-01-01T00:00:00" }
    ],
    "actions": [{ "type": "trash" }]
}
```
