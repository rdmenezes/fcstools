#ifndef ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCSHR_IO
#define ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCSHR_IO

#include<iostream>
#include<sstream>
#include<string>
#include<exception>

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

    struct fcshr_error : std::exception {
      virtual const char* what () const throw () {
	return "FCS:H.R error";}
    };
    struct improper_state_in_file : fcshr_error {
      virtual const char* what () const throw () {
	return "Improper state in file";}
    };

    typedef std::vector<double> element;
    typedef std::vector<element> dataset;

    std::string lower (std::string const& str)
    {
      std::string result (str);
      for (std::size_t i=0; i<result.size (); ++i)
	result[i] = std::tolower (result[i]);
      return result;
    }

    struct TrivialHandler
    {
      void operator () (std::string const& line) const
      {
      }
    };

    struct ColumnDatum
    {
      std::string Name;
      double Range;
      std::pair<double,double> Scale;
    };
    typedef std::vector<ColumnDatum> ColumnData;

    struct FCSHR
    {
      ColumnData Columns;
      dataset Data;
      template <typename LineHandler>
      istream& Reader (std::istream&, LineHandler&);
      std::istream& Reader (std::istream& file)
      {
	TrivialHandler TheLineHandler;
	return file (istream, TheLineHandler);
      }
      friend std::istream& operator >> (std::istream& file)
      {
	this->Reader (file);
	return file;
      }
    };

    template <typename LineHandler>
    void FCSHR::Reader (std::istream& file, LineHandler& Handler)
    {

      this->Columns.clear ();
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
		  pos = line.find_first_not_of (" \t\v", pos);
		  if (std::npos != pos)
		    Name = line.substr (pos, line.size ());

		  this->Columns.back ().Name = Name;
		}
	      else if ("range:" == lnonWs)
		{
		  double Range;
		  ssLine >> Range;

		  this->Columns.back ().Range = Range;
		}
	      else if ("scale:" == lnonWs)
		{
		  std::string comma;
		  double Scale1, Scale2;
		  ssLine >> Scale1 >> comma >> Scale2;

		  this->Columns.back ().Scale = std::make_pair (Scale1, Scale2);
		}
	      else if ("columnend" == lnonWs)
		{
		  Column = false;
		}
	    }
	  else if ('#' == lnonWs)
	    {
	      Handler (line);
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
	      this->Columns.push_back (ColumnDatum ());
	    }
	  else if (Data == State)
	    {
	      std::stringstream ssData (line);
	      // get data
	      element Element (this->Columns.size ());
	      for (std::size_t i=0; i<Element.size (); ++i)
		ssData >> Element[i];
	    }
	}
      throw file;
    }

  }

}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCSHR_IO
