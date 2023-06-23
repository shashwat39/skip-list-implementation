# Skip List Implementation

This code implements a skip list data structure, which is a probabilistic data structure used for efficient searching and insertion operations. It allows searching for an element in O(log n) time with high probability. The skip list is a linked list structure with multiple layers, where each layer is a subsequence of the layer below it. It uses randomization to determine the height of each node, allowing for efficient search and insertion operations.

## Features

- Search: The `search` function checks if a given element is present in the skip list in O(log n) time.
- Insert: The `insert` function inserts a new element into the skip list while maintaining the required order in O(log n) time.
- Delete: The `del` function deletes an element from the skip list while maintaining the required order in O(log n) time.
- Minimum: The `min` function returns the minimum value among the elements in the skip list in O(log n) time.
- Traverse: The `traverse` function prints the entire skip list, level by level.


## Technologies Used

- C++: The code is written in C++ programming language.

## Setup
To run the code, follow these steps:
- Compile the code using a C++ compiler.
- Execute the compiled binary file.

## Contributions
Contributions to the code are welcome. If you find any bugs or want to add new features, feel free to create a pull request.

## Preview
Below is a preview of the skip list implementation:

<img width="371" alt="Screenshot 2023-06-24 012417" src="https://github.com/shashwat39/skip-list-implementation/assets/110004769/46ddf5b8-4fae-4ed9-8e6b-ad5fb4740764">
