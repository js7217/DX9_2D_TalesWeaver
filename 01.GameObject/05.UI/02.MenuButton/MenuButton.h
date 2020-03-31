#pragma once
#include "GameObject.h"
class CMenuButton :
	public CGameObject
{
private:
	explicit CMenuButton();
	explicit CMenuButton(const wstring& wstrObjKey, const wstring& wstrStateKey, const D3DXVECTOR3& vPos);
public:
	virtual ~CMenuButton();

	// CGameObject��(��) ���� ��ӵ�
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static HRESULT Create(const wstring& wstrObjKey, const wstring& wstrStateKey, const D3DXVECTOR3& vPos);

private:
	int m_iFrame; // ������Ű �ٲ�
	CKeyMgr*	m_pKeyMgr;
};

