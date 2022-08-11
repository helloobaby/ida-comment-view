##### 编译:    

mkdir build && cd build && cmake -DIDA_INSTALL_DIR="E:\IDA Pro 7.6" ..


ida32和ida64都是64位的,但是分析的程序有32和64,如果分析32的话,需要改一下cmakelist,把x64_win_vc_64改成x64_win_vc_32


编译完之后ida打开会自动加载插件测试(上面指定了ida安装目录)



ida 7.5前到7.5后 api迁移记录  
https://www.hex-rays.com/products/ida/support/ida74_idapython_no_bc695_porting_guide.shtml







##### TODO:

数据库很大的话执行很慢。  



如果ida能够提供一条注释的前后边界就好很多。就不用地址不断地加一加一遍历了。  



双击那一栏跳转到指定位置  



函数头那一行地址的注释拿不到  

 https://solveforum.com/forums/threads/solved-how-to-get-the-auto-generated-comments-for-functions-in-ida.625795/



删除本身自带的一些注释  



F5界面的注释也拿不到(get_cmt这函数太垃圾了)