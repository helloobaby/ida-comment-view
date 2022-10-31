echo off
ren CMakeLists32.txt CMakeLists.txt 
cd src
ren CMakeLists32.txt CMakeLists.txt
cd ..
mkdir build
cd build
cmake -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..

cd..

cd src
ren CMakeLists.txt CMakeLists32.txt
cd ..

ren CMakeLists.txt CMakeLists32.txt 
pause