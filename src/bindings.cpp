#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <vector>

#include "Member.hpp"
#include "EvolAlg.hpp"
#include "StatsObserver.hpp"

char const *greet() { return "hello World"; }

template <class T> struct VecToList
{
    static PyObject *convert(const std::vector<T> &vec)
    {
        boost::python::list *l = new boost::python::list();
        for (size_t i = 0; i < vec.size(); i++)
        {
            l->append(vec[i]);
        }

        return l->ptr();
    }
};

namespace python = boost::python;

// Boost.Python wrapper defining the module name, classes  and methods
BOOST_PYTHON_MODULE(bindings)
{
    python::class_<Member>("Member", python::no_init)
        .def(python::init<unsigned int, const unsigned int, const unsigned int>())
        .def("isModified", &Member::isModified);
    python::class_<EvolAlg, boost::noncopyable>("EvolAlg", python::no_init)
        .def(python::init<const std::string, const unsigned int, const unsigned int, const unsigned int, bool>())
        //.def("getInputImage", &EvolAlg::getInputImage);
        .def("isRunning", &EvolAlg::isRunning)
        .def("stop", &EvolAlg::stop)
        .def("run", &EvolAlg::run);
    python::def("greet", greet);
    python::to_python_converter<std::vector<std::string, std::allocator<std::string>>, VecToList<std::string>>();
    python::to_python_converter<std::vector<int, std::allocator<int>>, VecToList<int>>();
}
