# Data-Parsing-and-Reporting-Toolkit

This repository contains a C-based application designed for processing, sorting, and summarizing structured data stored in Tab-Separated Values (TSV) files. Developed as part of an academic project for the COMP10002 Foundations of Algorithms course, this program showcases practical applications of core programming concepts and efficient data manipulation techniques.

## Overview
The program tackles a variety of tasks related to TSV files, a common format for structured textual data. It processes input data to provide actionable insights and formatted reports. The program consists of three main stages:

1. **Data Parsing and Validation**
   - Reads TSV data from standard input and stores it in a 2D array of strings.
   - Handles up to 1,000 rows, 30 columns, and 50-character entries.
   - Ensures clean and error-free parsing by adhering to consistent input formatting.

2. **Custom Sorting**
   - Allows sorting of rows based on one or more user-specified columns.
   - Implements stable insertion sort, preserving the original order for equal entries.
   - Supports hierarchical sorting, e.g., sorting by Country first, then Event.

3. **Summarized Reporting**
   - Generates detailed tabulated summaries of the data.
   - Counts occurrences of rows grouped by specific column values.
   - Outputs formatted reports aligned for readability and professionalism.

## Documentation
For detailed description about the tasks, please refer to `ass1 (1).pdf`. Furthermore, there are some sample testcases (like `test0.txt`) and outputs (like `test0-out.txt`) for testing functions. 


## Applications
This project provides a foundation for real-world data processing applications, including:
- Organizing and analyzing structured datasets in TSV format.
- Producing human-readable summaries and reports for tabular data.
- Implementing custom sorting techniques based on dynamic user preferences.

## How to Use
1. Compile the program using a C compiler like `gcc`.
2. Run the program, providing a TSV file as input and sorting keys as optional arguments.

### Example:
```bash
gcc -o tsv_processor main.c
./tsv_processor < input.tsv > output.txt

