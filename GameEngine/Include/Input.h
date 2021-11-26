#pragma once

#include "GameInfo.h"

#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe

struct KeyState
{
	unsigned char Key;
	bool State[KeyState_Max];

	KeyState()	:
		State{}
	{
	}
};

struct KeyInfo
{
	std::string Name;
	KeyState State;
	std::function<void(float)> CallBack[KeyState_Max];

	// 이 키의 CallBack이 Ctrl, Alt, Shift와 조합되어야 호출되는지에 대한 정보
	bool bIsCtrl;
	bool bIsAlt;
	bool bIsShift;

	KeyInfo() :
		bIsCtrl(false),
		bIsAlt(false),
		bIsShift(false)
	{
	}
};

class CInput
{
public:
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float deltaTime);

public:
	bool CreateKey(const std::string& name, const unsigned char key);
	bool SetCtrlKey(const std::string& name, bool state);
	bool SetAltKey(const std::string& name, bool state);
	bool SetShiftKey(const std::string& name, bool state);

public:
	// KeyInfo에 CallBack 함수를 등록하는 함수
	// 멤버 함수를 함수 포인터 형태로 전달
	template <typename T>
	void SetKeyCallBack(const std::string& name, eKeyState keyState, T* obj,
		void(T::* func)(float))
	{
		KeyInfo* info = findKeyInfo(name);

		if (!info)
		{
			return;
		}

		info->CallBack[keyState] = std::bind(func, obj, std::placeholders::_1);
	}

private:
	KeyInfo* findKeyInfo(const std::string& name);

private:
	bool initWindowInput();
	bool initDirectInput();

private:
	void readDirectInputKeyBoard();
	void readDirectInputMouse();
	void updateMouse(float deltaTime);
	void updateKeyState();
	void updateKeyInfo(float deltaTime);

private:
	unsigned char convertDIKeyToWindowKey(const unsigned char key);

	DECLARE_SINGLE(CInput)

private:
	HINSTANCE	mhInst;
	HWND		mhWnd;
	
	// DirectInput COM객체
	IDirectInput8* mInput;
	IDirectInputDevice8* mKeyBoard;
	IDirectInputDevice8* mMouse;
	
	// DirectInput 사용중인지, Window Input사용중인지 판별하는 Enum
	eInputType meInputType;

	// DirectInput에서 Key, Mouse값을 얻어오기 위한 정보
	unsigned char mKeyArray[256];
	DIMOUSESTATE mMouseState;

	// 사용자가 등록하는 Key정보
	std::unordered_map<std::string, KeyInfo*> mMapKeyInfo;
	
	// 키 입력 정보를 저장하기 위한 배열
	std::vector<KeyState> mVecKeyState;

	// Map에는 같은 키값이 등록될 수 있기 때문에, 등록되어있는 키를 중복 없이 저장하는 배열
	std::vector<unsigned char> mVecAddedKey;

	bool mbIsCtrlPressed;
	bool mbIsAltPressed;
	bool mbIsShiftPressed;
};

