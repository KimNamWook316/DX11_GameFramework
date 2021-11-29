#include "Input.h"
#include "Device.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	mhInst(0),
	mhWnd(0),
	mInput(nullptr),
	mKeyBoard(nullptr),
	mMouse(nullptr),
	mKeyArray{}
{
	mVecKeyState.resize(256);

	for (int i = 0; i < 256; ++i)
	{
		mVecKeyState[i].Key = (unsigned char)i;
	}
}

CInput::~CInput()
{
	auto iter = mMapKeyInfo.begin();
	auto iterEnd = mMapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_RELEASE(mKeyBoard);
	SAFE_RELEASE(mMouse);
	SAFE_RELEASE(mInput);
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	if (!hInst || !hWnd)
	{
		assert(false);
	}

	mhInst = hInst;
	mhWnd = mhWnd;
	meInputType = eInputType::Direct;

	HRESULT result = DirectInput8Create(mhInst, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&mInput, nullptr);

	if (FAILED(result))
	{
		meInputType = eInputType::Window;
	}

	switch (meInputType)
	{
	case eInputType::Direct:
		if (!initDirectInput())
		{
			return false;
		}
		break;
	case eInputType::Window:
		if (!initWindowInput())
		{
			return false;
		}
		break;
	default:
		assert(false);
	break;
	}

	return true;
}

void CInput::Update(float deltaTime)
{
	if (meInputType == eInputType::Direct)
	{
		// DI�� ��� �迭�� Ű ������ �޾ƿ��� ���� �ʿ�
		readDirectInputKeyBoard();
		readDirectInputMouse();
	}

	// ���� ���� Ű ������ ������Ʈ
	updateKeyState();

	// Ű�� ���ε� �Ǿ��ִ� �ݹ� �Լ��� ȣ��
	updateKeyInfo(deltaTime);

	updateMouse(deltaTime);
}

bool CInput::CreateKey(const std::string& name, const unsigned char key)
{
	KeyInfo* info = findKeyInfo(name);

	if (info)
	{
		return false;
	}

	info = new KeyInfo;
	info->Name = name;

	// Ű�� ��ȯ
	unsigned char convertedKey = convertWindowKeyToDIKey(key);
	info->State.Key = convertedKey;
	
	mMapKeyInfo.insert(std::make_pair(name, info));
	
	bool added = false;
	size_t size = mVecAddedKey.size();
	
	// �̹� �ʿ� �ٸ� �̸����� ��ϵǾ� �ִ� Ű���� Ű������ Ȯ���Ѵ�.
	for (size_t i = 0; i < size; ++i)
	{
		if (mVecAddedKey[i] == convertedKey)
		{
			added = true;
			break;
		}
	}

	// ó�� �߰��� Ű���� Ű���̶��, ����Ѵ�.
	if (!added)
	{
		mVecAddedKey.push_back(convertedKey);
	}

	return true;
}

bool CInput::SetCtrlKey(const std::string& name, bool state)
{
	KeyInfo* info = findKeyInfo(name);

	if (!info)
	{
		return false;
	}

	info->bIsCtrl = state;
	return true;
}

bool CInput::SetAltKey(const std::string& name, bool state)
{
	KeyInfo* info = findKeyInfo(name);

	if (!info)
	{
		return false;
	}

	info->bIsAlt = state;
	return true;
}

bool CInput::SetShiftKey(const std::string& name, bool state)
{
	KeyInfo* info = findKeyInfo(name);

	if (!info)
	{
		return false;
	}

	info->bIsShift = state;
	return true;
}

// �� ��ȯ���� ������ �ݹ� �Լ��� Ŭ���� �� �־�� �� ��
void CInput::ClearCallBack()
{
	auto iter = mMapKeyInfo.begin();
	auto iterEnd = mMapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < KeyState_Max; ++i)
		{
			iter->second->CallBack[i] = nullptr;
		}
	}
}

KeyInfo* CInput::findKeyInfo(const std::string& name)
{
	auto iter = mMapKeyInfo.find(name);

	if (iter == mMapKeyInfo.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool CInput::initWindowInput()
{
	return true;
}

bool CInput::initDirectInput()
{
	if (FAILED(mInput->CreateDevice(GUID_SysKeyboard, &mKeyBoard, nullptr)))
	{
		assert(false);
		return false;
	}

	if (FAILED(mKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
	{
		assert(false);
		return false;
	}

	if (FAILED(mInput->CreateDevice(GUID_SysMouse, &mMouse, nullptr)))
	{
		assert(false);
		return false;
	}

	if (FAILED(mMouse->SetDataFormat(&c_dfDIMouse)))
	{
		assert(false);
		return false;
	}

	return true;
}

void CInput::readDirectInputKeyBoard()
{
	// ���� Ű ������ �迭�� �� ���� ���´�.
	HRESULT result = mKeyBoard->GetDeviceState(256, &mKeyArray);

	if (FAILED(result))
	{
		// ��ġ�� lost �Ǿ��ų�, ������ �������� ������ ��, ��ġ�� �ٽ� �����Ѵ�.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			mKeyBoard->Acquire();
		}
	}
}

void CInput::readDirectInputMouse()
{
	HRESULT result = mMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			mMouse->Acquire();
		}
	}

	if (mMouseState.rgbButtons[0] != '\0')
	{
		int a = 1;
	}
	if (mMouseState.rgbButtons[1] != '\0')
	{
		int a = 1;
	}
}

// Mouse Position, Mouse Move Amount ������Ʈ
void CInput::updateMouse(float deltaTime)
{
	POINT mouseWindowPos;

	GetCursorPos(&mouseWindowPos);
	ScreenToClient(mhWnd, &mouseWindowPos);

	Vector2 ratio = CDevice::GetInst()->GetViewportAspectRatio();

	// ������ ���� ��ġ ����
	Vector2 mousePos = Vector2(mouseWindowPos.x * ratio.x, mouseWindowPos.y * ratio.y);

	mMouseMoveAmount = mMousePos - mousePos;

	mMousePos = mousePos;
}

void CInput::updateKeyState()
{
	// Ctrl, Alt, Shift Ű ���ȴ��� �Ǻ�
	switch (meInputType)
	{
	case eInputType::Direct:
		if (mKeyArray[DIK_LCONTROL] & 0x80)
		{
			mbIsCtrlPressed = true;
		}
		else
		{
			mbIsCtrlPressed = false;
		}

		if (mKeyArray[DIK_LALT] & 0x80)
		{
			mbIsAltPressed = true;
		}
		else
		{
			mbIsAltPressed = false;
		}

		if (mKeyArray[DIK_LSHIFT] & 0x80)
		{
			mbIsShiftPressed = true;
		}
		else
		{
			mbIsShiftPressed = false;
		}
		break;

	case eInputType::Window:
		if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		{
			mbIsCtrlPressed = true;
		}
		else
		{
			mbIsCtrlPressed = false;
		}

		if (GetAsyncKeyState(VK_MENU) & 0x8000)
		{
			mbIsAltPressed = true;
		}
		else
		{
			mbIsAltPressed = false;
		}

		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			mbIsShiftPressed = true;
		}
		else
		{
			mbIsShiftPressed = false;
		}
		break;
	}
	
	size_t size = mVecAddedKey.size();

	// ��ϵ� Ű�� ��ȸ�ϸ� ���ȴ��� �ǵ�
	for (size_t i = 0; i < size; ++i)
	{
		unsigned char key = mVecAddedKey[i];

		bool bIsKeyPushed = false;

		switch (meInputType)
		{
		
		// DirectInput
		case eInputType::Direct:
			switch (key)
			{
			case DIK_MOUSELBUTTON:
				// ���� Ŭ��
				if (mMouseState.rgbButtons[0] & 0x80)
				{
					bIsKeyPushed = true;
				}
				break;

			case DIK_MOUSERBUTTON:
				// ������ Ŭ��
				if (mMouseState.rgbButtons[1] & 0x80)
				{
					bIsKeyPushed = true;
				}
				break;

			case DIK_MOUSEWHEEL:
				break;

			default:
				// Ű���� �Է��� ���
				// DirectInput�� Ű���� �޾ƿ� ���� üũ�ؼ�, �� Ű�� ���� ���� �ִٸ�
				if (mKeyArray[key] & 0x80)
				{
					bIsKeyPushed = true;
				}
				break;
			}
		break;

		// WindowInput
		case eInputType::Window:
			// �� Ű�� ���� �����ִٸ�
			if (GetAsyncKeyState(key) & 0x8000)
			{
				bIsKeyPushed = true;
			}
			break;
		default:
			break;
		}

		// Ű�� ���ȴٸ�, ���¸� Ȯ���Ѵ�.
		if (bIsKeyPushed)
		{
			// ���� ���� ����, ������ ���µ� �ƴϸ�, Down & push
			if (!mVecKeyState[key].State[KeyState_Down] &&
				!mVecKeyState[key].State[KeyState_Push])
			{
				mVecKeyState[key].State[KeyState_Down] = true;
				mVecKeyState[key].State[KeyState_Push] = true;
			}

			// ��������, �̹� ���� �����ӿ� �������ٸ� Push
			else
			{
				mVecKeyState[key].State[KeyState_Down] = false;
			}
		}

		// �ش� Ű�� ������ �ʾҴµ�, Push ������ ��� Upó��
		else if (mVecKeyState[key].State[KeyState_Push])
		{
			mVecKeyState[key].State[KeyState_Down] = false;
			mVecKeyState[key].State[KeyState_Push] = false;
			mVecKeyState[key].State[KeyState_Up] = true;
		}

		// �ش� Ű�� ������ �ʾҰ�, ���� �����ӿ� Up ���¿��� ��� �ƹ� Ű�� ������ ���� ����
		else if (mVecKeyState[key].State[KeyState_Up])
		{
			mVecKeyState[key].State[KeyState_Up] = false;
		}
	}
}

// ����ڰ� �߰��� KeyInfo���� ��ȸ�ϸ�, ���ǿ� ������ CallBack�Լ� ȣ���Ѵ�.
void CInput::updateKeyInfo(float deltaTime)
{
	auto iter = mMapKeyInfo.begin();
	auto iterEnd = mMapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		// KeyInfo�� ��� �ִ� key�� �޾ƿͼ�
		unsigned char key = iter->second->State.Key;

		// ���� �����ִ� ctrl, alt, shift���� State ���� KeyInfo�� ���� ��ġ�ϸ�,
		// �ݹ� �Լ� ȣ���Ѵ�.
		if (mVecKeyState[key].State[KeyState_Down] &&
			iter->second->bIsCtrl == mbIsCtrlPressed &&
			iter->second->bIsAlt == mbIsAltPressed &&
			iter->second->bIsShift == mbIsShiftPressed)
		{
			if (iter->second->CallBack[KeyState_Down])
			{
				iter->second->CallBack[KeyState_Down](deltaTime);
			}
		}

		if (mVecKeyState[key].State[KeyState_Push] &&
			iter->second->bIsCtrl == mbIsCtrlPressed &&
			iter->second->bIsAlt == mbIsAltPressed &&
			iter->second->bIsShift == mbIsShiftPressed)
		{
			if (iter->second->CallBack[KeyState_Push])
			{
				iter->second->CallBack[KeyState_Push](deltaTime);
			}
		}

		if (mVecKeyState[key].State[KeyState_Up] &&
			iter->second->bIsCtrl == mbIsCtrlPressed &&
			iter->second->bIsAlt == mbIsAltPressed &&
			iter->second->bIsShift == mbIsShiftPressed)
		{
			if (iter->second->CallBack[KeyState_Up])
			{
				iter->second->CallBack[KeyState_Up](deltaTime);
			}
		}
	}
}

unsigned char CInput::convertWindowKeyToDIKey(const unsigned char key)
{
	if (meInputType == eInputType::Direct)
	{
		switch (key)
		{
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case '0':
			return DIK_0;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case '=':
			return DIK_EQUALS;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case 'Q':
			return DIK_Q;
		case 'W':
			return DIK_W;
		case 'E':
			return DIK_E;
		case 'R':
			return DIK_R;
		case 'T':
			return DIK_T;
		case 'Y':
			return DIK_Y;
		case 'U':
			return DIK_U;
		case 'I':
			return DIK_I;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case '[':
			return DIK_LBRACKET;
		case ']':
			return DIK_RBRACKET;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_CONTROL:
			return DIK_LCONTROL;
		case 'A':
			return DIK_A;
		case 'S':
			return DIK_S;
		case 'D':
			return DIK_D;
		case 'F':
			return DIK_F;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
			/*case DIK_L:
				return 'L';
			case DIK_SEMICOLON:
				return ';';
			case DIK_APOSTROPHE:
				return '\'';
			case DIK_GRAVE:
				return '`';
			case DIK_LSHIFT:
				return VK_SHIFT;
			case DIK_BACKSLASH:
				return '\\';
			case DIK_Z:
				return 'Z';
			case DIK_X:
				return 'X';
			case DIK_C:
				return 'C';
			case DIK_V:
				return 'V';
			case DIK_B:
				return 'B';
			case DIK_N:
				return 'N';
			case DIK_M:
				return 'M';
			case DIK_COMMA:
				return ',';
			case DIK_PERIOD:
				return '.';
			case DIK_SLASH:
				return '/';
			case DIK_RSHIFT:
				return VK_RSHIFT;
			case DIK_MULTIPLY:
				return VK_MULTIPLY;
			case DIK_LMENU:
				return VK_LMENU;
			case DIK_CAPITAL:
				return VK_CAPITAL;
			case DIK_F1:
				return VK_F1;
			case DIK_F2:
				return VK_F2;
			case DIK_F3:
				return VK_F3;
			case DIK_F4:
				return VK_F4;
			case DIK_F5:
				return VK_F5;
			case DIK_F6:
				return VK_F6;
			case DIK_F7:
				return VK_F7;
			case DIK_F8:
				return VK_F8;
			case DIK_F9:
				return VK_F9;
			case DIK_F10:
				return VK_F10;
			case DIK_NUMLOCK:
				break;
			case DIK_SCROLL:
				break;
			case DIK_NUMPAD7:
				break;
			case DIK_NUMPAD8:
				break;
			case DIK_NUMPAD9:
				break;
			case DIK_SUBTRACT:
				break;
			case DIK_NUMPAD4:
				break;
			case DIK_NUMPAD5:
				break;
			case DIK_NUMPAD6:
				break;
			case DIK_ADD:
				break;
			case DIK_NUMPAD1:
				break;
			case DIK_NUMPAD2:
				break;
			case DIK_NUMPAD3:
				break;
			case DIK_NUMPAD0:
				break;
			case DIK_DECIMAL:
				break;
			case DIK_OEM_102:
				break;
			case DIK_F11:
				return VK_F11;
			case DIK_F12:
				return VK_F12;
			case DIK_F13:
				break;
			case DIK_F14:
				break;
			case DIK_F15:
				break;
			case DIK_NEXTTRACK:
				break;
			case DIK_NUMPADENTER:
				break;
			case DIK_RCONTROL:
				return VK_RCONTROL;
			case DIK_MUTE:
				break;
			case DIK_CALCULATOR:
				break;
			case DIK_PLAYPAUSE:
				break;
			case DIK_MEDIASTOP:
				break;
			case DIK_VOLUMEDOWN:
				break;
			case DIK_VOLUMEUP:
				break;
			case DIK_WEBHOME:
				break;
			case DIK_DIVIDE:
				break;
			case DIK_SYSRQ:
				break;
			case DIK_RMENU:
				return VK_RMENU;
			case DIK_PAUSE:
				break;
			case DIK_HOME:
				return VK_HOME;
			case DIK_UP:
				return VK_UP;
			case DIK_PRIOR:
				return VK_PRIOR;
			case DIK_LEFT:
				return VK_LEFT;
			case DIK_RIGHT:
				return VK_RIGHT;
			case DIK_END:
				return VK_END;
			case DIK_DOWN:
				return VK_DOWN;
			case DIK_NEXT:
				return VK_NEXT;
			case DIK_INSERT:
				return VK_INSERT;
			case DIK_DELETE:
				return VK_DELETE;
			case DIK_LWIN:
				break;
			case DIK_RWIN:
				break;
			case DIK_APPS:
				break;
			case DIK_POWER:
				break;
			case DIK_SLEEP:
				break;
			case DIK_WAKE:
				break;
			case DIK_WEBSEARCH:
				break;
			case DIK_WEBFAVORITES:
				break;
			case DIK_WEBREFRESH:
				break;
			case DIK_WEBSTOP:
				break;
			case DIK_WEBFORWARD:
				break;
			case DIK_WEBBACK:
				break;
			case DIK_MYCOMPUTER:
				break;
			case DIK_MAIL:
				break;
			case DIK_MEDIASELECT:
				break;
			case DIK_MOUSELBUTTON:
				return VK_LBUTTON;
			case DIK_MOUSERBUTTON:
				return VK_RBUTTON;
			case DIK_MOUSEWHEEL:
				return DIK_MOUSEWHEEL;*/
		}

		return 0xff;
	}

	return key;
}
