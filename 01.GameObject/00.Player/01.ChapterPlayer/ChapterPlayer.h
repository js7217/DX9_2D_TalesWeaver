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
	void MovePlayer(); // �÷��̾� ������ �Լ�
	void MotionChange(); // ��� �ٲٱ�
	void RenderSorting(const wstring& wstrDirectionKey);
	void IsOffSet();
	void SelectRotate(const float& fResult); // ���� Ű�� �����ְ� ���� ������ �Ǵ�.
	void PlayerRotation(const float& fX); // �÷��̾� ����
private:
	bool			m_bIsRun;
	bool			m_bIsOffSet;
	float			m_fDist; //�̵��� �Ÿ�
	float			m_fSpeed; // �̵��ӵ�.
	float			m_fFrameSpeed; // ������ �ӵ�.
	float			m_fMaxFrameSpeed; // ������ �ƽ� ���ǵ�.
	float			m_fStandTimer;
	FRAME			m_tFrame;
	STATE			m_eCurState;
	STATE			m_ePreState;
	MODE			m_eMode;
	wstring			m_wstrObjKey_Weapon;
};

