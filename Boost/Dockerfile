FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
	build-essential \
	cmake \
	libboost-all-dev

RUN apt-get install -y git

RUN git clone https://github.com/google/googletest.git
WORKDIR /app
COPY . .
RUN cmake -B build -S . && cmake --build build
#RUN cmake --build build --tagret test

FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
	build-essential \
	libboost-system1.74 \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /root/
COPY --from=builder /app//build/bayan .
ENTRYPOINT ["./bayan"]
CMD ["-i ./TestFiles/"]
