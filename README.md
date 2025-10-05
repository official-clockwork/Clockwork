<p align="center"><img src="assets/logo.png" width="200"></p>

<div align="center">
<h1>Clockwork</h1>
<h3><i>The Collaborative HCE Chess Engine Project</i></h3>

<p></p>

[![Clockwork](https://github.com/official-clockwork/Clockwork/actions/workflows/clockwork.yml/badge.svg)](https://github.com/official-clockwork/Clockwork/actions/workflows/clockwork.yml)
[![License](https://img.shields.io/badge/license-AGPL_3.0-blue)](LICENSE)
[![OpenBench](https://img.shields.io/badge/OpenBench-view-orange)](http://clockworkopenbench.pythonanywhere.com)
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
  - Full testing and data-generation pipeline: [Clockwork OpenBench](http://clockworkopenbench.pythonanywhere.com).  

---

## Features  
- Support of UCI commands to be used with most Chess GUIs. Doesn't support go `searchmoves` or `ponder`.  
- Extra UCI commands:  
  - `d` : Print the current position.  
  - `eval` : Returns the static evaluation of the current position from the point of view of the side to move.
  - `genfens` : Internal command to generate starting positions for data generation workloads.    

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

- **Nonfunctional changes (no bench change):**  
  - Allowed without testing, but may be verified on OpenBench if performance is in question.  
  - If tested, include the result in your PR.  

- **Functional changes (bench changes):**  
  - Test against the latest `main` branch and include results plus bench number in your PR.  

- **Autograd engine changes:**  
  - Must be nonfunctional relative to bench.  
  - They will be subject to manual review by autograd developers.  
  - For optimization patches, include local speedup results in your PR message.
  - They must **NOT** break the tune for the current evaluation function.

---

## Data  
- Filtered datasets are available on our **Discord server** (`#data` channel).  
- For unfiltered datasets (too large for Discord), contact a maintainer directly. You can also find the filtering script under the `scripts` folder.

---

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

---

## Thanks 
Huge thanks to:  
- Our maintainers and growing developer community.  
- Developers from other engines, including top chess engines, for their support and collaboration.  
- Hardware donors who have and continue to make large-scale testing and datagen workloads possible and fast. 
- The engine tester community, who continues to provide an invaluable service to all the chess engine developers just for the love of computer chess.