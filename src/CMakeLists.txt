
string(REGEX MATCH "Clang"  IS_CLANG "${CMAKE_CXX_COMPILER_ID}")
string(REGEX MATCH "GNU"    IS_GNUXX "${CMAKE_CXX_COMPILER_ID}")
string(REGEX MATCH "Linux"  IS_LINUX "${CMAKE_SYSTEM_NAME}")
string(REGEX MATCH "Darwin" IS_MACOS "${CMAKE_SYSTEM_NAME}")



# compiler settings
if(IS_CLANG OR IS_GNUXX)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O0 -g0 -Wall -Wextra -pedantic -Wcast-align -Wunused -Wno-unused-parameter")
elseif(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "-nologo -Zc:wchar_t -FS -O2 -MD -Zc:strictStrings -W3")
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} -MDd")
    if(BUILD_MT_RELEASE)
        string(REPLACE "-MD"  "-MT"  CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
        string(REPLACE "-MDd" "-MTd" CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}")
    endif()
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE
            "${CMAKE_SHARED_LINKER_FLAGS_RELASE} \
            /Gy /GF /OPT:REF /OPT:ICF")
    if(BUILD_SHARED_LIBS)
        message(SEND_ERROR "@error: shared lib is not supported under Visual Studio.\n \
                please compile as a static lib.")
    endif()
endif()

# --[ Proto library
file(GLOB proto_files proto/*.proto)
peloton_protobuf_generate_cpp_py(${proto_gen_folder} proto_srcs proto_hdrs proto_python ${proto_files})
peloton_protobuf_generate_cpp_py(${proto_gen_folder2} proto_srcs proto_hdrs proto_python ${proto_files})

add_library(transcode-proto SHARED ${proto_hdrs} ${proto_srcs} ${proto_python})
set(Peloton_LINKER_LIBS transcode-proto ${Peloton_LINKER_LIBS})
peloton_default_properties(transcode-proto)
target_link_libraries(transcode-proto ${PROTOBUF_LIBRARY})

link_directories(/usr/local/Cellar/openssl/1.0.2m/lib/ /usr/local/lib)

# --[ Source files
file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR}/src/*/*.cc)
list(REMOVE_ITEM srcs ${PROJECT_SOURCE_DIR}/src/server/main.cc)
add_library(transcode SHARED ${srcs})
target_link_libraries(transcode transcode-proto corgi brpc ssl crypto leveldb snappy m)

# --[ MMS
set(mms_main ${PROJECT_SOURCE_DIR}/src/server/main.cc)
add_executable(mms-bin ${mms_main})
target_link_libraries(mms-bin corgi brpc transcode gflags pthread dl)
set_target_properties(mms-bin PROPERTIES OUTPUT_NAME mms)

