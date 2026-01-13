# Profile Manager CLI

A C++17 command-line application for creating, managing, and persisting user profiles.  
Built as a learning-focused but production-style project to practice clean architecture, state management, and file persistence in modern C++.

---

## Features

The application allows you to:

1. Create user profiles (name, age, city, country)
2. View a profile by ID
3. List all profiles
4. Delete profiles
5. Add and remove hobbies
6. Save profiles to disk
7. Load profiles from disk

Profiles and hobbies are persisted using a custom, delimiter-safe text format.

---

## Architecture Overview

The project is structured using clear separation of concerns:

- **Domain**
    - `Profile` — core data model and behavior
- **Service**
    - `ProfileStore` — manages profile lifecycle and unique IDs
- **Persistence**
    - `ProfileSerializer` — handles saving/loading profiles to/from disk
- **CLI**
    - `Menu` — user interaction and input handling

This mirrors real-world application layering rather than tutorial-style code.

---

## Persistence Format

Profiles are stored in a human-readable text file with a versioned header:

```text
PMCLI1
<id>\t<age>\t<name>\t<city>\t<country>\t<hobby1|hobby2|...>
```

- Special characters are safely escaped
- Hobbies containing delimiters (e.g. `Gym|Weights`) are handled correctly
- Windows and Unix line endings are supported

---

## Tech Stack

- **Language:** C++17
- **Build System:** CMake
- **Standard Library:** STL containers, streams, algorithms
- **IDE:** CLion (JetBrains)

---

## Build & Run

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Run the executable and follow the interactive menu.

---

## Current Status

Core CRUD functionality and persistence are complete.

Planned next steps:
- Update/edit profile fields
- Input validation improvements
- CLI UX refinements (auto-save, confirmations)
- Optional tests

---

## Why This Project Exists

This project was built to move beyond syntax exercises and practice:
- clean C++ design
- ownership and lifetime management
- file I/O and serialization
- defensive programming
- incremental development with meaningful commits
