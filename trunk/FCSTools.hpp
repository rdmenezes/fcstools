#ifndef ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2_TOOLS
#define ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2_TOOLS

#include"FCSIO.hpp"

namespace FCSTools
{

  struct merge_inconsistent_data_length : fcs_error {
    virtual const char* what () const throw () {
      return "Inconsistent data length while attempting to merge files";}
  };
  struct cat_inconsistent_row_length : fcs_error {
    virtual const char* what () const throw () {
      return "Inconsistent number of columns while attempting to concatenate files";}
  };

  template <typename ValueType>
  void merge (FCS<ValueType>& Result, std::vector<FCS<ValueType> > const& fcses)
  {
    // check data-length consistency
    if (0 == fcses.size ())
      return;
    const std::size_t DataLength = fcses[0].Data.size ();
    for (std::size_t i=0; i<fcses.size (); ++i)
      if (DataLength != fcses[i].Data.size ())
	throw merge_inconsistent_data_length ();

    Result.Head = fcses[0].Head; // simple-copy
    for (std::size_t i=1; i<fcses.size (); ++i)
      { // append parameters
	for (std::size_t j=0; j<fcses[i].Head.Parameter.size (); ++j)
	  Result.Head.Parameter.push_back (fcses[i].Head.Parameter[j]);
      }

    Result.Data = fcses[0].Data;

    for (std::size_t i=0; i<DataLength; ++i)
      for (std::size_t j=1; j<fcses.size (); ++j)
	for (std::size_t k=0; k<fcses[j].Data[i].size (); ++k)
	  Result.Data[i].push_back (fcses[j].Data[i][k]);
  }

  template <typename ValueType>
  void cat (FCS<ValueType>& Result, std::vector<FCS<ValueType> > const& fcses)
  {
    // check row-length consistency
    if (0 == fcses.size ())
      return;
    const std::size_t RowLength = fcses[0].Head.Parameter.size ();
    for (std::size_t i=0; i<fcses.size (); ++i)
      if (RowLength != fcses[i].Head.Parameter.size ())
	throw cat_inconsistent_row_length ();

    Result.Head = fcses[0].Head; // simple copy
    for (std::size_t i=0; i<fcses.size (); ++i)
      Result.Data.insert (Result.Data.end (), fcses[i].Data.begin (), fcses[i].Data.end ());
  }

}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2_TOOLS
