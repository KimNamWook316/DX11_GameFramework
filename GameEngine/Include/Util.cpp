#include "Util.h"

std::string CUtil::CollisionChannelToString(eCollisionChannel eChan)
{
	std::string out;

	switch (eChan)
	{
	case eCollisionChannel::Object:
		out = "Object";
		break;
	case eCollisionChannel::Player:
		out = "Player";
		break;
	case eCollisionChannel::Monster:
		out = "Monster";
		break;
	case eCollisionChannel::PlayerAttack:
		out = "PlayerAttack";
		break;
	case eCollisionChannel::MonsterAttack:
		out = "MonsterAttack";
		break;
	case eCollisionChannel::Custom1:
		out = "Custom1";
		break;
	case eCollisionChannel::Custom2:
		out = "Custom2";
		break;
	case eCollisionChannel::Custom3:
		out = "Custom3";
		break;
	case eCollisionChannel::Custom4:
		out = "Custom4";
		break;
	case eCollisionChannel::Custom5:
		out = "Custom5";
		break;
	case eCollisionChannel::Custom6:
		out = "Custom6";
		break;
	case eCollisionChannel::Custom7:
		out = "Custom7";
		break;
	case eCollisionChannel::Custom8:
		out = "Custom8";
		break;
	case eCollisionChannel::Custom9:
		out = "Custom9";
		break;
	case eCollisionChannel::Custom10:
		out = "Custom10";
		break;
	default:
		assert(false);
		break;
	}

	return out;
}

std::string CUtil::CollsionInteractionToString(eCollisionInteraction eInterAction)
{
	return eCollisionInteraction::Collision == eInterAction ? "TRUE" : "FALSE";
}
