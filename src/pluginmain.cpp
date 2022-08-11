#include <hexrays_sdk/include/hexrays.hpp>
#include <optional>
#include <format>
#include <codecvt>

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
            if (length) { //注释在指令范围内,越过这条指令
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
        }
        else
            return cmt;
    }
    void show_comment_list();

    ~plugin_ctx_t() {
        term_hexrays_plugin();
    }

};
//--------------------------------------------------------------------------
bool idaapi plugin_ctx_t::run(size_t) {
    //show_comment_list();

    calls_chooser_t* ch = new calls_chooser_t();
    ch->choose(); 

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