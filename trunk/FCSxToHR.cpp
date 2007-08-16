#include<fstream>
#include"FCSTools.hpp"
#include"FCSIO.hpp"
#include"FCSHRIO.hpp"

int main (int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  std::fstream output (argv[2], std::ios::out|std::ios::binary);
  if (! output)
    return 3;

  FCSTools::FCS<std::size_t> fcs;
  FCSTools::HR::FCSHR hr;
  
  bool fcs2_failed = false, fchr_failed = false;
  std::string fcs2_msg, fchr_msg;

  try
    {
      std::fstream input (argv[1], std::ios::in|std::ios::binary);
      if (! input)
	return 2;
      fcs = FCSTools::Reader<std::size_t> (input);
    }
  catch (FCSTools::fcs_error const& ferr)
    {
      fcs2_failed = true;
      fcs2_msg = ferr.what ();
    }
  if (! fcs2_failed)
    {
      FCSTools::convert (fcs, hr);
      output << hr;
      return 0;
    }

  try
    {
      std::fstream input (argv[1], std::ios::in|std::ios::binary);
      if (! input)
	return 2;
      input >> hr;

      std::cout << hr << std::endl;
      return 0;

    }
  catch (FCSTools::fcs_error const& ferr)
    {
      fchr_failed = true;
      fchr_msg = ferr.what ();
    }
  if (! fchr_failed)
    {
      FCSTools::convert (hr, fcs);
      FCSTools::Writer (output, fcs);
      return 0;
    }

  std::cout << fcs2_msg << std::endl << fchr_msg << std::endl;

  return 5;
}
