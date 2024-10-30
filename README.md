# MultiSet Library

The **MultiSet** library is a C++ implementation of a multiset data structure, which allows storing elements with multiple occurrences. This library utilizes `std::variant` to support different element types, including `std::string` and `std::shared_ptr<MultiSet>`, enabling the creation of nested multisets.

## Features

- **Element Storage**: Supports multiple occurrences of elements.
- **Nested Multisets**: Allows storing multisets as elements.
- **Hashing and Equality**: Custom hash and equality functions for elements.
- **Mathematical Operations**: Provides operations for union, intersection, and difference.
- **Serialization**: Supports reading and writing multisets from/to streams.

## Installation

To use the MultiSet library, you need to include the header file in your project:

```cpp
#include "multiset.hpp"
```

## Usage

### Creating a MultiSet

You can create a multiset by instantiating the `MultiSet` class:

```cpp
MultiSet mySet;
```

### Adding Elements

Use the `AddElement` method to add elements to the multiset:

```cpp
mySet.AddElement("apple");
mySet.AddElement("banana");
mySet.AddElement("apple");  // Increments the count of "apple"
```

### Removing Elements

You can remove elements using the `RemoveElement` method:

```cpp
mySet.RemoveElement("apple");  // Decreases the count of "apple"
```

### Checking Membership

To check if an element is in the multiset, use `IsContains`:

```cpp
if (mySet.IsContains("banana")) {
    // Do something
}
```

### Getting Size

To get the total number of elements (counting duplicates), use:

```cpp
size_t totalSize = mySet.Size();
```

### Building a Boolean Multiset

You can create a boolean version of the multiset where each element appears with a count of 1:

```cpp
MultiSet booleanSet = mySet.BuildBoolean();
```

### Operations

The library supports the following operations:

- **Union**: Combine two multisets:
  ```cpp
  MultiSet unionSet = mySet + otherSet;
  ```

- **Intersection**: Find common elements:
  ```cpp
  MultiSet intersectionSet = mySet * otherSet;
  ```

- **Difference**: Subtract one multiset from another:
  ```cpp
  MultiSet differenceSet = mySet - otherSet;
  ```

### Input/Output

You can read from and write to streams using the overloaded operators:

```cpp
std::cin >> mySet;  // Read MultiSet from input
std::cout << mySet; // Output MultiSet to console
```

## Testing

The MultiSet library includes a comprehensive suite of tests that cover over 90% of the codebase, ensuring reliability and correctness of the implemented features. The tests are designed to validate various functionalities of the library and can be executed to confirm that the library behaves as expected.

## Doxygen Documentation

Doxygen is used to generate documentation for the MultiSet library. It provides a detailed overview of the classes, methods, and functionalities, making it easier for developers to understand and utilize the library effectively.

## Example

Here’s a simple example of how to use the MultiSet library:

```cpp
#include "multiset.hpp"
#include <iostream>

int main() {
    MultiSet mySet;
    mySet.AddElement("apple");
    mySet.AddElement("banana");
    mySet.AddElement("apple");

    std::cout << "Size: " << mySet.Size() << std::endl; // Output: Size: 3

    mySet.RemoveElement("apple");

    std::cout << "Size after removal: " << mySet.Size() << std::endl; // Output: Size after removal: 2

    return 0;
}
```
