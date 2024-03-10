# skip

Skip is a simple tool useful mostly for bash scripting and command-line operations.

* _n_: Skip 'n' number of lines, and then output the rest
* blank: Skip any blank lines.
* header: Skip any lines at the start of the stream until a blank line is determined, and then will output the rest
* before "string": Skip any lines until the specified "string is matched" at the beginning of a line, then will output the rest (including line that matched unless 1 is the next parameter)
* until "string": Once the string is identified (at the begining of a line)



NOTE: 
 > It does not replace or include anything that grep can already do.  For example, skipping any lines that match a regex pattern, should be done by grep.  The main purpose is to provide functionality that grep is unable to do.
 

Examples:

```
# This will output somefile.txt but skip the first line
cat somefile.txt | skip

# This will output somefile.txt but skip the first 3 lines
cat somefile.txt | skip 3

# This will output somefile.txt but will not output any blank lines.
cat somefile.txt | skip blank

# This will output somefile.txt but will ignore all the lines at the start until an empty
# line is encountered, and then will output the rest.
cat somefile.txt | skip header

# This will output somefile.txt but will ignore all lines until it encounters a line that
# begins with "Data:" (and will output that "Data:" line)
cat somefile.txt | skip before "Data:"

# This will output somefile.txt but will ignore all lines until it encounters a line that
# begins with "Data:" (and will not output that "Data:" line)
cat somefile.txt | skip before "Data:" 1

# This will output somefile.txt but will ignore all lines until it encounters a line that
# begins with "Data:", will not remove the 'Data:' line, but will remove the next line.
cat somefile.txt | skip before "Data:" 0 1

# This will output somefile.txt and will output all lines until it encounters a line that
# begins with "Data:".  (Will not include the line that begins with 'End:')
cat somefile.txt | skip after "Data:"

# This will output somefile.txt but will ignore all lines until it encounters a line that
# begins with "Data:", will not remove the 'Data:' line and will output lines until it
# finds another line named 'End:'
cat somefile.txt | skip before "Data:" after "End:"

```

It should be noted that the order of the command options matter.  They are taken into account, in the order that they are specified.

Examples:
```
# This will skip all blank lines, skip 3 remaining lines, and then output the rest
cat somefile.txt | skip blank 3

# This will skip the first 3 lines (whether they contain any blank ones or not), and then
# output the rest except any blank lines
cat somefile.txt | skip 3 blank

# This will skip all lines until it reaches one that matches 'Data:'.  By default it would
# also output 'Data:' but in this case, it is being informed to skip that line.
cat somefile.txt | skip before 'Data:' 1
```
