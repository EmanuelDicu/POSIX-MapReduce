# Map Reduce Paradigm using POSIX API

This project implements the map-reduce paradigm using POSIX threads. It consists of two types of threads: **mappers** and **reducers**. Mappers process input files containing numbers, check if each number is of the form base^exponent, and add it to a specific list for each reducer. Once all mappers finish processing all input files, the reducers concatenate the specific lists obtained from each mapper, count the number of unique elements, and print the result to a designated output file.

## Project Structure

- **data_types:** This section includes structures for a list of integers, thread data (which contains all the shared data between all mappers and reducers), and thread info (which contains data specific to each thread, as well as the data shared between all threads).

- **handle_error:** This section contains methods used when procedures fail.

- **map_implement and reduce_implement:** These sections contain the specific implementations of the map and reduce methods described above.

- **map_reduce:** This section includes the generic map/reduce methods that call the specific methods from the sections mentioned earlier. For further details about the decoupling of generic map and reduce from the specific implementations, refer to the `README_BONUS` file.

- **reduce_helpers and numeric_helpers:** These sections contain additional methods used by mappers and reducers.

## Execution

1. Run `make` to compile the project.

2. Execute the program with the following command:

   ```
   ./mapreduce <num_mappers> <num_reducers> <input_file>
   ```

## Operation

1. The main thread reads the main file and creates a list of input files that is shared with all mapper threads.

2. Each mapper runs as a separate thread and processes input files until there are no unprocessed files left. To ensure that no two mapper threads attempt to process the same input file simultaneously, a mutex is shared among all mapper threads and is locked before a mapper selects an input file for processing.

3. After selecting an input file, the mapper starts reading the numbers from that file and adds them to the lists of exponents for that number.

4. All threads start simultaneously. Mappers wait for a barrier at the end (after processing all files), while reducers wait for a barrier at the beginning (before merging lists returned by mappers). This ensures that reducers only begin processing numbers after all mappers finish processing input files and adding the numbers to the corresponding exponent lists.

## Generic Procedures

- The mapper and reducer procedures are generic. Each mapper processes input files one by one and calls a specific implementation called `handle_input_file`, which receives an input file and a list of type `thread_list` and processes the input file, adding the resulting data to the list.

- Each reducer concatenates lists returned by mappers using a specific procedure and then calls a specific reduce implementation procedure, which counts the number of unique elements.

## User Implementation

- Users need to implement the thread types, which are called `thread_info` and `thread_data` from the `data_types` header. In this header, `struct thread_list *thread_list;` can be replaced by any implementation-specific structure.

## Results

After the reducers merge the lists returned by mappers, each reducer transforms the aggregated list into an array, sorts it, counts the number of unique elements, and prints the result to a specific output file.