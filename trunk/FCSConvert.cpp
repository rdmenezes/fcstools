#include"FCSIO.hpp"
#include"FCSTools.hpp"
#include<fstream>
#include<sstream>
#include<map>
#include<iostream>
#include<string>

void show_help ();

bool open_filenames (std::vector<FCSTools::FCS<std::size_t> >&,
		     std::vector<std::string> const& names);



int main (int argc, char *argv[])
{

  std::vector<std::string> args;
  for (int i=0; i<argc; ++i)
    args.push_back (argv[i]);

  if (1 == argc)
    {
      show_help ();
      return 0;
    }

  std::string Command = argv[1];

  if ("merge" == Command || "cat" == Command)
    {
      // fcsconvert merge file ... file out filename
      //    1        2      3            4    5
      if (args.size () < 5)
	{
	  std::cout << "Not enough parameters." << std::endl;
 	  return 1;
	}
      std::vector<std::string> filenames (args.begin () + 2, args.end () - 2);
      std::string outname = args.back ();
      std::vector<FCSTools::FCS<std::size_t> > fcses (filenames.size ());
      if (! open_filenames (fcses, filenames))
	return 1;
      FCSTools::FCS<std::size_t> result;
      try
	{
	  if ("merge" == Command)
	    FCSTools::merge (result, fcses);
	  else
	    FCSTools::cat (result, fcses);
	  std::fstream output (outname.c_str (), std::ios::out|std::ios::binary);
	  FCSTools::Writer (output, result);
	}
      catch (FCSTools::fcs_error ferr)
	{
	  std::cout << ferr.what () << std::endl;
	  return 1;
	}
    }
  else if ("datatype" == Command || "datatype!" == Command)
    {
    }
  else if ("width" == Command || "width!" == Command)
    {
    }
  else if ("trunc" == Command || "trunc!" == Command)
    {
    }
  else if ("resample" == Command || "resample!" == Command)
    {
    }
  else
    show_help ();

  return 0;
}

bool open_filenames (std::vector<FCSTools::FCS<std::size_t> >& fcses,
		     std::vector<std::string> const& filenames)
{
  for (std::size_t i=0; i<fcses.size (); ++i)
    {
      std::fstream fs (filenames[i].c_str (), std::ios::in|std::ios::binary);
      if (! fs)
	{
	  std::cout << "Could not open the file named \"" << filenames[i] << "\"!" << std::endl;
	  return false;
	}
      try
	{
	  FCSTools::FCS<std::size_t> fcs = FCSTools::Reader<std::size_t> (fs);
	  fcses[i] = fcs;
	}
      catch (FCSTools::fcs_error ferr)
	{
	  std::cout << ferr.what () << std::endl;
	  return false;
	}
    }
  return true;
}

void show_help ()
{
  std::cout << "fcsconvert command files... [out filename]" << std::endl;
  std::cout << "\tWhere command is one of: merge, cat, "
	    << "datatype, width, trunc, resample;" << std::endl
	    << "\tand \"files...\" is a list of files;" << std::endl
	    << "\tand \'filename\' is the name of the output file." << std::endl;
  std::cout << std::endl;
  std::cout << "\tmerge files... out filename:" << std::endl
	    << "\t  add the columns of the files into a single" << std::endl
	    << "\t  file whose name is \'filename\' only if" << std::endl
	    << "\t  the columns are the same length." << std::endl;
  std::cout << std::endl;
  std::cout << "\tcat files... out filename:" << std::endl
	    << "\t  append the files into a single file called" << std::endl
	    << "\t  \'filename\' only if they have the same number" << std::endl
	    << "\t  of rows." << std::endl;
  std::cout << std::endl;
  std::cout << "\tdatatype Type file out filename" << std::endl
	    << "\t ... or ..." << std::endl
	    << "\tdatatype! Type files..." << std::endl
	    << "\t  Change the Datatype to either Integer" << std::endl
	    << "\t  or ASCII (variable). The first form" << std::endl
	    << "\t  saves to the file named \'filename\' and" << std::endl
	    << "\t  the second form saves in place." << std::endl;
  std::cout << std::endl;
  std::cout << "\twidth N file out filename" << std::endl
	    << "\t ... or ..." << std::endl
	    << "\twidth! N files...:" << std::endl
	    << "\t  Change the number of bits (to N) that the Integer data"
	    << std::endl
	    << "\t  is stored in. The first form saves \'file\'" << std::endl
	    << "\t  in at \'filename\'. The second form saves in place."
	    << std::endl;
  std::cout << std::endl;
  std::cout << "\ttrunc N file out filename" << std::endl
	    << "\t ... or ..." << std::endl
	    << "\ttrunc! N files..." << std::endl
	    << "\t  Remove the first N elements from the dataset." << std::endl
	    << "\t  Alternately, -N removes the last N elements" << std::endl
	    << "\t  from the dataset. The first form saves to \'file\'" << std::endl
	    << "\t  and the second form saves in place." << std::endl;
  std::cout << std::endl;
  std::cout << "\tresample N K file out filename" << std::endl
	    << "\t ... or ..." << std::endl
	    << "\tresample! N K files..." << std::endl;
}
