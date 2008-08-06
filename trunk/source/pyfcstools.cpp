#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/args.hpp>
#include <boost/python/class.hpp>
#include <fcs/io.hpp>

typedef FCSTools::FCS<std::size_t> fcs_file;

fcs_file open_fcs_with (std::string const& filename, bool compliance) {
	std::fstream file (filename.c_str(), std::ios::binary|std::ios::in);
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

BOOST_PYTHON_MODULE(fcstools)
{
	boost::python::class_<FCSTools::Header>("Header")
		;

	boost::python::class_<fcs_file>("FCS")
		.def_readonly("Head", &fcs_file::Head)
		.def_readonly("Data", &fcs_file::Data)
		;

	boost::python::def("open", open_fcs_with,
										 boost::python::args("filename", "compliance"),
										 "Opens an FCS file from disk; allows an optional compliance mode flag.")
		;
	boost::python::def("open", open_fcs,
										 boost::python::args("filename"),
										 "Opens an FCS file from disk.")
		;
	boost::python::def("to_string", to_string,
										 boost::python::args("fcs"),
										 "Converts the FCS file into a string.")
		;
}
