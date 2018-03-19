FROM gcc:latest
COPY . ./workdir
WORKDIR ./workdir
RUN make
CMD ["./traceroute"]
