# Homework 3
Overview
This assignment, part of the CS:3620 course for Spring 2024, focuses on memory management in operating systems, particularly in the context of page replacement policies. Students are tasked with implementing and evaluating different page replacement strategies to understand their impact on the operational efficiency of an operating system. This exercise prepares students to handle complex memory management challenges in real-world applications.

Deadline: April 2nd, 2024, 11:59 PM CDT

# General Requirements
Programming language: All tasks must be implemented in C.
For tasks 4 and 5, submissions should be in PDF format.
A README file must be included with instructions for compiling and running the programs, along with any required dependencies.
Submissions should include source code files, makefiles (if applicable), README, and screenshots for the GUI tool.
Ensure that code files reused across tasks are copied to each respective directory, maintaining a clean and organized structure for evaluation.

# Compilation and Execution
To compile the program, navigate to the task-specific directory and run the make command (if a makefile is provided).
Execute the compiled program as instructed in the task-specific README, ensuring all dependencies are met.
Tasks Description
Input file: page_references.txt, containing page references and access types in the format 45 w, 82 w, 9 w, etc.
System has 20 physical frames initially.
# Task 1
Implement FIFO and LRU page replacement policies to report page fault rates based on the given input.

# Task 2
Implement and report the page fault rate using the second chance algorithm.

# Task 3
Implement the enhanced second chance algorithm and report:

# Page fault rate
Number of disk writes on page faults
# Task 4
Increase physical frames to 50 and run the aforementioned algorithms, reporting page fault rates. Discuss observations.

# Task 5
Increase physical frames to 200, run the algorithms again, and report page fault rates. Discuss observations.

# Ensure each task is properly documented and includes a description of the logic, methodology, and analysis of results.
