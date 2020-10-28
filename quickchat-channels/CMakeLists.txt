cmake_minimum_required(VERSION 3.17)
project(
  quickchat-channels
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json REQUIRED)
find_package(mysql REQUIRED)
find_path(MYSQLPP_HEADER mysql++)

find_library(MYSQLPP_LIB mysqlpp)
find_library(MYSQLCLIENT_LIB mysqlclient)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

find_package(gRPC REQUIRED)

find_package(hiredis REQUIRED)
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

find_package(spdlog REQUIRED)
find_package(CppKafka REQUIRED)
find_package(fmt REQUIRED)
find_package(uWebSockets REQUIRED)

add_executable(quickchat-channels
  ./src/main.cpp

  ./src/channel.pb.cc
  ./src/channel_service.grpc.pb.cc
  ./src/channel_service.pb.cc
  ./src/gen_id_service_impl.cpp

  ./include/proto/channel.pb.h
  ./include/proto/channel_service.grpc.pb.h
  ./include/proto/channel_service.pb.h

  ./include/gen_id_service.h
  ./include/gen_id_service_impl.h
  ./include/snowflake.hpp
  ./include/singleton.hpp
  ./include/repository.hpp
  ./include/repository_impl.hpp
  ./include/config.hpp
  ./include/config_json.hpp
  ./include/logger.hpp
        src/repository_impl.cpp src/logger.cpp)

#target_compile_options(quickchat-channels PRIVATE  -Wall -Wextra -Wredundant-move )

target_include_directories(quickchat-channels
  PRIVATE
  include
  include/proto
  mysql::mysql
  ${MYSQLPP_HEADER}
  ${REDIS_PLUS_PLUS_HEADER}
  )

target_link_libraries(quickchat-channels 
  nlohmann_json::nlohmann_json
  CppKafka::cppkafka
  gRPC::grpc++
  gRPC::grpc++_reflection
  protobuf::libprotobuf
  uWebSockets
  hiredis::hiredis
  ${REDIS_PLUS_PLUS_LIB}
  spdlog::spdlog
  fmt::fmt
  Threads::Threads
  z 
  mysqlpp 
  mysqlclient_r
  #"-Wl,--whole-archive -lgrpc++_reflection -ldl"
  #"-Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl"
  )


configure_file(config.json config.json COPYONLY)
