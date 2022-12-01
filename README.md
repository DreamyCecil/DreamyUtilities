# Dreamy Utilities

**Dreamy Utilities** is a framework of simple header libraries that extend C++ functionality via a collection of convenient modules.

The main inspirations behind many design choices are Boost and Qt frameworks.

## Utilizes
- C++ Standard Library
- [Boost 1.34.1](https://www.boost.org/users/history/version_1_34_1.html) classes per library:
  - **MarkovChain** (optional): `boost::mt19937`, `boost::uniform_int`, `boost::uniform_real`, `boost::random_device`

## Notes

- It is written primarily for `MSVC` and `GCC` compilers:
  - **MSVC**: 6.0 (`C++98`), 12.0 (`C++11`)
  - **GCC**: 9.4.0 (`C++98` and `C++11`)
- It's been decided to settle on Boost 1.34.1 because it's the highest version that's still properly supported by `MSVC 6.0`.
  - Any version higher than 1.34.1 causes internal compiler errors (without Service Pack 6) or just isn't supported by `MSVC 6.0`.
  - You may utilize any version you'd like if you're using `C++11`.
- **This project is still heavily work-in-progress as well as any documentation for it!**
  - It's made primarily for my own personal use to make all modules compatible with each other and to avoid copying the same code across several projects.
  - The framework is designed to be as self-documenting and easily understandable as possible by having a lot of comments attached to each class and function.

## Customizable macros

There are a few macros that can be defined before including any of the **Dreamy Utilities** headers to customize the framework's behavior. If left undefined, the framework either defines them with a default value or locks specific functionality.

| Macro | Description | Default behavior |
| --- | --- | --- |
| `_DREAMY_CPP11` | Allow using features from `C++11`. | Set to `0` or `1`, depending on the compiler used (e.g. `0` if using `MSVC 6.0`). |
| `_DREAMY_NDEBUG_ASSERT` | Allows run-time errors upon assertion failure in non-Debug mode. | Assertions are disabled in non-Debug mode. |
| `_DREAMY_BYTE_ORDER` | Sets default endianness for the current architecture. | Set to `0` (little endian) instead of `1` (big endian). |
| `_DREAMY_BOOST_RANDOM_DEVICE` | Allows usage of Boost's non-deterministic randomization algorithms in MarkovChain library (doesn't work with `MSVC 6.0`). | Uses STL's non-deterministic randomization algorithms (in `C++11`) or simplistic `rand()` randomization (in `C++98`). |
| `_DREAMY_STL_EXCEPTION` | Allows `dreamy::CException` class to be derived from STL's exception class. | Isn't derived from the STL's exception class. |
| `_DREAMY_INDENT_WITH_SPACES` | `dreamy::CVariant` values can be indented with a specific amount of spaces upon formatted printout. | If not defined with a positive integer, indents with a singular `\t` character. |
| `_DREAMY_DEFAULT_AXES` | Sets default axes in 3D coordinate space. | Y faces upwards instead of Z. |

## License

**Dreamy Utilities** is licensed under the MIT license (see `LICENSE`).
