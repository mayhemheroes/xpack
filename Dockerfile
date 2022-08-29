FROM fuzzers/libfuzzer:12.0

RUN apt-get update
RUN apt install -y build-essential wget git clang cmake  automake autotools-dev  libtool zlib1g zlib1g-dev libexif-dev \
    libjpeg-dev 
RUN git clone https://github.com/xyz347/xpack.git
WORKDIR /xpack
COPY fuzzers/f1.cpp .
RUN clang++ -fsanitize=fuzzer,address f1.cpp -o /fuzz_decoder 
ENTRYPOINT []
CMD ["/fuzz_decoder"]
