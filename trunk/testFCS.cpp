#include<fstream>
#include"FCS2.hpp"


int main (int argc, char *argv[])
{
  if (argc != 2)
    return 0;

  std::ifstream fcsfile (argv[1], std::ios::in|std::ios::binary);
  FCSTools::Reader (fcsfile);

  return 0;
}
