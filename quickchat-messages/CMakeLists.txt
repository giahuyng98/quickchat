cmake_minimum_required(VERSION 3.17)
project(
  quickchat-messages
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

add_executable(quickchat-messages
  ./src/main.cpp
  ./src/gen_id_service_impl.cpp
  ./src/logger.cpp
  #./src/message.cpp
  #./src/message_info.cpp
  ./src/message_dao_impl.cpp
  ./src/gen_id_service_impl.cpp
  ./src/internal_message.pb.cc
  ./src/chat_content.pb.cc
  ./src/message_service.pb.cc
  ./src/message_service.grpc.pb.cc
  ./src/message.pb.cc

  ./include/proto/message.pb.h
  ./include/proto/message_service.pb.h
  ./include/proto/message_service.grpc.pb.h
  ./include/proto/chat_content.pb.h
  ./include/proto/internal_message.pb.h

  ./include/seen.h
  ./include/message_repository.hpp
  ./include/message_repository_impl.hpp
  ./include/message_service_impl.hpp
  ./include/message.h
  ./include/config.hpp
  ./include/config_json.hpp
  ./include/gen_id_service.h
  ./include/gen_id_service_impl.h
  ./include/logger.hpp
  ./include/message_info.h
  ./include/message_dao.hpp
  ./include/message_consumer.hpp
  ./include/message_publisher.hpp
  ./include/message_controller.hpp
  ./include/message_dao_impl.hpp
  ./include/message_consumer_impl.hpp
  ./include/message_publisher_impl.hpp
  ./include/message_controller_impl.hpp
  ./include/snowflake.hpp
  )

target_include_directories(quickchat-messages
  PRIVATE
  include
  include/proto
  mysql::mysql
  ${MYSQLPP_HEADER}
  ${REDIS_PLUS_PLUS_HEADER}
  )

target_link_libraries(quickchat-messages 
  nlohmann_json::nlohmann_json
  CppKafka::cppkafka
  gRPC::grpc++_reflection
  gRPC::grpc++
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
  )

configure_file(config.json config.json COPYONLY)
