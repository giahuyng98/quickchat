cmake_minimum_required(VERSION 3.17)
project(
  quickchat-moneys
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

add_executable(quickchat-moneys
  ./src/main.cpp
  ./src/gen_id_service_impl.cpp
  ./src/logger.cpp
  ./include/config.hpp
  ./include/config_json.hpp
  ./include/gen_id_service.h
  ./include/gen_id_service_impl.h
  ./include/logger.hpp
  ./include/snowflake.hpp

  ./include/account.h
  ./include/request.h
  ./include/transaction_history.h

  ./include/proto/history.pb.h
  ./include/proto/account.pb.h
  ./include/proto/money_service.grpc.pb.h
  ./include/proto/money_service.pb.h
  ./include/proto/request.pb.h

  ./src/history.pb.cc
  ./src/account.pb.cc
  ./src/money_service.grpc.pb.cc
  ./src/money_service.pb.cc
  ./src/request.pb.cc
  )

target_include_directories(quickchat-moneys
  PRIVATE
  include
  include/proto
  mysql::mysql
  ${MYSQLPP_HEADER}
  ${REDIS_PLUS_PLUS_HEADER}
  )

target_link_libraries(quickchat-moneys 
  nlohmann_json::nlohmann_json
  CppKafka::cppkafka
  gRPC::grpc++_reflection
  gRPC::grpc++
  protobuf::libprotobuf
  hiredis::hiredis
  ${REDIS_PLUS_PLUS_LIB}
  spdlog::spdlog
  fmt::fmt
  Threads::Threads
  z 
  mysqlpp 
  mysqlclient_r
  )

configure_file(config.json config.json COPYONLY)
