<p align="center"><img src="assets/logo.png" width="200"></p>

<div align="center">
<h1>Clockwork</h1>
<h3><i>The Collaborative HCE Chess Engine Project</i></h3>

<p></p>

[![Clockwork](https://github.com/official-clockwork/Clockwork/actions/workflows/clockwork.yml/badge.svg)](https://github.com/official-clockwork/Clockwork/actions/workflows/clockwork.yml)
[![License](https://img.shields.io/badge/license-AGPL_3.0-blue)](LICENSE)
[![OpenBench](https://img.shields.io/badge/OpenBench-view-orange)](https://ob.cwchess.org)
[![Discord](https://img.shields.io/badge/chat-Discord-7289DA)](https://discord.gg/7ffYSCae2f)
</div>

## Overview
Clockwork is a **HCE chess engine** capable of playing normal and DFRC chess, developed collaboratively by some of the most active HCE and NNUE developers.
> ### What does HCE mean?
>A **HCE (hand-crafted evaluation)** chess engine uses manually designed algorithms to assess specific features of a position, often based on human chess theory and understanding of the game. In contrast, an **NNUE** chess engine relies on a neural network that automatically learns which features to evaluate, drawing solely from data.

While state-of-the-art HCE hasn’t meaningfully advanced since Stockfish 11 (the last release of Stockfish that didn't make use of NNUE), we believe there’s still untapped potential in handcrafted evaluation, especially in developing new HCE features and implementing new techniques discovered since Stockfish 11.

Several modern HCE projects (incl. Sirius, Weiss, Stash, and Perseus) have come more or less close to Stockfish 11, but remain **50–150 Elo short**. Our mission is to push HCE strength further and become the number one HCE chess engine, exploring new technical innovations.

---

## Mission
- Build a **strong, modern HCE engine** through open collaboration.
- Advance HCE research with **innovative technologies** not found elsewhere.
- Provide a **clean, modern, and open** codebase that encourages contributions.
- Prove that it is possible to create a HCE engine that can hold his own against top NNUE.
- Test search heuristics that are not possible when the evaluation function is a black-box.

### Our Innovations and features
- **Internal autograd engine**
  - Write the evaluation function once, then simply run `make evaltune` to tune on our publicly available data.

- **Incremental AVX2/AVX512 attack tables (affectionately "LilyTech")**
  - Extremely fast generation of attack tables, crucial for efficient HCE engines.

- **Completely original evaluation data**
  - Starting from handcrafted material values, refined via iterative data generation and tuning.

- **Modern C++ (C++20)** with strict styling guidelines and automatic formatting, enforced through our Continuous Integrations pipeline.

- **OpenBench integration**
  - Full testing and data-generation pipeline: [Clockwork OpenBench](https://ob.cwchess.org).

---

## Features
- Support of UCI commands to be used with most Chess GUIs. Doesn't support go `searchmoves` or `ponder`.
- Extra UCI commands:
  - `d` : Print the current position.
  - `eval` : Returns the static evaluation of the current position from the point of view of the side to move.
  - `genfens` : Internal command to generate starting positions for data generation workloads.
  - `speedtest` : Runs a benchmark over a series of internal positions to measure the engine's nps speed.

---

## Contributing
Clockwork welcomes contributions from anyone interested in advancing HCE technology.

### Benchmarks
- Use the built-in `bench` command, which runs a fixed-depth search over internal test positions and outputs a hash-like number (the sum of visited nodes). Make sure that the last commit includes a line containing `Bench: xxxxxx`, where `xxxxxx` is the result of the aforementioned command. Non-compliant commits get automatically flagged by our CI.
- You must always make sure that the engine compiles in both **normal** and **evaltune** mode:
```bash
  make
  make evaltune
```

  - Before committing:
    ```bash
    make format
    ```

### Guidelines

  - **Unit tests:**

      - Clockwork has unit tests which can be found in the `tests/` sub-directory.
      - Unit tests can be run by with `make test`.
      - When making a change that touches a component that has unit tests, you should ensure `make test` displays no errors, especially before submitting a test to OpenBench.
      - We encourage you to write unit tests for any new patches if appropriate.

  - **Nonfunctional changes (no bench change):** - Allowed without testing, but may be verified on OpenBench if performance is in question.

      - If tested, include the result in your PR.

  - **Functional changes (bench changes):** - Test against the latest `main` branch and include results plus bench number in your PR.

  - **Autograd engine changes:** - Must be nonfunctional relative to bench.

      - They will be subject to manual review by autograd developers.
      - For optimization patches, include local speedup results in your PR message.
      - They must **NOT** break the tune for the current evaluation function.

-----

## Data

  - Filtered datasets are available on the [Clockwork Data Downloads](https://data.cwchess.org) webpage. Data is released under the [Open Database License](https://opendatacommons.org/licenses/odbl/).
  - For unfiltered datasets contact a maintainer directly. You can also find the filtering script under the `scripts` folder.

-----

## Building

Clockwork requires **clang++ and LLVM** (latest versions always recommended, clang 17 being the oldest version we test).

  - Build engine:

    ```bash
    make
    ```

  - Build tuner:

    ```bash
    make evaltune
    ```

-----

## Contribute CPU Time

Our project uses a distributed testing framework called [OpenBench](https://github.com/AndyGrant/OpenBench) to rigorously test changes, simplify code, and tune the engine's parameters. By contributing your computer's idle CPU cycles, you can directly help us accelerate development and make Clockwork a stronger engine.

Here’s how you can get started in just a few steps:

-----

### Step 1: Create and Approve Your Account

1.  **Register an account** on our OpenBench website:

      * **https://ob.cwchess.org**

2.  **Request account enable** on our official Discord server:

      * [**Join the Clockwork Discord Server**](https://discord.gg/r2uNZ9x6mr)
      * Once you've joined, go to the `#openbench` channel and ping an `@Maintainer` with your OpenBench username. We'll enable your account as soon as possible.

### Step 2: Set Up the OpenBench Client

First, ensure you have **Python 3** (needed for the worker client), **Git** and **clang** installed on your system. These instructions are for Linux & Windows Subsystem for Linux (WSL).

1.  **Create a Python virtual environment.** This is a best practice that isolates the project's dependencies from your system's global Python packages.

    ```bash
    # Navigate to your home directory and create a virtual environment named 'venv'
    cd ~
    python3 -m venv venv
    ```

2.  **Clone the repository and install dependencies.**

    ```bash
    # Navigate to a directory of your choice (e.g., Desktop)
    cd ~/Desktop

    # Clone the OpenBench project (make sure you are not cloning another engine's fork!)
    git clone https://github.com/official-clockwork/OpenBench
    cd OpenBench/Client

    # Activate the virtual environment
    source ~/venv/bin/activate

    # Install the required Python packages
    pip install -r requirements.txt
    ```

> **Note:** If you close your terminal, the virtual environment will deactivate. To reactivate it, simply run `source ~/venv/bin/activate` again before running the client.

### Step 3: Start Contributing\! :D

#### Choosing Thread and Socket Counts

  * `-T <THREADS>`: The number of CPU cores/threads you want to dedicate. A good starting point is the number of physical cores in your CPU (if the machine isn't used by other processes). Otherwise, it is recomended to leave some cores free for any other tasks you might be performing on your machine. Don't worry about only donating some cores, everything helps!
  * `-N <SOCKETS>`: The number of match runners to use. A good rule of thumb is to use **one socket for every 8 cores/threads**. Divide your core/thread count by 8 and round up to the nearest whole number.

**Examples:**

  * If you want to contribute 8 threads:  `-T 8 -N 1`
  * If you want to contribute 14 threads: `-T 14 -N 2` (since 14 / 8 = 1.75, which rounds up to 2)
  * If you want to contribute 16 threads: `-T 16 -N 2` (since 16 / 8 = 2)

#### Taskset

To improve performance and control CPU usage, you can bind a program to specific CPU cores using the taskset command. On an 8-core / 16-thread system, each core has two hardware threads (logical CPUs). To ensure a program uses only one thread per physical core, determine the CPU layout with `lscpu -e` command, then select every second logical CPU.

> **Note on E-Cores (Intel) / Heterogeneous CPUs:**
> Many modern CPUs (like Intel 12th gen+ or ARM big.LITTLE) have a mix of fast Performance-cores (P-cores) and slower Efficiency-cores (E-cores). In this case please run the client only on your physical cores. Running on E-cores is very inefficient and can potentially slow down testing.
>
> Use `lscpu -e` to identify which logical CPUs belong to your P-cores (they are usually listed first, but you should verify). The goal is the same: select one logical thread from each *P-cores*.

For example:

```bash
CPU NODE SOCKET CORE
  0    0      0    0 <- 1 core
  1    0      0    0
  2    0      0    1 <- 2 core
  3    0      0    1
  4    0      0    2 <- 3 core
  5    0      0    2
  6    0      0    3 <- 4 core
  7    0      0    3
  8    0      0    4 <- 5 core
  9    0      0    4
 10    0      0    5 <- 6 core
 11    0      0    5
 12    0      0    6 <- 7 core
 13    0      0    6
 14    0      0    7 <- 8 core
 15    0      0    7
```

So:

```bash
taskset -c 0,2,4,6,8,10,12,14 python3 client.py \
  -U "YOUR_USERNAME" \
  -P "YOUR_PASSWORD" \
  -S "https://ob.cwchess.org" \
  -T <THREADS> \
  -N <SOCKETS>
```

**Full Command Example:**
If your username is `hce_chess_fan`, your password is `road_to_3600`, and you want to use 16 cores, then your full command should look like:

```bash
taskset -c 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30 python3 client.py \
  -U "hce_chess_fan" \
  -P "road_to_3600" \
  -S "https://ob.cwchess.org" \
  -T 16 \
  -N 2
```

if you don't want to waste time manually selecting cores, you can use this command to automatically select one thread per physical core.

**Warning:** On systems with E-cores, this command may incorrectly select *all* cores (P+E), which is not ideal. Manual selection using the `lscpu -e` output is more precise.

```bash
taskset -c $(lscpu -e | awk '!/CPU/ && !seen[$4]++ {printf "%s,", $1}' | sed 's/,$//') \
  python3 client.py \
  -U "YOUR_USERNAME" -P "YOUR_PASSWORD" \
  -S "https://ob.cwchess.org" \
  -T <THREADS> \
  -N <SOCKETS>
```

-----

## Thank you for helping us make Clockwork stronger!

## Huge thanks to:

  - Our maintainers and growing developer community.
  - Developers from other engines, including top chess engines, for their support and collaboration.
  - Hardware donors who have and continue to make large-scale testing and datagen workloads possible and fast.
  - The engine tester community, who continues to provide an invaluable service to all the chess engine developers just for the love of computer chess.
