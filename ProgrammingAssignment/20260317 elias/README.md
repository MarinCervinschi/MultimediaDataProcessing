# Elias Gamma Coding

To see the assignment description, please refer to the [assignment sheet](ProgrammingAssignment.pdf).

## Compilation and Execution

those are the results of compression of a file containing 20 million integers, each in the range [-1000, 1000]. The original file is `numbers02.txt` and the compressed output is `output.bin`. 

As you can see, the original file is 6.6 MB, while the compressed file is only 765 KB, which demonstrates the effectiveness of the Elias gamma coding for compressing integer data.

```bash
╰─❯ ll -h numbers02.txt output.bin
-rw-rw-r--@ 1 marin  staff   6.6M Mar 20  2023 numbers02.txt
-rw-r--r--  1 marin  staff   765K Mar 18 23:01 output.bin
```
