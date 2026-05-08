#include "classes/async_mtx_metadata.hpp"
#include "classes/mrx_metadata.hpp"
#include "classes/mtx_metadata.hpp"
#include "classes/time_tag.hpp"
#include "device_manager/device_manager.hpp"
#include "errors/error_codes.hpp"
#include "errors/metadata_errors.hpp"
#include "mantis/mantis.h"
#include "mchannel/mchannel.hpp"
#include "msdr/msdr_params.hpp"
#include "utilities/general.hpp"
#include "utilities/prints.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(mantis, m) {
    m.attr("__version__") = MANTIS_VERSION;
    // classes
    bind_time_tag(m);
    bind_errors(m);
    bind_msdr_params(m);
    bind_metadata_errors(m);
    bind_tx_metadata(m);
    bind_rx_metadata(m);
    bind_async_tx_metadata(m);
    bind_channels(m);
    bind_device_manager(m);

    // namespaces
    py::module_ utils = m.def_submodule("utils", "mantis utility functions");
    bind_prints(utils);
    bind_general(utils);
}