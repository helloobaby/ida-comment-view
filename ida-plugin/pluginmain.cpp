#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

//这个涉及到地址长度的问题
#define __EA64__
//
// ida7.5及以上的插件一定要定义这个宏
// 为了保证老代码能够编译，IDA还是会支持一些老结构体，但是在新IDA上全都不能用，会导致未定义问题
// 这个宏确保你没有使用sdk中废弃的代码
//
#define NO_OBSOLETE_FUNCS

#include "./hexrays_sdk/include/hexrays.hpp"

struct plugin_ctx_t : public plugmod_t
{
    ~plugin_ctx_t()
    {
        term_hexrays_plugin();
    }
    virtual bool idaapi run(size_t) override;
};



//--------------------------------------------------------------------------
bool idaapi plugin_ctx_t::run(size_t)
{
    msg("hello world");
    
    return true;
}

//--------------------------------------------------------------------------
// Initialize the plugin.
static plugmod_t* idaapi init()
{
    if (!init_hexrays_plugin())
        return nullptr; // no decompiler
    const char* hxver = get_hexrays_version();
    msg("Hex-rays version %s has been detected, %s ready to use\n",
        hxver, PLUGIN.wanted_name);
    return new plugin_ctx_t;
}

//--------------------------------------------------------------------------
static const char comment[] = "whatever you want";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  PLUGIN_MULTI|PLUGIN_UNL,//PLUGIN_UNL用来调试
  init,                 // initialize
  nullptr,
  nullptr,
  comment,              // long comment about the plugin
  nullptr,              // multiline help about the plugin
  "whatever you want", // 在Edit->Plugins中现实的名字
  nullptr,              // 热键
};
