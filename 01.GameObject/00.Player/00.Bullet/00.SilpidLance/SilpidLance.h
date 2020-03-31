#pragma once
#include "GameObject.h"
class CSilpidLance :
	public CGameObject
{
private:
	explicit CSilpidLance();
public:
	virtual ~CSilpidLance();

public:
	const int& Get_Att() { return m_iAtt; }
public:
	void Set_Info(const INFO& tInfo) { m_tInfo = tInfo; }
	void Set_DirectionKey(const wstring& wstrDirectionKey) { m_strDirectionKey = wstrDirectionKey; }
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
	static HRESULT Create(const wstring& wstrDirectionKey, const INFO& tInfo);

private:
	bool m_bIsShake;
	int m_iShakeNum;
	bool m_bIsDead;
	float m_fSpeed; // 속도?
	float m_fDist; // 이동거리
	int m_iAtt; // 공격력
	D3DXVECTOR3 m_vScroll;
};

