#ifndef ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2_TOOLS
#define ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2_TOOLS

#include<fcs/util.hpp>
#include<fcs/io.hpp>
#include<fcs/hrio.hpp>
#include<set>
#include<cstdlib>
#include<cmath>

namespace FCSTools
{

  struct merge_inconsistent_data_length : fcs_error {
    virtual const char* what () const throw () {
      return "Inconsistent data length while attempting to merge files"; }
  };
  struct cat_inconsistent_row_length : fcs_error {
    virtual const char* what () const throw () {
      return "Inconsistent number of columns while attempting to concatenate files";}
  };
	
  /*
    FCS Concept:
    FCS
      typedef ? head_type
      typedef ? data_type
      .Head
        typedef ? parameter_type
        .Parameter: RandomAccessContainer
	  .Name
	  .Range
	  .Scale
      .Data: RandomAccessContainer
        .: RandomAccessContainer
   */

  template <typename FCS>
  void merge (FCS& Result, std::vector<FCS> const& fcses)
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
	
  template <typename FCS>
  void cat (FCS& Result, std::vector<FCS> const& fcses)
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
      Result.Data.insert (Result.Data.end (),
													fcses[i].Data.begin (),
													fcses[i].Data.end ());
  }
	
  template <typename FCS>
  void split (std::vector<FCS>& Result, FCS const& fcs)
  {
    typedef typename FCS::data_type data_type;
    typedef typename data_type::value_type element_type;
    Result = std::vector<FCS> (fcs.Head.Parameter.size ());
    for (std::size_t i=0; i<Result.size (); ++i)
      {
				Result[i].Head = fcs.Head;
				Result[i].Head.Parameter.clear ();
				Result[i].Head.Parameter.push_back (fcs.Head.Parameter[i]);
				Result[i].Data = data_type (fcs.Data.size ());
      }
    for (std::size_t i=0; i<fcs.Data.size (); ++i)
      for (std::size_t j=0; j<fcs.Data[i].size (); ++j)
				Result[j].Data[i] = element_type (1, fcs.Data[i][j]);
  }
	
  template <typename FCS>
  void truncate (FCS& Result, FCS const& fcs, signed long N)
  {
    Result = fcs;
    truncate (Result, N);
  }
  template <typename FCS>
  void truncate (FCS& fcs, signed long N)
  {
    std::size_t aTrunc = std::abs (N);
    if (aTrunc > fcs.Data.size ())
      aTrunc = fcs.Data.size ();
    if (N > 0)
      fcs.Data.erase (fcs.Data.begin (), fcs.Data.begin () + aTrunc);
    else
      fcs.Data.erase (fcs.Data.end () - aTrunc, fcs.Data.end ());
  }
	
  template <typename FCS>
  void erase_columns (FCS& Result, FCS const& fcs,
											std::vector<std::size_t> const& Columns)
  {
    Result = fcs;
    erase_columns (Result, Columns);
  }
  template <typename FCS>
  void erase_columns (FCS& Result, std::vector<std::size_t> const& zColumns)
  {
    std::set<std::size_t> Columns (zColumns.begin (), zColumns.end ());
    
    typename FCS::parameter_type Parameter;
    for (std::size_t i=0; i<Result.Head.Parameter.size (); ++i)
      if (Columns.end () == Columns.find (i+1))
				Parameter.push_back (Result.Head.Parameter[i]);
    Result.Head.Parameter = Parameter;
		
    typename FCS::data_type Data (Result.Data.size ());
    for (std::size_t i=0; i<Result.Data.size (); ++i)
      for (std::size_t j=0; j<Result.Data[i].size (); ++j)
				if (Columns.end () == Columns.find (j+1))
					Data[i].push_back (Result.Data[i][j]);
    Result.Data = Data;
  }
	
  template <typename FCS>
  void convert (FCS const& fcs, HR::FCSHR& hr)
  {
    hr.Head.Parameter.clear ();
    hr.Data.clear ();
    for (std::size_t i=0; i<fcs.Head.Parameter.size (); ++i)
      {
				HR::FCSHR::ColumnDatum CD;
				CD.Name = fcs.Head.Parameter[i].Name;
				CD.Range = fcs.Head.Parameter[i].Range;
				CD.Scale = fcs.Head.Parameter[i].Scale;
				hr.Head.Parameter.push_back (CD);
      }
    convert (hr.Data, fcs.Data); 
  }
	
  /*
		std::string File;
    std::string Mode;
    std::string Datatype;
    std::string BeginTime;
    std::string EndTime;
    std::string System;
    std::string Cytometer;
    std::string Date;
    std::size_t Total;
    std::size_t Nextdata;
    byteorder_t ByteOrder;
	*/
	
  template <typename FCS, typename ValueType>
  void convert (FCS const& fcs, FCSTools::FCS<ValueType>& targ)
  {
    targ.Head.Parameter.clear ();
    targ.Data.clear ();
    targ.Head.File = "";
    targ.Head.Mode = "L";
    targ.Head.Datatype = "I";
    targ.Head.BeginTime = "";
    targ.Head.EndTime = "";
    targ.Head.System = "FCSTools";
    targ.Head.Cytometer = "FCSTools";
    targ.Head.Total = 0;
    targ.Head.Nextdata = 0;
    targ.Head.ByteOrder.clear ();
    const std::size_t ottf[] = {1, 2, 3, 4};
    targ.Head.ByteOrder.insert (targ.Head.ByteOrder.begin (), ottf, ottf+4);
    for (std::size_t i=0; i<fcs.Head.Parameter.size (); ++i)
      {
				NDatum CD;
				CD.Name = fcs.Head.Parameter[i].Name;
				CD.Range = fcs.Head.Parameter[i].Range;
				CD.Scale = fcs.Head.Parameter[i].Scale;
				CD.BitSize = sizeof(blessed_integral) * DEFACTO_BYTEL;
				CD.ByteOrder = targ.Head.ByteOrder;
				targ.Head.Parameter.push_back (CD);
      }
    convert (targ.Data, fcs.Data); 
  }
	
}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2_TOOLS
