# Use a stable Ubuntu base
FROM ubuntu:noble AS base

# Install basic tools and dependencies
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    ca-certificates \
    libuhd-dev \
    nlohmann-json3-dev \
    python3-pybind11 pybind11-dev python3-pip \
    libboost-program-options-dev && rm -rf /var/lib/apt/lists/*

RUN pip3 install --break-system-packages pybind11-stubgen

FROM base

# Set a working directory
WORKDIR /work

ADD . "mantis"

RUN if [ -d "mantis/build" ]; then rm -rf mantis/build; fi

# RUN cd mantis && mkdir -p build \
# && cd build && cmake .. && make -j $(nproc) install \
# && cd ../.. && rm -rf mantis

RUN ldconfig

# Default shell when container starts
ENTRYPOINT ["/bin/bash"]
