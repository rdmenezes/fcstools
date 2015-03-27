#FCSConvert width

# datatype #

`width` changes the `$PnB` of an FCS file. The user may set any bit-length, but the FCSTools will round this up to the nearest `DEFACTO_BYTEL`. Actually being able to store the value in memory is not necessary: the result is written and computed "on the fly" algebraically. Note that if the values are too large to fit into the `blessed_integer` of the system, then when _opening_ the file, the data may become corrupted.

There are two usage patterns:

**>** fcsconvert width _N_ _file_ out _filename_

Which changes the width of the FCS file to `N`-bits and writes the result to `filename`.

**>** fcsconvert width! _N_ _files..._

Which changes the data-type of the set of FCS files to `N`-bits and **writes the result back to the same files**. **THIS IS DESTRUCTIVE; THIS IS IN-PLACE!**