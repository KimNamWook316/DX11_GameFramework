#include "Input.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Scene/Scene.h"
#include "Component/CameraComponent.h"
#include "Engine.h";

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	mhInst(0),
	mhWnd(0),
	mInput(nullptr),
	mKeyBoard(nullptr),
	mMouse(nullptr),
	mbLButtonClicked(false),
	mbRButtonClicked(false),
	mbWidgetCollide(false),
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
	mhWnd = hWnd;
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

	// ���콺 �Է�ó��
	updateMouse(deltaTime);

	// UI - ���콺 �浹
	// ���⼭ ȣ���ϴ� ������, Widget�� ���콺�� �浹�ߴ��� ���� �ľ��س���,
	// UI���� ���콺 ������ �ϴµ� UI �ٱ��� �����ϴ� ���� ���� ���ؼ��̴�.
	mbWidgetCollide = CSceneManager::GetInst()->GetScene()->GetCollision()->DoCollideWidget(deltaTime);

	// ���� ���� Ű ������ ������Ʈ
	updateKeyState();

	// Ű�� ���ε� �Ǿ��ִ� �ݹ� �Լ��� ȣ��
	updateKeyInfo(deltaTime);
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
	
	// ������ ��ǥ�� -> directX ��ǥ��� ��ȯ
	mousePos.y = CDevice::GetInst()->GetResolution().Height - mousePos.y;

	mMouseMoveAmount = mMousePos - mousePos;

	mMousePos = mousePos;
	mMouseWorldPos = mMousePos;

	// 2D�϶��� ������������� ���콺 ��ǥ�� ���Ѵ�.
	if (eEngineSpace::Space2D == CEngine::GetInst()->GetEngineSpace())
	{
		CCameraComponent* cam = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		mMouseWorldPos += cam->GetLeftBottomPos();
	}
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

		if (mMouseState.rgbButtons[0] & 0x80)
		{
			mbLButtonClicked = true;
		}
		else
		{
			mbLButtonClicked = false;
		}

		if (mMouseState.rgbButtons[0] & 0x80)
		{
			mbRButtonClicked = true;
		}
		else
		{
			mbRButtonClicked = false;
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
				if (mMouseState.rgbButtons[0] & 0x80 && !mbWidgetCollide)
				{
					mbLButtonClicked = true;
					bIsKeyPushed = true;
				}
				break;

			case DIK_MOUSERBUTTON:
				// ������ Ŭ��
				if (mMouseState.rgbButtons[1] & 0x80 && !mbWidgetCollide)
				{
					mbLButtonClicked = true;
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
			// IMGUI �����쿡 �ö�� �ִ� ���¿��� ���콺 Ŭ�� �̺�Ʈ�� �߻��ϸ� �ݹ� ȣ�� �� ��
			if (iter->second->CallBack[KeyState_Down])
			{
				if ((key == DIK_MOUSELBUTTON || key == DIK_MOUSERBUTTON))
				{
					if (!ImGui::GetIO().WantCaptureMouse)
					{
						iter->second->CallBack[KeyState_Down](deltaTime);
					}
				}
				else
				{
					iter->second->CallBack[KeyState_Down](deltaTime);
				}
			}
		}

		if (mVecKeyState[key].State[KeyState_Push] &&
			iter->second->bIsCtrl == mbIsCtrlPressed &&
			iter->second->bIsAlt == mbIsAltPressed &&
			iter->second->bIsShift == mbIsShiftPressed)
		{
			if (iter->second->CallBack[KeyState_Push])
			{
				if ((key == DIK_MOUSELBUTTON || key == DIK_MOUSERBUTTON))
				{
					if (!ImGui::GetIO().WantCaptureMouse)
					{
						iter->second->CallBack[KeyState_Push](deltaTime);
					}
				}
				else
				{
					iter->second->CallBack[KeyState_Push](deltaTime);
				}
			}
		}

		if (mVecKeyState[key].State[KeyState_Up] &&
			iter->second->bIsCtrl == mbIsCtrlPressed &&
			iter->second->bIsAlt == mbIsAltPressed &&
			iter->second->bIsShift == mbIsShiftPressed)
		{
			if (iter->second->CallBack[KeyState_Up])
			{
				if ((key == DIK_MOUSELBUTTON || key == DIK_MOUSERBUTTON))
				{
					if (!ImGui::GetIO().WantCaptureMouse)
					{
						iter->second->CallBack[KeyState_Up](deltaTime);
					}
				}
				else
				{
					iter->second->CallBack[KeyState_Up](deltaTime);
				}
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
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
			break;
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
			break;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
			break;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
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
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}
		return 0xff;
	}

	return key;
}