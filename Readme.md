2022.4.17   
用cmake弄了一下。



Example:  
cmake -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..


ida32和ida64都是64位的,但是分析的程序有32和64,如果分析32的话,需要改一下cmakelist,把x64_win_vc_64改成x64_win_vc_32


编译完之后ida打开会自动加载插件测试(上面指定了ida安装目录)