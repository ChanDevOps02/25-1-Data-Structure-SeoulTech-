cmake_minimum_required(VERSION 3.15)
project(Hello_GTK C)

set(CMAKE_C_STANDARD 11)

# GTK 설정
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# libpq 수동 경로 설정
include_directories(/opt/homebrew/opt/libpq/include)
link_directories(/opt/homebrew/opt/libpq/lib)

# 실행 파일
add_executable(Hello_GTK main.c)

# 라이브러리 연결
target_link_libraries(Hello_GTK ${GTK3_LIBRARIES} pq)
