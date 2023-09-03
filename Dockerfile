ARG PROJECT_DEPENDENCIES
FROM registry.gitlab.com/dw4lt/ndless-toolchain

RUN echo "dash dash/sh boolean false" | debconf-set-selections \
 && DEBIAN_FRONTEND=noninteractive dpkg-reconfigure dash \
 && apt-get install -y $PROJECT_DEPENDENCIES

ENTRYPOINT ["tail", "-f", "/dev/null"]
