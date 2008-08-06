#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// python
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// fcs
#include <fcs/io.hpp>

// exeception
#include <stdexcept>

namespace bpy = boost::python;

struct no_such_file : std::exception {
	virtual ~ no_such_file () throw() {}
	virtual const char * what () const throw () {
		return "No such file.";
	}
};

typedef FCSTools::FCS<std::size_t> fcs_file;

fcs_file open_fcs_with (std::string const& filename, bool compliance) {
	std::fstream file (filename.c_str(), std::ios::binary|std::ios::in);
	if (not file)
		throw no_such_file();
	return FCSTools::Reader<std::size_t>(file, compliance);
}

fcs_file open_fcs (std::string const& filename) {
	return open_fcs_with(filename, false);
}

std::string to_string (fcs_file const& fcs) {
	std::ostringstream oss;
	FCSTools::Writer<std::size_t>(oss, fcs);
	return oss.str();
}

typedef std::vector<std::size_t> element_t;
typedef std::vector<element_t> data_list_t;

BOOST_PYTHON_MODULE(fcstools)
{
	bpy::class_<element_t>("Element")
		.def("size", &element_t::size)
		;

	bpy::class_<data_list_t>("Data")
		.def(bpy::vector_indexing_suite<data_list_t, true> ());
		;
	
	bpy::class_<FCSTools::Header>("Header")
		;
	
	bpy::class_<fcs_file>("FCS")
		.def_readonly("Head", &fcs_file::Head)
		.def_readonly("Data", &fcs_file::Data)
		;
	
	bpy::def("open", open_fcs_with,
					 bpy::args("filename", "compliance"),
					 "Opens an FCS file from disk; allows an optional compliance mode flag.")
		;
	bpy::def("open", open_fcs,
					 bpy::args("filename"),
					 "Opens an FCS file from disk.")
		;
	bpy::def("to_string", to_string,
					 bpy::args("fcs"),
					 "Converts the FCS file into a string.")
		;
}
