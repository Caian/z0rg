# z0rg
Helper class to enable zero copy when working with iterators that directly reference it's data

## Introduction

When dealing with iterators, it is a common practice to express loops in the following way:

```C++
for (It it = container.begin(); it != container.end(); it++) {
    Result result;
    object.something(result);
    *it = result;
}
```

The problem is that the temporary object `result` may not be necessary because some iterators allow direct referencing to the pointed object. In fact, most of them do, exceptions though are `std::back_inserter_iterator` and `std::front_inserter_iterator`, whose dereferencing operation pushes another element to the container so getting a reference to an object is impossible. Fortunately, there is the `std::iterator_traits<>::reference` type to tell if an iterator enables a proper dereferencing.

This project uses `iterator_traits` and constructs a small interface to switch, in compile-time, between the iterator's reference and a fallback internal state, such that the most performant version will be automatically selected.

## Usage

To use z0rg, one must simply rewrite the previous loop as it follows, using the `z0rg::zero_copy` template:

```C++
#include "z0rg/zero_copy.hpp"

...

for (It it = container.begin(); it != container.end(); it++) {
    z0rg::zero_copy<Result, It> result;
    object.something(result.get(it));
    result.set(it);
}
```

Calling `result.get(it)` always return a valid reference, whether it is the one from the iterator, or the fallback provided. The `result.set(it)` is called to ensure that the fallback is always assigned to the iterator.

Because the iterator is always passed to `get` and `set`, the object can also be reused:

```C++
z0rg::zero_copy<Result, It> result;
for (It it = container.begin(); it != container.end(); it++) {
    object.something(result.get(it));
    result.set(it);
}
```

There is also the `z0rg::zero_copy_scope` template, a RAII scoped implementation that avoids passing the iterator at every call. This implementation also calls `set` automatically when the destructor is called:

```C++
#include "z0rg/zero_copy_scoped.hpp"

...

for (It it = container.begin(); it != container.end(); it++) {
    z0rg::zero_copy_scope<Result, It> result;
    object.something(result.get(it));
}
```

### Ensuring zero-copy

To check if the zero-copy version of the template is being used instead of the fallback, z0rg also provides a trait compatible with `zero_copy` and `zero_copy_scoped`:

```C++
#include "z0rg/traits/is_zero_copy.hpp"

...

typedef z0rg::zero_copy<Result, It> ZC;
bool is_zero_copy = z0rg::traits::is_zero_copy<ZC>::value;
```