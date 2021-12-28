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

	// �� Ű�� CallBack�� Ctrl, Alt, Shift�� ���յǾ�� ȣ��Ǵ����� ���� ����
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
	void ClearCallBack();

public:
	Vector2 GetMousePos() const
	{
		return mMousePos;
	}
	
	Vector2 GetMouseWorldPos() const
	{
		return mMousePos;
	}

	Vector2 GetMouseMoveAmount() const
	{
		return mMouseMoveAmount;
	}

public:
	// KeyInfo�� CallBack �Լ��� ����ϴ� �Լ�
	// ��� �Լ��� �Լ� ������ ���·� ����
	template <typename T>
	void SetKeyCallBack(const std::string& name, eKeyState keyState, T* obj,
		void(T::* func)(float))
	{
		KeyInfo* info = findKeyInfo(name);

		if (!info)
		{
			return;
		}

		// ��� �Լ� �����͸� �޴´�.
		// ��� �Լ� ������ ù ��° ���ڷ� �Լ� �ڽ��� ������(this)�� �ޱ� ������
		// this�� obj������ �޾ƿ� ù ��° ���ڿ� �޾ƿ� func ù ��° ���ڷ� �Ҵ��ϰ�,
		// �ݹ� �Լ��� ���ε��Ѵ�.
		// �� ��° ���ڴ� CallBack�� ������ float �� ����
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
	unsigned char convertWindowKeyToDIKey(const unsigned char key);

	DECLARE_SINGLE(CInput)

private:
	HINSTANCE	mhInst;
	HWND		mhWnd;
	
	// DirectInput COM��ü
	IDirectInput8* mInput;
	IDirectInputDevice8* mKeyBoard;
	IDirectInputDevice8* mMouse;
	
	// DirectInput ���������, Window Input��������� �Ǻ��ϴ� Enum
	eInputType meInputType;

	// DirectInput���� Key, Mouse���� ������ ���� ����
	unsigned char mKeyArray[256];
	DIMOUSESTATE mMouseState;

	// ����ڰ� ����ϴ� Key����
	std::unordered_map<std::string, KeyInfo*> mMapKeyInfo;
	
	// Ű �Է� ������ �����ϱ� ���� �迭
	std::vector<KeyState> mVecKeyState;

	// Map���� ���� Ű���� ��ϵ� �� �ֱ� ������, ��ϵǾ��ִ� Ű�� �ߺ� ���� �����ϴ� �迭
	std::vector<unsigned char> mVecAddedKey;

	Vector2 mMousePos;
	Vector2 mMouseMoveAmount;

	bool mbIsCtrlPressed;
	bool mbIsAltPressed;
	bool mbIsShiftPressed;
};

