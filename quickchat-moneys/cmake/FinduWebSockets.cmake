include(FetchContent)

message(STATUS "Load uWebSockets: https://github.com/uNetworking/uWebSockets.git")

FetchContent_Declare(
  uWebSockets
  GIT_REPOSITORY https://github.com/uNetworking/uWebSockets.git
  GIT_TAG        v18.13.0
)
#FetchContent_MakeAvailable(uWebSockets)

FetchContent_GetProperties(uWebSockets)

if (NOT uwebsockets_POPULATED) 
  FetchContent_Populate(uWebSockets)

  if (NOT EXISTS "${uwebsockets_SOURCE_DIR}/uWebSockets")
    file(RENAME "${uwebsockets_SOURCE_DIR}/src" "${uwebsockets_SOURCE_DIR}/uWebSockets")

    execute_process(
      COMMAND make -C uSockets
      WORKING_DIRECTORY ${uwebsockets_SOURCE_DIR}
    )
  endif()


  add_library(uWebSockets STATIC IMPORTED)

  set_target_properties(uWebSockets PROPERTIES IMPORTED_LOCATION ${uwebsockets_SOURCE_DIR}/uSockets/uSockets.a)

  target_include_directories(
    uWebSockets 
    INTERFACE
    "${uwebsockets_SOURCE_DIR}"
    "${uwebsockets_SOURCE_DIR}/uSockets/src"
  )
  #target_link_libraries(uWebSockets
    #${uwebsockets_SOURCE_DIR}/uSockets/uSockets.a
    #z
  #)
  
endif()

