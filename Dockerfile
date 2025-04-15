FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    ca-certificates \
    cmake \
    g++ \
    git \
    libboost-all-dev \
    libssl-dev && \
    apt-get clean

WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake .. -DCPR_ENABLE_SSL=ON && \
    make

EXPOSE 80

CMD ["./build/hyperplanning-fix"]
