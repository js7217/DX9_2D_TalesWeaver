#pragma once
#include "GameObject.h"
class CChapterPlayer :
	public CGameObject
{
public:
	CChapterPlayer();
	virtual ~CChapterPlayer();
public:
	enum STATE { STAND, WALK, RUN, BASH, SLASH, SKILL, DAMAGE, COLLAPSE, SLEEP };
	enum MODE {ATT, NOATT};
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;

public:
	static CChapterPlayer* Create();

public:
	void KeyCheck(const D3DXVECTOR3& vPos);
	void Set_IsRun(const bool& bIsRun) {
		m_bIsRun = bIsRun;
	}
	void Set_OffSet(const bool& bIsOffSet) { m_bIsOffSet = bIsOffSet; }
	void Set_CurState(const wstring& wstrDirectionKey) { m_strDirectionKey = wstrDirectionKey; }
private:
	void MakeWorldMatrix();
	void MoveFrame(float fSpeed = 1.f);
	void MovePlayer(); // 플레이어 움직임 함수
	void MotionChange(); // 모션 바꾸기
	void RenderSorting(const wstring& wstrDirectionKey);
	void IsOffSet();
	void SelectRotate(const float& fResult); // 방향 키값 정해주고 왼쪽 오른쪽 판단.
	void PlayerRotation(const float& fX); // 플레이어 방향
private:
	bool			m_bIsRun;
	bool			m_bIsOffSet;
	float			m_fDist; //이동할 거리
	float			m_fSpeed; // 이동속도.
	float			m_fFrameSpeed; // 프레임 속도.
	float			m_fMaxFrameSpeed; // 프레임 맥스 스피드.
	float			m_fStandTimer;
	FRAME			m_tFrame;
	STATE			m_eCurState;
	STATE			m_ePreState;
	MODE			m_eMode;
	wstring			m_wstrObjKey_Weapon;
};

