cmake_minimum_required(VERSION 3.29)
project(LAB_EUVM)

set(CMAKE_CXX_STANDARD 20)

# Определяем переменную для имени целевой платформы
if(WIN32)
    set(TARGET_PLATFORM "Windows")
elseif(APPLE)
    set(TARGET_PLATFORM "macOS")
elseif(UNIX AND NOT APPLE)
    set(TARGET_PLATFORM "Linux")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# Добавляем исполняемый файл
add_executable(LAB_EUVM main.cpp)

# Связываем библиотеку PowrProf только для Windows
if(TARGET_PLATFORM STREQUAL "Windows")
    target_link_libraries(LAB_EUVM PRIVATE PowrProf.lib)
endif()
