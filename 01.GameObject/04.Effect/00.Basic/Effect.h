#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
private:
	explicit CEffect();
	explicit CEffect(const D3DXMATRIX& matWorld, const FRAME& tFrame, const float& fSpeed
		, const wstring& wstrStateKey, const wstring& wstrDirectionKey);
public:
	virtual ~CEffect();

	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;


public:
	static HRESULT Create(const D3DXMATRIX& matWorld, const FRAME& tFrame, const float& fSpeed
		, const wstring& wstrStateKey, const wstring& wstrDirectionKey = L"NONE");
private:
	FRAME m_tFrame;
	float m_fSpeed;
};

