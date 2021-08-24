#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

//
// ida7.5及以上的插件一定要定义这个宏
// 为了保证老代码能够编译，IDA还是会支持一些老结构体，但是在新IDA上全都不能用，会导致未定义问题
// 这个宏确保你没有使用sdk中废弃的代码
//
#define NO_OBSOLETE_FUNCS

#include "./hexrays_sdk/include/hexrays.hpp"
#include<Windows.h>

 //--------------------------------------------------------------------------
struct plugin_ctx_t : public plugmod_t
{
    bool inited = true;

    ~plugin_ctx_t()
    {
        if (inited)
            term_hexrays_plugin();
    }
    virtual bool idaapi run(size_t) override;
};

//--------------------------------------------------------------------------
static plugmod_t* idaapi init()
{
    //
    //判断IDA是否调用了我们的init，插件是否成功加载
    //
#if 0
    MessageBoxA(NULL, "plugin init called", "Info", MB_OK);
#endif
    if (!init_hexrays_plugin())
        return nullptr; 
    const char* hxver = get_hexrays_version();
    msg("Hex-rays version %s has been detected, %s ready to use\n",
        hxver, PLUGIN.wanted_name);
    return new plugin_ctx_t;
}

//--------------------------------------------------------------------------
bool idaapi plugin_ctx_t::run(size_t)
{
    func_t* pfn = get_func(get_screen_ea());
    if (pfn == NULL)
    {
        warning("Please position the cursor within a function");
        return true;
    }
    hexrays_failure_t hf;
    cfuncptr_t cfunc = decompile(pfn, &hf, DECOMP_WARNINGS);
    if (cfunc == NULL)
    {
        warning("#error \"%a: %s", hf.errea, hf.desc().c_str());
        return true;
    }
    msg("%a: successfully decompiled\n", pfn->start_ea);

    const strvec_t& sv = cfunc->get_pseudocode();
    for (int i = 0; i < sv.size(); i++)
    {
        qstring buf;
        tag_remove(&buf, sv[i].line);
        msg("%s\n", buf.c_str());
    }
    return true;
}

//--------------------------------------------------------------------------
static char comment[] = "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  PLUGIN_MULTI,         // The plugin can work with multiple idbs in parallel
  init,                 // initialize
  nullptr,
  nullptr,
  comment,              // long comment about the plugin
  nullptr,              // multiline help about the plugin
  "Decompile & Print",  // the preferred short name of the plugin
  nullptr,              // the preferred hotkey to run the plugin
};
