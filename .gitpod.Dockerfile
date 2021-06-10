FROM gitpod/workspace-full

# Install custom tools, runtimes, etc.
# For example "bastet", a command-line tetris clone:
# RUN brew install bastet
#
# More information: https://www.gitpod.io/docs/config-docker/

RUN sudo apt -q update && sudo apt install -yq python3 cmake && sudo rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/emscripten-core/emsdk.git
RUN cd emsdk && sudo ./emsdk install latest && sudo ./emsdk activate latest && source ./emsdk_env.sh
