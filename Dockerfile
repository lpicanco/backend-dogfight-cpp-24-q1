FROM debian:unstable-slim AS build

RUN apt update && \
    apt install -y \
    build-essential \
    git \
    cmake \
    libpq-dev \
    libpqxx-dev \
    libjsoncpp-dev uuid-dev zlib1g-dev

COPY src/ ./src/
COPY cmake/ ./cmake/
COPY CMakeLists.txt .

RUN apt install -y libssl-dev

WORKDIR /build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 8

FROM debian:unstable-slim

RUN apt update && \
    apt install -y \
    libpqxx-dev libssl-dev libjsoncpp-dev uuid-dev zlib1g-dev

COPY --chown=shs:shs --from=build \
    ./build/backend-dogfight-cpp-24-q1 \
    ./app/

ENTRYPOINT [ "./app/backend-dogfight-cpp-24-q1" ]
