#include<fstream>
#include"FCS2.hpp"


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
