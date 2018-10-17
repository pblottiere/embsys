FROM debian:stable-slim

# Buildroot deps
RUN apt-get update && apt-get install -y \
	bc \
	bzip2 \
	cpio \
	file \
	gcc \
	g++ \
	git \
	gzip \
	make \
	ncurses-dev \
	python \
	unzip \
	wget \
	autotools-dev \
	automake \
	libtool \
	qemu-user-static \
	&& rm -rf /var/lib/apt/lists/*

COPY buildroot-2017.08.tar.gz /root/
