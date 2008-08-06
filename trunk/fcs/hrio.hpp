#ifndef ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCSHR_IO
#define ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCSHR_IO

#include<iostream>
#include<sstream>
#include<string>
#include<exception>
#include<fcs/util.hpp>

namespace FCSTools
{

  namespace HR
  {
    /*
      FORMAT:
      Overall it must be:
      Sigil
      Section Head
        -- head information
      Section Data
        -- data information
      Section Analysis
        -- analysis information
      
      FCS:H.R # this sigil tells us this is a Human-Readable FCS file
      Section Head
      # comments begin with `#' and go to the end of
      # the  line; they are ignored
      
      # whitespace lines are also ignored
      # <name> is any string beginning with a non-whitespace
      # human-readable character, and terminating with return
      # carriage '\n'; we note that '\r\n' is treated as
      # two characters, but that '\r' is whitespace
      
      # A <number> is in integral or double format
      ColumnBegin
        # indentation is ignored
        Name: <name>
        Range: <number>
        Scale: <number>, <number>
      ColumnEnd
      Section Data
      # Data must have K columns:
      # one for each column-section as defined above
      # each row represents one element in the list
      # there may be comments in the data-section
      # for instance, vendor specific information
      # can be embedded in the data by using comments
      <number> <number> ... <number>
      # next element is special
      <number> <number> ... <number>
      ...      ...          ...
      <number> <number> ... <number>
      Section Analysis
      # this section is vendor-specific (for now)

   */

    struct FCSHR
    {
      struct ColumnDatum
      {
	std::string Name;
	std::size_t Range;
	std::pair<std::size_t,std::size_t> Scale;
      };
      typedef std::vector<ColumnDatum> ColumnData;
      struct Header
      {
	typedef ColumnData parameter_type;
	ColumnData Parameter;
      };
      
      typedef Header head_type;
      typedef dataset data_type;

      Header Head;
      dataset Data;
      void Reader (std::istream& file);
      friend std::istream& operator >> (std::istream& file, FCSHR& hr)
      {
	hr.Reader (file);
	return file;
      }
      void Writer (std::ostream& file) const;
      friend std::ostream& operator << (std::ostream& file, FCSHR const& hr)
      {
	hr.Writer (file);
	return file;
      }
    };

    void FCSHR::Writer (std::ostream& file) const
    {
      file << "FCS:H.R" << std::endl
	   << "Section Head" << std::endl;
      for (std::size_t i=0; i<this->Head.Parameter.size (); ++i)
	{
	  file << "ColumnBegin" << std::endl
	       << "  Name: " << this->Head.Parameter[i].Name << std::endl
	       << "  Range: " << this->Head.Parameter[i].Range << std::endl
	       << "  Scale: " << this->Head.Parameter[i].Scale.first
	       << ", " << this->Head.Parameter[i].Scale.second << std::endl
	       << "ColumnEnd" << std::endl;
	}
      file << "Section Data" << std::endl;
      for (std::size_t i=0; i<this->Data.size (); ++i)
	for (std::size_t j=0; j<this->Data[i].size (); ++j)
	  file << (0==j?"\n":" ") << this->Data[i][j];
    }

    void FCSHR::Reader (std::istream& file)
    {

      this->Head.Parameter.clear ();
      this->Data.clear ();

      // rolling interpretation
      const std::size_t Uninitialized = 0;
      const std::size_t Sigil = 1;
      const std::size_t Head = 2;
      const std::size_t Data = 3;
      const std::size_t Analysis = 4;

      std::size_t State = Uninitialized;

      std::string line;

      bool Column = false;

      while (std::getline (file, line))
	{
	  // strip off any comments
	  std::size_t comment = line.find_first_of ('#');
	  if (std::string::npos != comment)
	    line = line.substr (0, comment);
	  std::stringstream ssLine (line);
	  std::string nonWs;
	  ssLine >> nonWs;
	  std::string lnonWs = lower (nonWs);
	  if (Column)
	    {
	      if ("name:" == lnonWs)
		{
		  std::string Name;
		  std::size_t pos = line.find_first_of (':');
		  pos = line.find_first_not_of (" \t\v", pos+1);
		  if (std::string::npos != pos)
		    Name = line.substr (pos, line.size ());

		  this->Head.Parameter.back ().Name = Name;
		}
	      else if ("range:" == lnonWs)
		{
		  std::size_t Range;
		  ssLine >> Range;

		  this->Head.Parameter.back ().Range = Range;
		}
	      else if ("scale:" == lnonWs)
		{
		  std::string comma;
		  std::size_t Scale1, Scale2;
		  ssLine >> Scale1 >> comma >> Scale2;

		  this->Head.Parameter.back ().Scale
		    = std::make_pair (Scale1, Scale2);
		}
	      else if ("columnend" == lnonWs)
		{
		  Column = false;
		}
	    }
	  else if ('#' == lnonWs[0])
	    {
	      continue;
	    }
	  else if ("fcs:h.r" == lnonWs)
	    {
	      State = Sigil;
	      continue;
	    }
	  else if ("section" == lnonWs)
	    {
	      std::string WhichSection;
	      ssLine >> WhichSection;
	      std::string lWS = lower (WhichSection);
	      if ("head" == lWS)
		{
		  if (Sigil == State)
		    State = Head;
		  else
		    throw improper_state_in_file ();
		}
	      else if ("data" == lWS)
		{
		  if (Head == State)
		    State = Data;
		  else
		    throw improper_state_in_file ();
		}
	      else if ("analysis" == lWS)
		{
		  if (Data == State)
		    State = Analysis;
		  else
		    throw improper_state_in_file ();
		}
	    }
	  else if ("columnbegin" == lnonWs)
	    {
	      if (Head == State)
		Column = true;
	      else
		throw improper_state_in_file ();
	      this->Head.Parameter.push_back (ColumnDatum ());
	    }
	  else if (Data == State && lnonWs.size () > 0)
	    {
	      std::stringstream ssData (line);
	      // get data
	      element Element (this->Head.Parameter.size ());
	      for (std::size_t i=0; i<Element.size (); ++i)
		ssData >> Element[i];
	      this->Data.push_back (Element);
	    }
	}
    }

  }

}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCSHR_IO
