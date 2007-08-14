#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

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

int main (int argc, char *argv[])
{

  std::vector<std::size_t> preNativeByteOrder;
  for (std::size_t i=0; i<8; ++i)
    preNativeByteOrder.push_back (8-i-1);

  if (argc != 2)
    return 0;

  std::ifstream fcsfile (argv[1]);
  std::string FCSKind;
  std::size_t KeysBegin, KeysEnd, KeySection, DataBegin, DataEnd, DataSection;
  fcsfile >> FCSKind >> KeysBegin >> KeysEnd >> DataBegin >> DataEnd;
  KeySection = KeysEnd - KeysBegin + 1;
  DataSection = DataEnd - DataBegin + 1;

  fcsfile.seekg (KeysBegin);

  char Delimiter = (char)fcsfile.peek ();
  char cKeyWordBuffer[KeySection+2];
  fcsfile.get (cKeyWordBuffer, KeySection+1, 0);
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

  fcsfile.close ();
  std::fstream bfcsfile (argv[1], std::ios::in|std::ios::binary);
  bfcsfile.seekg (DataBegin);

  unsigned char cDataBuffer[DataSection+12];
  bfcsfile.read ((char*)cDataBuffer, DataSection+1);
  
  std::size_t nParameters = convert<std::size_t>(keywords["$PAR"]);
  std::vector<std::size_t> BitSize (nParameters, 0);
  std::vector<std::size_t> Range (nParameters, 0);
  std::vector<std::string> Name (nParameters, "");
  std::vector<std::pair<double,double> > Exponent (nParameters);
  std::vector<std::string> Source (nParameters, "");

  std::size_t stdBitSize = 0;

  for (std::size_t npar = 0; npar < nParameters; ++npar)
    {
      std::stringstream par;
      par << "$P" << (npar+1) << "B";
      std::string key;
      par >> key;
      if ("*" == keywords[key])
	BitSize[npar] = 0; // variable bit-size
      else
	BitSize[npar] = convert<std::size_t>(keywords[key]);
      if (stdBitSize <= 0)
	{
	  stdBitSize = BitSize[npar];
	  if ((stdBitSize%CHAR_BIT) != 0)
	    std::cout << "Invalid bit-length for integer." << std::endl;
	}
      if (stdBitSize != BitSize[npar])
	{
	  std::cout << "Error: must have consistent bits "
		    << stdBitSize << "=!=" << BitSize[npar] << std::endl;
	  return 0;
	}
      par.clear ();
      par.str ("");

      par << "$P" << (npar+1) << "R";
      par >> key;
      Range[npar] = convert<std::size_t>(keywords[key]);
      par.clear ();
      par.str ("");

      par << "$P" << (npar+1) << "N";
      par >> key;
      if (keywords.end () != keywords.find (key))
	{
	  Name[npar] = keywords[key];
	}
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
	{
	  Source[npar] = keywords[key];
	}
      par.clear ();
      par.str ("");
    }

  std::stringstream byteorder (keywords["$BYTEORD"]);
  std::vector<std::size_t> preByteOrder;
  while (true)
    {
      std::string byteo;
      std::getline (byteorder, byteo, ',');
      if (! byteorder)
	break;
      preByteOrder.push_back (convert<std::size_t>(byteo)-1);
    }

  std::size_t MaxByteSize = stdBitSize / CHAR_BIT;
  std::vector<std::size_t> ByteOrder;
  for (std::size_t i=0; i<preByteOrder.size (); ++i)
    if (preByteOrder[i] < MaxByteSize)
      ByteOrder.push_back (preByteOrder[i]);

  std::vector<std::size_t> NativeByteOrder;
  for (std::size_t i=0; i<preNativeByteOrder.size (); ++i)
    if (preNativeByteOrder[i] < MaxByteSize)
      NativeByteOrder.push_back (preNativeByteOrder[i]);

  std::size_t Total = 0;
  if (keywords.end () != keywords.find ("$TOT"))
    Total = convert<std::size_t>(keywords["$TOT"]);

  // legal: [L]ist, [U]ncorrelated, [C]orrelated
  std::string Mode = keywords["$MODE"];

  // legal: [A]SCII, [I]nteger, [D]ouble, [F]loat
  std::string Datatype = keywords["$DATATYPE"];

  std::string Cytometer;
  if (keywords.end () != keywords.find ("$CYT"))
    {
      Cytometer = keywords["$CYT"];
    }

  if ("L" == Mode)
    { // list of vectors
      // we grab up to, but no more than Total
      std::size_t CurChar = 0;
      for (std::size_t i=0; (i<Total) && (CurChar<DataSection+1); ++i)
	{
	  for (std::size_t j=0; j<nParameters; ++j)
	    {
	      unsigned char bytes[MaxByteSize];
	      for (std::size_t k=0; k<MaxByteSize; ++k, ++CurChar)
		{ // this assumes a 0,1,2,3 order!
		  bytes[NativeByteOrder[ByteOrder[k]]] = cDataBuffer[CurChar];
		}
	      // NOT CORREKT
	      std::cout << std::hex << (int)bytes[0] << " "
			<< (int)bytes[1] << std::endl;
	      std::size_t ival
		= *reinterpret_cast<std::size_t*>(bytes);
	      std::cout << std::dec << ival << std::endl;
	    }
	  break;
	}
    }
  else if ("U" == Mode)
    { // histograms of ranged-size(???)
      std::cout << "Uncorrelated mode is not supported" << std::endl;
    }
  else if ("C" == Mode)
    { // histograms of ranged-size(???)
      std::cout << "Correlated mode is not supported" << std::endl;
    }

  return 0;
}
