#!/bin/zsh
openssl req -x509 -newkey rsa:4096 \
    -keyout serverPrivateKey.pem \
    -out serverCertificate.pem \
    -sha256 -noenc 
