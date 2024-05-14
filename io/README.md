```bash
05-14_16:53:18 sanghuck-yu io (master)$ make
gcc -Wall -Wextra -std=c99 -c main.c
gcc -Wall -Wextra -std=c99 -o main main.o io.o block_io.o fio.o
05-14_16:53:29 sanghuck-yu io (master)$ ./main
Elapsed time for       io_write;  1kb: 0.000867 seconds
Elapsed time for       io_write;  1mb: 0.154965 seconds
Elapsed time for       io_write;  1gb: 138.929994 seconds
Elapsed time for block_io_write;  1kb: 0.000239 seconds
Elapsed time for block_io_write;  1mb: 0.005915 seconds
Elapsed time for block_io_write;  1gb: 1.482934 seconds
Elapsed time for      fio_write;  1kb: 0.000206 seconds
Elapsed time for      fio_write;  1mb: 0.009524 seconds
Elapsed time for      fio_write;  1gb: 3.422100 seconds
Elapsed time for        io_read;  1kb: 0.000314 seconds
Elapsed time for        io_read;  1mb: 0.051122 seconds
Elapsed time for        io_read;  1gb: 39.934273 seconds
Elapsed time for  block_io_read;  1kb: 0.000027 seconds
Elapsed time for  block_io_read;  1mb: 0.000892 seconds
Elapsed time for  block_io_read;  1gb: 0.795364 seconds
Elapsed time for       fio_read;  1kb: 0.000030 seconds
Elapsed time for       fio_read;  1mb: 0.002751 seconds
Elapsed time for       fio_read;  1gb: 2.778334 seconds
```