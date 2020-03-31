#include "stdafx.h"
#include "Chapter2.h"
#include "Linda.h"
#include "ChapterPlayer.h"
#include "Terrain.h"
CChapter2::CChapter2()
{
}


CChapter2::~CChapter2()
{
	Release();
}

void CChapter2::Update()
{
	if (m_iIndex >= m_iMaxIndex)
		return;
	m_pObjectMgr->Update();
	CScene::LateInit();
	if (CKeyMgr::GetInstance()->KeyDown(KEY_RETURN) || CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON))
	{
		if (m_fLength < m_vecDialog[m_iIndex]->strDialog.length())
			m_fLength = m_vecDialog[m_iIndex]->strDialog.length();
		else
		{
			m_iIndex++;
			m_fLength = 0;
			MovePlayer(m_iIndex);
		}
	}

	m_pPlayer->Update();
}

void CChapter2::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	m_pPlayer->LateUpdate();
	if (m_iIndex >= m_iMaxIndex)
	{
		//씬체인지
		m_fStart -= CTimeMgr::GetInstance()->GetDeltaTime() * 50.f;
		if (m_fStart <= -32.5f)
			CSceneMgr::GetInstance()->SceneChange(CScene::CLAD);
	}
	else
	{
		m_fStart += CTimeMgr::GetInstance()->GetDeltaTime() * 50.f;
		if (m_fStart >= 32.5f)
			m_fStart = 32.5f;
	}
}

void CChapter2::Render()
{
	m_pObjectMgr->Render();
	m_pPlayer->Render();
	ShowDialogBar();

	if (m_iIndex > m_iMaxIndex)
		return;

	if (m_fStart >= 32.5f)
	{
		if (m_vecDialog[m_iIndex]->strName == L"Player")
		{
			const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
				L"Chapter", m_vecDialog[m_iIndex]->iDrawID, L"PlayerImage");
			NULL_CHECK(pTexInfo);

			float fCenterX = (float)pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = (float)pTexInfo->tImgInfo.Height * 0.5f;

			CGraphicDev::GetInstance()->Render_Begin();

			D3DXMATRIX matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, fCenterX, WINCY - fCenterY - 75.f, 0.f);
			matWorld = matScale * matTrans;

			CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
				nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			ShowDialogBox(m_iIndex);
			ShowDialogText(m_iIndex);
		}
		else
		{
			ShowDialogBox(m_iIndex);
			ShowDialogText(m_iIndex);
		}
	}
}

void CChapter2::LineRender()
{
	m_pObjectMgr->LineRender();
}

HRESULT CChapter2::Initialize()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(L"../Data/Clad_Tile.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	HRESULT hr = m_pObjectMgr->AddObject(CGameObject::TERRAIN, pGameObject);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	LoadDialog(L"../DialogData/Chapter2.txt");
	m_iIndex = 0;
	m_fStart = -75.f;

	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::PLAYER);

	m_pPlayer = CChapterPlayer::Create();
	m_pPlayer->Set_Pos(2080.f, 1060.f);
	m_pPlayer->Set_CurState(L"Left");
	CSoundMgr::GetInstance()->PlayBGM(L"Chapter2.mp3");
	return S_OK;
}

HRESULT CChapter2::LateInit()
{
	CLinda::Create(1880.f, 1060.f);

	return S_OK;
}

void CChapter2::Release()
{
//	CScene::m_bIsInit = false;
	for_each(m_vecDialog.begin(), m_vecDialog.end(), SafeDelete<DIALOG*>);
	m_vecDialog.clear();
	m_vecDialog.shrink_to_fit();

	SafeDelete(m_pPlayer);

	CObjectMgr::GetInstance()->DestroyRenderList();
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::PLAYER);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::BUTTON);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::UI);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::MONSTER); // 몬스터 삭제
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::NPC);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::TERRAIN);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);

	CSoundMgr::GetInstance()->PlayBGM(L"Clad.mp3");
}

void CChapter2::LoadDialog(const wstring & wstrFilePath)
{
	std::locale::global(std::locale("Korean"));
	wifstream fin;

	fin.open(wstrFilePath.c_str());

	if (fin.fail())
		return;

	if (!m_vecDialog.empty())
	{
		for_each(m_vecDialog.begin(), m_vecDialog.end(), SafeDelete<DIALOG*>);
		m_vecDialog.clear();
		m_vecDialog.shrink_to_fit();
	}

	TCHAR szName[MIN_STR] = L"";
	TCHAR sziDrawID[MIN_STR] = L"";
	TCHAR szDialog[MAX_STR] = L"";

	wstring strName = L"";

	DIALOG* pDialog = nullptr;
	int iDrawID = 0;

	while (true)
	{
		fin.getline(szName, MIN_STR, '|');
		fin.getline(sziDrawID, MIN_STR, '|');
		fin.getline(szDialog, MAX_STR);

		if (fin.eof())
			break;

		pDialog = new DIALOG;
		pDialog->strName = szName;
		pDialog->iDrawID = _wtoi(sziDrawID);
		pDialog->strDialog = szDialog;

		m_vecDialog.emplace_back(pDialog);

		m_iMaxIndex++;
	}
	fin.close();
}

void CChapter2::ShowDialogBox(int iIndex)
{
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
		L"Chapter", 0, L"Dialog");
	NULL_CHECK(pTexInfo);

	float fCenterX = (float)pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = (float)pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY - fCenterY - 75.f, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_fLength >= m_vecDialog[iIndex]->strDialog.length())
	{
		const TEX_INFO* pTexInfo2 = CTextureMgr::GetInstance()->GetTexInfo(
			L"Chapter", 2, L"Dialog");
		NULL_CHECK(pTexInfo2);

		fCenterX = (float)pTexInfo2->tImgInfo.Width * 0.5f;
		fCenterY = (float)pTexInfo2->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, pTexInfo->tImgInfo.Width - 10.f, (WINCY - fCenterY - 75.f), 0.f);
		matWorld = matScale * matTrans;

		CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo2->pTexture,
			nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CChapter2::ShowDialogText(int iIndex)
{
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 100, WINCY - 132.5f, 0.f);
	matWorld = matScale * matTrans;

	m_fLength += CTimeMgr::GetInstance()->GetDeltaTime() * 10.f;

	if (m_fLength >= m_vecDialog[iIndex]->strDialog.length())
		m_fLength = m_vecDialog[iIndex]->strDialog.length();

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetDialogFont()->DrawTextW(
		CGraphicDev::GetInstance()->GetSprite(), /* 다이렉트 폰트는 글자마다 이미지를 생성하여 출력한다. */
		m_vecDialog[iIndex]->strDialog.c_str(), /* 출력할 문자열 */
		(size_t)m_fLength, /* 문자열 길이 */
		nullptr, /* RECT 주소 */
		0, /* 가운데 정렬 등과 같은 속성 값 */
		D3DCOLOR_ARGB(255, 255, 255, 255) /* 폰트 색상 */);
}

void CChapter2::ShowDialogBar()
{
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
		L"Chapter", 3, L"Dialog");
	NULL_CHECK(pTexInfo);

	float fCenterX = (float)pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = (float)pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY - m_fStart, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, m_fStart, 0.f);
	matWorld = matScale * matTrans;

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CChapter2::MovePlayer(int iIndex)
{

}

CChapter2 * CChapter2::Create()
{
	CChapter2* pInstance = new CChapter2;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
