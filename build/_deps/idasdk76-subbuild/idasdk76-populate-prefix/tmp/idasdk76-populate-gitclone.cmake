
if(NOT "D:/vs-code/ida-plugin/build/_deps/idasdk76-subbuild/idasdk76-populate-prefix/src/idasdk76-populate-stamp/idasdk76-populate-gitinfo.txt" IS_NEWER_THAN "D:/vs-code/ida-plugin/build/_deps/idasdk76-subbuild/idasdk76-populate-prefix/src/idasdk76-populate-stamp/idasdk76-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'D:/vs-code/ida-plugin/build/_deps/idasdk76-subbuild/idasdk76-populate-prefix/src/idasdk76-populate-stamp/idasdk76-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "D:/vs-code/ida-plugin/build/_deps/idasdk76-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'D:/vs-code/ida-plugin/build/_deps/idasdk76-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "D:/Git/cmd/git.exe"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/helloobaby/idasdk76.git" "idasdk76-src"
    WORKING_DIRECTORY "D:/vs-code/ida-plugin/build/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/helloobaby/idasdk76.git'")
endif()

execute_process(
  COMMAND "D:/Git/cmd/git.exe"  checkout bd07eb74db4dbf30466c30f6e264fdb9c00eda10 --
  WORKING_DIRECTORY "D:/vs-code/ida-plugin/build/_deps/idasdk76-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'bd07eb74db4dbf30466c30f6e264fdb9c00eda10'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "D:/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "D:/vs-code/ida-plugin/build/_deps/idasdk76-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'D:/vs-code/ida-plugin/build/_deps/idasdk76-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "D:/vs-code/ida-plugin/build/_deps/idasdk76-subbuild/idasdk76-populate-prefix/src/idasdk76-populate-stamp/idasdk76-populate-gitinfo.txt"
    "D:/vs-code/ida-plugin/build/_deps/idasdk76-subbuild/idasdk76-populate-prefix/src/idasdk76-populate-stamp/idasdk76-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'D:/vs-code/ida-plugin/build/_deps/idasdk76-subbuild/idasdk76-populate-prefix/src/idasdk76-populate-stamp/idasdk76-populate-gitclone-lastrun.txt'")
endif()

