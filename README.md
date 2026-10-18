# slm – Small Language Model

`slm` is a command-line tool that trains a character-level n-gram language model on a text file and uses it to generate new, random "text". The output is not meant to make sense; it is an Artificial Idiot. With a larger n, more real words appear.

## Building

Requires a C++17 compiler (g++) and make.

```
make          # builds the executable slm
make test     # builds and runs the unit tests
make clean    # removes all generated files
```

## Usage

```
slm [-a pseudocount] n training_file length
```

| Argument | Meaning |
|---|---|
| `n` | n-gram length, a positive integer |
| `training_file` | text file to train the model on |
| `length` | number of characters to output, including the starting n-gram; at least `n` |
| `-a pseudocount` | optional pseudocount α ≥ 0 added to every transition count (default 0) |

Examples:

```
$ ./slm 2 moby_dick.txt 5
it we
$ ./slm -a 0.01 3 moby_dick.txt 40
```

With α = 0 the generator stops early if it reaches an n-gram that only occurs at the very end of the training text, so the output can be shorter than requested. With α > 0 this cannot happen.

## How it works

Training slides a window of length n over the text and records:

- how many times each n-gram w occurs, which gives its frequency f(w) = count(w) / (number of n-grams), and
- N(w, c), the number of times character c follows w. An occurrence of w at the very end of the text is followed by nothing and is not counted.

The probability of the next character is estimated as

P(c | w) = (N(w, c) + α) / (N(w) + α·|A|)

where N(w) is the sum of N(w, c) over all c, and A is the alphabet, i.e. the distinct characters in the training text. With α = 0 this is the plain relative frequency; with α > 0 every character gets a positive probability, and an n-gram never seen in the training text gets the uniform distribution.

Generation picks a starting n-gram according to the n-gram frequencies, then repeatedly samples the next character from P(· | w) with `std::discrete_distribution`, appends it, and shifts the window one step.

## Code structure

```
Makefile
src/
  Model.h/.cpp      class Model: training, frequencies, transition probabilities
  Generator.h/.cpp  class Generator: random text generation from a Model
  Args.h/.cpp       command-line parsing
  main.cpp          ties it together and catches exceptions
tests/
  test_model.cpp    unit tests based on the examples in the lab text
```

`Model` stores counts in `std::map`s: n-gram → count, and n-gram → (character → count). Probabilities are computed from the counts when needed, so changing α does not require retraining. `Generator` only depends on the public interface of `Model`, so the model can be reused for other purposes, for example computing probabilities of a given text.

## Error handling

User errors are detected where they occur and reported by throwing standard exceptions (`std::invalid_argument` for bad arguments, `std::runtime_error` for file and text problems). `main` catches them, prints the message to `std::cerr` and exits with status 1. Handled errors:

- wrong number of arguments
- `n` or `length` not a positive integer, or `length` < `n`
- `-a` without a non-negative number, or an unknown option
- a training file that cannot be opened
- a training text too short to contain an n-gram followed by a character

```
$ ./slm 2 no_such_file.txt 100
slm: cannot open file 'no_such_file.txt'
```

## Testing

`make test` checks the model against Examples 1 and 2 and the pseudocount example in the lab text, checks that generated output has the right length and starts with an n-gram from the text, and checks that the error cases throw.
