#FCSConvert merge utility

# merge #

Basic usage is like this:

**>** fcsconvert merge _files..._ out _filename_

This utility merges the columns of the set of FCS files specified by `files...` and writes the data to the file `filename`. Every file in the `merge` must have the same number of data elements.

**>** fcsconvert merge file1.fcs file2.fcs file3.fcs out files-1-2-3.fcs