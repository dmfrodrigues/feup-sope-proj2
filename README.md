# Bathroom management system

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

![test](https://github.com/dmfrodrigues/feup-sope-proj2/workflows/test/badge.svg)

- **Project name:** Informatic access to Bathrooms
- **Short description:** Server-client to handle conflicts over common resources (in this case, bathroom places)
- **Environment:** Linux
- **Tools:** C, Linux API
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [SOPE](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=436440) (Operating Systems)
- **Project grade:** 17.56/20.00
- **Group members:**
    - [Diogo Miguel Ferreira Rodrigues](https://github.com/dmfrodrigues) ([diogo.rodrigues@fe.up.pt](mailto:diogo.rodrigues@fe.up.pt)/[up201806429@fe.up.pt](mailto:up201806429@fe.up.pt))
    - [João António Cardoso Vieira e Basto de Sousa](https://github.com/JoaoASousa) ([up201806613@fe.up.pt](up201806613@fe.up.pt))
    - [Rafael Soares Ribeiro](https://github.com/up201806330) ([up201806330@fe.up.pt](mailto:up201806330@fe.up.pt))

## Installing

If you are cloning from GitHub, you should pull all submodules with

```sh
git submodule update --init --recursive # If for the first time
```

Once that is done, you can run `make`.

### Make commands

```sh
make          # Compile
make test     # Test
make clean    # Cleanup
make zip      # Create zip for delivery
make testzip  # Create zip, copy to other location, unzip and run tests
```

## Usage

```
Un <-t nsecs> FIFONAME
Qn <-t nsecs> [-l nplaces] [-n nthreads] FIFONAME

FIFONAME        Name of public FIFO used to request/receive requests (created in /tmp).
```

### Options

```
-t nsecs        (approximate) number of seconds the program will run
-l nplaces      maximum occupation of the bathroom
-n nthreads     maximum number of threads handling requests
```

### Examples

- To run the server (bathroom) for 30 seconds, with 6 places and 15 threads, `Q2 -t 30 -l 6 -n 15 door`
- To run the user for 20 seconds, `U2 -t 20 door`

## Implementation details

### Managing threads

On the server side, where a restriction of the number of creatable threads can be imposed by way of the argument `-n`, we used a semaphore to manage the number of active threads, since the number of threads is a common resource over which there will be race conditions.

On the client side however, we use a custom wrapper around a long long int [atomic_lli](common/src/common_atomic.c), where by making use of a mutex, we also solve the syncronization problem. In this case the variable is simply used to wait for all threads to finish, not to impose on the creation of new threads like in the server.

### Managing bathroom access

For the access to the bathroom we used another semaphore, that tracks the number of free spots in the bathroom, alongside an array of bools indicating which exact spots are free. This last part was needed so that the responses to the client threads could carry information of the place the client entered at.

### Closing the service

Upon the end of the server functioning time, the server will read from the public FIFO for 100 milliseconds and warn those clients that the service is closed. This is done to both make sure no request is left in the public FIFO but also to make sure the client receives at least one of these warnings, since the alternatives to this "timer" would either be: open the FIFO with the `O_NONBLOCK` flag and run the risk of exiting immediately without reporting to the client that the bathroom is closed, or opening without the flag and run the risk of blocking if there were in fact no more requests being sent through it.

## License

© 2020 Diogo Rodrigues, João António Sousa, Rafael Ribeiro

All files are licensed under [GNU General Public License v3](LICENSE), to the exception of:
- `.pdf` file, which were authored by the lecturers
- submodules, which are subject to specific licenses

The files not authored by us are presented as a fundamental complement to the presented solutions, and are made available under *fair use for education*.