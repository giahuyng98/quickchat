#pragma once
#include <spdlog/spdlog.h>

namespace logger {
using namespace spdlog;

void setUp(); 
  //set_pattern("[%H:%M:%S] [%^%L%$] [thread %t] %v");
  //[process %P] 

} // namespace logger
