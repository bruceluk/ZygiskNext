cmake_minimum_required(VERSION 3.22.1)
project("loader")

find_package(cxx REQUIRED CONFIG)

add_definitions(-DZKSU_VERSION=\"${ZKSU_VERSION}\")

aux_source_directory(common COMMON_SRC_LIST)
add_library(common STATIC ${COMMON_SRC_LIST})
target_include_directories(common PRIVATE include)
target_link_libraries(common cxx::cxx log)

aux_source_directory(injector INJECTOR_SRC_LIST)
if(${ANDROID_ABI} STREQUAL "arm64-v8a1")
    aux_source_directory(injector/shadow INJECTOR_SRC_LIST)
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a1")
    aux_source_directory(injector/shadow INJECTOR_SRC_LIST)
else()
    aux_source_directory(injector/lsplt INJECTOR_SRC_LIST)
endif()

add_library(zygisk SHARED ${INJECTOR_SRC_LIST})
target_include_directories(zygisk PRIVATE include)

if(${ANDROID_ABI} STREQUAL "arm64-v8a1")
    target_link_libraries(zygisk cxx::cxx log common lsplt_static phmap shadowhook)
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a1")
    target_link_libraries(zygisk cxx::cxx log common lsplt_static phmap shadowhook)
else()
    target_link_libraries(zygisk cxx::cxx log common lsplt_static phmap)
endif()

aux_source_directory(ptracer PTRACER_SRC_LIST)
add_executable(libzygisk_ptrace.so ${PTRACER_SRC_LIST})
target_include_directories(libzygisk_ptrace.so PRIVATE include)
target_link_libraries(libzygisk_ptrace.so cxx::cxx log common)

add_subdirectory(external)
