FROM debian:buster

RUN \
    apt-get -qq update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y wget gnupg2 \
    && wget -q -O - https://apt.mopidy.com/mopidy.gpg | APT_KEY_DONT_WARN_ON_DANGEROUS_USAGE=1 apt-key add - \
    && wget -q -O /etc/apt/sources.list.d/mopidy.list https://apt.mopidy.com/buster.list \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential \
        clang clang-tools \
        git \
        autotools-dev \
        autoconf \
        libtool \
        gettext \
        gawk \
        gperf \
        antlr3 \
        libantlr3c-dev \
        libconfuse-dev \
        libunistring-dev \
        libsqlite3-dev \
        libavcodec-dev \
        libavformat-dev \
        libavfilter-dev \
        libswscale-dev \
        libavutil-dev \
        libasound2-dev \
        libmxml-dev \
        libgcrypt20-dev \
        libavahi-client-dev \
        zlib1g-dev \
        libevent-dev \
        libplist-dev \
        libsodium-dev \
        libcurl4-openssl-dev \
        libjson-c-dev \
        libprotobuf-c-dev \
        libpulse-dev \
        libwebsockets-dev \
        libgnutls28-dev \
        libspotify-dev \
        gosu \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY entrypoint.sh /

RUN chmod 755 /entrypoint.sh \
    && useradd -d /home/owntone -m owntone \
    && mkdir -p /workdir \
    && chown owntone:owntone /workdir

ENV LANG=en_US.UTF-8
WORKDIR /workdir
VOLUME /workdir
ENTRYPOINT [ "/entrypoint.sh" ]
CMD /bin/bash
