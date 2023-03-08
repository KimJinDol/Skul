#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	{
		size_t	Size = m_vecChannel.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_vecChannel[i]);
		}
	}

	{
		auto	iter = m_mapProfile.begin();
		auto	iterEnd = m_mapProfile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
	}
}

bool CCollisionManager::Init()
{
	CCollisionManager::GetInst()->CreateChannel("Static", Collision_Interaction::Block);
	CCollisionManager::GetInst()->CreateChannel("StaticLine", Collision_Interaction::Block);
	CCollisionManager::GetInst()->CreateChannel("Player", Collision_Interaction::Block);
	CCollisionManager::GetInst()->CreateChannel("Monster", Collision_Interaction::Block);
	CCollisionManager::GetInst()->CreateChannel("Head", Collision_Interaction::Block);
	CCollisionManager::GetInst()->CreateChannel("PlayerAttack", Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->CreateChannel("MonsterAttack", Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->CreateChannel("Ray", Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->CreateChannel("Trigger", Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->CreateChannel("Ragdoll", Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->CreateChannel("MonsterAttackBlockStatic", Collision_Interaction::Ignore);


	CCollisionManager::GetInst()->CreateProfile("Ragdoll", Collision_Channel::Ragdoll);
	CCollisionManager::GetInst()->SetProfileChannelState("Ragdoll", Collision_Channel::Static, Collision_Interaction::Block);
	CCollisionManager::GetInst()->SetProfileChannelState("Ragdoll", Collision_Channel::StaticLine, Collision_Interaction::Block);


	CCollisionManager::GetInst()->CreateProfile("Trigger", Collision_Channel::Trigger);
	CCollisionManager::GetInst()->SetProfileChannelState("Trigger", Collision_Channel::Player, Collision_Interaction::Overlap);
	CCollisionManager::GetInst()->CreateProfile("Ray", Collision_Channel::Ray);
	CCollisionManager::GetInst()->SetProfileChannelState("Ray", Collision_Channel::Static, Collision_Interaction::Block);
	CCollisionManager::GetInst()->CreateProfile("Static", Collision_Channel::Static);
	CCollisionManager::GetInst()->SetProfileChannelState("Static", Collision_Channel::StaticLine, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->CreateProfile("StaticLine", Collision_Channel::StaticLine);
	CCollisionManager::GetInst()->SetProfileChannelState("StaticLine", Collision_Channel::Static, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->SetProfileChannelState("Static", Collision_Channel::Ragdoll, Collision_Interaction::Block);
	CCollisionManager::GetInst()->SetProfileChannelState("StaticLine", Collision_Channel::Ragdoll, Collision_Interaction::Block);
	CCollisionManager::GetInst()->SetProfileChannelState("Static", Collision_Channel::MonsterAttackBlockStatic, Collision_Interaction::Block);
	CCollisionManager::GetInst()->SetProfileChannelState("StaticLine", Collision_Channel::MonsterAttackBlockStatic, Collision_Interaction::Block);


	CCollisionManager::GetInst()->CreateProfile("Player", Collision_Channel::Player);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::Player, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::Head, Collision_Interaction::Overlap);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::Monster, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::Trigger, Collision_Interaction::Overlap);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::MonsterAttack, Collision_Interaction::Overlap);
	CCollisionManager::GetInst()->SetProfileChannelState("Player", Collision_Channel::MonsterAttackBlockStatic, Collision_Interaction::Overlap);

	CCollisionManager::GetInst()->CreateProfile("Head", Collision_Channel::Head);
	CCollisionManager::GetInst()->SetProfileChannelState("Head", Collision_Channel::Monster, Collision_Interaction::Overlap);

	CCollisionManager::GetInst()->CreateProfile("Monster", Collision_Channel::Monster);
	CCollisionManager::GetInst()->SetProfileChannelState("Monster", Collision_Channel::Monster, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->SetProfileChannelState("Monster", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	CCollisionManager::GetInst()->SetProfileChannelState("Monster", Collision_Channel::PlayerAttack, Collision_Interaction::Overlap);
	CCollisionManager::GetInst()->SetProfileChannelState("Monster", Collision_Channel::Head, Collision_Interaction::Overlap);

	CCollisionManager::GetInst()->CreateProfile("PlayerAttack", Collision_Channel::PlayerAttack);
	CCollisionManager::GetInst()->SetProfileChannelState("PlayerAttack", Collision_Channel::Monster, Collision_Interaction::Overlap);

	CCollisionManager::GetInst()->CreateProfile("MonsterAttack", Collision_Channel::MonsterAttack);
	CCollisionManager::GetInst()->SetProfileChannelState("MonsterAttack", Collision_Channel::Player, Collision_Interaction::Overlap);

	CCollisionManager::GetInst()->CreateProfile("MonsterAttackBlockStatic", Collision_Channel::MonsterAttackBlockStatic);
	CCollisionManager::GetInst()->SetProfileChannelState("MonsterAttackBlockStatic", Collision_Channel::Player, Collision_Interaction::Overlap);
	CCollisionManager::GetInst()->SetProfileChannelState("MonsterAttackBlockStatic", Collision_Channel::Static, Collision_Interaction::Block);
	CCollisionManager::GetInst()->SetProfileChannelState("MonsterAttackBlockStatic", Collision_Channel::StaticLine, Collision_Interaction::Block);

	return true;
}

bool CCollisionManager::CreateChannel(const std::string& Name, 
	Collision_Interaction Interaction)
{
	if (m_vecChannel.size() == (size_t)Collision_Channel::End)
		return false;

	CollisionChannel* Channel = new CollisionChannel;
	Channel->Name = Name;
	Channel->Interaction = Interaction;
	Channel->Channel = (Collision_Channel)m_vecChannel.size();

	m_vecChannel.push_back(Channel);

	auto	iter = m_mapProfile.begin();
	auto	iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->vecChannel.push_back(*Channel);
	}

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& Name, Collision_Channel Channel)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new CollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;

	m_mapProfile.insert(std::make_pair(Name, Profile));

	size_t	Size = m_vecChannel.size();

	for (size_t i = 0; i < Size; ++i)
	{
		Profile->vecChannel.push_back(*m_vecChannel[i]);
	}

	return true;
}

bool CCollisionManager::SetProfileChannelState(const std::string& Name, 
	Collision_Channel DestChannel, Collision_Interaction Interaction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->vecChannel[(int)DestChannel].Interaction = Interaction;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	auto	iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
