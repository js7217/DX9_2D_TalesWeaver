#pragma once
#include "Scene.h"
class CClad :
	public CScene
{
private:
	explicit CClad();
public:
	virtual ~CClad();

	// CScene을(를) 통해 상속됨
public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;
public:
	static CClad* Create();
	bool m_bIsChapter2;

};