#include <iostream>
#include <fstream>
#include <boost/python.hpp>
#include <fcs/io.hpp>

typedef FCSTools::FCS<std::size_t> fcs_file;

fcs_file open_fcs (std::string const& filename, bool compliance=false) {
	std::fstream file (filename.c_str(), std::ios::binary|std::ios::in);
	return FCSTools::Reader<std::size_t>(file, compliance);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(open_fcs_overloads, open_fcs, 1, 2);

BOOST_PYTHON_MODULE(fcstools)
{
	boost::python::class_<fcs_file>("FCS")
		;

	boost::python::def("open", open_fcs, open_fcs_overloads( args("filename", "compliance"), "Opens an FCS file from disk."));
}
