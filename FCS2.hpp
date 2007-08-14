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
  
  struct NDatum
  {
    std::string Name, Specification;
    std::size_t BitSize, Range;
    std::pair<double,double> Exponent;
    byteorder_t ByteOrder;
  };
  typedef std::vector<NDatum> NData;

  struct Header
  {
    std::string File, Mode, Datatype,
      BeginTime, EndTime, System,
      Cytometer, Date;
    std::size_t Total, Nextdata;
    byteorder_t ByteOrder;
    NData Parameter;
    std::map<std::string,std::string> AllKeywords;
    bool has_keyword (std::string const& key) const {
      return this->AllKeywords.end () != this->AllKeywords.find (key); }
    std::string& operator [] (std::string const& key) {
      return this->AllKeywords[key]; }
  };

  integral_vector_list_t
  mode_L_integral (const unsigned char* cData,
		   const unsigned char* cEnd,
		   Header const& Data)
  {
    const std::size_t bordsz = Data.Parameter.size ();
    std::size_t veclength = 0;
    byteorders_t nByteOrders (bordsz);
    for (std::size_t i=0; i<bordsz; ++i)
      for (std::size_t j=0; j<Data.Parameter[i].ByteOrder.size (); ++j, ++veclength)
	nByteOrders[i].push_back (1<<(Data.Parameter[i].ByteOrder[j]*CHAR_BIT));
    
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
		   Header const& Data)
  {
    const std::size_t Dimension = Data.Parameter.size ();
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
  struct FCS
  {
    Header Head;
    std::vector<std::vector<ValueType> > Data;
  };

  template <typename ValueType>
  bool Writer (std::ostream& FCSFile, FCS<ValueType> const& fcs)
  {
    // first, compute how long the data is going to be
    std::string VariableString;
    std::size_t DataLength;
    if ("*" == fcs.Head.Mode)
      {
	std::stringstream ss;
	for (std::size_t i=0; i<fcs.Data.size (); ++i)
	  for (std::size_t j=0; j<fcs.Data[i].size (); ++j)
	    ss << fcs.Data[i][j] << " ";
	VariableString = ss.str ();
	DataLength = VariableString.size ();
      }
    if ("I" == fcs.Head.Mode)
      { // precompute the length
	// we ignore user pleas for byte-lengths or whatnot
	DataLength = sizeof(signed int) * fcs.Data.size () * fcs.Head.Parameter.size ();
      }
    unsigned char Data[DataLength];
    if ("*" == fcs.Head.Mode)
      for (std::size_t i=0; i<DataLength; ++i)
	Data[i] = VariableString[i];
    if ("I" == fcs.Head.Mode)
      {
	std::size_t Cursor = 0;
	for (std::size_t i=0; i<fcs.Data.size (); ++i)
	  for (std::size_t j=0; j<fcs.Data[i].size (); ++j, ++Cursor)
	    {
	      unsigned int val = fcs.Data[i][j];
	      // finish here...
	    }
      }
    throw std::exception ();
  }

  template <typename ValueType>
  FCS<ValueType>
  Reader (std::istream& FCSFile, bool Compliance_P = false)
  {
    typedef std::vector<ValueType> ElementT;
    typedef std::vector<ElementT> VElementT;

    FCS<ValueType> fcs;
    Header& Head = fcs.Head;
    VElementT& Data = fcs.Data;

    std::string FCSKind;
    std::size_t KeysBegin, KeysEnd, KeySection, HeadBegin, HeadEnd, HeadSection;
    FCSFile >> FCSKind >> KeysBegin >> KeysEnd >> HeadBegin >> HeadEnd;
    KeySection = KeysEnd - KeysBegin + 1;
    HeadSection = HeadEnd - HeadBegin + 1;
    
    FCSFile.seekg (KeysBegin);
    
    char Delimiter = (char)FCSFile.peek ();
    char cKeyWordBuffer[KeySection+2];
    FCSFile.get (cKeyWordBuffer, KeySection+1, 0);
    std::string KeyWordBuffer (cKeyWordBuffer+1, KeySection+1);
    
    std::stringstream ss (KeyWordBuffer);
    do
      {
	std::string key, value;
	std::getline (ss, key, Delimiter);
	std::getline (ss, value, Delimiter);
	Head[key] = value;
      }
    while (ss);
    
    FCSFile.seekg (HeadBegin);
    
    unsigned char cHeadBuffer[HeadSection+12];
    FCSFile.read ((char*)cHeadBuffer, HeadSection+1);
    
    if (Head.has_keyword ("$PAR"))
      Head.Parameter = NData(convert<std::size_t>(Head["$PAR"]));
    else
      throw no_par_keyword ();

    const std::size_t nParameters = Head.Parameter.size ();

    if (Head.has_keyword ("$NEXTDATA"))
      Head.Nextdata = convert<std::size_t>(Head["$NEXTDATA"]);
    else if (Compliance_P)
      throw no_nextdata_keyword ();

    if (Head.has_keyword ("$SYSTEM"))
      Head.System = Head["$SYS"];

    if (Head.has_keyword ("$BTIM"))
      Head.BeginTime = Head["$BTIM"];

    if (Head.has_keyword ("$ETIM"))
      Head.EndTime = Head["$ETIM"];

    if (Head.has_keyword ("$FIL"))
      Head.File = Head["$FIL"];

    if (Head.has_keyword ("$DATE"))
      Head.Date = Head["$DATE"];

    bool Variable = false;
    
    for (std::size_t npar = 0; npar < nParameters; ++npar)
      {
	std::stringstream par;
	par << "$P" << (npar+1) << "B";
	std::string key;
	par >> key;
	if (Head.has_keyword (key))
	  {
	    if ("*" == Head[key])
	      {
		Variable = true;
		Head.Parameter[npar].BitSize = 0; // variable bit-size
	      }
	    else
	      Head.Parameter[npar].BitSize = convert<std::size_t>(Head[key]);
	  }
	else
	  throw no_bitsize_keyword (npar);
	if ((Head.Parameter[npar].BitSize%CHAR_BIT) != 0)
	  throw invalid_bit_length ();
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "R";
	par >> key;
	if (Head.has_keyword (key))
	  Head.Parameter[npar].Range = convert<std::size_t>(Head[key]);
	else
	  throw no_range_keyword (npar);
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "N";
	par >> key;
	if (Head.has_keyword (key))
	  Head.Parameter[npar].Name = Head[key];
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "E";
	par >> key;
	if (Head.has_keyword (key))
	  {
	    std::stringstream exp (Head[key]);
	    double L, R;
	    std::string sL, sR;
	    std::getline (exp, sL, ',');
	    std::getline (exp, sR);
	    L = convert<double>(sL);
	    R = convert<double>(sR);
	    Head.Parameter[npar].Exponent = std::make_pair (L, R);
	  }
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "S";
	par >> key;
	if (Head.has_keyword (key))
	  Head.Parameter[npar].Specification = Head[key];
	par.clear ();
	par.str ("");
      }
    
    std::stringstream byteorder;
    if (Head.has_keyword ("$BYTEORD"))
      byteorder << Head["$BYTEORD"];
    else
      throw no_byteorder_keyword ();

    while (true)
      {
	std::string byteo;
	std::getline (byteorder, byteo, ',');
	if (! byteorder)
	  break;
	Head.ByteOrder.push_back (convert<std::size_t>(byteo)-1);
      }
    
    for (std::size_t i=0; i<nParameters; ++i)
      for (std::size_t j=0; j<Head.ByteOrder.size (); ++j)
	if (Head.ByteOrder[j] < Head.Parameter[i].BitSize/CHAR_BIT)
	  Head.Parameter[i].ByteOrder.push_back (Head.ByteOrder[j]);
    
    if (Head.has_keyword ("$TOT"))
      Head.Total = convert<std::size_t>(Head["$TOT"]);
    else if (Compliance_P)
      throw no_total_keyword ();
    
    // legal: [L]ist, [U]ncorrelated, [C]orrelated
    if (Head.has_keyword ("$MODE"))
      Head.Mode = Head["$MODE"];
    else
      throw no_mode_keyword ();
    
    // legal: [A]SCII, [I]nteger, [D]ouble, [F]loat
    if (Head.has_keyword ("$DATATYPE"))
      Head.Datatype = Head["$DATATYPE"];
    else
      throw no_datatype_keyword ();
    if ("A" == Head.Datatype && Variable)
      Head.Datatype = "*";
    
    if (Head.has_keyword ("$CYT"))
      Head.Cytometer = Head["$CYT"];
     
    if ("L" == Head.Mode)
      { // list of vectors
	// we grab up to, but no more than Total
	integral_vector_list_t ivl;
	switch (Head.Datatype[0])
	  {
	  case 'I': ivl
	      = mode_L_integral (cHeadBuffer,
				 cHeadBuffer+HeadSection+1,
				 Head);
	    break;
	  case '*': ivl
	      = mode_A_variable (cHeadBuffer,
				 cHeadBuffer+HeadSection+1,
				 Head);
	    break;
	  default : break;
	  }
	convert (Data, ivl);
      }
    else if ("U" == Head.Mode)
      { // histograms of ranged-size(???)
	throw uncorrelated_mode ();
      }
    else if ("C" == Head.Mode)
      { // histograms of ranged-size(???)
	throw correlated_mode ();
      }

    return fcs;
  }

}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2
