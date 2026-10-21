# Accepter

A C++ command-line program that recognizes regular languages using deterministic finite automata (DFAs).

The program prints three regular expressions, asks the user to pick one, then reads input strings (spaces allowed) and reports whether each one belongs to the chosen language. It repeats until the user enters the termination symbol.

## Design

- **One struct per language (`DFA1`, `DFA2`, `DFA3`).** Each struct bundles three things:
  - its own `State` enum, with shared `kStart` and `kEnd` names;
  - its regular expression, as an `inline static const char *`;
  - its transition function, as a `std::unordered_map<std::pair<State, char>, State>`.
- **Table-driven transitions.** A templated `PairHash<State>` hashes the `(state, char)` keys, so no `switch` statements are needed.
- **Polymorphism.** An abstract `Accepter` base class declares a pure virtual `accept(std::string)`. The template subclass `AccepterImpl<DFA>` runs the chosen DFA, and `main` creates it on the heap from the user's choice.
- **Extensible.** The list of regexes is built from each DFA's `regex` field, and its size comes from `sizeof`. Adding or removing a language only means editing its struct.

## Build & Run

```bash
g++ -std=c++17 -o accepter accepter.cpp
./accepter
```

## Testing

```bash
chmod u+x test_pa1.sh
./test_pa1.sh
```

## Tech

C++17, STL (`unordered_map`, templates, virtual dispatch).

## Author

Dennis T Sherpa, Compilers, Boston College (Fall 2026)
