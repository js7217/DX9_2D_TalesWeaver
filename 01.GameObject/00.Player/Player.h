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
	// CGameObject��(��) ���� ��ӵ�
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
	void MovePlayer(); // �÷��̾� ������ �Լ�
	void MotionChange(); // ��� �ٲٱ�
	void KeyCheck(); // Ű����, ���콺 ��ư üũ
	void IsOffSet(); //�÷��̾� ��ġ ���� ��ũ���� ���ϴ� �Լ�.
	void SelectRotate(const float& fResult); // ���� Ű�� �����ְ� ���� ������ �Ǵ�.
	void PlayerRotation(const float& fX); // �÷��̾� ����
	void RayCastTile(); // ����ĳ��Ʈ�� Ÿ�� �ɼǰ� �ް� AStar ���� ����
	void AttackCombo(); // ���� �޺� ����.
	void TargetTracking(); // Ÿ���� ������� ����.
	void NotifyData(); // �����͸� �������� �ֽ�ȭ.
	void UpdateStats();
	void RenderSorting(const wstring& wstrDirectionKey);
	void WorkType(bool bIsAstar);
	void RenderCombo();
	void Yeon();
private:
	bool			m_bIsOffSet;
	bool			m_bIsAttSkill;
	bool			m_bIsAttack = false; // �����ѹ��� ���ϰ�
	bool			m_bIsMove; // �����ϼ� �ִ��� ������.
	bool			m_bIsRun;
	bool			m_bIsRest; // �����ִ� �����ľ�
	bool			m_bIsSleep; // �ڰ��ִ� �����ľ�
	bool			m_bIsAStar; // A��Ÿ�� ���� �ƴ��� �ȵƴ���
	bool			m_bStartCombo; // �޺� ����.
	bool			m_bIsAtt;	// ���� ������ �ص� �Ǵ��� �ȵǴ���.
	int				m_iComboCount; // �޺� ī��Ʈ. -> ��� �������� ���ؾ���.
	int				m_iMaxMp;
	int				m_iSeed; // ��
	int				m_iExp; // ����ġ
	int				m_iMaxExp; // �ش� ���� ����ġ.
	int				m_iLevel; // ����
	float			m_fSp;
	float			m_fMaxSp;
	float			m_fDist; //�̵��� �Ÿ�
	float			m_fFrameSpeed; // ������ �ӵ�.
	float			m_fMaxFrameSpeed; // ������ �ƽ� ���ǵ�.
	float			m_fSpeed; // �̵��ӵ�.
	float			m_fComboTimer; // �޺� Ÿ�̸�.
	float			m_fStandTimer; // ���ĵ� ���� ���� Ÿ�̸�.(���̿ø�)
	float			m_fRaycast; // ����ĳ��Ʈ�� �Ÿ�.
	float			m_fDamageTimer; // ������ ����� ���ӵǴ� �ð�.
	float			m_iCount = 6;
	UNIT_DATA		m_tData; // �÷��̾� ������.
	D3DXVECTOR3		m_vRaycast; // ���� ����ĳ��Ʈ. : �ش� �Ÿ��ȿ� �浹Ÿ���� ������ AStar�� �ߵ� ��ų����.
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

