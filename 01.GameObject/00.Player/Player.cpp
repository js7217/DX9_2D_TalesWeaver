#include "stdafx.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "AStar.h"
#include "Monster.h" // ���� Ŭ������ �ٲܰ�.
#include "TimeUI.h"
#include "PlayerState.h"
#include "StateBar.h"
#include "QuickSlot.h"
#include "SmallBar.h"
#include "ChatUI.h"
#include "Minimap.h"
#include "BtnLst.h"
#include "Effect.h"
#include "ComboEffect.h"
#include "DmgEffect.h"
#include "Moon.h"
#include "SilpidLance.h"
CPlayer::CPlayer()
	: m_pKeyMgr(CKeyMgr::GetInstance())
	, m_fDist(0.f)
	, m_fSpeed(0.f)
	, m_bIsRun(false)
	, m_bIsAStar(false)
	, m_bStartCombo(false)
	, m_bIsAtt(false)
	, m_fRaycast(50.f)
	, m_fFrameSpeed(1.f)
	, m_fComboTimer(0.f)
	, m_fMaxFrameSpeed(3.f)
	, m_iComboCount(0)
	, m_fStandTimer(0.f)
	, m_eMode(NOATT)
	, m_pTarget(nullptr)
	, m_pSkillTarget(nullptr)
	, m_fDamageTimer(0.5f)
	, m_bIsRest(false)
	, m_bIsSleep(false)
	, m_iSeed(10000)
	, m_bIsMove(true)
	, m_iExp(0)
	, m_iMaxExp(100)
	, m_iLevel(1)
	, m_bIsAttSkill(false)
	, m_bIsOffSet(true)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CPlayer::~CPlayer()
{
	Release();
}

BYTE CPlayer::Get_PlayerTileOption()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_pAStar->SelectOption(m_tInfo.vPos);
}

void CPlayer::Set_Damage(int iDamage)
{
	CDmgEffect::Create(m_tInfo.vPos, L"PlayerDmgFont", iDamage); 
	m_tData.iHp -= iDamage;
}

void CPlayer::Set_HPHeal(int iHeal)
{
	CEffect::Create(m_tInfo.matWorld, { 0, 20 }, 5.f, L"HpPotion");
	m_tData.iHp += iHeal;
}

void CPlayer::Set_MPHeal(int iHeal)
{
	CEffect::Create(m_tInfo.matWorld, { 0, 20 }, 5.f, L"MpPotion");
	m_tData.iMP += iHeal;
}

void CPlayer::MovePos(const D3DXVECTOR3 & vPos)
{
	switch (m_eCurState)
	{
	case CPlayer::STAND:
	case CPlayer::WALK:
	case CPlayer::RUN:
	{
		m_tInfo.vDir = vPos - m_tInfo.vPos;
		m_fDist = D3DXVec3Length(&m_tInfo.vDir); // ���� ����.
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir); // ����

		PlayerRotation(vPos.x);
		break;
	}
	default:
		break;
	}
}

int CPlayer::Update()
{
 	CGameObject::LateInit();
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();
	BYTE byMouseState = m_pAStar->SelectOption(vMousePos);
	if(2 == byMouseState || 3 == byMouseState || 4 == byMouseState) // ����Ʈ��� ���콺 ��� �ٲٱ�.
		dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObject(MOUSE))->Set_MouseState(CMouse::GATE);
	CMouse::STATE eMouseState = dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObjectW(MOUSE))->Get_MouseState();

	// ���콺�� ���� ���ϴ°��� ����Ű�� �������� ����ó��. Ȥ�� ���콺�� UI�� ����Ű��������
	if (1 == byMouseState || !m_bIsMove || 
		CMouse::NOPASS == eMouseState)
	{
		MoveFrame(m_fFrameSpeed);
		WorkType(false); // AStar ���� ����������.
		MotionChange(); // ��� �ٲٱ�
		UpdateStats();
		if(m_bIsMove)
			dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObject(MOUSE))->Set_MouseState(CMouse::NOPASS);

		m_bIsMove = true; // �÷��̾ �����̱�Բ� �Ұ�. �㳪 �ؿ��� �ٽ� UI�� ���콺�� �ö������� false�� �ٲ�.
		return NO_EVENT;
	}
	CGameObject::UpdateRect_Scroll(0.f, -40.f);
	TargetTracking(); // Ÿ�� Ʈ��ŷ(Ÿ���� �ִ��� �������� ���� ó��)
	KeyCheck(); // ���� Ű üũ
	AttackCombo(); // Űüũ ���� �־�� �ѹ��� ������ ����?
	MoveFrame(m_fFrameSpeed); // �ִϸ��̼� ������
	RayCastTile(); // ����ĳ��Ʈ (A��Ÿ ����ȭ)
	MovePlayer(); // �÷��̾� �̵�
	MotionChange(); // ��� �ٲٱ�
	UpdateStats(); // �����϶����� ���̽��� ���� sp�� ������Ʈ �Ǿ���Ѵ�.
	cout << m_iComboCount << endl;
	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	IsOffSet();
	MakeWorldMatrix();
	NotifyData();
}

void CPlayer::Render()
{
	RenderSorting(m_strDirectionKey);
}


void CPlayer::LineRender()
{
	CGameObject::UpdateRect_Scroll(0.f, -40.f);
	D3DXVECTOR2 vec[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	m_pGraphicDev->GetLine()->Draw(vec, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
}

HRESULT CPlayer::Initialize()
{
	m_tInfo.vPos = { 2500.f, 600.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 80.f;
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 4.f;

	m_strObjKey = L"Player2";
	m_strStateKey = L"Stand_NoAtt";
	m_strDirectionKey = L"Down";

	m_tData.strObjKey = m_strObjKey;
	m_tData.strName = L"���ù�";
	m_tData.iAtt = 10;
	m_tData.iHp = 50;
	m_tData.iMaxHp = 100;
	m_tData.iMP = 50;
	m_iMaxMp = 100;
	m_fMaxSp = 2000.f;
	m_fSp = 2000.f;

	m_fSpeed = 100.f;

	m_pAStar = CAStar::Create();
	m_vRaycast = m_tInfo.vPos + m_tInfo.vDir * m_fRaycast;

	m_eCurState = CPlayer::STAND;
	m_ePreState = CPlayer::STAND;
	m_eObjType = CGameObject::PLAYER;
	m_wstrObjKey_Weapon = L"Sword";
	return S_OK;
}

HRESULT CPlayer::LateInit()
{
	//�⺻ UI �߰��Ұ�.
	CPlayerState::Create();
	CQuickSlot::Create();
	CStateBar::Create();
	CSmallBar::Create(this);
	CChatUI::Create();
	CMinimap::Create();
	CBtnLst::Create();
	CTimeUI::Create(this);

	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_HP, &m_tData.iHp);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_MP, &m_tData.iMP);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_SP, &m_fSp);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_MAXHP, &m_tData.iMaxHp);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_MAXMP, &m_iMaxMp);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_MAXSP, &m_fMaxSp);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_SEED, &m_iSeed);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_LEVEL, &m_iLevel);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_EXP, &m_iExp);
	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_MAXEXP, &m_iMaxExp);

	CDataSubject::GetInstance()->AddData(CDataSubject::PLAYER_MATRIX, &m_tInfo.matWorld);

	return S_OK;
}

void CPlayer::Release()
{
	SafeDelete(m_pAStar);
}

CPlayer * CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::MakeWorldMatrix()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1) - 40.f,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_MATRIX, &m_tInfo.matWorld);
}

void CPlayer::MoveFrame(float fSpeed) // ��ȯ.
{
	switch (m_eCurState)
	{
	case CPlayer::STAND:
		switch (m_eMode)
		{
		case CPlayer::NOATT:
			m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * 0.3f;

			if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			{
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 0.f;
			}
	
			if (L"LD" == m_strDirectionKey) // LD / LU ������ ��츸 �ش��ϴ� ������ ������
			{
				m_fStandTimer += m_pTimeMgr->GetDeltaTime(); // 3�ʰ� �ɶ����� ���Ұ�.
				if (m_fStandTimer >= 3.f)
				{
					m_fStandTimer = 0.f;
					m_tFrame.fMaxFrame = 28.f;
				}
			}
			else if (L"LU" == m_strDirectionKey)
			{
				m_fStandTimer += m_pTimeMgr->GetDeltaTime(); // 3�ʰ� �ɶ����� ���Ұ�.
				if (m_fStandTimer >= 3.f)
				{
					m_fStandTimer = 0.f;
					m_tFrame.fMaxFrame = 26.f;
				}
			}
			else
			{
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 0.f;
				m_fStandTimer = 0.f;
				break;
			}
			break;
		case CPlayer::ATT:
			m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

			if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
				m_tFrame.fCurFrame = 0.f;
			break;
		default:
			break;
		}
		break;
	case CPlayer::WALK:// Stand, Walk, Run�� ������ ��ȯ.
	case CPlayer::RUN:
	case CPlayer::SLEEP:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			m_tFrame.fCurFrame = 0.f;
		break;
	case CPlayer::BASH:
	case CPlayer::SLASH:
	case CPlayer::COLLAPSE: // ������ �ش� �ൿ�� ������ Stand ���·� ���ư���.
	case CPlayer::SKILL:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;


		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		{
			m_eCurState = CPlayer::STAND;
			m_bIsAttSkill = false;
		}
		break;
	case CPlayer::DAMAGE:
		if (m_fDamageTimer <= 0.f)
		{
			m_fDamageTimer = 0.5f;
			m_eCurState = CPlayer::STAND;
		}
		else
			m_fDamageTimer -= m_pTimeMgr->GetDeltaTime();
		break;
	default:
		break;
	}
}

void CPlayer::MovePlayer()
{
	switch (m_eCurState)
	{
	case CPlayer::WALK:
	case CPlayer::RUN:
		WorkType(m_bIsAStar);
		break;
	default:
		break;
	}
}

void CPlayer::MotionChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CPlayer::STAND:
			switch (m_eMode)
			{
			case CPlayer::NOATT:
				m_strStateKey = L"Stand_NoAtt";
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 0.f;
				break;
			case CPlayer::ATT:
				m_strStateKey = L"Stand";
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 4.f;
				m_bIsAttack = false;
				break;
			default:
				break;
			}
			break;
		case CPlayer::WALK:
			m_strStateKey = L"Walk";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 9.f;
			break;
		case CPlayer::RUN:
			m_strStateKey = L"Run";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 9.f;
			break;
		case CPlayer::BASH:
			m_strStateKey = L"Bash";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 5.f;
			break;
		case CPlayer::SLASH:
			m_strStateKey = L"Slash";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 4.f;
			break;
		case CPlayer::SKILL:
			m_strStateKey = L"Skill";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 4.f;
			m_bIsAttSkill = true;
			break;
		case CPlayer::DAMAGE:
			m_strStateKey = L"Damage";
			m_tFrame.fCurFrame = 1.f;
			m_tFrame.fMaxFrame = 1.f;
			CEffect::Create(m_tInfo.matWorld, { 0, 3 }, 5.f, L"MonsterHit");
			CSoundMgr::GetInstance()->MyPlaySound(L"Hit.mp3", CSoundMgr::PLAYER_HIT);
			break;
		case CPlayer::COLLAPSE:
			m_strStateKey = L"Collapse";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 0.f;
			break;
		case CPlayer::SLEEP:
			m_strStateKey = L"Sleep";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 3.f;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::KeyCheck()			/////////////// ���� Ű üũ ///////
{
	CMouse::STATE eMouseState = dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObjectW(MOUSE))->Get_MouseState();

	if (CMouse::CHAT == eMouseState)
		return;
	if (m_pKeyMgr->KeyPressing(KEY_LBUTTON))
	{
		m_bIsMove = true;
		if (m_eMode == CPlayer::ATT && m_bIsAtt) // ���ݻ���(��������)
		{
			if (CMonster::DIE == dynamic_cast<CMonster*>(m_pTarget)->Get_State()) // ������ ���°� ���� �����϶�.
				return;
			if (m_eCurState == CPlayer::SKILL) // ��ų�����϶� ��� �ȹٲ�.
				return;
			m_eCurState = CPlayer::BASH;
			if (!m_bStartCombo) // �޺��� �����Ҷ� �ʱ�ȭ : �޺� ī��Ʈ, Ÿ�̸� 1��
			{
				m_iComboCount = 1;
				m_fComboTimer = 1.f;
				m_bStartCombo = true;
			}
		}
		else
		{
			D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

			switch (m_eCurState)
			{
			case CPlayer::STAND:
			case CPlayer::WALK:
			case CPlayer::RUN:
			{
				m_tInfo.vDir = vMousePos - m_tInfo.vPos;
				m_fDist = D3DXVec3Length(&m_tInfo.vDir); // ���� ����.
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir); // ����

				PlayerRotation(vMousePos.x);

				m_bIsAStar = false;
				m_pAStar->Set_ClearBestLst();

				break;
			}
			default:
				break;
			}
		}
	}
	else if (m_pKeyMgr->KeyPressing(KEY_RBUTTON))
	{
		m_eCurState = CPlayer::SKILL;
		int iSelect = CPlayerState::Get_IconFrame();
		if(nullptr == m_pSkillTarget)
			return;
		if (0 == iSelect)
		{
			if (m_bIsAttSkill)
			{
				CSilpidLance::Create(m_strDirectionKey, m_tInfo);
				m_tData.iMP -= 20;
				m_bIsAttSkill = false;
				m_iComboCount++;
			}
		}
		else if (1 == iSelect)
		{
			if (m_bIsAttSkill)
			{
				FAILED_CHECK(CMoon::Create(m_pSkillTarget));
				dynamic_cast<CMonster*>(m_pSkillTarget)->Set_TargetSkillDamage(5, 5);
				m_tData.iMP -= 10;
				m_bIsAttSkill = false;
				m_iComboCount++;
			}
		}

		if (!m_bStartCombo) // �޺��� �����Ҷ� �ʱ�ȭ : �޺� ī��Ʈ, Ÿ�̸� 1��
		{
			m_iComboCount = 1;
			m_fComboTimer = 1.f;
			m_bStartCombo = true;
		}

	}
	if (m_pKeyMgr->KeyDown(KEY_DELETE))
	{
		if (!m_bIsRest)
		{
			m_bIsRest = true;
			m_eCurState = CPlayer::COLLAPSE;
			dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObject(MOUSE))->Set_MouseState(CMouse::NOPASS);
		}
		else
		{
			m_bIsRest = false;
			m_eCurState = CPlayer::STAND;
			dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObject(MOUSE))->Set_MouseState(CMouse::NORMAL);
		}
	}
	if (m_pKeyMgr->KeyDown(KEY_INSERT))
	{
		if (!m_bIsSleep)
		{
			m_bIsSleep = true;
			m_eCurState = CPlayer::SLEEP;
			dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObject(MOUSE))->Set_MouseState(CMouse::NOPASS);
		}
		else
		{
			m_bIsSleep = false;
			m_eCurState = CPlayer::STAND;
			dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObject(MOUSE))->Set_MouseState(CMouse::NORMAL);
		}
	}
	if (m_pKeyMgr->KeyDown(KEY_R))
	{
		if (m_fSp <= 0)
			return;
		if (m_bIsRun)
		{
			m_bIsRun = false;
			m_fSpeed = 100.f;
			m_eCurState = CPlayer::WALK;
		}
		else
		{
			m_bIsRun = true;
			m_fSpeed = 200.f;
			m_eCurState = CPlayer::RUN;
		}
	}
	if (m_pKeyMgr->KeyDown(KEY_F1))
	{
		CPlayerState::Set_IconFrame(0);
	}
	else if(m_pKeyMgr->KeyDown(KEY_F2))
	{
		CPlayerState::Set_IconFrame(1);
	}

	if (m_pKeyMgr->KeyDown(KEY_DOWN))
	{
		m_eCurState = CPlayer::STAND;
		switch (m_eMode)
		{
		case CPlayer::NOATT:
			m_eMode = ATT;
			m_strStateKey = L"Stand";
			m_tFrame.fCurFrame = 0.f;
			break;
		case CPlayer::ATT:
			m_eMode = NOATT;
			m_strStateKey = L"Stand_NoAtt";
			m_tFrame.fCurFrame = 0.f;
			break;
		default:
			break;
		}
	}
	if (m_pKeyMgr->KeyDown(KEY_UP))
		Yeon();
}

void CPlayer::IsOffSet()
{
	if (!m_bIsOffSet)
		return;
	D3DXVECTOR3 vOffSet = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	D3DXVECTOR3 vScroll = {
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f };

	D3DXVECTOR3 vLength = vScroll - vOffSet;
	float fScrollLength = D3DXVec3Length(&(vLength));
	D3DXVec3Normalize(&vLength, &vLength);

	if (5.f < fScrollLength)
	{
		CScrollMgr::MoveScroll(vLength * m_fSpeed * m_pTimeMgr->GetDeltaTime());
	}
	else
		CScrollMgr::MoveScroll(vLength * 0.f);
}

void CPlayer::SelectRotate(const float & fResult)
{
	if (fResult <= 0.5f)
		m_strDirectionKey = L"Up";
	else if (fResult > 0.5f && fResult <= 1.5f)
		m_strDirectionKey = L"LU";
	else if (fResult > 1.5f && fResult <= 2.5f)
		m_strDirectionKey = L"Left";
	else if (fResult > 2.5f && fResult <= 3.5f)
		m_strDirectionKey = L"LD";
	else
		m_strDirectionKey = L"Down";

	switch (m_eCurState)
	{
	case CPlayer::STAND:
	case CPlayer::WALK:
	case CPlayer::RUN:
		if (m_fSp <= 0)
			m_bIsRun = false;
		if (m_bIsRun)
		{
			m_fSpeed = 200.f;
			m_eCurState = CPlayer::RUN;
		}
		else
		{
			m_fSpeed = 100.f;
			m_eCurState = CPlayer::WALK;
		}
		break;
	default:
		break;
	}
}

void CPlayer::PlayerRotation(const float& fX)
{
	float fDegree = D3DXToDegree(acosf(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook)));
	float fResult = (fDegree / 45.f);

	SelectRotate(fResult); // ���� ������.

	if (fX > m_tInfo.vPos.x && fResult >= 0.5f && fResult <= 3.5f) // ������
		m_tInfo.vSize.x = -1.f;
	else
		m_tInfo.vSize.x = 1.f;
}

void CPlayer::RayCastTile()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();// ����ƽ�̶� ��ø �������Ѿ���.
	m_vRaycast = m_tInfo.vPos + m_tInfo.vDir * m_fRaycast; // ����ĳ��Ʈ ����.

	BYTE byOption = m_pAStar->SelectOption(m_vRaycast); // ����ĳ��Ʈ ���� Ÿ�� �ɼ��� ����.
	if (1 == byOption) // �浹 �ɼ��̰� ����Ʈ ����Ʈ�� ����ִٸ� AStar�� �����Ѵ�.
	{
		if (!m_bIsAStar && m_pAStar->GetBestLst().empty())
		{
			m_bIsAStar = true;
			m_pAStar->StartAstar(m_tInfo.vPos, vMousePos);
		}
	}
}

void CPlayer::AttackCombo()
{
	m_fComboTimer -= m_pTimeMgr->GetDeltaTime() * 2.f; // �ð��� ��´�.

	switch (m_eCurState)
	{
	case CPlayer::STAND:
	case CPlayer::WALK:
	case CPlayer::RUN:
	case CPlayer::DAMAGE:
	case CPlayer::SLEEP:
	case CPlayer::COLLAPSE:
		if (0.f > m_fComboTimer)
			m_iComboCount = 0;
		break;
	case CPlayer::BASH:
	case CPlayer::SLASH:
		if (m_bStartCombo) // �޺��Ҷ� ī���� �ؾ���!!!
		{
			if (0.f < m_fComboTimer) // Ÿ�̸Ӱ� 0.5�ʰ� �ȵ� ���¿��� �����ٸ� 
			{
				if (m_tFrame.fCurFrame >= 2.f && m_tFrame.fCurFrame <= 3.f)
				{
					if (nullptr == m_pTarget)
						break;
					dynamic_cast<CMonster*>(m_pTarget)->Set_State(CMonster::DAMAGE);
					dynamic_cast<CMonster*>(m_pTarget)->Set_Damage(m_tData.iAtt);
					if (!m_bIsAttack)
					{
						m_iComboCount++;
						m_bIsAttack = true;
					}
				}

				m_fFrameSpeed += m_pTimeMgr->GetDeltaTime() * 0.5f; //������ ���ǵ带 ���� ������Ų��.
				if (m_fFrameSpeed >= m_fMaxFrameSpeed) // �ִ� ������ ���ǵ带 �Ѿ�� ����ش�.
					m_fFrameSpeed = m_fMaxFrameSpeed;

				m_fComboTimer = 1.f; // ���� �ð��� �ٽ� 1��
			}
			else
			{
				m_iComboCount = 0;
				m_bStartCombo = false;
				m_bIsAtt = false;
				m_fFrameSpeed = 1.f;
			}
		}
		break;
	case CPlayer::SKILL:
		if (m_bStartCombo) // �޺��Ҷ� ī���� �ؾ���!!!
		{
			if (0.f < m_fComboTimer) // Ÿ�̸Ӱ� 0.5�ʰ� �ȵ� ���¿��� �����ٸ� 
			{
				
				m_fFrameSpeed += m_pTimeMgr->GetDeltaTime() * 0.5f; //������ ���ǵ带 ���� ������Ų��.
				if (m_fFrameSpeed >= m_fMaxFrameSpeed) // �ִ� ������ ���ǵ带 �Ѿ�� ����ش�.
					m_fFrameSpeed = m_fMaxFrameSpeed;

				m_fComboTimer = 1.f; // ���� �ð��� �ٽ� 1��
			}
			else
			{
				m_iComboCount = 0;
				m_bStartCombo = false;
				m_bIsAtt = false;
				m_fFrameSpeed = 1.f;
			}
		}
		break;
	default:
		break;
	}
}

void CPlayer::TargetTracking()
{
	if (nullptr == m_pTarget)
	{
		m_bIsAtt = false;
		return;
	}

	RECT rc = {};
	if (IntersectRect(&rc, &m_tRect, &m_pTarget->Get_Rect()))
	{
		if (m_eMode == ATT && !m_bIsAtt)
		{
			m_bIsAtt = true;
			m_eCurState = CPlayer::BASH;
			if (!m_bStartCombo) // �޺��� �����Ҷ� �ʱ�ȭ : �޺� ī��Ʈ, Ÿ�̸� 1��
			{
				m_fComboTimer = 1.f;
				m_bStartCombo = true;
			}
			PlayerRotation(m_pTarget->Get_Info().vPos.x);
		}
	}
	else
	{
		m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
		m_fDist = D3DXVec3Length(&m_tInfo.vDir); // ���� ����.
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir); // ����

		PlayerRotation(m_pTarget->Get_Info().vPos.x);
	}
}

void CPlayer::NotifyData()
{
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_HP, &m_tData.iHp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_MP, &m_tData.iMP);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_SP, &m_fSp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_MAXHP, &m_tData.iMaxHp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_MAXMP, &m_iMaxMp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_MAXSP, &m_fMaxSp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_LEVEL, &m_iLevel);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_EXP, &m_iExp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_MAXEXP, &m_iMaxExp);
	CDataSubject::GetInstance()->Notify(CDataSubject::PLAYER_SEED, &m_iSeed);
}

void CPlayer::UpdateStats()
{
	if (m_bIsRest || m_bIsSleep) // ����������
	{
		m_tData.iHp += m_pTimeMgr->GetDeltaTime() * 100.f;
	}

	if (m_tData.iHp <= 0 || m_tData.iHp >= m_tData.iMaxHp) // HP
		m_tData.iHp = m_tData.iMaxHp;
	if (m_tData.iMP <= 0 || m_tData.iMP >= m_iMaxMp) // MP
		m_tData.iMP = m_iMaxMp;
	if (CPlayer::RUN == m_eCurState) //SP
	{
		m_fSp -= m_pTimeMgr->GetDeltaTime() * 10.f;
		if (m_fSp <= 0)
			m_fSp = 0;
	}
	else
	{
		m_fSp += m_pTimeMgr->GetDeltaTime() * 10.f;
		if (m_fSp > m_fMaxSp)
			m_fSp = m_fMaxSp;
	}

	if (m_iExp >= m_iMaxExp) // ������!!
	{
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::LEVELUP);
		D3DXMATRIX matLevelUp;
		D3DXMatrixTranslation(&matLevelUp, 
			m_tInfo.vPos.x - CScrollMgr::GetScroll(0), 
			m_tInfo.vPos.y - m_tInfo.fCY * 1.5f - CScrollMgr::GetScroll(1), 
			0.f);
		CEffect::Create(matLevelUp, { 0, 29 }, 1.f, L"LevelUp");
		int iExp = m_iExp - m_iMaxExp;
		m_iLevel++;
		m_iMaxExp += 100;
		m_iExp = iExp;

		m_tData.iMaxHp += 200;
		m_tData.iHp = m_tData.iMaxHp;
		m_iMaxMp += 100;
		m_tData.iMP = m_iMaxMp;

		m_tData.iAtt += 20;
		CSoundMgr::GetInstance()->MyPlaySound(L"lvup.mp3", CSoundMgr::LEVELUP);
	}
}

void CPlayer::RenderSorting(const wstring & wstrDirectionKey)
{
	const TEX_INFO* pTexShadow = m_pTextureMgr->GetTexInfo(
		L"Shadow",
		0);
	NULL_CHECK(pTexShadow);

	float fCenterX_Shadow = pTexShadow->tImgInfo.Width * 0.5f;
	float fCenterY_Shadow = pTexShadow->tImgInfo.Height * 0.5f - m_tInfo.fCY * 0.4f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	if (CPlayer::SLEEP != m_eCurState) // �ڰ��ִ� ���°� �ƴϸ� �׸��ڸ� �׸���.
	{
		m_pGraphicDev->GetSprite()->Draw(pTexShadow->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX_Shadow, fCenterY_Shadow, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255)); // �׸���
	}

	const TEX_INFO* pTexPlayer = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey,
		m_strDirectionKey);

	NULL_CHECK(pTexPlayer);

	float fCenterX = pTexPlayer->tImgInfo.Width * 0.5f;
	float fCenterY = pTexPlayer->tImgInfo.Height * 0.5f;



	if (L"LU" == wstrDirectionKey || L"Up" == wstrDirectionKey) // �÷��̾� -> ���� ������ �׸�.
	{

		m_pGraphicDev->GetSprite()->Draw(pTexPlayer->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (CPlayer::ATT == m_eMode)
		{
			if (CPlayer::STAND == m_eCurState || CPlayer::BASH == m_eCurState || CPlayer::SLASH == m_eCurState)
			{
				const TEX_INFO* pTexWeapon = m_pTextureMgr->GetTexInfo(
					m_wstrObjKey_Weapon,
					int(m_tFrame.fCurFrame),
					m_strStateKey,
					m_strDirectionKey);

				NULL_CHECK(pTexWeapon);

				m_pGraphicDev->GetSprite()->Draw(pTexWeapon->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				if (CPlayer::BASH == m_eCurState) // ���� ����Ʈ
				{
					const TEX_INFO* pTexEffect = m_pTextureMgr->GetTexInfo(
						L"Effect",
						int(m_tFrame.fCurFrame),
						L"BashEffect",
						m_strDirectionKey);

					NULL_CHECK(pTexEffect);

					float fCenterX = pTexEffect->tImgInfo.Width * 0.5f;
					float fCenterY = pTexEffect->tImgInfo.Height * 0.5f;


					m_pGraphicDev->GetSprite()->Draw(pTexEffect->pTexture, nullptr,
						&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	}
	else // ���� -> �÷��̾� ������ �׸�.
	{
		if (CPlayer::ATT == m_eMode)
		{
			if (CPlayer::STAND == m_eCurState || CPlayer::BASH == m_eCurState || CPlayer::SLASH == m_eCurState)
			{
				const TEX_INFO* pTexWeapon = m_pTextureMgr->GetTexInfo(
					m_wstrObjKey_Weapon,
					int(m_tFrame.fCurFrame),
					m_strStateKey,
					m_strDirectionKey);

				NULL_CHECK(pTexWeapon);

				m_pGraphicDev->GetSprite()->Draw(pTexWeapon->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				if (CPlayer::BASH == m_eCurState) // ���� ����Ʈ(Bash)
				{
					const TEX_INFO* pTexEffect = m_pTextureMgr->GetTexInfo(
						L"Effect",
						int(m_tFrame.fCurFrame),
						L"BashEffect",
						m_strDirectionKey);

					NULL_CHECK(pTexEffect);

					float fCenterX = pTexEffect->tImgInfo.Width * 0.5f;
					float fCenterY = pTexEffect->tImgInfo.Height * 0.5f;

					m_pGraphicDev->GetSprite()->Draw(pTexEffect->pTexture, nullptr,
						&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

			}
		}

		m_pGraphicDev->GetSprite()->Draw(pTexPlayer->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if(1 < m_iComboCount)
		RenderCombo();
}

void CPlayer::WorkType(bool bIsAstar)
{
	if (!bIsAstar) // AStar�� ���� �Ϲ����� ���� �̵�
	{
		if (m_fDist > 0)
		{
			m_fDist -= m_fSpeed * m_pTimeMgr->GetDeltaTime();
			m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();
		}
		else
		{
			m_tInfo.vPos -= m_tInfo.vDir * m_fDist;
			m_fDist = 0.f;
			m_eCurState = CPlayer::STAND;
		}
	}
	else // AStar�� �̵��ؾ��ϴ� ���.
	{
		list<TILE*>& BestLst = m_pAStar->GetBestLst();

		if (BestLst.empty())
		{
			m_bIsAStar = false; // A��Ÿ�� ������ �ٽ� �˷��ش�.
			return;
		}

		m_tInfo.vDir = BestLst.front()->vPos - m_tInfo.vPos;

		m_fDist = D3DXVec3Length(&m_tInfo.vDir);
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		PlayerRotation(BestLst.front()->vPos.x); // �� �� ���� �Լ�

		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();

		if (m_fDist < 3.f)
		{
#ifdef _DEBUG
			BestLst.front()->byDrawID = 0;
#endif
			BestLst.pop_front();
		}
	}
}

void CPlayer::RenderCombo()
{
	int iCombo = m_iComboCount;
	float fX = 20.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Interface",
		0,
		L"Combo");

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1) - m_tInfo.fCY * 1.2f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	while (iCombo != 0)
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			L"Effect",
			int(iCombo % 10),
			L"Combo");

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixTranslation(&m_tInfo.matWorld,
			m_tInfo.vPos.x - CScrollMgr::GetScroll(0) + fX,
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1) - 10.f - m_tInfo.fCY * 1.2f,
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		iCombo /= 10;
		fX -= pTexInfo->tImgInfo.Width;
	}
}

void CPlayer::Yeon()
{
	if (m_iCount <= 0)
	{
		m_iCount = 6;
		m_eCurState = CPlayer::STAND;
	}
	else
	{
		switch (m_eCurState)
		{
		case CPlayer::BASH:
			m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

			if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			{
				m_tFrame.fCurFrame = 0.f;
				m_eCurState = CPlayer::SLASH;
			}
			break;
		case CPlayer::SLASH:
			m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

			if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			{
				m_tFrame.fCurFrame = 0.f;
				m_eCurState = CPlayer::BASH;
			}
			break;
		default:
			break;
		}

		m_iCount--;
	}
}
