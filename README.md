# Bathroom management system

<p align="left">
  <img src="https://github.com/dmfrodrigues/feup-sope-proj2/workflows/test/badge.svg">
</p>

- **Project name:** Informatic access to Bathrooms
- **Short description:** Server-client to handle conflicts over common resources (in this case, bathroom places)
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [SOPE](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=436440) (Operating Systems)
<!-- - **Project grade:** ??/20 -->
- **Group members:**
    - [Diogo Miguel Ferreira Rodrigues](https://github.com/dmfrodrigues) ([diogo.rodrigues@fe.up.pt](mailto:diogo.rodrigues@fe.up.pt)/[up201806429@fe.up.pt](mailto:up201806429@fe.up.pt))
    - [João António Cardoso Vieira e Basto de Sousa](https://github.com/JoaoASousa) ([up201806613@fe.up.pt](up201806613@fe.up.pt))
    - [Rafael Soares Ribeiro](https://github.com/up201806330) ([up201806330@fe.up.pt](mailto:up201806330@fe.up.pt))

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
