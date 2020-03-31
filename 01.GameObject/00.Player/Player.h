#pragma once
#include "GameObject.h"
class CAStar;
class CPlayer :
	public CGameObject
{
public:
	enum STATE { STAND, WALK, RUN, BASH, SLASH, SKILL, DAMAGE, COLLAPSE, SLEEP };
	enum MODE { NOATT, ATT };
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	const STATE& Get_State() { return m_eCurState; }
	BYTE Get_PlayerTileOption();
	const int& Get_Seed() { return m_iSeed; }
	CGameObject* Get_Target() { return m_pTarget; }
	CGameObject* Get_SkillTarget() { return m_pSkillTarget; }
public:
	void Set_State(STATE eState) { m_eCurState = eState; }
	void Set_IsAtt(bool bIsAtt) { m_bIsAtt = bIsAtt; }
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void Set_SkillTarget(CGameObject* pSkillTarget) { m_pSkillTarget = pSkillTarget; }
	void Set_Damage(int iDamage);
	void Set_Move(const bool& bIsMove) { m_bIsMove = bIsMove; }
	void Set_Exp(int iExp) { m_iExp += iExp; }
	void Set_Mode(MODE eMode) { m_eMode = eMode; }
	void ChangeWeapon(const wstring& wstrObjKey) { m_wstrObjKey_Weapon = wstrObjKey; }
	void Set_Seed(int iSeed) { m_iSeed += iSeed; }
	void Set_HPHeal(int iHeal);
	void Set_MPHeal(int iHeal);
	void Set_Att(int iAtt) { m_tData.iAtt += iAtt; }
	void Set_DirectionKey(const wstring& wstrDirectionKey) { m_strDirectionKey = wstrDirectionKey; }
	void Set_IsRun(const bool& bIsRun) { m_bIsRun = bIsRun; }
	void Set_IsOffSet(const bool& bIsOffSet) { m_bIsOffSet = bIsOffSet; }
public:
	void MovePos(const D3DXVECTOR3& vPos);

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
	static CPlayer* Create();

private:
	void MakeWorldMatrix();
	void MoveFrame(float fSpeed = 1.f);
	void MovePlayer(); // 플레이어 움직임 함수
	void MotionChange(); // 모션 바꾸기
	void KeyCheck(); // 키보드, 마우스 버튼 체크
	void IsOffSet(); //플레이어 위치 따라 스크롤이 변하는 함수.
	void SelectRotate(const float& fResult); // 방향 키값 정해주고 왼쪽 오른쪽 판단.
	void PlayerRotation(const float& fX); // 플레이어 방향
	void RayCastTile(); // 레이캐스트의 타일 옵션값 받고 AStar 실행 유무
	void AttackCombo(); // 공격 콤보 설정.
	void TargetTracking(); // 타겟이 있을경우 추적.
	void NotifyData(); // 데이터를 옵저버에 최신화.
	void UpdateStats();
	void RenderSorting(const wstring& wstrDirectionKey);
	void WorkType(bool bIsAstar);
	void RenderCombo();
	void Yeon();
private:
	bool			m_bIsOffSet;
	bool			m_bIsAttSkill;
	bool			m_bIsAttack = false; // 공격한번만 먹일것
	bool			m_bIsMove; // 움직일수 있는지 없는지.
	bool			m_bIsRun;
	bool			m_bIsRest; // 쉬고있는 상태파악
	bool			m_bIsSleep; // 자고있는 상태파악
	bool			m_bIsAStar; // A스타가 시작 됐는지 안됐는지
	bool			m_bStartCombo; // 콤보 시작.
	bool			m_bIsAtt;	// 공격 시작을 해도 되는지 안되는지.
	int				m_iComboCount; // 콤보 카운트. -> 어따 넣을건지 정해야함.
	int				m_iMaxMp;
	int				m_iSeed; // 돈
	int				m_iExp; // 경험치
	int				m_iMaxExp; // 해당 레벨 경험치.
	int				m_iLevel; // 레벨
	float			m_fSp;
	float			m_fMaxSp;
	float			m_fDist; //이동할 거리
	float			m_fFrameSpeed; // 프레임 속도.
	float			m_fMaxFrameSpeed; // 프레임 맥스 스피드.
	float			m_fSpeed; // 이동속도.
	float			m_fComboTimer; // 콤보 타이머.
	float			m_fStandTimer; // 스탠드 상태 시작 타이머.(바이올린)
	float			m_fRaycast; // 레이캐스트의 거리.
	float			m_fDamageTimer; // 데미지 모션이 지속되는 시간.
	float			m_iCount = 6;
	UNIT_DATA		m_tData; // 플레이어 데이터.
	D3DXVECTOR3		m_vRaycast; // 벡터 레이캐스트. : 해당 거리안에 충돌타일이 있으면 AStar를 발동 시킬것임.
	CKeyMgr*		m_pKeyMgr;
	FRAME			m_tFrame;
	STATE			m_eCurState;
	STATE			m_ePreState;
	MODE			m_eMode;
	CAStar*			m_pAStar;
	CGameObject*	m_pTarget;
	CGameObject*	m_pSkillTarget;
	wstring			m_wstrObjKey_Weapon;
};

