FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

RUN apt-get update && apt-get install -y \
    tzdata \
    build-essential \
    cmake \
    git \
    qtbase5-dev \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5network5 \
    libqt5core5a \
    qt5-qmake \
    nlohmann-json3-dev 

RUN ln -fs /usr/share/zoneinfo/$TZ /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata

WORKDIR /app

COPY . .

RUN cmake -B build -S . \
    -DCMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt5"

RUN cmake --build build --config Release

RUN chmod 644 /app/build/Apps_for_computers /app/build/computers.json

CMD ["bash", "-c", "mkdir -p /Computer_app && cp /app/build/Apps_for_computers /Computer_app && /bin/bash"]
