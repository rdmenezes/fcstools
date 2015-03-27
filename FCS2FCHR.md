#fcstofchr

## Why another standard? ##

The `fcstofchr` and `fchrtofcs` are actually the same tool. The FCS- 2.0 and 3.0 formats are remarkably poorly designed. Specifically, while it is (relatively) straightforward to write in these formats, in theory it is impossible to read the format _a priori_. This is because the format itself does not provide enough information to decode the binary portion of the FCS file.

In addition to this, the format itself is not human-readable in any sense of the word (except for the **head** portion). While there have been attempts at XML and DICOM formats for transition from the FCS formats, they are not popular at all. Probably this is due to the closed and/or proprietary nature of their implementation; in addition, both of those formats are rather heavy-duty compared to binary-mode FCS formats.

To help remedy the situation we have provided tools to transform to and from a human-readable format called "[FCHR](FCHR.md)" or "Flow Cytometry Human Readable". The format uses a light-weight modified XOrg.conf style; it is somewhat of a hybrid between XOrg.conf, the FCS gross-structure, and a lightweight key-file.

## Using the tool ##

Using the tool(s) is easy:

**>** fcstofchr sample1.fcs sample1.fchr

The resulting file (sample1.fchr) is in the FCHR format. To reverse the transformation:

**>** fchrtofcs sample1.fchr sample1-r.fcs

Should return a file similar to sample1.fcs. (In fact it will be most similar to the file produced by [fcstest](FCSTest.md).

Note that `fcstofchr` and `fchrtofcs` are actually the same piece of code; the tool first attempts to open as an FCS file, then as an FCHR file. Whichever is successful, it translates to the other.