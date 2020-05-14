# Bathroom management system

<p align="left">
  <img src="https://github.com/dmfrodrigues/feup-sope-proj2/workflows/test/badge.svg">
</p>

- **Project name:** Informatic access to Bathrooms
- **Short description:** Server-client to handle conflicts over common resources (in this case, bathroom places)
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [SOPE](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=436440) (Operating Systems)
- **Project grade (1st delivery):** 26/30 (87%)
- **Group members:**
    - [Diogo Miguel Ferreira Rodrigues](https://github.com/dmfrodrigues) ([diogo.rodrigues@fe.up.pt](mailto:diogo.rodrigues@fe.up.pt)/[up201806429@fe.up.pt](mailto:up201806429@fe.up.pt))
    - [João António Cardoso Vieira e Basto de Sousa](https://github.com/JoaoASousa) ([up201806613@fe.up.pt](up201806613@fe.up.pt))
    - [Rafael Soares Ribeiro](https://github.com/up201806330) ([up201806330@fe.up.pt](mailto:up201806330@fe.up.pt))

### Implementation details

#### Managing threads

On the server side, where a restriction of the number of creatable threads can be imposed by way of the argument "-n", we used a semaphore to manage the number of active threads, since the number of threads is a common resource over which there will be race conditions.

On the client side however, we use a custom wrapper around a long long int [atomic_lli](common/src/common_atomic.c), where by making use of a mutex, we also solve the syncronization problem.

#### Managing bathroom access

For the access to the bathroom we used another semaphore, that tracks the number of free spots in the bathroom, alongside an array of bools indicating which exact spots are free. This last part was needed so that the responses to the client threads could carry information of the place the client entered at.

## Make commands

```sh
make          # Compile
make test     # Test
make clean    # Cleanup
make zip      # Create zip for delivery
make testzip  # Create zip, copy to other location, unzip and run tests
```

## User Commands

```sh
Un <-t nsecs> FIFONAME
Qn <-t nsecs> [-l nplaces] [-n nthreads] FIFONAME
```

- `-t nsecs`: (approximate) number of seconds the program will run
- `-l nplaces`: maximum occupation of the bathroom
- `-n nthreads`: maximum number of threads handling requests
- `FIFONAME`: Name of public FIFO used to request/receive requests (created in `/tmp`).
