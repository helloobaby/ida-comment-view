﻿
if(${buildfor} STREQUAL "ida")
    add_library(comment_view SHARED dllmain.cpp pch.cpp pch.h pluginmain.cpp framework.h code_cvt.h) 
    SET_TARGET_PROPERTIES(comment_view PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${IDA_INSTALL_DIR}/plugins/)
    SET_TARGET_PROPERTIES(comment_view PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${IDA_INSTALL_DIR}/plugins/)
elseif(${buildfor} STREQUAL "ida64")
    add_library(comment_view64 SHARED dllmain.cpp pch.cpp pch.h pluginmain.cpp framework.h code_cvt.h) 
    SET_TARGET_PROPERTIES(comment_view64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${IDA_INSTALL_DIR}/plugins/)
    SET_TARGET_PROPERTIES(comment_view64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${IDA_INSTALL_DIR}/plugins/)
endif()


#下面这个会自动后面加/debug或者/release的
#SET_TARGET_PROPERTIES(comment_view64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${IDA_INSTALL_DIR}/plugins/)


#DEBUG模式下启用DEBUG宏
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

if(${buildfor} STREQUAL "ida64")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__EA64__")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__EA64__")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__EA64__")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__EA64__")
endif()
