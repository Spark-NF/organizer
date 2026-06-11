# Command-line interface

## Help

Use `-h` or `--help` to trigger. Will also trigger if no file is passed to the program.

### Example
```
$ Organizer-cli --help

Usage: Organizer-cli [options] files...
Organizer

Options:
  -h, --help               Displays help on commandline options.
  -v, --version            Displays version information.
  -p, --profile <profile>  The rule profile file to use (required).
  -n, --dry-run            Preview actions without modifying any files.
  -r, --recursive          Process directories recursively.
      --check              Validate the profile file and exit without processing any files.
  -q, --quiet              Suppress all non-error output.
  -w, --watch              Watch the given directories for new files and process them automatically.

Arguments:
  files                    The files to organize.
```


## Version

Use `-v` or `--version` to trigger.

### Example
```
$ Organizer-cli --version

Organizer 1.2.3
```


## Organizing files

Pass a profile file with `-p` or `--profile` and the files to organize. Directories are also accepted; use `-r` to descend into subdirectories.

File paths can also be piped via stdin by passing `-` as an argument:

```
$ find . -name "*.jpg" | Organizer-cli --profile rules.json -
```

### Example
```
$ Organizer-cli --profile "rules.json" "image1.jpg" "image2.jpg" "image3.jpg"

Ran rule Test on file image1.jpg
No matching rule for image2.jpg, ignoring
Ran rule Something on file image3.jpg
```


## Validating a profile

Use `--check` to validate a profile file without processing any files. Exits 0 if valid, 1 if not.

```
$ Organizer-cli --profile "rules.json" --check
Profile is valid.
```


## Watching directories

Use `-w` or `--watch` with one or more directory paths to watch for new files and process them automatically as they arrive. The profile is applied to each new file exactly as a normal invocation would. Requires at least one directory argument; file paths are not accepted. Incompatible with `--check`.

On startup, existing files in the watched directories are processed once (same as a normal invocation). Press Ctrl+C to stop.

```
$ Organizer-cli --profile "rules.json" --watch ~/Downloads

Watching /home/user/Downloads for changes. Press Ctrl+C to stop.
```

Use `--recursive` to also watch subdirectories. New subdirectories created after startup are watched automatically.

Use `--dry-run` to preview what would happen without modifying any files.

### Notes

- Any file not moved or deleted by a rule (including files a rule moved into a watched subdirectory) will be reprocessed on the next restart.
- On Linux, `QFileSystemWatcher` uses inotify, which has a default per-user watch limit of ~8192 paths. Deeply nested trees may exhaust this limit; raise it via `/proc/sys/fs/inotify/max_user_watches` if needed.


## Verbosity

- `--quiet` / `-q`: suppress all non-error output. Only errors are written to stderr.
