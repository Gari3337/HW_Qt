cmake_minimum_required(VERSION 3.1)
project(Apps_for_computers)

set(CMAKE_CXX_STANDARD 20)

# Поиск необходимых библиотек
find_package(nlohmann_json 3.10.5 REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Добавляем исполняемый файл
add_executable(Apps_for_computers
        hw_bmstu.h
        hw_bmstu.cpp
        fileworker.h
        cheking.h
)

add_custom_command(
        TARGET Apps_for_computers POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/computers.json
        $<TARGET_FILE_DIR:Apps_for_computers>/computers.json
)

# Связываем необходимые библиотеки
target_link_libraries(Apps_for_computers Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network nlohmann_json::nlohmann_json)
