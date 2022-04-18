#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

//这个涉及到地址长度的问题，用ida64.exe就得加这个宏，ida32.exe不用
#define __EA64__

//
// ida7.5及以上的插件一定要定义这个宏
// 为了保证老代码能够编译，IDA还是会支持一些老结构体，但是在新IDA上全都不能用，会导致未定义问题
// 这个宏确保你没有使用sdk中废弃的代码
//
#define NO_OBSOLETE_FUNCS
#include <hexrays_sdk/include/hexrays.hpp>
#include <optional>
#include <format>

ssize_t idaapi ui_hook(void* user_data, int notification_code, va_list va);

struct plugin_ctx_t : public plugmod_t {
  plugin_ctx_t() {}
  virtual bool idaapi run(size_t) override;
  std::optional<qstring> get_ea_comment(ea_t ea) {
    qstring cmt;
    get_cmt(&cmt, ea, 0);
    if (cmt.empty()) {
      get_cmt(&cmt, ea, 1);
      if (cmt.empty())
        return std::nullopt;
      else
        return cmt;
    } else
      return cmt;
  }
  void show_comment_list();

    ~plugin_ctx_t() {
    unhook_from_notification_point(HT_UI, ui_hook);
    term_hexrays_plugin();
    }

};
//--------------------------------------------------------------------------
bool idaapi plugin_ctx_t::run(size_t) {
  hook_to_notification_point(HT_UI, ui_hook);
  show_comment_list();
  
  return true;
}

//--------------------------------------------------------------------------
// Initialize the plugin.
static plugmod_t* idaapi init() {
  if (!init_hexrays_plugin()) {
    return nullptr;  // plugin will not be loaded
  }
  const char* hxver = get_hexrays_version();
  
  msg(std::format("[{}]make sure your IDA version not lower than 7.5\n", PLUGIN.wanted_name).c_str());
  return new plugin_ctx_t;
}

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN = {
    IDP_INTERFACE_VERSION,
#ifdef DEBUG
    PLUGIN_MULTI |
        PLUGIN_UNL,  // PLUGIN_UNL作用是在插件run被调用的时候自动unload,再次run的时候ida会去尝试重新加载新编译的DLL
#else
    PLUGIN_MULTI,
#endif  // DBG
    init,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    "comment_list",
    nullptr,
};

/*
shift+;  是unrepeatable cmt
; 是repeatable cmt

shift+;会覆盖;

所以如果有unrep的,应该显示unrep,没有unrep再显示rep

大多数人都是用;号直接注释的
但是如果一个地址有很多行(函数开头),;号优先会注释在开头,但是shift+;可以注释在任意位置
*/
void plugin_ctx_t::show_comment_list() {

    
    
    
}

ssize_t ui_hook(void* user_data, int notification_code,
                              va_list va) {

    
  return 0;
}