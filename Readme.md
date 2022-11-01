##### 如何自己编译：

例子：

mkdir build

cd build

cmake -Dbuildfor="ida64" -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..

如果是为ida.exe编译，最后一个命令为

cmake -Dbuildfor="ida" -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..



