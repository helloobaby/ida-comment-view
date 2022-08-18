#include <hexrays_sdk/include/hexrays.hpp>
#include <optional>
#include <format>
#include <codecvt>

#define action_internal_name_1 "pdbheader::CommentView"
#define action_show_name_1 "CommentView"

// encoding function
std::string to_utf8(const std::wstring& wide_string)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;

    return utf8_conv.to_bytes(wide_string);
}


std::wstring to_utf16(const std::string& u8string) {
    
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16_conv;


    return utf16_conv.from_bytes(u8string);
}

ssize_t idaapi ui_hook(void* user_data, int notification_code, va_list va)
{
    if (notification_code == ui_populating_widget_popup)
    {
        TWidget* view = va_arg(va, TWidget*);
        if (get_widget_type(view) == BWN_DISASM) {
            TPopupMenu* p = va_arg(va, TPopupMenu*);

            attach_action_to_popup(view, p, action_internal_name_1, nullptr, SETMENU_POSMASK);
        }
    }

    return false;
}

struct calls_chooser_t : public chooser_t
{
protected:
    static const int widths_[2];
    static const char* const header_[2];

public:
    struct info {
        qstring address;
        qstring comment;
    };
    // remember the call instruction addresses in this qvector
    qvector<info> list;

    // this object must be allocated using `new`
    calls_chooser_t();
    virtual void idaapi select(ssize_t n) const;
    // function that is used to decide whether a new chooser should be opened
    // or we can use the existing one.
    // The contents of the window are completely determined by its title
    virtual const void* get_obj_id(size_t* len) const override
    {
        *len = strlen(title);
        return title;
    }

    // function that returns number of lines in the list
    virtual size_t idaapi get_count() const override { return list.size(); }

    const info* get_item(uint32_t n)const {
        return n >= list.size() ? NULL : &list[n];
    }

    // function that generates the list line
    virtual void idaapi get_row(
        qstrvec_t* cols,
        int* icon_,
        chooser_item_attrs_t* attrs,
        size_t n) const override;

    std::optional<qstring> get_ea_comment(ea_t ea) {
        qstring cmt;
        get_cmt(&cmt, ea, 0);
        if (cmt.empty()) {
            get_cmt(&cmt, ea, 1);
            if (cmt.empty())
                return std::nullopt;
            else
                return cmt;
        }
        else
            return cmt;
    }


protected:
    void build_list();
};

// column widths
const int calls_chooser_t::widths_[] =
{
  25,
  50,             // Instruction
};
// column headers
const char* const calls_chooser_t::header_[] =
{
  "Address",      // 0
  "Comments",  // 1
};


calls_chooser_t::calls_chooser_t() : chooser_t(0, sizeof(widths_)/sizeof(int), widths_, header_, "Comment Lists")
{
    build_list();
}

void idaapi calls_chooser_t::select(ssize_t n) const {
    auto item = list[n];
    msg("[+]select address %s\n",item.address.c_str());

    //将IDA TEXT-View跳转到指定地址

}



void calls_chooser_t::build_list() {
    
    int seg_count = get_segm_qty();
    msg("[+]seg count %d\n", seg_count);


    for (int s = 0; s < seg_count; s++) {
        segment_t* seg = getnseg(s);

        ea_t seg_start = seg->start_ea;
        ea_t seg_end = seg->end_ea;
        insn_t insn;

        msg("[+]seg_start : %llx,seg_end : %llx\n", seg_start, seg_end);

        for (ea_t i = seg_start; i < seg_end;)
        {
            auto length = decode_insn(&insn, i);
            std::optional<qstring> ret = get_ea_comment(i);
            if (ret) {
                std::string utf8 = to_utf8(to_utf16((*ret).c_str()));
                list.push_back({ std::format("{:#x}", i).c_str() ,utf8.c_str() });
            }
            if (length) {       //注释在指令范围内,越过这条指令
                i = i + length;
            }
            else i++;
        }
    }
    
}


void idaapi calls_chooser_t::get_row(
    qstrvec_t* cols_,
    int*,
    chooser_item_attrs_t*,
    size_t n) const
{
    // generate the line
    qstrvec_t& cols = *cols_;
    cols[0] = list[n].address;
    cols[1] = list[n].comment;
}

struct example_action : public action_handler_t
{
    virtual int idaapi activate(action_activation_ctx_t* ctx) override
    {
        calls_chooser_t* ch = new calls_chooser_t();
        ::choose(ch, nullptr);
        return true;
    }
    virtual action_state_t idaapi update(action_update_ctx_t* ctx) override
    {
        return AST_ENABLE_ALWAYS;
    }

};

example_action action1;
struct plugin_ctx_t : public plugmod_t {
    plugin_ctx_t() {
        register_action(ACTION_DESC_LITERAL_PLUGMOD(
            action_internal_name_1,// action name
            action_show_name_1, // show name
            &action1,
            this,
            nullptr,
            nullptr,
            -1));
        hook_to_notification_point(HT_UI, ui_hook);
    }
    virtual bool idaapi run(size_t) override;

    ~plugin_ctx_t() {
        unregister_action(action_internal_name_1);
        unhook_from_notification_point(HT_UI, ui_hook);
        term_hexrays_plugin();
    }

};


//--------------------------------------------------------------------------
bool idaapi plugin_ctx_t::run(size_t) {

    calls_chooser_t* ch = new calls_chooser_t();
    ::choose(ch, nullptr);

    return true;
}

//--------------------------------------------------------------------------
// Initialize the plugin.
static plugmod_t* idaapi init() {
    if (!init_hexrays_plugin()) {
        return nullptr;  // plugin will not be loaded
    }

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
    PLUGIN_MULTI,       //有些功能不能搞PLUGIN_UNL这个属性的
    init,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    "CommentList",
    nullptr,
};
