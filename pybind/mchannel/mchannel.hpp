#include "mantis/mantis.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace mantis {
    namespace pybind {

        inline int mrx_channel_receive(mantis::go::mrx_channel& self, py::buffer b, mantis::go::mrx_metadata& meta) {
            py::buffer_info info = b.request();
            self.receive(static_cast<char*>(info.ptr), info.itemsize, info.size, meta);
            return info.size;
        }

        inline void mtx_channel_send(mantis::go::mtx_channel& self, py::buffer b, mantis::go::mtx_metadata& meta) {
            py::buffer_info info = b.request();
            self.send(static_cast<const char*>(info.ptr), info.itemsize, info.size, meta);
        }

    } // namespace pybind
} // namespace mantis

// Template helper to bind common i_channel methods
template <typename T, typename Holder>
void bind_common_channel_methods(py::class_<mantis::interfaces::i_channel<T>, Holder>& cl) {
    cl.def("healthcheck", &mantis::interfaces::i_channel<T>::healthcheck)
        .def("sync", &mantis::interfaces::i_channel<T>::sync, py::arg("time_stamp"))
        .def("get_time_now", &mantis::interfaces::i_channel<T>::get_time_now)
        .def("get_channel_num", &mantis::interfaces::i_channel<T>::get_channel_num)
        .def("is_valid", &mantis::interfaces::i_channel<T>::is_valid)
        .def("invalidate", &mantis::interfaces::i_channel<T>::invalidate)
        .def("set_rate", &mantis::interfaces::i_channel<T>::set_rate)
        .def("get_rate", &mantis::interfaces::i_channel<T>::get_rate)
        .def("set_freq", &mantis::interfaces::i_channel<T>::set_freq, py::arg("freq"), py::arg("lo") = py::none())
        .def("get_freq", &mantis::interfaces::i_channel<T>::get_freq)
        .def("set_gain", &mantis::interfaces::i_channel<T>::set_gain)
        .def("get_gain", &mantis::interfaces::i_channel<T>::get_gain)
        .def("get_valid_sample_rates", &mantis::interfaces::i_channel<T>::get_valid_sample_rates)
        .def("set_clock_source", &mantis::interfaces::i_channel<T>::set_clock_source)
        .def("set_time_source", &mantis::interfaces::i_channel<T>::set_time_source)
        .def("can_set_power_ref", &mantis::interfaces::i_channel<T>::can_set_power_ref)
        .def("set_power_ref", &mantis::interfaces::i_channel<T>::set_power_ref)
        .def("get_power_ref", &mantis::interfaces::i_channel<T>::get_power_ref)
        .def("get_params", &mantis::interfaces::i_channel<T>::get_params, py::return_value_policy::reference_internal);
}

void bind_channels(py::module_& m) {
    // 1. RX Channel Binding
    py::class_<mantis::go::mrx_channel, std::shared_ptr<mantis::go::mrx_channel>> rx(m, "mrx_channel");
    rx.def("receive", &mantis::pybind::mrx_channel_receive, py::arg("buffer"), py::arg("metadata"),
           "Receive samples directly into a NumPy array.");
    bind_common_channel_methods(rx);

    // 2. TX Channel Binding
    py::class_<mantis::go::mtx_channel, std::shared_ptr<mantis::go::mtx_channel>> tx(m, "mtx_channel");
    tx.def("send", &mantis::pybind::mtx_channel_send, py::arg("buffer"), py::arg("metadata"),
           "Send samples directly from a NumPy array.")

        .def("recv_async_message", &mantis::go::mtx_channel::recv_async_message);
    bind_common_channel_methods(tx);
}