#ifndef ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2
#define ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2

#include<cmath>
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<sstream>
#include<exception>

namespace FCSTools
{

  struct fcs_error : std::exception {};
  struct no_keyword : fcs_error {};
  struct no_keyword_fatal : no_keyword {};
  template <typename Base>
  struct T_no_nkeyword : Base {
    T_no_nkeyword (int n=0) : n_(n) {}
    virtual int N () const { return this->n_; }
    int n_;
  };
  typedef T_no_nkeyword<no_keyword> no_nkeyword;
  typedef T_no_nkeyword<no_keyword_fatal> no_nkeyword_fatal;
  
  struct invalid_bit_length : fcs_error {
    virtual const char* what () const throw () {
      return "Invalid bit length for parameter"; }
  };
  struct uncorrelated_mode : fcs_error {
    virtual const char* what () const throw () {
      return "Uncorrelated mode is unsupported"; }
  };
  struct correlated_mode : fcs_error {
    virtual const char* what () const throw () {
      return "Correlated mode is unsupported"; }
  };

  // COMPLIANT KEYWORDS
  struct no_total_keyword : no_keyword {
    virtual const char* what () const throw () {
      return "TOT"; }
  };
  struct no_nextdata_keyword : no_keyword {
    virtual const char* what () const throw () {
      return "NEXTDATA"; }
  };

  // FATAL KEYWORDS
  struct no_datatype_keyword : no_keyword_fatal {
    virtual const char* what () const throw () {
      return "DATATYPE"; }
  };
  struct no_byteorder_keyword : no_keyword_fatal {
    virtual const char* what () const throw () {
      return "BYTEORDER"; }
  };
  struct no_mode_keyword : no_keyword_fatal {
    virtual const char* what () const throw () {
      return "MODE"; }
  };
  struct no_par_keyword : no_keyword_fatal {
    virtual const char* what () const throw () {
      return "PAR"; }
  };
  struct no_bitsize_keyword : no_nkeyword_fatal {
    no_bitsize_keyword (int n=0) : no_nkeyword_fatal (n) {}
    virtual const char* what () const throw () {
      return "PNB"; }
  };
  struct no_range_keyword : no_nkeyword_fatal {
    no_range_keyword (int n=0) : no_nkeyword_fatal (n) {}
    virtual const char* what () const throw () {
      return "PNR"; }
  };

  template <typename T>
  T convert (std::string const& str)
  {
    std::stringstream ss(str);
    T t;
    ss >> t;
    return t;
  }
  
  template <typename T>
  std::ostream& operator << (std::ostream& ostr, std::vector<T> const& V)
  {
    for (std::size_t i=0; i<V.size(); ++i)
      ostr << V[i] << " ";
    return ostr;
  }
  
  typedef std::vector<std::size_t> byteorder_t;
  typedef std::vector<byteorder_t> byteorders_t;
  
  typedef std::vector<signed long> integral_vector_t;
  typedef std::vector<integral_vector_t> integral_vector_list_t;

  typedef std::vector<long double> ldouble_vector_t;
  typedef std::vector<ldouble_vector_t> ldouble_vector_list_t;
  
  integral_vector_list_t
  mode_L_integral (const unsigned char* cData,
		   const unsigned char* cEnd,
		   byteorders_t const& ByteOrders)
  {
    const std::size_t bordsz = ByteOrders.size ();
    std::size_t veclength = 0;
    byteorders_t nByteOrders (bordsz);
    for (std::size_t i=0; i<bordsz; ++i)
      for (std::size_t j=0; j<ByteOrders[i].size (); ++j, ++veclength)
	nByteOrders[i].push_back (1<<(ByteOrders[i][j]*CHAR_BIT));
    
    const std::size_t nElements = (cEnd-cData) / veclength;
    
    integral_vector_list_t ivl (nElements);
    std::size_t element = 0;
    while (element < nElements)
      {
	integral_vector_t iv (bordsz, 0);
	for (std::size_t i=0; i<bordsz; ++i)
	  {
	    signed long sl = 0;
	    for (std::size_t j=0; j<nByteOrders[i].size (); ++j, ++cData)
	      sl += nByteOrders[i][j] * *cData;
	    iv[i] = sl;
	  }
	ivl[element] = iv;
	++element;
      }
    return ivl;
  }
  
  integral_vector_list_t
  mode_A_variable (const unsigned char* cData,
		   const unsigned char* cEnd,
		   std::size_t Dimension)
  {
    std::string sData (cData, cEnd);
    std::stringstream ss (sData);
    integral_vector_list_t ivl;
    integral_vector_t iv (Dimension, 0);
    bool loop = true;
    while (loop)
      {
	for (std::size_t i=0; i<Dimension; ++i)
	  {
	    ss >> iv[i];
	    if (! ss)
	      {
		loop = false;
		break;
	      }
	  }
	if (loop)
	  ivl.push_back (iv);
      }
    return ivl;
  }
  
  template <typename LValueType, typename RValueType>
  void
  convert (std::vector< std::vector<LValueType> >& Result,
	   std::vector< std::vector<RValueType> > const& Data)
  {
    const std::size_t DSize = Data.size ();
    Result = std::vector< std::vector<LValueType> >(DSize);
    for (std::size_t i=0; i<DSize; ++i)
      for (std::size_t j=0; j<Data[i].size (); ++j)
    	Result[i].push_back ((LValueType)Data[i][j]);
  }

  template <typename ValueType>
  std::pair<std::map<std::string,std::string>,
	    std::vector< std::vector<ValueType> > >
  Reader (std::istream& FCSFile, bool Compliance_P = false)
  {
    typedef std::vector<ValueType> ElementT;
    typedef std::vector<ElementT> VElementT;

    VElementT Result;

    std::string FCSKind;
    std::size_t KeysBegin, KeysEnd, KeySection, DataBegin, DataEnd, DataSection;
    FCSFile >> FCSKind >> KeysBegin >> KeysEnd >> DataBegin >> DataEnd;
    KeySection = KeysEnd - KeysBegin + 1;
    DataSection = DataEnd - DataBegin + 1;
    
    FCSFile.seekg (KeysBegin);
    
    char Delimiter = (char)FCSFile.peek ();
    char cKeyWordBuffer[KeySection+2];
    FCSFile.get (cKeyWordBuffer, KeySection+1, 0);
    std::string KeyWordBuffer (cKeyWordBuffer+1, KeySection+1);
    
    std::stringstream ss (KeyWordBuffer);
    std::map<std::string,std::string> keywords;
    do
      {
	std::string key, value;
	std::getline (ss, key, Delimiter);
	std::getline (ss, value, Delimiter);
	keywords[key] = value;
      }
    while (ss);
    
    FCSFile.seekg (DataBegin);
    
    unsigned char cDataBuffer[DataSection+12];
    FCSFile.read ((char*)cDataBuffer, DataSection+1);
    
    std::size_t nParameters = 0;
    if (keywords.end () != keywords.find ("$PAR"))
      nParameters = convert<std::size_t>(keywords["$PAR"]);
    else
      throw no_par_keyword ();

    std::size_t Nextdata = 0;
    if (keywords.end () != keywords.find ("$NEXTDATA"))
      Nextdata = convert<std::size_t>(keywords["$NEXTDATA"]);
    else if (Compliance_P)
      throw no_nextdata_keyword ();

    std::string System;
    if (keywords.end () != keywords.find ("$SYSTEM"))
      System = keywords["$SYS"];

    std::string BeginTime;
    if (keywords.end () != keywords.find ("$BTIM"))
      BeginTime = keywords["$BTIM"];

    std::string EndTime;
    if (keywords.end () != keywords.find ("$ETIM"))
      EndTime = keywords["$ETIM"];

    std::string File;
    if (keywords.end () != keywords.find ("$FIL"))
      File = keywords["$FIL"];

    std::string Date;
    if (keywords.end () != keywords.find ("$DATE"))
      Date = keywords["$DATE"];

    std::vector<std::size_t> BitSize (nParameters, 0);
    std::vector<std::size_t> Range (nParameters, 0);
    std::vector<std::string> Name (nParameters, "");
    std::vector<std::pair<double,double> > Exponent (nParameters);
    std::vector<std::string> Source (nParameters, "");
    
    std::size_t stdBitSize = 0;

    bool Variable = false;
    
    for (std::size_t npar = 0; npar < nParameters; ++npar)
      {
	std::stringstream par;
	par << "$P" << (npar+1) << "B";
	std::string key;
	par >> key;
	if (keywords.end () != keywords.find (key))
	  {
	    if ("*" == keywords[key])
	      {
		Variable = true;
		BitSize[npar] = 0; // variable bit-size
	      }
	    else
	      BitSize[npar] = convert<std::size_t>(keywords[key]);
	  }
	else
	  throw no_bitsize_keyword (npar);
	if ((stdBitSize%CHAR_BIT) != 0)
	  throw invalid_bit_length ();
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "R";
	par >> key;
	if (keywords.end () != keywords.find (key))
	  Range[npar] = convert<std::size_t>(keywords[key]);
	else
	  throw no_range_keyword (npar);
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "N";
	par >> key;
	if (keywords.end () != keywords.find (key))
	  Name[npar] = keywords[key];
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "E";
	par >> key;
	if (keywords.end () != keywords.find (key))
	  {
	    std::stringstream exp (keywords[key]);
	    double L, R;
	    std::string sL, sR;
	    std::getline (exp, sL, ',');
	    std::getline (exp, sR);
	    L = convert<double>(sL);
	    R = convert<double>(sR);
	  }
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "S";
	par >> key;
	if (keywords.end () != keywords.find (key))
	  Source[npar] = keywords[key];
	par.clear ();
	par.str ("");
      }
    
    std::stringstream byteorder;
    if (keywords.end () != keywords.find ("$BYTEORD"))
      byteorder << keywords["$BYTEORD"];
    else
      throw no_byteorder_keyword ();
    std::vector<std::size_t> preByteOrder;
    while (true)
      {
	std::string byteo;
	std::getline (byteorder, byteo, ',');
	if (! byteorder)
	  break;
	preByteOrder.push_back (convert<std::size_t>(byteo)-1);
      }
    
    std::vector<std::vector<std::size_t> > ByteOrders (nParameters);
    for (std::size_t i=0; i<nParameters; ++i)
      for (std::size_t j=0; j<preByteOrder.size (); ++j)
	if (preByteOrder[j] < BitSize[i]/CHAR_BIT)
	  ByteOrders[i].push_back (preByteOrder[j]);
    
    std::size_t Total = 0;
    if (keywords.end () != keywords.find ("$TOT"))
      Total = convert<std::size_t>(keywords["$TOT"]);
    else if (Compliance_P)
      throw no_total_keyword ();
    
    // legal: [L]ist, [U]ncorrelated, [C]orrelated
    std::string Mode;
    if (keywords.end () != keywords.find ("$MODE"))
      Mode = keywords["$MODE"];
    else
      throw no_mode_keyword ();
    
    // legal: [A]SCII, [I]nteger, [D]ouble, [F]loat
    std::string Datatype;
    if (keywords.end () != keywords.find ("$DATATYPE"))
      Datatype = keywords["$DATATYPE"];
    else
      throw no_datatype_keyword ();
    if ("A" == Datatype && Variable)
      Datatype = "*";
    
    std::string Cytometer;
    if (keywords.end () != keywords.find ("$CYT"))
      Cytometer = keywords["$CYT"];
     
    if ("L" == Mode)
      { // list of vectors
	// we grab up to, but no more than Total
	integral_vector_list_t ivl;
	switch (Datatype[0])
	  {
	  case 'I': ivl
	      = mode_L_integral (cDataBuffer,
				 cDataBuffer+DataSection+1,
				 ByteOrders);
	    break;
	  case '*': ivl
	      = mode_A_variable (cDataBuffer,
				 cDataBuffer+DataSection+1,
				 nParameters);
	    break;
	  default : break;
	  }
	convert (Result, ivl);
      }
    else if ("U" == Mode)
      { // histograms of ranged-size(???)
	throw uncorrelated_mode ();
      }
    else if ("C" == Mode)
      { // histograms of ranged-size(???)
	throw correlated_mode ();
      }

    return std::make_pair (keywords, Result);
  }

}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2
