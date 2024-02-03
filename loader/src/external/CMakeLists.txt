project(external)

OPTION(LSPLT_BUILD_SHARED OFF)
add_subdirectory(lsplt/lsplt/src/main/jni)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    add_subdirectory(shadowhook)
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    add_subdirectory(shadowhook)
endif()

add_library(phmap INTERFACE)
target_include_directories(phmap INTERFACE parallel-hashmap)
target_compile_options(phmap INTERFACE -Wno-unused-value)
