#include "Key.h"
#include"GalaxyCreater.h"
#include"Physics.h"
#include"ModelManager.h"
#include"Mission.h"
#include"SoundManager.h"

namespace
{
	const char* kGetSoundEffectName = "GetCardKey.mp3";
}

Key::Key(Vec3 pos, int connectObjectNumber, bool antigravity) : Item(pos,ObjectTag::Key,antigravity)
{
	m_getSoundEffectHandle = SoundManager::GetInstance().GetSoundData(kGetSoundEffectName);
	m_connectObjectNumber = connectObjectNumber;
	m_modelHandle=ModelManager::GetInstance().GetModelData("KeyCard");
	MV1SetScale(m_modelHandle, VGet(0.02f, 0.02f, 0.02f));
	MV1SetPosition(m_modelHandle, pos.VGet());
	Init();
}

Key::~Key()
{
}

void Key::Draw()
{
	MV1DrawModel(m_modelHandle);
}

void Key::OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		PlaySoundMem(m_getSoundEffectHandle,DX_PLAYTYPE_BACK);
		auto obj = GalaxyCreater::GetInstance().GetCollidable(m_connectObjectNumber);
		obj->SetIsActive(true);
		MyEngine::Physics::GetInstance().Entry(obj);
		m_isDestroyFlag = true;
		Mission::GetInstance().MissionClear();
	}
}
