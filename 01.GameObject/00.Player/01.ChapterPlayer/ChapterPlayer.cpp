#include "stdafx.h"
#include "ChapterPlayer.h"
#include "ScrollMgr.h"
#include "BtnLst.h"
CChapterPlayer::CChapterPlayer()
	: m_fDist(0.f)
	, m_fStandTimer(0.f)
	, m_fFrameSpeed(1.f)
	, m_fMaxFrameSpeed(3.f)
	, m_bIsRun(false)
	, m_bIsOffSet(true)
{
}


CChapterPlayer::~CChapterPlayer()
{
}

int CChapterPlayer::Update()
{
	CGameObject::LateInit();
	MoveFrame(m_fFrameSpeed);
	MovePlayer(); // 플레이어 이동
	MotionChange(); // 모션 바꾸기
	return NO_EVENT;
}

void CChapterPlayer::LateUpdate()
{
	IsOffSet();
	MakeWorldMatrix();
}

void CChapterPlayer::Render()
{
	RenderSorting(m_strDirectionKey);
}

void CChapterPlayer::LineRender()
{
}

HRESULT CChapterPlayer::Initialize()
{
	m_tInfo.vPos = { 2500.f, 600.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 80.f;
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 28.f;

	m_strObjKey = L"Player2";
	m_strStateKey = L"Stand_NoAtt";
	m_strDirectionKey = L"LD";


	m_fSpeed = 100.f;
	m_eCurState = CChapterPlayer::STAND;
	m_ePreState = CChapterPlayer::STAND;
	m_eObjType = CGameObject::UI;
	m_wstrObjKey_Weapon = L"Sword";

	m_eMode = CChapterPlayer::NOATT;
	CBtnLst::Create();
	return S_OK;
}

HRESULT CChapterPlayer::LateInit()
{
	return S_OK;
}

void CChapterPlayer::Release()
{
}

CChapterPlayer * CChapterPlayer::Create()
{
	CChapterPlayer* pInstance = new CChapterPlayer;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CChapterPlayer::MakeWorldMatrix()
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

void CChapterPlayer::MoveFrame(float fSpeed)
{
	switch (m_eCurState)
	{
	case CChapterPlayer::STAND:
		switch (m_eMode)
		{
		case CChapterPlayer::NOATT:
			m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * 0.3f;

			if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			{
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 0.f;
			}

			if (L"LD" == m_strDirectionKey) // LD / LU 방향일 경우만 해당하는 프레임 움직임
			{
				m_fStandTimer += m_pTimeMgr->GetDeltaTime(); // 3초가 될때까지 더할것.
				if (m_fStandTimer >= 3.f)
				{
					m_fStandTimer = 0.f;
					m_tFrame.fMaxFrame = 28.f;
				}
			}
			else if (L"LU" == m_strDirectionKey)
			{
				m_fStandTimer += m_pTimeMgr->GetDeltaTime(); // 3초가 될때까지 더할것.
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
		case CChapterPlayer::ATT:
			m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

			if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
				m_tFrame.fCurFrame = 0.f;
			break;
		default:
			break;
		}
		break;
	case CChapterPlayer::WALK:
	case CChapterPlayer::RUN:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			m_tFrame.fCurFrame = 0.f;
		break;
	default:
		break;
	}
}

void CChapterPlayer::MovePlayer()
{
	switch (m_eCurState)
	{
	case CChapterPlayer::WALK:
	case CChapterPlayer::RUN:
		if (m_fDist > 0)
		{
			m_fDist -= m_fSpeed * m_pTimeMgr->GetDeltaTime();
			m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();
		}
		else
		{
			m_tInfo.vPos -= m_tInfo.vDir * m_fDist;
			m_fDist = 0.f;
			m_eCurState = CChapterPlayer::STAND;
		}
		break;
	default:
		break;
	}
}

void CChapterPlayer::MotionChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CChapterPlayer::STAND:
			switch (m_eMode)
			{
			case CChapterPlayer::NOATT:
				m_strStateKey = L"Stand_NoAtt";
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 0.f;
				break;
			case CChapterPlayer::ATT:
				m_strStateKey = L"Stand";
				m_tFrame.fCurFrame = 0.f;
				m_tFrame.fMaxFrame = 4.f;
				break;
			default:
				break;
			}
			break;
		case CChapterPlayer::WALK:
			m_strStateKey = L"Walk";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 9.f;
			break;
		case CChapterPlayer::RUN:
			m_strStateKey = L"Run";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 9.f;
			break;
		case CChapterPlayer::BASH:
			m_strStateKey = L"Bash";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 5.f;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CChapterPlayer::RenderSorting(const wstring & wstrDirectionKey)
{
	const TEX_INFO* pTexShadow = m_pTextureMgr->GetTexInfo(
		L"Shadow",
		0);
	NULL_CHECK(pTexShadow);

	float fCenterX_Shadow = pTexShadow->tImgInfo.Width * 0.5f;
	float fCenterY_Shadow = pTexShadow->tImgInfo.Height * 0.5f - m_tInfo.fCY * 0.4f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	if (CChapterPlayer::SLEEP != m_eCurState) // 자고있는 상태가 아니면 그림자를 그릴것.
	{
		m_pGraphicDev->GetSprite()->Draw(pTexShadow->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX_Shadow, fCenterY_Shadow, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255)); // 그림자
	}

	const TEX_INFO* pTexPlayer = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey,
		m_strDirectionKey);

	NULL_CHECK(pTexPlayer);

	float fCenterX = pTexPlayer->tImgInfo.Width * 0.5f;
	float fCenterY = pTexPlayer->tImgInfo.Height * 0.5f;



	if (L"LU" == wstrDirectionKey || L"Up" == wstrDirectionKey) // 플레이어 -> 무기 순서로 그림.
	{

		m_pGraphicDev->GetSprite()->Draw(pTexPlayer->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (CChapterPlayer::ATT == m_eMode)
		{
			if (CChapterPlayer::STAND == m_eCurState || CChapterPlayer::BASH == m_eCurState || CChapterPlayer::SLASH == m_eCurState)
			{
				const TEX_INFO* pTexWeapon = m_pTextureMgr->GetTexInfo(
					m_wstrObjKey_Weapon,
					int(m_tFrame.fCurFrame),
					m_strStateKey,
					m_strDirectionKey);

				NULL_CHECK(pTexWeapon);

				m_pGraphicDev->GetSprite()->Draw(pTexWeapon->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				if (CChapterPlayer::BASH == m_eCurState) // 무기 이펙트
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
	else // 무기 -> 플레이어 순서로 그림.
	{
		if (CChapterPlayer::ATT == m_eMode)
		{
			if (CChapterPlayer::STAND == m_eCurState || CChapterPlayer::BASH == m_eCurState || CChapterPlayer::SLASH == m_eCurState)
			{
				const TEX_INFO* pTexWeapon = m_pTextureMgr->GetTexInfo(
					m_wstrObjKey_Weapon,
					int(m_tFrame.fCurFrame),
					m_strStateKey,
					m_strDirectionKey);

				NULL_CHECK(pTexWeapon);

				m_pGraphicDev->GetSprite()->Draw(pTexWeapon->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				if (CChapterPlayer::BASH == m_eCurState) // 무기 이펙트(Bash)
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
}

void CChapterPlayer::KeyCheck(const D3DXVECTOR3& vPos)
{
	switch (m_eCurState)
	{
	case CChapterPlayer::STAND:
	case CChapterPlayer::WALK:
	case CChapterPlayer::RUN:
	{
		m_tInfo.vDir = vPos - m_tInfo.vPos;
		m_fDist = D3DXVec3Length(&m_tInfo.vDir); // 길이 구함.
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir); // 방향

		PlayerRotation(vPos.x);
		break;
	}
	default:
		break;
	}
}

void CChapterPlayer::IsOffSet()
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

void CChapterPlayer::SelectRotate(const float & fResult)
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
	case CChapterPlayer::STAND:
	case CChapterPlayer::WALK:
	case CChapterPlayer::RUN:
		if (m_bIsRun)
		{
			m_fSpeed = 200.f;
			m_eCurState = CChapterPlayer::RUN;
		}
		else
		{
			m_fSpeed = 100.f;
			m_eCurState = CChapterPlayer::WALK;
		}
		break;
	default:
		break;
	}
}

void CChapterPlayer::PlayerRotation(const float & fX)
{
	float fDegree = D3DXToDegree(acosf(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook)));
	float fResult = (fDegree / 45.f);

	SelectRotate(fResult); // 방향 정해줌.

	if (fX > m_tInfo.vPos.x && fResult >= 0.5f && fResult <= 3.5f) // 오른쪽
		m_tInfo.vSize.x = -1.f;
	else
		m_tInfo.vSize.x = 1.f;
}
