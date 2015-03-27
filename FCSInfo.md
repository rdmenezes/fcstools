#fcsinfo

# FCSInfo #

_fcsinfo_ is straightforward to use:

**>** fcsinfo _file_

For instance:

**>** fcsinfo sample1.fcs2

```
Name:        result.fcs
Events:      35253
Mode:        L
Datatype:    I

Column Information

FSC-H
   BitSize: 32
     Range: 1024
SSC-H
   BitSize: 32
     Range: 1024
FL1-H
   BitSize: 32
     Range: 1024
FL2-H
   BitSize: 32
     Range: 1024
FL3-H
   BitSize: 32
     Range: 1024

Example Data

431          223          586          705          282
350          224          549          658          211
446          399          589          431          246
...          ...          ...          ...          ...
322          204          536          585          147
386          195          553          727          304
296          206          550          708          227
```

It is also possible to show more information; the three additional switches are "--extended-information", "--dump-keys-only", and "--all-data". The first switch displays more information about the file; the second switch dumps all of the keyword-value pairs; the third switch dumps all of the data in ASCII (human-readable) format. For instance:

**>** fcsinfo sample1.fcs2 --extended-information

```
Name:        Samples/sample1.fcs2
Events:      35253
Mode:        L
Datatype:    I
Byte Order:  4, 3, 2, 1
Nextdata:    0

Column Information

FSC-H
   BitSize: 16
     Range: 1024
  Exponent: 0, 0
     Spec.: Forward Scatter

SSC-H
   BitSize: 16
     Range: 1024
  Exponent: 0, 0
     Spec.: Side Scatter

FL1-H
   BitSize: 16
     Range: 1024
  Exponent: 4, 0
     Spec.: 530 nm

FL2-H
   BitSize: 16
     Range: 1024
  Exponent: 4, 0
     Spec.: 585 nm

FL3-H
   BitSize: 16
     Range: 1024
  Exponent: 4, 0
     Spec.: >650 nm


Example Data

431          223          586          705          282          
350          224          549          658          211          
446          399          589          431          246          
...          ...          ...          ...          ...          
322          204          536          585          147          
386          195          553          727          304          
296          206          550          708          227 
```

**>** fcsinfo sample1.fcs2 --dump-keys-only

```
               $BTIM: 12:32:10
            $BYTEORD: 4,3,2,1
                $CYT: FACScan
           $DATATYPE: I
               $DATE: 03-Mar-04
               $ETIM: 12:32:37
                $FIL: 030304.006
               $MODE: L
           $NEXTDATA: 0
                $P1B: 16
                $P1E: 0,0
                $P1N: FSC-H
                $P1R: 1024
                $P1S: Forward Scatter
                $P2B: 16
                $P2E: 0,0
                $P2N: SSC-H
                $P2R: 1024
                $P2S: Side Scatter
                $P3B: 16
                $P3E: 4,0
                $P3N: FL1-H
                $P3R: 1024
                $P3S: 530 nm
                $P4B: 16
                $P4E: 4,0
                $P4N: FL2-H
                $P4R: 1024
                $P4S: 585 nm
                $P5B: 16
                $P5E: 4,0
                $P5N: FL3-H
                $P5R: 1024
                $P5S: >650 nm
                $PAR: 5
                $SYS: Macintosh System Software 7.6.1
                $TPT: 35253
```

**>** fcsinfo sample1.fcs2 --all-data

```
// header elided
Data

431           223           586           705           282
350           224           549           658           211
446           399           589           431           246
333           182           514           681           293
499           919           706           860           683
322           142           495           688           329
339           197           525           654           185
// more data...
```