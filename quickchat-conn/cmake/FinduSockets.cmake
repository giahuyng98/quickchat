include(FetchContent)

message(STATUS "Downloading uSockets: https://github.com/uNetworking/uSockets.git")
FetchContent_Declare(
  uSockets
  GIT_REPOSITORY https://github.com/uNetworking/uSockets.git
  GIT_TAG        v0.6.0
)
#FetchContent_MakeAvailable(uSockets)

FetchContent_GetProperties(uSockets)

if (NOT usockets_POPULATED)
  FetchContent_Populate(uSockets)

  if (NOT EXISTS "${usockets_SOURCE_DIR}/uSockets.a")
    message(STATUS "Building uSockets: ${usockets_SOURCE_DIR}")
    execute_process(
      COMMAND make
      WORKING_DIRECTORY ${usockets_SOURCE_DIR}
    )
    
  endif()

  add_library(uSockets STATIC IMPORTED)
  set_target_properties(uSockets PROPERTIES IMPORTED_LOCATION ${usockets_SOURCE_DIR}/uSockets.a)
  target_include_directories(uSockets INTERFACE ${usockets_SOURCE_DIR}/src)

  #target_link_libraries(uSockets ${uSockets_LIB_DIR})
  #set_target_properties(uSockets PROPERTIES LINKER_LANGUAGE C)
  #target_include_directories(
    #uSockets
    #PUBLIC
    #$<BUILD_INTERFACE:${usockets_SOURCE_DIR}/src>
  #)
  
endif()

