# Data Audit

This project is still under development.

It can be used to efficiently check the integrity of files that are stored on remote
servers. 

It relies on the method described in [Compact Proofs of Retrievability.](https://cseweb.ucsd.edu/~hovav/dist/verstore.pdf)

### Project structure:
* `app`: Frontend for client-side app
* `client/`: Backend for client-side app
* `client/upload`: Code related to uploading and tagging a file
* `client/verify`: Code related to verifying a file
* `files`: File browser
* `proto`: Protocol Buffers
* `providers`: Code for working with different cloud storage providers
* `server`: Simple implementation of a cloud storage provider

### Dependencies:
* protobuf
* OpenSSL
* CryptoPP
* cpprestsdk
* proxygen
* lz4
