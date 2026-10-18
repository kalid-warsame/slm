# slm – Small Language Model

A command-line tool that trains a character-level n-gram language model on a text file and generates random text from it.

Work in progress.

## Building

```
make        # builds slm
make clean  # removes generated files
```

## Usage

```
slm [-a pseudocount] n training_file length
```

Invalid arguments are reported on standard error, and the program exits with status 1.
