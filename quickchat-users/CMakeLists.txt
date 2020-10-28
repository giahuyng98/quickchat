cmake_minimum_required(VERSION 3.17)

project(
        quickchat-user
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

find_path(BCRYPT_HEADER bcrypt)
find_library(BCRYPT_LIB bcrypt)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

find_package(gRPC REQUIRED)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)


#set(CMAKE_CXX_CLANG_TIDY clang-tidy "-checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*")

add_executable(
        quickchat-user 
        src/main.cpp
        src/user_info.cpp
        src/user_dao_impl.cpp
        src/gen_id_service_impl.cpp 
        src/session_mgr_impl.cpp 
        src/user_controller.cpp 
        src/logger.cpp

        src/user.pb.cc
        src/user_service.pb.cc
        src/user_service.grpc.pb.cc

        include/proto/user.pb.h
        include/proto/user_service.grpc.pb.h
        include/proto/user_service.pb.h

        include/user_repository.hpp
        include/user_repository_impl.hpp

        include/logger.hpp
        include/user.h
        include/user_info.h
        include/user_dao_impl.h
        include/session_mgr.hpp 
        include/session_mgr_impl.hpp 
        include/user_controller.hpp 
        include/user_dao.h 
        include/gen_id_service.h 
        include/gen_id_service_impl.h)

#add_executable(grpc_server src/proto/mathtest.pb.cc src/proto/mathtest.grpc.pb.cc src/grpc-server.cpp)
#add_executable(grpc_client src/proto/mathtest.pb.cc src/proto/mathtest.grpc.pb.cc src/grpc-client.cpp)
#add_library(libbb src/proto/mathtest.pb.cc src/proto/mathtest.grpc.pb.cc src/grpc-server.cpp)

#target_link_libraries(grpc_server pthread protobuf gRPC::grpc++)
#target_link_libraries(grpc_client pthread protobuf gRPC::grpc++)

target_include_directories(
        quickchat-user
        PRIVATE
        include
        include/proto
        mysql::mysql
        ${MYSQLPP_HEADER}
        ${BCRYPT_HEADER}
        ${REDIS_PLUS_PLUS_HEADER}
)

target_link_libraries(
        quickchat-user
        pthread
        /usr/lib/uSockets.a
        nsl
        z
        m
        mysqlpp
        mysqlclient_r
        bcrypt
        hiredis
        ${REDIS_PLUS_PLUS_LIB}
        protobuf 
        gRPC::grpc++
)

configure_file(config.json config.json COPYONLY)
