cmake_minimum_required(VERSION 3.17)

project(quickchat-conn
        VERSION 1.0
        DESCRIPTION "QuickChatConn..."
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

configure_file(config.json config.json COPYONLY)
#include(cotire)
#include(tools)
#clang_tidy(-format-style=file -checks=* -header-filter='${CMAKE_SOURCE_DIR}/*')
#include_what_you_use(-Xiwyu)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy "-checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*")

find_package(nlohmann_json REQUIRED)
find_package(CppKafka REQUIRED)
#find_package(doctest REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads)
find_package(Doxygen)
find_package(uWebSockets REQUIRED)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)


find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

#message(STATUS "here: ${REDIS_PLUS_PLUS_LIB}")

#find_package(hiredis REQUIRED)
#find_package(sw REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
#set (CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#protobuf_generate_cpp( PROTO_HEADER )
#add_library(mysql++ INTERFACE)

# redis-cli --scan --pattern users:* | xargs redis-cli unlink 
#protoc --js_out=import_style=commonjs,binary:. message.proto
#protoc --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` --grpc_out=. service.proto
#git clean -d -f -x

add_executable(${PROJECT_NAME}

        src/main.cpp
        src/util.cpp
        src/body_parser.cpp
        src/user_info.cpp
        src/internal_message.pb.cc
        src/payload.pb.cc
        src/chat_content.pb.cc
        src/user.pb.cc
        src/channel.pb.cc
        src/channel_service.pb.cc
        src/channel_service.grpc.pb.cc

        include/proto/history.proto
        src/history.pb.cc

        include/session_service.hpp
        include/session_service_impl.hpp

        include/proto/channel.pb.h
        include/proto/channel_service.pb.h
        include/proto/channel_service.grpc.pb.h
        include/proto/chat_content.pb.h
        include/proto/internal_message.pb.h
        include/proto/payload.pb.h

        include/channel_service.hpp
        include/channel_service_impl.hpp
        include/friend_service.hpp
        include/friend_service_impl.hpp
        include/authen_service.hpp
        include/authen_service_impl.hpp
        include/session_mgr.hpp
        include/session_mgr_impl.hpp
        include/body_parser.hpp
        include/util.h
        include/websocket_impl.hpp
        include/websocket.hpp
        include/message_producer_kafka.hpp
        include/message_producer.hpp
        )

#add_executable(stub
#        src/stub.cpp
#        src/service.grpc.pb.cc
#        src/service.pb.cc
#
#        )
#target_include_directories(stub
#        PRIVATE
#        include
#        include/proto
#        )
#target_link_libraries(stub
#        PRIVATE
#        gRPC::grpc++_reflection
#        gRPC::grpc++
#        protobuf::libprotobuf
#        )
#
target_include_directories(${PROJECT_NAME}
        PRIVATE
        include
        include/proto
        uWebSockets

        ${HIREDIS_HEADER}
        ${REDIS_PLUS_PLUS_HEADER}
        spdlog::spdlog
        #nlohmann_json::nlohmann_json
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        #ZLIB::ZLIB
        nlohmann_json::nlohmann_json 
        CppKafka::cppkafka
        gRPC::grpc++_reflection
        gRPC::grpc++
        protobuf::libprotobuf
        uWebSockets
        #${HIREDIS_LIB}
        libhiredis.a
        libredis++.a
        #${REDIS_PLUS_PLUS_LIB}
        spdlog::spdlog
        fmt::fmt
        Threads::Threads

        #hiredis_static
        #cppkafka
        )

#cotire(quickchat)

#add_executable(quickchat-conn-test
#        tests/test_app.cpp
#        tests/test_session_mgr.cpp
#        )
#
#target_include_directories(quickchat-conn-test
#        PRIVATE
#        include
#        )
#
#target_link_libraries(quickchat-conn-test
#        GTest::GTest
#        Threads::Threads
#        )
#
#enable_testing()
#add_test(tests quickchat-conn-test)

