#include<fstream>
#include"FCSTools.hpp"

int main (int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  std::fstream input (argv[1], std::ios::in|std::ios::binary);
  if (! input)
    return 2;
  std::fstream output (argv[2], std::ios::out|std::ios::binary);
  if (! output)
    return 3;

  FCSTools::FCS<std::size_t> fcs;
  try
    {
      fcs = FCSTools::Reader<std::size_t> ();
    }
  catch (FCSTools::fcs_error const& ferr)
    {
      std::cout << ferr.what () << std::endl;
    }
  
}
