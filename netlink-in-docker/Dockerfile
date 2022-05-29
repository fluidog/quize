FROM ubuntu:latest

RUN apt-get --yes update && \
	apt-get --yes install kmod && \
	apt-get --yes install make && \
	rm -rf /var/lib/apt/lists/* /var/cache/*

COPY . /test-netlink

WORKDIR /test-netlink

CMD make test