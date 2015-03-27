#FCSConvert datatype

# datatype #

`datatype` changes the `$DATATYPE` of an FCS file. The only two data-types supported are "Integer" and "ASCII". In particular, "ASCII" refers to (only) variable length ASCII (A) encoding. Integer refers to "I" encoding. For "I" default `$PnB` values (bit-lengths) are used: `sizeof(blessed_integer) * DEFACTO_BYTEL`. (N.B.: `blessed_integer` is whatever the type of `signed long` is on the system being compiled and `DEFACTO_BYTEL` is always **8**.)

There are two usage patterns:

**>** fcsconvert datatype _Type_ _file_ out _filename_

Which changes the data-type of the FCS file to `Type` and writes the result to `filename`.

**>** fcsconvert datatype! _Type_ _files..._

Which changes the data-type of the set of FCS files to `Type` and **writes the result back to the same files**. **THIS IS DESTRUCTIVE; THIS IS IN-PLACE!**