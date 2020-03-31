#pragma once
#include "GameObject.h"
class CPlayerState :
	public CGameObject
{
public:
	CPlayerState();
	virtual ~CPlayerState();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static HRESULT Create();
	static void Set_IconFrame(int iIcon) { m_iIconFrame = iIcon; }
	static const int& Get_IconFrame() { return m_iIconFrame; }
private:
	static int m_iIconFrame;
};

