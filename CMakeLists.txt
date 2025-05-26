cmake_minimum_required(VERSION 3.10)
project(dialogProgram VERSION 1.0)

# Стандарт C++ и флаги
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Папка для исполняемых файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Заголовочные файлы
include_directories(include)

# Исходные файлы
file(GLOB SOURCES "src/*.cpp")

# Сборка исполняемого файла
add_executable(dialogProgram ${SOURCES})

# Копирование данных
configure_file(data/db.csv ${CMAKE_BINARY_DIR}/data/db.csv COPYONLY)