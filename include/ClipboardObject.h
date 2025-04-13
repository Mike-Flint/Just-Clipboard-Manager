#ifndef CLIPBORD_OBJECT_CLASS_H
#define CLIPBORD_OBJECT_CLASS_H

#include <ClipboardManager.h>

enum class ClipboardType {
    None,
    Text
};

struct ClipboardObject{
public:
    ClipboardType type;
    std::wstring text = L"";

    ClipboardObject(): type(ClipboardType::None){};

    ClipboardObject(const std::wstring& txt)
        : type(ClipboardType::Text), text(txt) {}

};
#endif