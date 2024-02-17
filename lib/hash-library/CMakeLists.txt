cmake_minimum_required(VERSION 3.24)

project(hash-library VERSION 8.0.0 DESCRIPTION "Portable C++ hashing library by Stephan Brumme.")

include(GNUInstallDirs)

add_library(hash-library

        src/crc32.cpp
        src/digest.cpp
        src/keccak.cpp
        src/md5.cpp
        src/sha1.cpp
        src/sha3.cpp
        src/sha256.cpp
        include/crc32.h
        include/hash.h
        include/hmac.h
        include/keccak.h
        include/md5.h
        include/sha1.h
        include/sha3.h
        include/sha256.h
        )


set_target_properties(hash-library PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(hash-library PROPERTIES SOVERSION 8)

target_include_directories(hash-library PUBLIC include)