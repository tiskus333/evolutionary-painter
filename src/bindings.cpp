#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <vector>

#include "EvolAlg.hpp"
#include "Member.hpp"
#include "StatsObserver.hpp"

char const *greet() { return "hello World"; }

using namespace boost::python;

template <class T> struct VecToList
{
    static PyObject *convert(const std::vector<T> &vec)
    {
        boost::python::list *l = new boost::python::list();
        for (auto &&i : vec)
        {
            l->append(i);
        }

        return l->ptr();
    }
};

template <class T> struct ListToPyList
{
    static PyObject *convert(const std::list<T> &list)
    {
        boost::python::list *l = new boost::python::list();
        for (auto &&i : list)
        {
            l->append(i);
        }
        return l->ptr(); // TODO improve it
    }
};

// Boost.Python wrapper defining the module name, classes  and methods
BOOST_PYTHON_MODULE(bindings)
{
    class_<Member>("Member", no_init).def(init<unsigned int, const unsigned int, const unsigned int>()).def("isModified", &Member::isModified);
    class_<EvolAlg, boost::noncopyable>("EvolAlg", no_init)
        .def(init<const std::string, const unsigned int, const unsigned int, const unsigned int, bool>())
        .def("isRunning", &EvolAlg::isRunning)
        .def("stop", &EvolAlg::stop)
        .def("run", &EvolAlg::run);
    def("greet", greet);
    class_<StatsObserver, boost::noncopyable>("StatsObserver")
        .def("setObserved", &StatsObserver::setObservedEvolAlg)
        .def("get", &StatsObserver::getGenerationsAndFitness);
    class_<ResultsContainer>("ResultsContainer")
        .def("fit", &ResultsContainer::getFitnesses)
        .def("gen", &ResultsContainer::getGenerations);
    to_python_converter<std::vector<std::string, std::allocator<std::string>>, VecToList<std::string>>();
    to_python_converter<std::vector<int, std::allocator<int>>, VecToList<int>>();
    to_python_converter<std::list<uint, std::allocator<uint>>, ListToPyList<uint>>();
    to_python_converter<std::list<unsigned long long, std::allocator<unsigned long long>>, ListToPyList<unsigned long long>>();
}
