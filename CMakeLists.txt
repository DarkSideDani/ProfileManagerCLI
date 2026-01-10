cmake_minimum_required(VERSION 3.20)
project(ProfileManagerCLI)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProfileManagerCLI src/main.cpp
        src/cli/Menu.cpp
        src/cli/Menu.hpp
        src/domain/Profile.cpp
        src/domain/Profile.hpp
        src/persistence/ProfileSerializer.cpp
        src/persistence/ProfileSerializer.hpp
        src/service/ProfileStore.cpp
        src/service/ProfileStore.hpp)
