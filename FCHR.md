#Flow Cytometry  Human Readable

# Description of the Format #

The format here is a combination of the UNIX XOrg.conf and keyword formats, and the FCS format. If you are unfamiliar with XOrg.conf or keyword formats the rules are relatively straightforward:

  1. Only one piece of information per line
  1. A key/value pair is written key: value
  1. To write a ":" use the escape character "\": \: (_usual escape rules apply_)
  1. Logical blocks are demarcated by `Begin`--`End` blocks
  1. Comments run from the "#" character to the end-of-line character; comments may occur anywhere, along with blank lines

A large subset of a real FCHR file is [here](FCHRExample.md).

At the high level a Flow Cytometry Human Readable (FCHR) file is composed of (at least) two blocks, and commonly three:

```
Head
Data
Analysis
```

At minimum the `Head` and `Data` blocks are required. The order of these blocks is immutable.

## Start of File ##

The start of an FCHR file is demarcated by the sigil "FCS:H.R" occurring on a single line.

## Head ##

The `Head` of an FCHR file is demarcated by the sigil "Section Head" occurring on a single line by itself.

The `Head` must be composed of (at minimum) one Column block (`ColumnBegin`--`ColumnEnd`) block. There must be one Column block per column in the data file. Other information may appear both in and around the Column block; it will be ignored.

The following is an example Column block:

```
ColumnBegin
  Name: FSC-H # name of the column, does _not_ have to be unique
  Range: 1024 # range from [0, 1024]
  Scale: 0, 0 # log/linear scale
ColumnEnd
```

## Data ##

The `Data` section is demarcated by the sigil "Section Data" occurring on a single line by itself.

The data in this section is in _list_ mode. Each line in the `Data` section represents one element in the data-set. The data is whitespace delimited and in human-readable format. Here is an example of the beginning of a data-section:

```
Section Data

431 223 586 705 282
350 224 549 658 211
446 399 589 431 246
333 182 514 681 293
```

## Analysis ##

The `Analysis` section is demarcated by the sigil "Section Analysis" occurring on a single line by itself. There is no specification for the `Analysis` section.