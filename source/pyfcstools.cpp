#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <FCSIO.hpp>

typedef FCSTools::FCS<std::size_t> fcs_file;

BOOST_PYTHON_MODULE(fcstools)
{
	boost::python::class_<fcs_file>("FCS")
		;
}
