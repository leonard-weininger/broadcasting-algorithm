# Broadcasting Algorithm

A C++ implementation of an algorithm for solving the telephone broadcasting problem on graphs in $O(nk) + 2^{O(k \text{ log } k)}$ time, where $n$ is the number of vertices and $k$ is the cyclomatic number. It is based on the algorithm presented in ([Fomin et al. 2024](#references)).

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Building the Project](#building-the-project)
3. [Running the Solver](#running-the-solver)
   - [Graph Format](#graph-format)
   - [Protocol Format](#protocol-format)
   - [Example Input and Ouput](#example-input-and-output)
   - [Optional Flags](#optional-flags)
4. [Generating Random Graphs](#generating-random-graphs)
5. [Running Tests](#running-tests)
6. [Benchmark](#benchmark)
   - [Results](#results)
7. [Background](#background)
   - [Definition Telephone Broadcasting Problem](#definition-telephone-broadcasting-problem)
   - [Definition Cyclomatic Number](#definition-cyclomatic-number)
8. [License](#license)
9. [References](#references)

---

## Prerequisites

To build and run this project, you need the following:

- A C++17 compatible compiler (e.g., `g++` 7+, `clang++` 5+, or MSVC 2017+)
- [CMake](https://cmake.org/) version 3.16 or higher

---

## Building the Project

1. Clone the repository:

```bash
git clone https://github.com/leonard-weininger/broadcasting-algorithm.git
cd broadcasting-algorithm
```

2. Create a build directory, configure the project with CMake and build the executables:
```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build
```

---

## Running the Solver

To solve a broadcasting problem, run the `solve` executable from the `build` directory. The program reads an undirected, simple, and connected graph from standard input, and outputs an optimal broadcasting protocol to standard output.

**Linux / macOS:**
```bash
./build/solve
```

**Windows:**
```cmd
build\solve.exe
```

### Graph Format

The input graph must be simple and connected and must be provided in the following format:

```
n m s
u1 v1
u2 v2
...
um vm
```

- `n`: number of vertices (numbered `0` to `n-1`)
- `m`: number of edges
- `s`: source vertex
- Each `ui vi` is an undirected edge

### Protocol Format

The output format of the computed broadcasting protocol is:

```
r
ROUND_1
ROUND_2
...
ROUND_r
```

- `r` is the number of rounds
- Each `ROUND_i` is formatted as:

  ```
  k
  u1 v1
  u2 v2
  ...
  uk vk
  ```

- `k` is the number of transmissions in round `i`
- Each line `uj vj` means vertex `uj` informs vertex `vj` in that round

The computed protocol will satisfy the following constraints:

1. If `u` informs `v`, then `{u, v}` is an edge in the given graph.
2. If `u` informs `v`, then `u` is either the source vertex or `u` has been informed in a previous round.
3. The source vertex is never informed. All other vertices are informed exactly once.
4. Each vertex informs at most one other vertex per round.
5. The number of rounds is minimized.

### Example Input and Output

<table>
<tr>
<th>Example Input Graph</th>
<th>Example Output Protocol</th>
</tr>
<tr>
<td>

```
9 10 0
0 1
1 2
1 6
1 8
2 3
2 6
4 8
5 6
5 7
7 8
```

</td>
<td>

```
4
1
0 1
1
1 8
2
1 2
8 7
4
1 6
2 3
8 4
7 5
```

</td>
</tr>
</table>

Here is a visualization of this example graph and protocol:

```
     Graph        Source        Round 1      Round 2      Round 3       Round 4

   2---6---5     .---.---.     .---.---.    .---.---.    2---.---.     2---6---5
  / \ /   /     / \ /   /     / \ /   /    / \ /   /    / ↖ /   /     ↙ \ ↗   ↗
 3   1   7     .   .   .     .   1   .    .   1   .    .   1   7     3   1   7
    / \ /         / \ /         ↗ \ /        / ↘ /        / \ ↗         / \ /
   0   8---4     0   .---.     0   .---.    0   8---.    0   8---.     0   8 → 4
```


### Optional Flags

- `--verbose` - print runtime statistics to `std::cerr`
- `--no-output` - the computed protocol is not printed

---

## Generating Random Graphs

To generate random graphs with given parameters, run the `generate` executable from the `build` directory. The generated graph will be printed to standard output in the same format as described in the [Graph Format](#graph-format) section.

**Linux / macOS:**
```bash
./build/generate <n> <k> [seed] [--verbose]
```

**Windows:**
```cmd
build\generate.exe <n> <k> [seed] [--verbose]
```

- `n` - number of vertices (`n ≥ 1`)
- `k` - cyclomatic number (`0 ≤ k ≤ n(n - 1)/2 - n + 1`)
- `seed` - optional integer seed (by default a random seed is used)
- `--verbose` - print runtime statistics to `std::cerr`

---

## Running Tests

By default, tests are not built. To include them, configure the project with the `ENABLE_TESTS` option set to `ON`:

```bash
cmake -DENABLE_TESTS=ON -S . -B build
cmake --build build
```

You can run all tests using CTest:

```bash
cd build
ctest
```

---

## Benchmark

To run a benchmark of the algorithm on a randomly generated graph without saving any intermediate files, use:

```bash
./benchmark/benchmark.sh <n> <k> [seed]
```

- `n` - number of vertices (`n ≥ 1`)
- `k` - cyclomatic number (`0 ≤ k ≤ n(n - 1)/2 - n + 1`)
- `seed` - optional integer seed (by default a random seed is used)

> **Note:** This benchmark script is supported only on **Linux** and **macOS**.

### Results

The following tables show execution times of the algorithm (excluding graph generation, file I/O, and protocol validation). All times are in seconds and represent averages over 10 runs on random graphs with the given parameters.

#### Varying `n` with fixed `k = 5`

| `n`      | 10¹  | 10²  | 10³  | 10⁴  | 10⁵   | 10⁶   | 10⁷   | 10⁸    |
| -------- | ---- | ---- | ---- | ---- | ----- | ----- | ----- | ------ |
| Time (s) | 0.00 | 0.00 | 1.10 | 6.85 | 20.79 | 26.67 | 44.53 | 448.50 |

#### Varying `k` with fixed `n = 10⁵`

| `k`      | 1    | 2    | 3    | 4    | 5    | 6     | 7      | 8         |
| -------- | ---- | ---- | ---- | ---- | ---- | ----- | ------ | --------- |
| Time (s) | 0.05 | 0.10 | 0.15 | 0.18 | 0.56 | 10.94 | 661.87 | 105041.80 |

---

## Background

### Definition Telephone Broadcasting Problem

**Given:**

- $G = (V, E)$, a connected simple graph
- $s \in V$, the source vertex of a message

**Procedure per round:**

Every informed vertex may forward the message to at most one of its neighbors.

**Problem:**

Find a strategy to inform all vertices in the graph in the minimum number of rounds. 

### Definition Cyclomatic Number

Minimum number of edges that must be removed from a graph to make it acyclic.
In connected graphs with `n` vertices and `m` edges the cyclomatic number is `m - (n - 1)`.

---

## License

This project is licensed under the terms of the [MIT License](./LICENSE).

---

## References

- Fedor V. Fomin, Pierre Fraigniaud, and Petr A. Golovach.
  *Parameterized complexity of broadcasting in graphs*.
  Theoretical Computer Science, Volume 997, 2024, Article 114508.
  [https://doi.org/10.1016/j.tcs.2024.114508](https://doi.org/10.1016/j.tcs.2024.114508)
