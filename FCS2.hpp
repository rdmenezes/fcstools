#ifndef ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2
#define ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2

#include<cmath>
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<sstream>
#include<exception>
#include<iomanip>

namespace FCSTools
{

  // ERROR MESSAGES
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
  struct invalid_datatype : fcs_error {
    virtual const char* what () const throw () {
      return "Invalid datatype"; }
  };
  
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
  struct float_datatype : invalid_datatype {
    virtual const char* what () const throw () {
      return "Float is an invalid datatype";}
  };
  struct double_datatype : invalid_datatype {
    virtual const char* what () const throw () {
      return "Double is an invalid datatype";}
  };
  struct fixed_ascii_datatype : invalid_datatype {
    virtual const char* what () const throw () {
      return "Fixed ASCII is an invalid datatype";}
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

  // String2T conversion
  template <typename T>
  T convert (std::string const& str)
  {
    std::stringstream ss(str);
    T t;
    ss >> t;
    return t;
  }
  
  /* // DEBUGGING UTILITY TO PRINT VECTORS
  template <typename T>
  std::ostream& operator << (std::ostream& ostr, std::vector<T> const& V)
  {
    for (std::size_t i=0; i<V.size(); ++i)
      ostr << V[i] << " ";
    return ostr;
  }
  */
  
  // ByteOrder stores the arbitrary byte-order
  // of the FCS file. 1234 is x86; 4321 is PPC
  // 2143 is Sparc, etc.
  typedef std::vector<std::size_t> byteorder_t;
  typedef std::vector<byteorder_t> byteorders_t;
  
  // shorthand
  typedef unsigned long blessed_integral;
  typedef long double blessed_float;

  typedef std::vector<blessed_integral> integral_vector_t;
  typedef std::vector<integral_vector_t> integral_vector_list_t;

  typedef std::vector<long double> ldouble_vector_t;
  typedef std::vector<ldouble_vector_t> ldouble_vector_list_t;
  
  // Some data in the FCS format is specified
  // as `N'; that is, it is defined for each parameter
  //
  // Name $PnN: name of the column (in List mode)
  // Specification $PnS: light-source
  // BitSize $PnB: number of bits (bytes) used
  //   to store this column's data
  // Range $PnR: specifies the data-range from [0,value(PnR)]
  // Exponent $PnE: decade range of the data, 0,0 is linear
  // ByteOrder ---: local ordering of the bytes to ease
  //   reading the data
  struct NDatum
  {
    std::string Name;
    std::string Specification;
    std::size_t BitSize;
    std::size_t Range;
    std::pair<double,double> Exponent;
    byteorder_t ByteOrder;
  };
  typedef std::vector<NDatum> NData;

  // Header is the Key/Value section of the FCS file
  //
  // File $FIL: original name of the file for this data
  // Mode $MODE: List, Uncorrelated, Correlated; only List
  //   is supported and used (de facto)
  // Datatype $DATATYPE: ASCII, Integer, Double, Float;
  //   we support (Variable) ASCII [when $PnB is a '*'],
  //   and Integer
  // Begin/EndTime $BTIM, $ETIM: start and stop of initial
  //   data acquisition
  // System $SYS: operating system the data was acquired
  //   under
  // Cytometer(?) $CYT: machine the data was acquired with
  // Date $DATE: day (unknown format) data was acquired on
  // Total $TOT: the total number of events in the file
  // Nextdata $NEXTDATA: offset to the next data set in the
  //   file
  // ByteOrder $BYTEORDER: permutation (ordering) of the
  //   bytes for Integer
  // Parameter: the per-parameter data
  // AllKeywords: optional/unknown/vender-specific keywords
  //   in addition to those above
  struct Header
  {
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
    NData Parameter;
    std::map<std::string,std::string> AllKeywords;
    bool has_keyword (std::string const& key) const {
      return this->AllKeywords.end () != this->AllKeywords.find (key); }
    std::string& operator [] (std::string const& key) {
      return this->AllKeywords[key]; }
  };


  // Acquires data in List format in integral form
  // List data is a sequential set of vectors (positions in space)
  // representing elements. Integral data is stored in bytes
  // (although unspecified) with some arbitrary permutation
  integral_vector_list_t
  mode_L_integral (const unsigned char* cData,
		   const unsigned char* cEnd,
		   Header const& Data)
  {
    const std::size_t bordsz = Data.Parameter.size ();
    std::size_t veclength = 0;
    byteorders_t nByteOrders (bordsz);

    // This is for the algebraic permutation of the data
    // to get it to the native format.
    // The idea is that we grab a byte of data from
    // the stream and multiply it by the appropriate scale.
    // For instance, if the byte-order is: 1342 then we
    // scale the first byte by `1', the second byte by
    // 2^(CHAR_BIT*2) {e.g. 256*256}, the third byte by
    // 2^(CHAR_BIT*3) {e.g. 256^3}, and the fourth byte
    // by 2^CHAR_BIT {e.g. 256}
    // This computes those values.
    for (std::size_t i=0; i<bordsz; ++i)
      for (std::size_t j=0; j<Data.Parameter[i].ByteOrder.size (); ++j, ++veclength)
	nByteOrders[i].push_back (1<<(Data.Parameter[i].ByteOrder[j]*CHAR_BIT));
    
    // The number of elements, this will agree with
    // $TOT if all goes well
    const std::size_t nElements = (cEnd-cData) / veclength;
    
    // Acquire the data; terminate when we have
    // all of the data
    integral_vector_list_t ivl (nElements);
    std::size_t element = 0;
    while (element < nElements)
      {
	integral_vector_t iv (bordsz, 0);
	for (std::size_t i=0; i<bordsz; ++i)
	  {
	    blessed_integral sl = 0;
	    // perform the algebraic permutation
	    for (std::size_t j=0; j<nByteOrders[i].size (); ++j, ++cData)
	      sl += nByteOrders[i][j] * *cData;
	    iv[i] = sl;
	  }
	ivl[element] = iv;
	++element;
      }
    return ivl;
  }
  
  // ASCII data (variable length) in List mode is similar
  // to Integral List data, but the data is stored in a 
  // human-readable format
  integral_vector_list_t
  mode_L_ASCII_variable (const unsigned char* cData,
			 const unsigned char* cEnd,
			 Header const& Data)
  {
    const std::size_t Dimension = Data.Parameter.size ();
    std::string sData (cData, cEnd);
    std::stringstream ss (sData);
    integral_vector_list_t ivl;
    integral_vector_t iv (Dimension, 0);

    // This is personal opinion: trying to guess
    // the reserve limit: sData.size () / (5*Dimension)
    // gives a rough-bound assuming (typical) 10-bit
    // precision data plus one space
    ivl.reserve (sData.size () / (5*Dimension));

    // Simplicity itself: pack the data into a stream
    // and read the numbers out one at a time; stuff
    // them into vectors and append to the list!
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
  
  // Converts from one type of List mode data to
  // another type of List mode data
  // The cast is UNSAFE. No promises here.
  // We do not use the Boost casting mechanism
  // because Boost is too heavy-duty to add for
  // now.
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

  // An FCS file is a Head and a Data section
  template <typename ValueType>
  struct FCS
  {
    Header Head;
    std::vector<std::vector<ValueType> > Data;
  };

  // Writes an FCS file to a stream
  template <typename ValueType>
  bool Writer (std::ostream& FCSFile, FCS<ValueType> const& fcs)
  {
    // first, compute how long the data is going to be
    std::string VariableString;
    std::size_t DataLength; // <- place here
    if ("*" == fcs.Head.Datatype)
      {
	std::stringstream ss;
	for (std::size_t i=0; i<fcs.Data.size (); ++i)
	  for (std::size_t j=0; j<fcs.Data[i].size (); ++j)
	    ss << fcs.Data[i][j] << " ";
	VariableString = ss.str ();
	DataLength = VariableString.size ();
      }
    else if ("I" == fcs.Head.Datatype)
      { // precompute the length
	// we ignore user pleas for byte-lengths or whatnot
	DataLength
	  = sizeof(blessed_integral)
	  * fcs.Data.size ()
	  * fcs.Head.Parameter.size ();
      }
    else if ("A" == fcs.Head.Datatype)
      throw fixed_ascii_datatype ();
    else if ("F" == fcs.Head.Datatype)
      throw float_datatype ();
    else if ("D" == fcs.Head.Datatype)
      throw double_datatype ();
    else
      throw invalid_datatype ();

    // buffer for the output data
    unsigned char Data[DataLength];

    // Pack the data in
    if ("*" == fcs.Head.Datatype)
      for (std::size_t i=0; i<DataLength; ++i)
	Data[i] = VariableString[i];
    else if ("I" == fcs.Head.Datatype)
      {
	// we ignore the user's request for both
	// data-width and byte-order and do it *our* way
	std::size_t Cursor = 0;
	const std::size_t ByteLength = sizeof (blessed_integral);
	for (std::size_t i=0; i<fcs.Data.size (); ++i)
	  for (std::size_t j=0; j<fcs.Data[i].size (); ++j)
	    {
	      unsigned int val = fcs.Data[i][j];
	      // we're not going to fuck with clever byte-reording
	      // we're going to do this algebraically
	      // it's slower, but guaranteed to be platform independent
	      for (std::size_t k=0; k<ByteLength; ++k, ++Cursor)
		{
		  const std::size_t Modulus = (1UL<<(CHAR_BIT * (k+1)));
		  const std::size_t Divisor = (1UL<<(CHAR_BIT * k));
		  Data[Cursor] = (val % Modulus) / Divisor;
		}
	    }
      }

    std::stringstream ssKeywords;
    ssKeywords << "/$TOT/" << fcs.Data.size ()
	       << "/$PAR/" << fcs.Head.Parameter.size ()
	       << "/$NEXTDATA/0"
	       << "/$DATATYPE/" << fcs.Head.Datatype
	       << "/$MODE/" << fcs.Head.Mode
	       << "/$BYTEORD";
    for (std::size_t i=0; i<sizeof(blessed_integral); ++i)
      ssKeywords << (0==i?"/":",") << i;

    std::size_t sBitSize;
    if ("I" == fcs.Head.Datatype)
      sBitSize = sizeof(blessed_integral) * CHAR_BIT;
    for (std::size_t i=0; i<fcs.Head.Parameter.size (); ++i)
      {
	std::size_t N = i+1;
	ssKeywords << "/$P" << N << "B/";
	if (sBitSize > 0) ssKeywords << sBitSize;
	else ssKeywords << "*";
	ssKeywords << "/$P" << N << "R/" << fcs.Head.Parameter[i].Range;
	if (fcs.Head.Parameter[i].Name.size () > 0)
	  ssKeywords << "/$P" << N << "N/" << fcs.Head.Parameter[i].Name;
      }
    ssKeywords << "/";
    std::string Keywords = ssKeywords.str ();

    FCSFile << "FCS2.0    ";
    FCSFile.fill (' ');
    FCSFile << std::setw (8) << 256
	    << std::setw (8) << (256+Keywords.size()-1)
	    << std::setw (8) << (256+Keywords.size()+25)
	    << std::setw (8) << (256+Keywords.size()+25+DataLength)
	    << std::setw (8) << 0
	    << std::setw (8) << 0
	    << std::setw (256-58) << " "
	    << Keywords
	    << std::setw (25) << " ";
    FCSFile.write ((char*)Data, DataLength);
    return FCSFile;
  }

  // Attempts to read FCS files with the following caveats:
  // 1) there is, deliberately, no attempt to read fixed ASCII,
  //    correlated, or uncorrelated data
  // 2) there is no interpreter for Float or Double data
  // 3) is quite liberal with the set of `necessary' keywords
  // 4) if there is a problem, it throws an exception; I know
  //    some people really hate exceptions and call a pox on
  //    my head for using them. However, they -are- standard,
  //    they -are- used throughout iostreams, and they -are-
  //    understandable. Sorry.
  template <typename ValueType>
  FCS<ValueType>
  Reader (std::istream& FCSFile, bool Compliance_P = false)
  {
    typedef std::vector<ValueType> ElementT;
    typedef std::vector<ElementT> VElementT;

    FCS<ValueType> fcs;
    Header& Head = fcs.Head;
    VElementT& Data = fcs.Data;

    // This is the `header' section as defined in the FCS2.0 standard;
    // There are some sort of noises about having `8' characters for this,
    // `10' characters for that, etc. etc.
    // I assume the data is white-space delimited. This is non-standard
    // but if you *do* have a crap-load of data, FCS2.0 is not the standard
    // for you.

    std::string FCSKind;
    std::size_t KeysBegin, KeysEnd, KeySection, DataBegin, DataEnd, DataSection;
    //FCSFile >> FCSKind >> KeysBegin >> KeysEnd >> DataBegin >> DataEnd;
    // Want any teeth pulled?
    const std::size_t KindLength = 10;
    const std::size_t LocationSize = 8;
    const std::size_t NumberLocations = 4;
    char DumbBuffer[50];
    FCSFile.read (DumbBuffer, KindLength);
    FCSKind = DumbBuffer;
    std::stringstream ssLocations;
    for (std::size_t i=0; i<NumberLocations; ++i)
      {
	FCSFile.read (DumbBuffer, LocationSize);
	ssLocations << DumbBuffer << " ";
      }
    ssLocations >> KeysBegin >> KeysEnd >> DataBegin >> DataEnd;

    if ("FCS2.0" != FCSKind.substr(0,6))
      throw fcs_error (); // fuckers

    // Lengths of the various sections
    KeySection = KeysEnd - KeysBegin + 1;
    DataSection = DataEnd - DataBegin + 1;
    
    FCSFile.seekg (KeysBegin);
    
    // Now we get the key/value pairs; they
    // are delimited by the first character in the
    // `Text' section. I don't know why.
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
    
    // Go to the Data section; get the data in a buffer
    FCSFile.seekg (DataBegin);
    
    unsigned char cHeadBuffer[DataSection+12];
    FCSFile.read ((char*)cHeadBuffer, DataSection+1);
    
    // Pick through and get a bunch of keywords real-quick
    // Keywords that "throw" are "fatal": they are -required-
    // to interpret the FCS file.
    if (Head.has_keyword ("$PAR"))
      Head.Parameter = NData(convert<std::size_t>(Head["$PAR"]));
    else
      throw no_par_keyword ();

    const std::size_t nParameters = Head.Parameter.size ();

    // some keywords throw if they are 'required' by the standard
    // but are not required to actually read the file; they
    // throw when the Compliance_P switch is set
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
	    // Bit-Size OR Byte-Size, depending upon
	    // the data type; since we ignore fixed ASCII,
	    // it is -always- Bit-Size
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
	// must be byte-aligned to a char
	if ((Head.Parameter[npar].BitSize%CHAR_BIT) != 0)
	  throw invalid_bit_length ();
	par.clear ();
	par.str ("");
	
	par << "$P" << (npar+1) << "R";
	par >> key;
	if (Head.has_keyword (key))
	  Head.Parameter[npar].Range = convert<std::size_t>(Head[key]);
	else if (Compliance_P)
	  // if we ever support Correlated or Uncorrelated data
	  // we will need this keyword
	  throw no_range_keyword (npar);
	par.clear ();
	par.str ("");
	
	// an `optional' keyword; morons
	par << "$P" << (npar+1) << "N";
	par >> key;
	if (Head.has_keyword (key))
	  Head.Parameter[npar].Name = Head[key];
	par.clear ();
	par.str ("");
	
	// I have no clue what this is; it is, however,
	// ubiquitous; a number of readers fail if it is
	// missing
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
	
	// Yeah. No clue.
	par << "$P" << (npar+1) << "S";
	par >> key;
	if (Head.has_keyword (key))
	  Head.Parameter[npar].Specification = Head[key];
	par.clear ();
	par.str ("");
      }
    
    // Get the permutation of the data. The permutation
    // is delimted by a ','. I suppose, in theory, we
    // could have data that uses enough bytes to require
    // two-digits, which could be troublesome. Ugh. Now that
    // I think about it, this goddamn file format may last
    // that long.
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
    
    // This creates a custom permutation for each parameter;
    // this just trivializes some later computations. What it
    // does is throw away permutation values that are "too big"
    // for our particular value, making look-up trivial.
    for (std::size_t i=0; i<nParameters; ++i)
      for (std::size_t j=0; j<Head.ByteOrder.size (); ++j)
	if (Head.ByteOrder[j] < Head.Parameter[i].BitSize/CHAR_BIT)
	  Head.Parameter[i].ByteOrder.push_back (Head.ByteOrder[j]);
    
    // Why? is this some lame attempt at redundancy checking?
    // Idiocy.
    if (Head.has_keyword ("$TOT"))
      Head.Total = convert<std::size_t>(Head["$TOT"]);
    else if (Compliance_P)
      throw no_total_keyword ();
    
    // legal: [L]ist, [U]ncorrelated, [C]orrelated
    if (Head.has_keyword ("$MODE"))
      Head.Mode = Head["$MODE"];
    else
      throw no_mode_keyword ();
    if ("U" == Head.Mode)
      throw uncorrelated_mode ();
    if ("C" == Head.Mode)
      throw correlated_mode ();
    
    // legal: [A]SCII, [I]nteger, [D]ouble, [F]loat
    if (Head.has_keyword ("$DATATYPE"))
      Head.Datatype = Head["$DATATYPE"];
    else
      throw no_datatype_keyword ();
    if ("A" == Head.Datatype && Variable)
      Head.Datatype = "*";
    if ("A" == Head.Datatype)
      throw fixed_ascii_datatype ();
    if ("F" == Head.Datatype)
      throw float_datatype ();
    if ("D" == Head.Datatype)
      throw double_datatype ();
    
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
				 cHeadBuffer+DataSection+1,
				 Head);
	    break;
	  case '*': ivl
	      = mode_L_ASCII_variable (cHeadBuffer,
				       cHeadBuffer+DataSection+1,
				       Head);
	    break;
	  default : break;
	  }
	convert (Data, ivl);
      }

    return fcs;
  }

}

#endif//ARBITRARY_GUARD_MACRO_CODE_OF_DOOM_WHICH_IS_CALLED_FCS_2
