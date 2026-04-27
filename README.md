# Project Title

Honeybee Word Solver in C++

# Overview / Description

This project is a custom implementation of a word solver inspired by the New York Times “Spelling Bee” puzzle, built entirely in C++. The program takes in a set of letters (referred to as a "hive") and generates all valid English words that can be constructed under specific constraints. These constraints include requiring a central letter in every word and only using letters from the given hive.

I developed this project as part of CS 211 to strengthen my understanding of low-level programming concepts, including C-style string manipulation, memory management, and efficient search algorithms. The main goal of the project was to simulate real-world problem solving by working with large datasets (dictionary files) and optimizing performance for fast lookups.

This project demonstrates how fundamental computer science concepts—such as searching, string processing, and algorithm optimization—are applied to build efficient and scalable programs. It also highlights the importance of writing clean, efficient code when working with constrained resources and large input sizes.

# Key Features and Functionality

- Generates valid words from a constrained set of letters (hive input)
- Enforces required letter usage in every generated word
- Validates words against a dictionary using efficient search techniques
- Identifies special word categories such as:
  - Pangrams (words using all hive letters)
  - Perfect pangrams (use each letter exactly once)
- Uses optimized lookup methods to improve performance on large datasets
- Outputs results including total valid words, pangram counts, and categorized results
- Handles dynamic input and supports both preset and randomized hive configurations

# Tech Stack

- **Language:** C++
- **Concepts:** String manipulation, search algorithms, binary search, memory management, dynamic arrays
- **Tools:** GCC, command line, Makefile, Valgrind, GDB

# Implementation Details

The program operates by loading a dictionary of valid words into memory and iterating through possible word combinations based on the hive letters. Each candidate word is validated using a custom checking function that ensures all constraints are satisfied.

To improve efficiency, I implemented a modified binary search approach for dictionary lookup instead of using brute-force searching. This significantly reduced the runtime when working with large word lists. Additionally, I used pointer-based data structures to manage memory dynamically and avoid unnecessary duplication of data.

One of the more complex aspects of the implementation was handling C-style strings, including comparisons, copying, and ensuring null-termination. Careful attention was required to avoid memory leaks and segmentation faults, especially when dynamically allocating and freeing memory.

# Installation and Usage Instructions

1. Clone or download the project files
2. Open the project in a C++ development environment or terminal
3. Compile the program using a C++ compiler:
   ```bash
   g++ main.cpp -o solver
