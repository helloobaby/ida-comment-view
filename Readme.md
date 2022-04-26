2022.4.17   
用cmake弄了一下。 原来是个ida插件模板,现在重写为show all comments的脚本,支持ida7.5及以上(新api接口就行)



Example:  
mkdir build && cd build && cmake -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..


ida32和ida64都是64位的,但是分析的程序有32和64,如果分析32的话,需要改一下cmakelist,把x64_win_vc_64改成x64_win_vc_32


编译完之后ida打开会自动加载插件测试(上面指定了ida安装目录)



ida 7.5前到7.5后 api迁移记录  
https://www.hex-rays.com/products/ida/support/ida74_idapython_no_bc695_porting_guide.shtml



2022.4.26
这个东西可以被ida的一个叫bookmark的功能替代,所以没有开发的必要。

