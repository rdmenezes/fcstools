#include"FCS2.hpp"
#include<fstream>
#include<sstream>
#include<map>
#include<iostream>
#include<string>

void show_help ();

int main (int argc, char *argv[])
{

  if (1 == argc)
    {
      show_help ();
      return 0;
    }

  return 0;
}

void show_help ()
{
  std::cout << "fcsconvert command files... [out filename]" << std::endl;
  std::cout << "\tWhere command is one of: merge, cat, "
	    << "datatype, width, trunc;" << std::endl
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
	    << "\t  \'filename\' only if they have the same number"
	    << "\t  of rows." << std::endl;
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
}
