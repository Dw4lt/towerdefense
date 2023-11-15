FROM ghcr.io/dw4lt/ndless-docker:master
ARG PROJECT_DEPENDENCIES

RUN echo "dash dash/sh boolean false" | debconf-set-selections \
 && DEBIAN_FRONTEND=noninteractive dpkg-reconfigure dash \
 && apt-get install -y ${PROJECT_DEPENDENCIES:-""}

CMD ["tail", "-f", "/dev/null"]
