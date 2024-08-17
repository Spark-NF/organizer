# Command-line interface

## Help

Use `-h` or `--help` to trigger. Will also trigger if no file is passed to the program.

Will show the program's help then exit.

### Example
```
$ Organizer-cli --help

Usage: Organizer-cli [options] files...
Organizer

Options:
  -h, --help               Displays help on commandline options.
  -v, --version            Displays version information.
  -p, --profile <profile>  The rule profile file to use.

Arguments:
  files                    The files to organize.
```


## Version

Use `-v` or `--version` to trigger.

Will show the program's version then exit.

### Example
```
$ Organizer-cli --version

Organizer 1.2.3
```


## Organizing files

Simply pass a profile file with `-p` or `--profile` and the files you want to be organized, and the rules will be executed as needed.

### Example
```
$ Organizer-cli --profile "rules.json" "image1.jpg" "image2.jpg" "image3.jpg"

Ran rule Test on file image1.jpg
No matching rule for image2.jpg, ignoring
Ran rule Something on file image3.jpg
```
