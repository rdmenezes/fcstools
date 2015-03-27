#FCSConvert trunc

# trunc #

`trunc` removes data either from the beginning (negative numbers) or from the end (positive numbers) of an FCS file.

There are two usage patterns:

**>** fcsconvert trunc _N_ _file_ out _filename_

Which removes N elements from the data-set and writes the output to `filename`.

**>** fcsconvert trunc! _N_ _files..._

Which removes N elements from the data-set from each file in the set of FCS files and **writes the result back to the same files**. **THIS IS DESTRUCTIVE; THIS IS IN-PLACE!**

Specifically:

**>** fcsconvert trunc 10000 file1.fcs

Removes the _**last**_ 10000 elements from the file `file1.fcs`.

**>** fcsconvert trunc -10000 file1.fcs

Removes the _**first**_ 10000 elements from the file `file1.fcs`.