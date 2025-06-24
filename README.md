# RedSeen

## Intro

This is a 3D game engine written in C++ and designed around the event driven architecture and extensibility.  
It supports OpenGL for now, but support for more APIs is possible in the future.

## Usage

Look into the demos/ directory for examples how to use the engine in action.

## Requirements

- glfw3
- glm
- sqlite3

## Building

To build a static library of RedSeen:
```sh
$ cmake -Bbuild
$ cd build
$ make
```

To build demos:
```sh
$ cmake -DBUILD_DEMOS -Bbuild
$ cd build
$ make
```

## Contributing

This project is highly WIP and all people who wish to contribute are welcome!  
See CONTRIBUTING.md if you're interested.

## Copyright

RedSeen is licensed under the GNU Lesser General Public License v3 (LGPLv3). See LICENSE.  
For the list of contributors, see CONTRIBUTORS.md.
