#include "IMGUITextInput.h"

CIMGUITextInput::CIMGUITextInput()  :
    meTextInputType(eImGuiTextInputType::String),
    mText{},
    mwText{},
    mTextUTF8{},
    mHintText{},
    mwHintText{},
    mHintTextUTF8{},
    mbMultiLine(false),
    mFlag(0),
    mValueInt(0),
    mValueFloat(0.f)
{
}

CIMGUITextInput::~CIMGUITextInput()
{
}

bool CIMGUITextInput::Init()
{
    return true;
}

void CIMGUITextInput::Render()
{
    bool bInput = false;

    ImGui::PushItemWidth(mSize.x);

    if (mbMultiLine)
    {
        bInput = ImGui::InputTextMultiline(mName.c_str(), mText, 1024, mSize, mFlag);
    }

    // non multiline
    else
    {
        switch (meTextInputType)
        {
        case eImGuiTextInputType::String:
            if (strlen(mHintText) > 0)
            {
                // Hint Text
                bInput = ImGui::InputTextWithHint(mName.c_str(), mHintText, mText, 1024, mFlag);
            }
            else
            {
                bInput = ImGui::InputText(mName.c_str(), mText, 1024, mFlag);
            }
            break;
        case eImGuiTextInputType::Int:
            bInput = ImGui::InputInt(mName.c_str(), &mValueInt, 1, 10, mFlag);
            break;
        case eImGuiTextInputType::Float:
            bInput = ImGui::InputFloat(mName.c_str(), &mValueFloat, 0.f, 0.f, "%.3f", mFlag);
            break;
        default:
            assert(false);
            break;
        }
    }

    // input 감지되면
    if (bInput)
    {
        // format convert
        if (meTextInputType == eImGuiTextInputType::String)
        {
            memset(mwText, 0, sizeof(wchar_t) * 1024);
            memset(mTextUTF8 , 0, sizeof(char) * 1024);
            
            // window는 ANSI 입력 사용함. wide text (unicode로 변환)
            int length = MultiByteToWideChar(CP_ACP, 0, mText, -1, 0, 0);
            MultiByteToWideChar(CP_ACP, 0, mText, -1, mwText, length);

            // utf8로 다시 변환
            length = WideCharToMultiByte(CP_UTF8, 0, mwText, -1, 0, 0, 0, 0);
            WideCharToMultiByte(CP_UTF8, 0, mwText, -1, mTextUTF8, length, 0, 0);
        }

        if (mInputCallBack)
        {
            mInputCallBack();
        }
    }
}
