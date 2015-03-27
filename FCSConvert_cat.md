#FCSConvert cat

# cat #

`cat` concatenates FCS files together. The usage pattern is this:

**>** fcsconvert cat _files..._ out filename

All of the files specified in `files...` must have the same number of columns. The output is is written to the file _filename_. Example:

**>** fcsconvert cat file1.fcs file2.fcs file3.fcs out files-1-2-3.cat