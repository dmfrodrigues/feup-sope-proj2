# Bathroom management system

<p align="left">
  <img src="https://github.com/dmfrodrigues/feup-sope-proj2/workflows/test/badge.svg">
</p>

- **Project name:** Informatic access to Bathrooms
- **Short description:** Server-client to handle conflicts over common resources (in this case, bathroom places)
- **Institution:** FEUP
- **Course:** SOPE (Operating Systems)
- **Group members:**
    - Diogo Miguel Ferreira Rodrigues (diogo.rodrigues@fe.up.pt / up201806429@fe.up.pt)
    - João António Cardoso Vieira e Basto de Sousa (up201806613@fe.up.pt)
    - Rafael Soares Ribeiro (up201806330@fe.up.pt)

### Implementation details

### Development

To perform a simple test, run this command:
```sh
server -t 10 fifo
```
and 1 second after that run
```sh
client -t 1 fifo
```
and another 1 second after that run
```sh
client -t 1 fifo
```


## Make commands
### Compile

```sh
make
```

### Test

```sh
make test
```

### Cleanup

```sh
make clean
```

### Create ZIP file

```sh
make zip
```

### Test ZIP file contents

```sh
make testzip
```

## User Commands

```sh
Un <-t nsecs> FIFONAME
Qn <-t nsecs> [-l nplaces] [-n nthreads] FIFONAME
```

- `-t nsecs`: (approximate) number of seconds the program will run
- `-l nplaces`: maximum occupation of the bathroom
- `-n nthreads`: maximum number of threads handling requests
- `FIFONAME`: Filesystem path to named FIFO used to request/receive requests.
