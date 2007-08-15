#include<fstream>
#include<string>
#include<map>
#include<iomanip>
#include"FCSIO.hpp"

void show_help ();

int main (int argc, char *argv[])
{

  std::map<std::string,bool> flags;
  flags["--compliant-mode"] = false;
  flags["--extended-information"] = false;

  if (2 > argc)
    {
      show_help ();
      return 0;
    }

  if (std::string("--help") == argv[1])
    {
      show_help ();
      return 0;
    }

  for (std::size_t i=2; i<argc; ++i)
    if (flags.end () != flags.find (argv[i]))
      flags[argv[i]] = true;
    
  using namespace FCSTools;

  std::fstream file (argv[1], std::ios::binary|std::ios::in);
  try
    {
      FCS<std::size_t> fcs
	= Reader<std::size_t> (file, flags["--compliant-mode"]);
      std::cout << "Name:      " << argv[1] << std::endl
		<< "Events:    " << fcs.Data.size () << std::endl
		<< "Mode:      " << fcs.Head.Mode << std::endl
		<< "Datatype:  " << fcs.Head.Datatype << std::endl << std::endl
		<< "Column Information" << std::endl << std::endl;
      if (flags["--extended-information"])
	{
	  std::cout << "Byte Order:";
	  for (std::size_t i=0; i<fcs.Head.ByteOrder.size (); ++i)
	    std::cout << (0==i?" ":", ") << (fcs.Head.ByteOrder[i]+1);
	  std::cout << std::endl;
	  std::cout << "Nextdata:   ???" << std::endl;
	}
      for (std::size_t i=0; i<fcs.Head.Parameter.size (); ++i)
	{
	  std::cout << std::setw(3) << (i+1) << ")"
		    << "  Name:          " 
		    << std::setw(30) << fcs.Head.Parameter[i].Name
		    << "  BitSize:       "
		    << std::setw(4) << fcs.Head.Parameter[i].BitSize
		    << "  Range:         "
		    << std::setw(9) << fcs.Head.Parameter[i].Range << std::endl;
	  if (! flags["--extended-information"])
	    continue;
	  std::cout << "      Exponent:      ";
	  std::stringstream ssExp;
	  ssExp << fcs.Head.Parameter[i].Exponent.first
		<< ", " << fcs.Head.Parameter[i].Exponent.second;
	  std::cout << std::setw(30) << ssExp.str ()
		    << "  Specification: "
		    << std::setw(12) << fcs.Head.Parameter[i].Specification
		    << std::endl;
	}
      std::size_t SomeData = (6>fcs.Data.size ()?fcs.Data.size ():6);
      std::size_t FirstHalf = SomeData / 2;
      std::size_t SecondHalf = SomeData - FirstHalf;
      std::cout << std::endl << "Example Data" << std::endl << std::endl;
      for (std::size_t i=0; i<fcs.Head.Parameter.size (); ++i)
	{
	  std::stringstream col;
	  col << "(" << (i+1) << ")";
	  std::cout << std::setw (10) << col.str ();
	}
      std::cout << std::endl << std::endl;
      for (std::size_t i=0; i<FirstHalf; ++i)
	{
	  for (std::size_t j=0; j<fcs.Data[i].size (); ++j)
	    std::cout << std::setw (10) << fcs.Data[i][j];
	  std::cout << std::endl;
	}
      for (std::size_t i=0; i<fcs.Head.Parameter.size (); ++i)
	std::cout << std::setw(10) << "...";
      std::cout << std::endl;
      std::size_t DataSz = fcs.Data.size ();
      for (signed int i=SecondHalf; i>0; --i)
	{
	  for (std::size_t j=0; j<fcs.Data[DataSz-i].size (); ++j)
	    std::cout << std::setw (10) << fcs.Data[DataSz-i][j];
	  std::cout << std::endl;
	}
    }
  catch (fcs_error err)
    {
      std::cout << "Error reading file: " << err.what () << std::endl;
    }


}

void show_help ()
{
  std::cout << "fcsinfo file [flags]" << std::endl;
  std::cout << "\tWhere flags are:" << std::endl;
  std::cout << "\t--compliant-mode: force compliancy in reading" << std::endl;
  std::cout << "\t--extended-information: report extended information" << std::endl; 
}
