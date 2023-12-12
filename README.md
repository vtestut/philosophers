# Philosophers

Welcome to my `philosophers` project !  

In this school project we must solve the dining philosophers problem using multithreading and synchronization. See the [subject](https://github.com/VirgileT/42_Philosphers/blob/main/SUBJECT_philosophers.pdf).

## Project Overview

The `philosophers` project delves into concurrent programming and synchronization techniques. The main goal is to prevent deadlock and ensure fair access to resources. Key features include:

- **Dining Philosophers Problem:**
  - Implement a solution to the classic dining philosophers problem.

- **Multithreading:**
  - Utilize threads to represent philosophers and forks.

- **Synchronization:**
  - Implement proper synchronization mechanisms to avoid conflicts and ensure fairness.

## Contents

- **`/srcs/`:** Contains the source files, including the main `philosophers` program.
- **`/include/`:** Header files, including `philosophers.h`.

## How to Use

1. Clone the repository to your local machine.
2. Navigate to the `/srcs/` directory and review the source files.
3. Compile the project using the provided Makefile (`make`).
4. Run the executable with the specified number of philosophers and other parameters (`./philosophers 5 800 200 200`).

## Parameters

- **Number of Philosophers:**
  - Specify the number of philosophers at the table.

- **Time to Die:**
  - Set the time a philosopher can survive without eating.

- **Time to Eat:**
  - Define the time it takes for a philosopher to eat.

- **Time to Sleep:**
  - Set the time a philosopher spends sleeping.


Special thanks to Ecole 42 for the inspiration and learning opportunity.

Happy coding! 🚀
