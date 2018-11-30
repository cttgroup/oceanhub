# Get NodeJS LTS release
FROM node:boron

RUN mkdir /workdir

WORKDIR /opt
RUN ln -s /workdir/package.json /opt/.
COPY /workdir/package.json /opt/.
RUN ln -s /opt/node_modules /workdir/.
RUN yarn

WORKDIR /workdir
