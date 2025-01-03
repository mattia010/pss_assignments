FROM gcc
WORKDIR /usr/local/app

COPY bin/myProg.out .
EXPOSE 8080

RUN useradd usr 
USER usr

CMD ./myProg.out