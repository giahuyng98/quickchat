cmake_minimum_required(VERSION 3.17)

project(quickchat-middleware
        VERSION 1.0
        DESCRIPTION "QuickChatMiddleWare..."
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

configure_file(config.json config.json COPYONLY)

find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads)
find_package(Doxygen)
find_package(uWebSockets REQUIRED)
find_package(CppKafka REQUIRED)
find_package(cpp-jwt REQUIRED)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

find_package(hiredis REQUIRED)


find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/body_parser.cpp
        src/channel_service.grpc.pb.cc
        src/channel.pb.cc
        src/channel_service.pb.cc
        src/util.cpp
        src/user_info.cpp
        src/payload.pb.cc

        src/chat_content.pb.cc
        src/message.pb.cc
        src/message_service.grpc.pb.cc
        src/message_service.pb.cc

        src/user.pb.cc
        src/user_service.pb.cc
        src/user_service.grpc.pb.cc

        src/friend.pb.cc
        src/friend_service.pb.cc
        src/friend_service.grpc.pb.cc

        include/proto/friend.pb.h
        include/proto/friend_service.pb.h
        include/proto/friend_service.grpc.pb.h

        include/proto/money_service.pb.h
        include/proto/money_service.grpc.pb.h
        include/proto/account.pb.h
        include/proto/internal_message.pb.h
        src/internal_message.pb.cc
        src/history.pb.cc
        
        src/money_service.pb.cc
        src/money_service.grpc.pb.cc
        src/account.pb.cc

        include/proto/user_service.pb.h
        include/proto/user_service.grpc.pb.h
        include/proto/user.pb.h

        include/proto/message_service.pb.h
        include/proto/message_service.grpc.pb.h
        include/proto/message.pb.h
        include/proto/channel_service.grpc.pb.h
        include/proto/channel_service.pb.h
        include/proto/channel.pb.h
        include/proto/chat_content.pb.h
        include/proto/payload.pb.h

        include/message_producer.hpp
        include/message_producer_kafka.hpp

        include/util.h
        include/authen_service.hpp
        include/authen_service_impl.hpp
        include/body_parser.hpp
        include/channel_service.hpp
        include/channel_service_impl.hpp
        include/config.hpp
        include/config_json.hpp
        include/controller.hpp
        include/friend_service_impl.hpp
        include/group_service_impl.hpp
        include/logger.hpp
        include/message_service.hpp
        include/message_service_impl.hpp
        include/user_service_impl.hpp
        )
target_include_directories(${PROJECT_NAME}
        PRIVATE
        include
        include/proto
        uWebSockets

        ${HIREDIS_HEADER}
        ${REDIS_PLUS_PLUS_HEADER}
        spdlog::spdlog
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
        CppKafka::cppkafka
        gRPC::grpc++_reflection
        gRPC::grpc++
        hiredis::hiredis
        ${REDIS_PLUS_PLUS_LIB}
        protobuf::libprotobuf
        uWebSockets
        spdlog::spdlog
        fmt::fmt
        Threads::Threads
        cpp-jwt::cpp-jwt
        )
