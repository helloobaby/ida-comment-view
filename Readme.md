此项目旨在搭建一个IDA 插件模板，需要开发插件的时候git clone下来就可以使用，并且包含7.6版本的sdk，不需要重复配置环境。

因为IDA32和IDA64都是64位的，所以在选择x86/x64编译的时候一致选Release x64，但是需要额外做的是，

当你需要用ida32.exe的时候，也就是反编译32位的程序，需要在链接->常规->附加包含目录  改为x64_win_vc_32

链接器-常规-输出文件 改为你的IDA plugins的路径，方便测试

还有问题的话baidu/google

目前插件加载会自动注册一个action，右键可以看到，每个action继承于IDA提供的action_handler_t

成员函数activate跟qt的信号槽差不多，需要实现什么就写在那里
