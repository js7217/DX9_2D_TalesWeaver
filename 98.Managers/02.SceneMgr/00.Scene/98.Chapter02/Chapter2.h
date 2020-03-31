#pragma once
#include "Scene.h"
class CChapterPlayer;
class CChapter2 :
	public CScene
{
public:
	CChapter2();
	virtual ~CChapter2();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;

private:
	void LoadDialog(const wstring& wstrFilePath);
	void ShowDialogBox(int iIndex);
	void ShowDialogText(int iIndex);
	void ShowDialogBar();
	void MovePlayer(int iIndex);
public:
	static CChapter2* Create();
private:
	vector<DIALOG*> m_vecDialog;
	int				m_iMaxIndex;
	int				m_iIndex;
	float			m_fLength;
	float			m_fStart;
	CChapterPlayer*	m_pPlayer;
};

