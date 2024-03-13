# Versions


<details open>
<summary>v0.0.4</summary>

* minor change to makefile. 
</details>
<details>
<summary>v0.0.3</summary>

* Added '+n' option for indicating to display a certain number of lines.  Often likely used when wanting to skip a number of lines, display a number of lines, then skip some more.
</details>
<details>
<summary>v0.0.2</summary>

* Added 'all' option for indicating to skip everything after that point.  Often likely only used with the '0' entries (which means dont skip that line).
</details>
<details>
<summary>v0.0.1</summary>

* Initial implementation includes the options:
   * **number of lines to skip** : (often referred to as 'n').  eg `cat somefile.txt | skip 3`
   * **header** : skip lines until it reaches a blank line, then outputs the rest
   * **blank** : skip blank lines
   * **before "string"** : skips lines before line that starts with "string"
   * **after "string"** : skips lines after the line that start with "string"
   * some other bits to help with those above options.
</details>
