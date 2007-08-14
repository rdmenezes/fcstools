#include<fstream>
#include"FCS2.hpp"


int main (int argc, char *argv[])
{

  using namespace FCSTools;

  if (argc != 2)
    return 0;

  std::ifstream fcsfile (argv[1], std::ios::in|std::ios::binary);
  std::cout << Reader<std::size_t> (fcsfile).second << std::endl;

  return 0;
}
