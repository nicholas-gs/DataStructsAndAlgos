# Searching Algorithms

All search algorithms are templated. They accept stl standard containers such as `vector` or `list` and traverses through them to search for occurance of the given element. Elements have to be sorted in the containers first before searching.

## Comparator

By default, the search is done using `operator<`. A custom binary predicate can be passed in as a template argument, which is used to compare the elements.

A custom binary predicate is required if

1. Elements are not sorted in ascending order
2. Custom type which do not overload `operator<`

## Return type

Search result is a pair of type `std::pair<std::optional<typename ContainerType::const_iterator>, std::size_t>`. The first element of the pair is a `const_iterator` wrapped by `std::optional`. If the specified element was not found in the container, then `std::optional` is equal to `std::nullopts`. If found, then its value is the iterator pointing to the element in the container.

The second value is the index position of the found element. That is, how much must the front iterator be incremented by to point to the specified element. This value is `-1` if element was not found.

## Algorithm Usage

### Default predicate

```c++
std::vector<double> vector;
// Ascending order
vector.push_back(-5.1);
vector.push_back(-5.05);
vector.push_back(1.1);
vector.push_back(2);
vector.push_back(5.6);

auto result = wtl::TernarySearch<std::vector<double>>::search(vector, 1.1);
if(result.first.has_value()) {
    std::size_t position = result.second;
} else {
    std::cout << "Value not found" << '\n';
}
```

### Custom predicate
```c++
std::vector<int> vector1;
// Descending order
vector1.push_back(10);
vector1.push_back(7);
vector1.push_back(2);
vector1.push_back(0);
vector1.push_back(-2);

auto result1 = wtl::BinarySearch<std::vector<int>, Compare>::search(vector1, 0);
if(result1.first.has_value()) {
    std::size_t position = result1.second;
} else {
    std::cout << "Value not found" << '\n';
}

// Binary predicate
struct Compare {
    bool operator()(int lhs, int rhs) const {
        return lhs > rhs;
    }
};
```

### Custom type
```c++
std::vector<Entity> vector1;
// Descending order
vector1.push_back(Entity(10));
vector1.push_back(Entity(7));
vector1.push_back(Entity(2));
vector1.push_back(Entity(0));
vector1.push_back(Entity(-2));

auto result1 = wtl::BinarySearch<std::vector<Entity>, Compare>::search(vector1, Entity(0));
if(result1.first.has_value()) {
    std::size_t position = result1.second;
} else {
    std::cout << "Value not found" << '\n';
}

// Binary predicate
struct Compare {
    bool operator()(const Entity& lhs, const Entity& rhs) const {
        return lhs.m_Id > rhs.m_Id;
    }
};
```

The custom predicate is unnecessary if `Entity` overloaded `operator<` and the vector was sorted in ascending order.
