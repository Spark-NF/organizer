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


## Verbosity

- `--quiet` / `-q`: suppress all non-error output. Only errors are written to stderr.
