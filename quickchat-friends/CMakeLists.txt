cmake_minimum_required(VERSION 3.17)

project(
        quickchat-friends
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(mysql REQUIRED)
find_path(MYSQLPP_HEADER mysql++)

find_library(MYSQLPP_LIB mysqlpp)
find_library(MYSQLCLIENT_LIB mysqlclient)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

find_package(gRPC REQUIRED)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy "-checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*")

add_executable(
        quickchat-friends 
        src/main.cpp
        src/gen_id_service_impl.cpp 
        src/logger.cpp

        src/friend_service.grpc.pb.cc
        src/friend_service.pb.cc
        src/user.pb.cc
        src/friend.pb.cc

        include/proto/friend_service.grpc.pb.h
        include/proto/friend_service.pb.h
        include/proto/user.pb.h

        include/logger.hpp
        include/gen_id_service.h 
        include/gen_id_service_impl.h)

target_include_directories(
        quickchat-friends
        PRIVATE
        include
        include/proto
        mysql::mysql
        ${MYSQLPP_HEADER}
)

target_link_libraries(
        quickchat-friends
        protobuf 
        gRPC::grpc++
        Threads::Threads
        z
        mysqlpp
        mysqlclient_r
)

configure_file(config.json config.json COPYONLY)
