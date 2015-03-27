#FCSConvert erase

# erase #

`erase` removes columns from an FCS file. The usage is:

**>** fcsconvert erase _file_ out _filename_ columns _N..._

Where `file` is the name of the FCS from which to remove columns; _filename_ is the name of the output file to write to; and _N..._ is the set of columns to remove from the file. An example is:

**>** fcsconvert erase file1.fcs out file-less134.fcs columns 1 3 4