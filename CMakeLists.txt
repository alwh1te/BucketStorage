cmake_minimum_required(VERSION 3.27)
project(BucketStorage)

set(CMAKE_CXX_STANDARD 20)

add_executable(BucketStorage main.cpp
        Container.h
        BucketStorage.h
        LinkedList.h
        Bucket.h
        Vector.h)
