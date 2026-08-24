FROM ubuntu:latest

# Install packages (list of requirements copied from setup.sh)
RUN apt update -y && apt \
    install  \
    pkg-config  \
    g++-13  \
    clang-format  \
    clang-tidy  \
    cmake  \
    git  \
    build-essential  \
    libwayland-dev  \
    libxkbcommon-dev  \
    xorg-dev  \
    libgl1-mesa-dev  \
    mesa-common-dev  \
    mesa-utils  \
    doxygen  \
    graphviz  \
    libassimp-dev \
    python3-pip \
    python3-venv -y

WORKDIR /project

COPY . .

RUN ./build-project.sh

WORKDIR /project/build

ENTRYPOINT ["./APP"]
