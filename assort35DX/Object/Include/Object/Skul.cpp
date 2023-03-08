#include "Skul.h"
#include "../Animation2D/SkulAnimation.h"
#include "Input.h"
#include "SkulDefaultHead.h"
#include "Scene/Scene.h"
#include "Component/ColliderLine.h"
#include "../UI/SkulWidget.h"
#include "Scene/Viewport.h"
#include "SkulDefaultHead.h"
#include "Timer.h"
#include "Engine.h"
#include "Effect.h"
#include "Scene/SceneResource.h"
#include "FireBall.h"
#include "Component\HPComponent.h"
#include <random>
#include "Component/ColliderCircle.h"

CSkul::CSkul()  :
    m_bCanAttack(true),
    m_bCanJump(true),
    m_bCanMove(true),
	m_bCanSkill1(true),
	m_bCanSkill2(true),
	m_bIsWeapon(true),
	m_bIsAttack(false),
	m_CurCombo(0),
	m_ComboMax(2),
	m_bNextComboInput(false),
	m_bIsJumpAttack(false),
	m_bIsSkill1(false),
	m_bIsDash(false),
	m_DashTime(0.5f),
	m_CurDashTime(0.f),
	m_DashVelocity(2000.f),
	m_DashMaxCount(1),
	m_CurDashCount(0),
	m_bIsDownJump(false),
	m_bCanDownJump(false),
	m_SkulHead(nullptr),
	m_bCanSwitch(true),
	m_bIsSwitch(false),
	m_SwitchDefault_MoveSpeed(250.f),
	m_bIsSwitchCool(false),
	m_bIsSkill1Cool(false),
	m_bIsSkill2Cool(false),
	m_SwitchChiefGuard_MoveSpeed(500.f),
	m_SwitchChiefGuard_MoveDir(0.f),
	m_bIsSwitchCool_ChiefGuard(false),
	m_bIsSkill1Cool_ChiefGuard(false),
	m_bIsSkill2Cool_ChiefGuard(false), 
	m_bIsInteraction(false),
	m_Skill1ChiefGuard_MoveSpeed(500.f),
	m_bIsIlseom(false)
{
	m_SkulState = ESkulState::None;
	m_SubSkulState = ESkulState::None;
}

CSkul::~CSkul()
{
	CEngine::GetInst()->GetTimer()->RemoveAllTimer(this);
}

ESkulState CSkul::GetSkulState() const
{
	return m_SkulState;
}

ESkulState CSkul::GetSkulSubState() const
{
	return m_SubSkulState;
}

void CSkul::Start()
{
	CCharacter::Start();
}

bool CSkul::Init()
{
    CCharacter::Init();

	m_pScene->GetResource()->LoadSound("Effect", false, "Skul_Atk1",
		"AudioClip/Effect/Skul/Skul_Atk 1.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Skul_Atk2",
		"AudioClip/Effect/Skul/Skul_Atk 2.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Skul_Jump_Atk",
		"AudioClip/Effect/Skul/Skul_Jump_Atk.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Default_Jump_Air",
		"AudioClip/Effect/Skul/Default_Jump_Air.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Dash_Small",
		"AudioClip/Effect/Skul/Dash_Small.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Default_Switch",
		"AudioClip/Effect/Skul/Default_Switch.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Mummy_Weapon_Spawn_2",
		"AudioClip/Effect/Skul/Mummy_Weapon_Spawn_2.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Skul_SkullBack",
		"AudioClip/Effect/Skul/Skul_SkullBack.wav");
	m_pScene->GetResource()->LoadSound("Effect", false, "Atk_Flame_Large 1",
		"AudioClip/Effect/Skul/Atk_Flame_Large 1.wav");
	

 	CSceneResource* SceneResource = m_pScene->GetResource();
	SceneResource->LoadTexture("Effect_Altas", TEXT("Effect/Effect_Altas.png"));
	SceneResource->LoadTexture("SkulAtlas", TEXT("Player/Default/DefaultMode.png"));
	SceneResource->LoadTexture("SkulAtlas_NoHead", TEXT("Player/Default/Default_NoHead.png"));
	SceneResource->LoadTexture("SkulAtlas_ChiefGuard", TEXT("Player/ChiefGuardMode/ChiefGuardMode.png"));


	{
		SceneResource->CreateAnimationSequence2D("SkulIdle", 0.4f, "SkulAtlas", 0, 0, 4);
		SceneResource->CreateAnimationSequence2D("SkulRun", 0.8f, "SkulAtlas", 4, 0, 8);
		SceneResource->CreateAnimationSequence2D("SkulAttack1", 0.4f, "SkulAtlas", 11, 0, 4);
		SceneResource->CreateAnimationSequence2D("SkulAttack2", 0.4f, "SkulAtlas", 15, 0, 4);
		SceneResource->CreateAnimationSequence2D("SkulJumpAttack", 0.4f, "SkulAtlas", 9, 1, 4);
		SceneResource->CreateAnimationSequence2D("SkulJumpUp", 0.2f, "SkulAtlas", 2, 1, 2);
		SceneResource->CreateAnimationSequence2D("SkulJumpDownStart", 0.2f, "SkulAtlas", 4, 1, 2);
		SceneResource->CreateAnimationSequence2D("SkulJumpDownLoop", .3f, "SkulAtlas", 6, 1, 3);
		SceneResource->CreateAnimationSequence2D("SkulDash", 0.5f, "SkulAtlas", 1, 1, 1);
		SceneResource->CreateAnimationSequence2D("SkulSkill1", 0.4f, "SkulAtlas", 13, 1, 4);
		SceneResource->CreateAnimationSequence2D("SkulSwitch", .5f, "SkulAtlas", 8, 9, 7);
		SceneResource->CreateAnimationSequence2D("SkulRevival", 2.7f, "SkulAtlas", 17, 1, 27);
		SceneResource->CreateAnimationSequence2D("SkulSkill2", 0.9f, "SkulAtlas", 3, 12, 9);

		/*------------------No Head------------------*/
		SceneResource->CreateAnimationSequence2D("SkulIdle_NoHead", 0.4f, "SkulAtlas_NoHead", 0, 0, 4, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulRun_NoHead", 0.8f, "SkulAtlas_NoHead", 4, 0, 8, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulAttack1_NoHead", 0.4f, "SkulAtlas_NoHead", 4, 1, 5, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulAttack2_NoHead", 0.4f, "SkulAtlas_NoHead", 1, 2, 4, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpAttack_NoHead", 0.4f, "SkulAtlas_NoHead", 5, 3, 4, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpUp_NoHead", 0.2f, "SkulAtlas_NoHead", 6, 2, 2, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpDownStart_NoHead", 0.2f, "SkulAtlas_NoHead", 0, 3, 2, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpDownLoop_NoHead", .3f, "SkulAtlas_NoHead", 2, 3, 3, Vector2(256.f, 256.f));
		SceneResource->CreateAnimationSequence2D("SkulDash_NoHead", .5f, "SkulAtlas_NoHead", 5, 2, 1, Vector2(256.f, 256.f));

		/*---------------------Cheif Guard------------------------*/
		SceneResource->CreateAnimationSequence2D("SkulIdle_ChiefGuard", 0.6f, "SkulAtlas_ChiefGuard", 0, 0, 6, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulRun_ChiefGuard", 0.6f, "SkulAtlas_ChiefGuard", 6, 0, 6, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulAttack1_ChiefGuard", 0.5f, "SkulAtlas_ChiefGuard", 12, 0, 5, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulAttack2_ChiefGuard", 0.5f, "SkulAtlas_ChiefGuard", 1, 1, 5, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulAttack3_ChiefGuard", 0.5f, "SkulAtlas_ChiefGuard", 6, 1, 5, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpAttack_ChiefGuard", 0.5f, "SkulAtlas_ChiefGuard", 4, 2, 5, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpUp_ChiefGuard", 0.3f, "SkulAtlas_ChiefGuard", 12, 1, 3, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpDownStart_ChiefGuard", 0.3f, "SkulAtlas_ChiefGuard", 15, 1, 3, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulJumpDownLoop_ChiefGuard", 0.2f, "SkulAtlas_ChiefGuard", 2, 2, 2, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulDash_ChiefGuard", 0.5f, "SkulAtlas_ChiefGuard", 9, 2, 1, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulSkill1_ChiefGuard", 1.5f, "SkulAtlas_ChiefGuard", 6, 2, 27, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulSkill1_Slash_ChiefGuard", 0.3f, "SkulAtlas_ChiefGuard", 1, 4, 3, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulSkill2_ChiefGuard", 0.5f, "SkulAtlas_ChiefGuard", 6, 4, 7, Vector2(500.f, 500.f));
		SceneResource->CreateAnimationSequence2D("SkulSwitch_ChiefGuard", 0.7f, "SkulAtlas_ChiefGuard", 12, 4, 9, Vector2(500.f, 500.f));

		/*-----------------Effect---------------*/
		SceneResource->CreateAnimationSequence2D("JumpEffect", 0.5f, "Effect_Altas", 0, 0, 10, Vector2(200.f, 200.f));
		SceneResource->CreateAnimationSequence2D("AttackEffect", 0.3f, "Effect_Altas", 0, 23, 10, Vector2(200.f, 200.f));
		SceneResource->CreateAnimationSequence2D("AttackEffect_ChiefGuard", 0.3f, "Effect_Altas", 0, 21, 12, Vector2(200.f, 200.f));
		SceneResource->CreateAnimationSequence2D("FireBall", 0.5f, "Effect_Altas", 0, 9, 12, Vector2(200.f, 200.f));
		SceneResource->CreateAnimationSequence2D("Hit_Fire", 0.5f, "Effect_Altas", 0, 11, 12, Vector2(200.f, 200.f));

	}

	m_HPComponent = CreateObjectComponent<CHPComponent>("HPComponent");

	m_AttackCollision = CreateSceneComponent<CColliderBox2D>("AttackCollision");
	m_AttackCollisionExtent_Default = Vector2(50.f, 50.f);
	m_AttackCollisionExtent_ChiefGuard = Vector2(65.f, 65.f);
	m_AttackCollision->SetExtent(m_AttackCollisionExtent_Default.x, m_AttackCollisionExtent_Default.y);
	m_AttackCollision->SetRelativePos(30.f, 50.f, 0.f);
	m_AttackCollision->SetPivot(0.5f, 0.5f, 0.f);
	m_AttackCollision->SetCollisionProfile("PlayerAttack");
	m_AttackCollision->AddCollisionOverlapCallbackFunction<CSkul>(Collision_State::Begin, this, &CSkul::CollisionBeginOverlap);
	m_RootComponent->AddChild(m_AttackCollision);
	m_AttackCollision->Enable(false);

	m_Arm = CreateSceneComponent<CSpringArm2D>("Arm");
	m_Arm->SetOffset(-640.f, -200, 0.f);
	m_Arm->SetInheritPosZ(false);
	m_Sprite->AddChild(m_Arm);

	m_Camera = CreateSceneComponent<CCamera>("Camera");
	m_Arm->AddChild(m_Camera);

	m_Sprite->CreateAnimation2D<CSkulAnimation>();
	m_Anim = (CSkulAnimation*)m_Sprite->GetAnimation2D();

    m_Body->SetCollisionProfile("Player");
	m_BodyExtent = Vector2(25.f, 40.f);
	m_Body->SetExtent(m_BodyExtent.x, m_BodyExtent.y);
	m_Body->AddCollisionOverlapCallbackFunction<CSkul>(Collision_State::Begin, this, &CSkul::CollisionBeginOverlap);
	m_Body->AddCollisionBlockCallbackFunction<CSkul>(Collision_State::Begin, this, &CSkul::CollisionBeginBlock);


	SetMoveSpeed(400.f);

    CInput::GetInst()->AddKeyCallback<CSkul>("MoveRight", KT_Push, this, &CSkul::MoveRight);
    CInput::GetInst()->AddKeyCallback<CSkul>("MoveLeft", KT_Push, this, &CSkul::MoveLeft);
	CInput::GetInst()->AddKeyCallback<CSkul>("MoveRight", KT_Up, this, &CSkul::InitMoveDirX);
	CInput::GetInst()->AddKeyCallback<CSkul>("MoveLeft", KT_Up, this, &CSkul::InitMoveDirX);
    CInput::GetInst()->AddKeyCallback<CSkul>("MoveDown", KT_Down, this, &CSkul::DownKeyInput);
    CInput::GetInst()->AddKeyCallback<CSkul>("MoveDown", KT_Up, this, &CSkul::DownKeyUp);
    CInput::GetInst()->AddKeyCallback<CSkul>("Jump", KT_Down, this, &CSkul::Jump);
    CInput::GetInst()->AddKeyCallback<CSkul>("Attack", KT_Down, this, &CSkul::AttackInput);
    CInput::GetInst()->AddKeyCallback<CSkul>("Dash", KT_Down, this, &CSkul::Dash);
    CInput::GetInst()->AddKeyCallback<CSkul>("Skill1", KT_Down, this, &CSkul::Skill1Input);
    CInput::GetInst()->AddKeyCallback<CSkul>("Skill2", KT_Down, this, &CSkul::Skill2Input);
    CInput::GetInst()->AddKeyCallback<CSkul>("Switch", KT_Down, this, &CSkul::SwitchInput);
    CInput::GetInst()->AddKeyCallback<CSkul>("Interaction", KT_Down, this, &CSkul::Interaction);


	m_Anim->SetNotifyFunction<CSkul>("SkulAttack1", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack1", this, &CSkul::AttackComboCheck);
	m_Anim->SetNotifyFunction<CSkul>("SkulAttack1_NoHead", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack1_NoHead", this, &CSkul::AttackComboCheck);
	m_Anim->SetNotifyFunction<CSkul>("SkulAttack1_ChiefGuard", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack1_ChiefGuard", this, &CSkul::AttackComboCheck);

	m_Anim->SetNotifyFunction<CSkul>("SkulAttack2", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack2", this, &CSkul::AttackComboCheck);
	m_Anim->SetNotifyFunction<CSkul>("SkulAttack2_NoHead", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack2_NoHead", this, &CSkul::AttackComboCheck);
	m_Anim->SetNotifyFunction<CSkul>("SkulAttack2_ChiefGuard", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack2_ChiefGuard", this, &CSkul::AttackComboCheck);

	m_Anim->SetNotifyFunction<CSkul>("SkulAttack3_ChiefGuard", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulAttack3_ChiefGuard", this, &CSkul::AttackComboCheck);

	m_Anim->SetNotifyFunction<CSkul>("SkulJumpAttack", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulJumpAttack", this, &CSkul::AttackEnd);
	m_Anim->SetNotifyFunction<CSkul>("SkulJumpAttack_NoHead", 2, this, &CSkul::Attack);
	m_Anim->SetFrameEndFunction<CSkul>("SkulJumpAttack_NoHead", this, &CSkul::AttackEnd);

	m_Anim->SetFrameEndFunction<CSkul>("SkulSkill1", this, &CSkul::Skill1End);
	m_Anim->SetFrameEndFunction<CSkul>("SkulSkill1_ChiefGuard", this, &CSkul::Ilseom);
	m_Anim->SetFrameEndFunction<CSkul>("SkulSkill1_Slash_ChiefGuard", this, &CSkul::Skill1End);
	m_Anim->SetFrameEndFunction<CSkul>("SkulSkill2", this, &CSkul::Skill2End);
	m_Anim->SetFrameEndFunction<CSkul>("SkulSkill2_ChiefGuard", this, &CSkul::Skill2End);
	m_Anim->SetNotifyFunction<CSkul>("SkulSkill2_ChiefGuard", 5, this, &CSkul::FireBall);

	// m_Anim->SetFrameEndFunction<CSkul>("SkulSwitch", this, &CSkul::SwitchEnd); 애니메이션이 짧아서 Loop돌리고 타이머로 SwitchEnd함수 호출.
	m_Anim->SetFrameEndFunction<CSkul>("SkulSwitch_ChiefGuard", this, &CSkul::SwitchEnd);
	m_Anim->SetNotifyFunction<CSkul>("SkulSwitch_ChiefGuard", 2, this, &CSkul::SwitchChiefGuardMoveDirInverse);
	m_Anim->SetNotifyFunction<CSkul>("SkulSwitch_ChiefGuard", 4, this, &CSkul::SwitchChiefGuardMoveDirInverse);

	m_Widget = m_pScene->GetViewport()->AddWindow<CSkulWidget>("SkulWidget");
	m_Widget->SetSkul(this);

	SetJumpMaxCount(2);
	SetJumpVelocity(55.f);

	SetState(ESkulState::Default);

	m_AD = 10.f;

    return true;
}

void CSkul::Update(float DeltaTime)
{
    CCharacter::Update(DeltaTime);


	if (m_bIsDash)
	{
		m_CurDashTime += DeltaTime;

		if (m_CurDashTime >= m_DashTime)
			DashEnd();

		else
		{
			if (m_View == Object_View::Right)
				AddMove(((m_DashTime - m_CurDashTime) * DeltaTime) * m_DashVelocity, 0.f, 0.f);
			else
				AddMove(-((m_DashTime - m_CurDashTime) * DeltaTime) * m_DashVelocity, 0.f, 0.f);
		}
	}

	else if (m_bIsSwitch)
	{
		switch (m_SkulState)
		{
		case ESkulState::Default:
            if (m_View == Object_View::Right)
                AddMove(m_SwitchDefault_MoveSpeed * DeltaTime, 0.f, 0.f);
            else
                AddMove(-m_SwitchDefault_MoveSpeed * DeltaTime, 0.f, 0.f);
			break;
		case ESkulState::ChiefGuard:
			break;
		}
	}

}

void CSkul::PostUpdate(float DeltaTime)
{
    CCharacter::PostUpdate(DeltaTime);
}

void CSkul::Collision(float DeltaTime)
{
    CCharacter::Collision(DeltaTime);
}

void CSkul::Render(float DeltaTime)
{
    CCharacter::Render(DeltaTime);


	if (m_bIsDownJump)
	{
		m_bIsDownJump = false;
		m_Body->GetProfile()->vecChannel[(int)Collision_Channel::StaticLine].Interaction = Collision_Interaction::Block;
	}

	m_bIsInteraction = false;
}

void CSkul::SetState(ESkulState State)
{
	m_SkulState = State;
	m_Widget->SwitchSkulState(State);

	switch (State)
	{
	case ESkulState::Default:
		m_DashMaxCount = 2;
		m_ComboMax = 2;
		m_bCanSkill2 = false;
		m_AttackCollision->SetExtent(m_AttackCollisionExtent_Default.x, m_AttackCollisionExtent_Default.y);
		break;
	case ESkulState::NoHead:
		m_DashMaxCount = 2;
		m_ComboMax = 2;
		m_bCanSkill2 = true;
		break;
	case ESkulState::ChiefGuard:
		m_DashMaxCount = 2;
		m_ComboMax = 3;
        m_bCanSkill1 = true;
        m_bCanSkill2 = true;
		m_AttackCollision->SetExtent(m_AttackCollisionExtent_ChiefGuard.x, m_AttackCollisionExtent_ChiefGuard.y);
		break;
	}

}

void CSkul::SetSubState(ESkulState State)
{
	m_SubSkulState = State;
	m_Widget->SwitchSkulSubState(State);
}

void CSkul::Switch()
{
	SwitchInput(0.f);
}

void CSkul::Interaction(float DeltaTime)
{
	m_bIsInteraction = true;
}

void CSkul::SetSkulHead(CSkulDefaultHead* SkulHead)
{
	m_SkulHead = SkulHead;
}

void CSkul::SwitchInput(float DeltaTime)
{
	if (!m_bCanAttack || !m_bCanSwitch || m_SubSkulState == ESkulState::None || m_bIsSwitch || m_bIsSwitchCool) return;

	m_bIsSwitch = true;

	if (m_SkulState == ESkulState::NoHead)
		m_SkulState = ESkulState::Default;

	AttackEnd();
	Skill1End();
	Skill2End();

	SetEnable(false, false, false);

	m_bIsSwitchCool = true;
	ESkulState TempState = m_SkulState;
	SetState(m_SubSkulState);
	SetSubState(TempState);

	if (m_SkulHead)
	{
		m_SkulHead->Active(false);
		m_SkulHead = nullptr;
	}

	switch (m_SkulState)
	{
	case ESkulState::Default:
		CEngine::GetInst()->GetTimer()->StartTimer<CSkul>("Skul_Switch_Default_Animaiton", 1.f, this, &CSkul::SwitchEnd);
		m_pScene->GetResource()->SoundPlay("Default_Switch");
		break;
	case ESkulState::ChiefGuard:
				Box2DInfo Info = m_Body->GetInfo();
		if (m_View == Object_View::Right)
			m_SwitchChiefGuard_MoveDir = 1.f;
		else
			m_SwitchChiefGuard_MoveDir = -1.f;

		AddMove(m_SwitchChiefGuard_MoveSpeed * m_SwitchChiefGuard_MoveDir, 0.f, 0.f);
		CEffect* Effect = m_pScene->SpawnObject<CEffect>("SwitchEffect_ChiefGuard");
		Effect->SetWorldScale(676.f, 200.f, 1.f);
		CAnimation2D* Anim = Effect->GetAnim();
		Anim->AddAnimationSequence2D("SwitchEffect_ChiefGuard");
		Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 30.f, 0.f);
		Effect->SetAutoDestroy();
		CColliderLine* Line = Effect->CreateSceneComponent<CColliderLine>("Line");
		Effect->GetRootComponent()->AddChild(Line);
		Line->SetCollisionProfile("PlayerAttack");
		Line->AddCollisionOverlapCallbackFunction<CSkul>(Collision_State::Begin, this, &CSkul::CollisionBeginOverlap);

		if (m_SwitchChiefGuard_MoveDir == 1)
		{
			Effect->SetView(Object_View::Right);
			Effect->SetPivot(0.f, 0.5f, 0.f);
			Line->SetStart(Vector2(GetWorldPos().x, GetWorldPos().y + 30.f));
			Line->SetEnd(Vector2(GetWorldPos().x + 600.f, GetWorldPos().y + 30.f));
		}
		else
		{
			Effect->SetView(Object_View::Left);
			Effect->SetPivot(1.f, 0.5f, 0.f);
			Line->SetStart(Vector2(GetWorldPos().x, GetWorldPos().y + 30.f));
			Line->SetEnd(Vector2(GetWorldPos().x - 600.f, GetWorldPos().y + 30.f));
		}
		break;
	}

	m_Anim->SetSwitch(true);
	m_bCanSwitch = false;

	CEngine::GetInst()->GetTimer()->StartTimer("SwitchCool", 1.f, this, &CSkul::EndCoolSwitch);
}

void CSkul::SwitchEnd()
{
    switch (m_SkulState)
    {
	case ESkulState::Default:
		break;
    case ESkulState::ChiefGuard:
		m_Body->SetExtent(m_BodyExtent.x, m_BodyExtent.y);
		m_Body->SetPivot(0.5f, 0.f, 0.f);
        break;
    }

	SetEnable();
	m_bIsSwitch = false;
	m_Anim->SetSwitch(false);
}

void CSkul::SwitchChiefGuardMoveDirInverse()
{
	m_SwitchChiefGuard_MoveDir *= -1.f;

	/*m_Body->SetExtent(m_SwitchChiefGuard_MoveSpeed / 2.f + m_BodyExtent.x, m_BodyExtent.y);
	if (m_SwitchChiefGuard_MoveDir == 1)
		m_Body->SetPivot(m_BodyExtent.x / m_Body->GetInfo().Length[0], 0.f, 0.f);
	else
		m_Body->SetPivot(1.f - (m_BodyExtent.x / m_Body->GetInfo().Length[0]), 0.f, 0.f);*/


	AddMove(m_SwitchChiefGuard_MoveSpeed * m_SwitchChiefGuard_MoveDir, 0.f, 0.f);
	CEffect* Effect = m_pScene->SpawnObject<CEffect>("SwitchEffect_ChiefGuard");
	Effect->SetWorldScale(676.f, 200.f, 1.f);
	CAnimation2D* Anim = Effect->GetAnim();
	Anim->AddAnimationSequence2D("SwitchEffect_ChiefGuard");
	Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 30.f, 0.f);
	Effect->SetAutoDestroy();
	CColliderLine* Line = Effect->CreateSceneComponent<CColliderLine>("Line");
	Effect->GetRootComponent()->AddChild(Line);
	Line->SetCollisionProfile("PlayerAttack");
	Line->AddCollisionOverlapCallbackFunction<CSkul>(Collision_State::Begin, this, &CSkul::CollisionBeginOverlap);

	if (m_SwitchChiefGuard_MoveDir == 1)
	{
		Effect->SetView(Object_View::Right);
		Effect->SetPivot(0.f, 0.5f, 0.f);
		Line->SetStart(Vector2(GetWorldPos().x, GetWorldPos().y + 30.f));
		Line->SetEnd(Vector2(GetWorldPos().x + 600.f, GetWorldPos().y + 30.f));
	}
	else
	{
		Effect->SetView(Object_View::Left);
		Effect->SetPivot(1.f, 0.5f, 0.f);
		Line->SetStart(Vector2(GetWorldPos().x, GetWorldPos().y + 30.f));
		Line->SetEnd(Vector2(GetWorldPos().x - 600.f, GetWorldPos().y + 30.f));
	}
}

void CSkul::MoveLeft(float DeltaTime)
{
	if (!m_bCanMove || !m_Active) return;

	if (GetMoveDir().x == 1)
		return;

	if (m_View == Object_View::Right)
		m_AttackCollision->RelativePosFlip(true, false, false);

	AddMove(-GetMoveSpeed() * DeltaTime, 0.f, 0.f);

}

void CSkul::MoveRight(float DeltaTime)
{
	if (!m_bCanMove || !m_Active) return;

	if (GetMoveDir().x == -1)
		return;

	if (m_View == Object_View::Left)
		m_AttackCollision->RelativePosFlip(true, false, false);

	AddMove(GetMoveSpeed() * DeltaTime, 0.f, 0.f);

}

void CSkul::DownKeyInput(float DeltaTime)
{
	m_bCanDownJump = true;
}

void CSkul::DownKeyUp(float DeltaTime)
{
	m_bCanDownJump = false;
}

void CSkul::InitMoveDirX(float DeltaTime)
{
	if (!m_bCanMove) return;

	SetMoveDir(0.f, 0.f, 0.f);
}

void CSkul::Jump(float DeltaTime)
{
	if (!m_bCanJump) return;

	AttackEnd();

	if (m_bCanDownJump && m_Body->GetGroundCollisions().size() == 0)
	{
		m_bIsDownJump = true;
		m_Body->GetProfile()->vecChannel[(int)Collision_Channel::StaticLine].Interaction = Collision_Interaction::Overlap;
		std::list<class CColliderLine*> ColliderList = m_Body->GetGroundLineCollisions();
		
		auto iter = ColliderList.begin();
		auto iterEnd = ColliderList.end();

		for (; iter != iterEnd; iter++)
			m_Body->DeletePrevCollider(*iter);

		Falling();

		return;
	}

	if (GetCurrentJumpCount() > 0 && GetCurrentJumpCount() == 1 )
	{
		CEffect* Effect = m_pScene->SpawnObject<CEffect>("JumpEffect");
		Effect->SetWorldScale(200.f, 200.f, 1.f);
		Effect->SetPivot(0.5f, 0.5f, 0.f);
		CAnimation2D* Anim = Effect->GetAnim();
		Anim->AddAnimationSequence2D("JumpEffect");
		Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y - 50.f, 0.f);
		Effect->SetAutoDestroy();

		m_pScene->GetResource()->SoundPlay("Default_Jump_Air");
	}

	CCharacter::Jump();
	
}

void CSkul::Dash(float DeltaTime)
{
	if (!m_bCanMove || m_bIsDash) return;

	m_bCanMove = false;
	m_bIsDash = true;
	SetPhysicsSimulate(false);

	m_Anim->SetDash(true);

	m_pScene->GetResource()->SoundPlay("Dash_Small");
}

void CSkul::DashEnd()
{
	m_bCanMove = true;
	m_bIsDash = false;
	SetPhysicsSimulate(true);
	Falling();
	m_CurDashTime = 0.f;

	m_Anim->SetDash(false);
}

void CSkul::AttackInput(float DeltaTime)
{
	if (!m_Active) return;

	if (!m_bCanAttack || !m_bIsWeapon) return;


	if (IsInAir())
	{
		JumpAttackInput(DeltaTime);
		return;
	}

	// 첫 콤보 입력 시
	if (!m_bIsAttack)
	{
		m_pScene->GetResource()->SoundPlay("Skul_Atk1");
		m_bIsAttack = true;
		m_CurCombo++;
		/*m_bAutoIdle = false;*/
		// m_vMoveDir = Vector2(0.f, 0.f);

		if (m_View == Object_View::Left)
			m_AttackView = Object_View::Left;

		else
			m_AttackView = Object_View::Right;

		m_Anim->SetAttackComboCount(m_CurCombo);
	}
	// 콤보 중...
	else
	{
		// 현재 콤보 카운트에서 아직 다음 콤보 입력이 들어오지 않았다면
		if (!m_bNextComboInput)
		{
			m_pScene->GetResource()->SoundPlay("Skul_Atk2");
			m_bNextComboInput = true;
		}
	}
}

void CSkul::AttackComboCheck()
{
	if (m_bNextComboInput && m_AttackView == m_View)
	{
		m_CurCombo <= m_ComboMax ? m_CurCombo++ : m_CurCombo = 1;

		m_Anim->SetAttackComboCount(m_CurCombo);

		m_bNextComboInput = false;

		m_AttackCollision->Enable(false);
	}
	else
	{
		AttackEnd();
	}
}

void CSkul::Attack()
{
	m_AttackCollision->Enable(true);
}

void CSkul::AttackEnd()
{
	m_CurCombo = 0;
	m_bIsAttack = false;
	m_Anim->SetJumpAttackOn(false);
	m_AttackCollision->Enable(false);
}

bool CSkul::IsAttack()	const
{
	return m_bIsAttack;
}

void CSkul::JumpAttackInput(float DeltaTime)
{
	m_bIsJumpAttack = true;
	m_Anim->SetJumpAttackOn(true);
}

void CSkul::Skill1Input(float DeltaTime)
{
	if (!m_bCanAttack || !m_bCanSkill1 || m_bIsSkill1 || m_bIsSwitch || m_bIsSkill2) return;


	bool AttackCollisionEnable = false;

	switch (m_SkulState)
	{
	case ESkulState::Default:
		if (m_bIsSkill1Cool) return;
		m_bIsSkill1Cool = true;
		ShotHead();
		CEngine::GetInst()->GetTimer()->StartTimer("Skill1Cool", 6.f, this, &CSkul::EndCoolSkill1);
		break;
	case ESkulState::ChiefGuard:
		if (m_bIsSkill1Cool_ChiefGuard) return;
		m_bIsSkill1Cool_ChiefGuard = true;
		CEngine::GetInst()->GetTimer()->StartTimer("Skill1Cool_ChiefGuard", 6.f, this, &CSkul::EndCoolSkill1_ChiefGuard);
		AttackCollisionEnable = true;
		break;
	}

	AttackEnd();
	if (AttackCollisionEnable)
		m_AttackCollision->Enable(true);

	m_bIsSkill1 = true;
	m_bCanMove = false;
	m_Anim->SetSkill1(true);
}

void CSkul::ShotHead()
{
	SetPhysicsSimulate(false);

	m_SkulHead = m_pScene->SpawnObject<CSkulDefaultHead>("SkulHead");
	m_SkulHead->SetWorldPos2D(GetWorldPos().x, GetWorldPos().y + 50.f);
	m_SkulHead->SetDirection((int)m_View);

	m_bCanSkill2 = false;

	m_pScene->GetResource()->SoundPlay("Mummy_Weapon_Spawn_2");
}

void CSkul::Ilseom()
{
	m_bIsIlseom = true;
	m_Anim->SetIlseom(true);
	
	AddMove(m_Skill1ChiefGuard_MoveSpeed * (int)m_View, 0.f, 0.f);
	CEffect* Effect = m_pScene->SpawnObject<CEffect>("Skill1Effect_ChiefGuard");
	Effect->SetWorldScale(676.f, 200.f, 1.f);
	CAnimation2D* Anim = Effect->GetAnim();
	Anim->AddAnimationSequence2D("Skill1Effect_ChiefGuard");
	Effect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 30.f, 0.f);
	Effect->SetAutoDestroy();
	CColliderLine* Line = Effect->CreateSceneComponent<CColliderLine>("Line");
	Effect->GetRootComponent()->AddChild(Line);
	Line->SetCollisionProfile("PlayerAttack");
	Line->AddCollisionOverlapCallbackFunction<CSkul>(Collision_State::Begin, this, &CSkul::CollisionBeginOverlap);

	if (m_View == Object_View::Right)
	{
		Effect->SetView(Object_View::Right);
		Effect->SetPivot(0.f, 0.5f, 0.f);
		Line->SetStart(Vector2(GetWorldPos().x, GetWorldPos().y + 30.f));
		Line->SetEnd(Vector2(GetWorldPos().x + 600.f, GetWorldPos().y + 30.f));
	}
	else
	{
		Effect->SetView(Object_View::Left);
		Effect->SetPivot(1.f, 0.5f, 0.f);
		Line->SetStart(Vector2(GetWorldPos().x, GetWorldPos().y + 30.f));
		Line->SetEnd(Vector2(GetWorldPos().x - 600.f, GetWorldPos().y + 30.f));
	}

	m_AttackCollision->Enable(false);
}

void CSkul::Skill1End()
{
	switch (m_SkulState)
	{
	case ESkulState::Default:
		if (!m_bIsSwitch)
		{
			SetState(ESkulState::NoHead);
			Falling();
		}
		m_bCanSkill2 = true;
		SetPhysicsSimulate(true);
		break;
	case ESkulState::ChiefGuard:
		m_bIsIlseom = false;
		m_Anim->SetIlseom(false);
		//m_AttackCollision->Enable(false);
		//m_AttackCollision->SetExtent(m_AttackCollisionExtent.x, m_AttackCollisionExtent.y);
	}

	m_bIsSkill1 = false;
	m_bCanMove = true;
	m_Anim->SetSkill1(false);
}

void CSkul::Skill2Input(float DeltaTime)
{
	if (!m_bCanAttack || !m_bCanSkill2 || m_bIsSkill2 || m_bIsSkill1 || m_bIsSwitch) return;


	switch (m_SkulState)
	{
	case ESkulState::NoHead:
		if (m_bIsSkill2Cool) return;
		m_bIsSkill2Cool = true;
		TeleportToHead();
		CEngine::GetInst()->GetTimer()->StartTimer("Skill2Cool", 7.f, this, &CSkul::EndCoolSkill2);
		break;
	case ESkulState::ChiefGuard:
		if (m_bIsSkill2Cool_ChiefGuard) return;
		m_bIsSkill2Cool_ChiefGuard = true;
		CEngine::GetInst()->GetTimer()->StartTimer("Skill2Cool_ChiefGuard", 7.f, this, &CSkul::EndCoolSkill2_ChiefGuard);
		break;
	}

	m_bIsSkill2 = true;
	m_bCanMove = false;
	m_Anim->SetSkill2(true);
}

void CSkul::TeleportToHead()
{
	Vector3 HeadPos = m_SkulHead->GetWorldPos();
	Vector3 SkulPos = GetWorldPos();
	Vector3 MovePos = HeadPos - SkulPos;
	AddMove(MovePos.x, MovePos.y - 50.f, 0.f);

	SetPhysicsSimulate(false);
	EquipHead();
}

void CSkul::FireBall()
{
	CFireBall* FireBall = m_pScene->SpawnObject<CFireBall>("FireBall");
	FireBall->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 30.f, 0.f);
	FireBall->SetView(m_View);

	m_pScene->GetResource()->SoundPlay("Atk_Flame_Large 1");
}

void CSkul::Skill2End()
{
	switch (m_SkulState)
	{
	case ESkulState::Default:
		SetPhysicsSimulate(true);
		m_pScene->GetResource()->SoundPlay("Skul_SkullBack");
		break;
	case ESkulState::ChiefGuard:

		break;
	}
	m_bCanMove = true;
	m_bIsSkill2 = false;
	m_Anim->SetSkill2(false);
}


void CSkul::EndCoolSkill1()
{
	m_bIsSkill1Cool = false;
	if (m_SkulState == ESkulState::Default)
		m_bCanSkill1 = true;
}

void CSkul::EndCoolSkill1_ChiefGuard()
{
	m_bIsSkill1Cool_ChiefGuard = false;
	if (m_SkulState == ESkulState::ChiefGuard)
		m_bCanSkill1 = true;
}

void CSkul::EndCoolSkill2()
{
	m_bIsSkill2Cool = false;
	if (m_SkulState == ESkulState::Default || m_SkulState == ESkulState::NoHead)
		m_bCanSkill2 = true;
}

void CSkul::EndCoolSkill2_ChiefGuard()
{
	m_bIsSkill2Cool_ChiefGuard = false;
	if (m_SkulState == ESkulState::ChiefGuard)
		m_bCanSkill2 = true;
}

void CSkul::EndAllBehavior()
{

}

float CSkul::TakeDamage(float Damage, CGameObject* DamageObject)
{
	return m_HPComponent->TakeDamage(Damage);
}

void CSkul::EndCoolSwitch()
{
	m_bCanSwitch = true;
	m_bIsSwitchCool = false;
}

void CSkul::EquipHead()
{
	SetState(ESkulState::Default);
	m_SkulHead->Active(false);
	m_SkulHead = nullptr;
}

void CSkul::CollisionBeginOverlap(const HitResult& Result, CCollider* Collider)
{
	if (Result.DestCollider->GetProfile()->Channel == Collision_Channel::Head
		&& m_SkulHead && m_SkulHead->IsBlock() && m_SkulState == ESkulState::NoHead)
	{
		EquipHead();
		return;
	}

	IHitable* Hitable = Cast<IHitable*>(Result.DestObject);
	if (Hitable && Collider->GetProfile()->Name == "PlayerAttack")
	{
		Hitable->TakeDamage(m_AD, this);

		CEffect* Effect = m_pScene->SpawnObject<CEffect>("DamageEffect");
		Effect->SetView(m_View);
		Effect->SetPivot(0.5f, 0.5f, 0.f);
		CAnimation2D* Anim = Effect->GetAnim();

		Vector2 EffectPos;
		CColliderBox2D* DestBox = (CColliderBox2D*)Result.DestCollider;

		if (DestBox)
		{
			Vector2 Center = DestBox->GetInfo().Center;
			float Width = DestBox->GetInfo().Length[0];
			float Height = DestBox->GetInfo().Length[1];

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(-Width, Width);
			int RandomWidth = dis(gen);
			int RandomHeight = dis(gen);

			EffectPos = Vector2(Center.x + RandomWidth, Center.y + RandomHeight);
		}

		Effect->SetWorldPos(EffectPos.x, EffectPos.y, 0.f);

		if (m_bIsAttack)
		{
			Anim->AddAnimationSequence2D("AttackEffect");
		}
		else if (m_bIsSkill1 || m_bIsSkill2 || m_bIsSwitch)
		{
			switch (m_SkulState)
			{
			case ESkulState::Default:
				Anim->AddAnimationSequence2D("AttackEffect");
				break;
			case ESkulState::NoHead:
				Anim->AddAnimationSequence2D("AttackEffect");
				break;
			case ESkulState::ChiefGuard:
				Anim->AddAnimationSequence2D("AttackEffect_ChiefGuard");
				break;
			}
		}
		Effect->SetAutoDestroy();
	}

}

void CSkul::CollisionBeginBlock(const HitResult& Result, CCollider* Collider)
{
	if (Result.DestCollider->GetProfile()->Name == "Static" && m_bIsSwitch )
	{
		//if (m_View == Object_View::Right)
		//	SetWorldPosX(m_Body->GetInfo().Center.x + m_Body->GetInfo().Length[0] - m_BodyExtent.x);
		//else
		//	SetWorldPosX(m_Body->GetInfo().Center.x - m_Body->GetInfo().Length[0] + m_BodyExtent.x);
		//m_Body->SetExtent(m_BodyExtent.x, m_BodyExtent.y);
		//m_Body->SetPivot(0.5f, 0.f, 0.f);
	}
}
