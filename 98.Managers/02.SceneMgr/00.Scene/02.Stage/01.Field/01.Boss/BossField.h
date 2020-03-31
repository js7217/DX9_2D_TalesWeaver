#pragma once
#include "Scene.h"
class CBossField :
	public CScene
{
public:
	CBossField();
	virtual ~CBossField();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBossField* Create();
};

