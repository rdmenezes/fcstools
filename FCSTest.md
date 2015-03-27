#fcstest

# FCSTest #

fcstest performs a "round-trip" on your data. That is:

**>** fcstest sample1.fcs

Results in a file being created called "Result.fcs2". This file should be (similar) if not identical to the input file. If it is not, please send us a bug report!

The code for fcstest is trivially short, so we will include it here and describe the parts of it:

```
#include<fstream>
#include"FCSIO.hpp"

int main (int argc, char *argv[])
{

  using namespace FCSTools;

  if (argc != 2)
    return 0;

  std::ifstream fcsfile (argv[1], std::ios::in|std::ios::binary);
  FCS<std::size_t> fcs = Reader<std::size_t> (fcsfile);
  
  std::ofstream ofcsfile ("Result.fcs2", std::ios::out|std::ios::binary);
  Writer (ofcsfile, fcs);

  return 0;
}
```

The line `using namespace FCSTools` tells the compiler that we will be using tools from the FCSTools namespace. In general this is a _very_ _bad_ _idea_ because the FCSTools namespace includes several functions which could cause the rest of your code to behave poorly.

The next two lines guard against invalid parameters.

With the line `std::ifstream fcsfile (...)` we open a _stream_---which is C++'s way of saying "a file"---in **binary** mode. Please note that the FCS- 2.0 and 3.0 formats require `binary` mode to function properly. The next line opens an FCS file---either 2.0 or 3.0. The data is stored in the `Data` portion of the FCS data-structure.

The next line, `std::ofstream ofcsfile (...)` opens an output file in `binary` mode. The `Writer` function is then used to write the FCS data-structure to disk.