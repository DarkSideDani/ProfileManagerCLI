# Profile Manager CLI

A C++17 command-line application for creating, managing, and persisting user profiles.  
Built as a learning-focused but production-style project to practice clean architecture, state management, and file persistence using modern C++.

## Features

The application allows you to:

1. Create user profiles (name, age, city, country)
2. View a profile by ID
3. List all profiles
4. Delete profiles
5. Add and remove hobbies
6. Save profiles to disk
7. Load profiles from disk
8. Update profiles

Profiles and hobbies are persisted using a custom, delimiter-safe text format.


## Architecture Overview

The project is structured with a clear separation of concerns, closely mirroring real-world application design rather than tutorial-style code:

- **Domain**
  - `Profile` — core data model, encapsulating state, validation, and domain behavior
- **Service**
  - `ProfileStore` — manages profile lifecycle, ownership, and unique ID generation
- **Persistence**
  - `ProfileSerializer` — responsible for serializing and deserializing profiles to/from disk
- **CLI**
  - `Menu` — handles all user interaction, input validation, and command dispatch

Each layer has a single responsibility and communicates through well-defined interfaces.



## Persistence Format

Profiles are stored in a human-readable text file with a versioned header:

```text
PMCLI1
<id>\t<age>\t<name>\t<city>\t<country>\t<hobby1|hobby2|...>
```
Key characteristics:
- Text fields are safely escaped to prevent delimiter conflicts
- Hobbies containing separators (e.g. `Gym|Weights`) are handled correctly
- Both Windows (CRLF) and Unix (LF) line endings are supported
- The version header allows future format evolution

---

## Current Status

The application implements complete CRUD functionality with persistence.

Implemented highlights:
- In-memory storage with deterministic ID management
- Robust CLI input handling (invalid input does not crash the application)
- Field updates with validation
- Safe serialization and deserialization with defensive parsing

## Why This Project Exists

This project was built to move beyond syntax exercises and intentionally practice:
- object-oriented design in C++
- encapsulation and const-correctness
- ownership and lifetime management
- file I/O and custom serialization
- defensive programming
- incremental development with meaningful Git commits

The focus was on writing maintainable, understandable code rather than maximizing features.

## Build & Run

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
Run the executable and follow the interactive menu.
