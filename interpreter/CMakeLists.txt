# CMakeList.txt: interpreter 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
#add_compile_options("/std:c++20")

project ("interpreter")
# set C++ standard
set(CMAKE_CXX_STANDARD 20)

# 添加boost库
set(BOOST_ROOT E:/81-github/03-c++/boost_1_87_0/boost_MSVC)
#set(Boost_INCLUDE_DIRS  ${BOOST_ROOT}/include)
#set(BOOST_LIBRARYDIR  ${BOOST_ROOT}/lib)
set(Boost_LIBRARY_DIRS  ${BOOST_ROOT}/lib)
#set(Boost_NO_SYSTEM_PATHS ON) #只搜索上语句设置的搜索路径
set(Boost_USE_STATIC_LIBS OFF) # 只使用静态库
set(Boost_USE_STATIC_RUNTIME ON) # 
#find_package(Boost COMPONENTS math  REQUIRED)
find_package(Boost REQUIRED)

if(Boost_FOUND)
    #include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})

    MESSAGE("Boost version: ${Boost_VERSION_STRING}.")
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")

endif()


# 将源代码添加到此项目的可执行文件。
add_executable (interpreter "interpreter.cpp" "interpreter.h")

target_include_directories(interpreter PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (interpreter ${Boost_LIBRARIES})
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET interpreter PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
