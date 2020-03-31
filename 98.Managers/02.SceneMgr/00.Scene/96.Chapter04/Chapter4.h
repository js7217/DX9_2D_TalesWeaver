#pragma once
#include "Scene.h"
class CChapter4 :
	public CScene
{
public:
	CChapter4();
	virtual ~CChapter4();
public:
	// CScene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

private:
	void LoadDialog(const wstring& wstrFilePath);
	void ShowDialogBox(int iIndex);
	void ShowDialogText(int iIndex);
	void ShowDialogBar();
	void MovePlayer(int iIndex);
public:
	static CChapter4* Create();
private:
	bool			m_bIsClear;
	vector<DIALOG*> m_vecDialog;
	int				m_iMaxIndex;
	int				m_iIndex;
	float			m_fLength;
	float			m_fStart;
};

