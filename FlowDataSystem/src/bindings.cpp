#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "feature/featureBuilder.hpp"
#include "feature/futureFeatureBuilder.hpp"
#include "factor/factorBuilder.hpp"

namespace py = pybind11;

PYBIND11_MODULE(flow_data_system, m) {
    m.doc() = "Python bindings for FeatureBuilder and FactorBuilder";

    // 绑定 FeatureBuilder 类
    py::class_<FeatureBuilder>(m, "FeatureBuilder")
        .def(py::init<std::vector<std::string>>())  // 构造函数
        .def("build_snap_features", &FeatureBuilder::build_snap_features)
        .def("build_snap_features_byDDB", &FeatureBuilder::build_snap_features_byDDB)
        .def("fill_snap_features", &FeatureBuilder::fill_snap_features)
        .def("add_transaction", &FeatureBuilder::add_transaction)
        .def("add_entrust", &FeatureBuilder::add_entrust)
        .def("build_other_features", &FeatureBuilder::build_other_features);

    // 绑定 FactorBuilder 类
    py::class_<FactorBuilder>(m, "FactorBuilder")
        .def(py::init<std::vector<std::string>, std::shared_ptr<FeatureBuilderBase>, const std::string&>())  // 构造函数
        .def("computeAllFactors", &FactorBuilder::computeAllFactors)
        .def("getFactorResults", &FactorBuilder::getFactorResults);
}