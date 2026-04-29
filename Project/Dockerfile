FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
	build-essential \
	cmake \
	libboost-all-dev

RUN apt-get update && apt-get install -y \
	ca-certificates \
	wget \ 
	curl \
	tar 

RUN apt-get install -y git

RUN apt-get update && apt-get install -y qtbase5-dev && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/google/googletest.git
WORKDIR /app

CMD ["./build.sh"]
#COPY . .
#RUN cmake -B build -S . && cmake --build build
#RUN cmake --build build --target test
#CMD [" ./build/run_benchmarks"]
#RUN ./build/run_benchmarks

