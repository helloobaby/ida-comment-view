﻿cmake_minimum_required (VERSION 3.11) # fetchcontent after 3.11 
project(comment_view)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/idasdk76/include)

if(${buildfor} STREQUAL "ida")
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/idasdk76/lib/x64_win_vc_32/) #链接器->常规->附加库目录
elseif(${buildfor} STREQUAL "ida64")
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/idasdk76/lib/x64_win_vc_64/)
else()
message(FATAL_ERROR "nononono")
endif()

link_libraries(ida.lib)#链接器->输入
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)  ## 增加一个叫src的子目录(连着一起cmake)
add_subdirectory(dependencies)

message(${IDA_INSTALL_DIR}) # cmake -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..