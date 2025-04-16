#include <pybind11/pybind11.h>

int add(int a, int b) {
    return a + b;
}

PYBIND11_MODULE(mymodule, m) {
    m.doc() = "pybind11 example plugin";  // 模块文档
    m.def("add", &add, "A function that adds two numbers");
}
