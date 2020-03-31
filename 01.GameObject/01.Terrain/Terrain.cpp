#include "stdafx.h"
#include "Terrain.h"
#include "ScrollMgr.h"
#include "Gate.h"
CTerrain::CTerrain()
	: m_pBackGround(nullptr)
	, m_fWidth(0.f)
	, m_fHeight(0.f)
	, m_bIsTileRender(false)
{
}

CTerrain::CTerrain(const wstring & strFilePath)
	: m_pBackGround(nullptr)
	, m_fWidth(0.f)
	, m_fHeight(0.f)
	, m_bIsTileRender(true)
	, m_strFilePath(strFilePath)
	, m_pKeyMgr(CKeyMgr::GetInstance())
{
}


CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize()
{
	FAILED_CHECK_RETURN(LoadTile(m_strFilePath), E_FAIL);
	FAILED_CHECK_RETURN(MakeGraph(), E_FAIL);

	m_eObjType = CGameObject::TERRAIN;
	m_eGroupID = GroupID::BACKGROUND;

	return S_OK;
}

HRESULT CTerrain::LateInit()
{
	return S_OK;
}

int CTerrain::Update()
{
	CGameObject::LateInit();
	KeyCheck();
	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
	CScrollMgr::ScrollLock(m_fWidth, m_fHeight);
}

void CTerrain::Render()
{
	if (nullptr == m_pBackGround)
		return;
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_pBackGround->vSize.x, m_pBackGround->vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_pBackGround->vPos.x - CScrollMgr::GetScroll(0),
		m_pBackGround->vPos.y - CScrollMgr::GetScroll(1), 0.f);

	m_pBackGround->matWorld = matScale * matTrans;

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"Map",
		m_pBackGround->byDrawID);
	NULL_CHECK(pTexInfo);

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&(m_pBackGround->matWorld));
	CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bIsTileRender)
	{
		int iCullX = int(CScrollMgr::GetScroll(0)) / TILECX;
		int iCullY = int(CScrollMgr::GetScroll(1)) / (TILECY / 2);

		int iCullEndX = iCullX + WINCX / TILECX;
		int iCullEndY = iCullY + WINCY / (TILECY / 2);

		int TileX = int(m_fWidth / TILECX);

		for (int i = iCullY; i < iCullEndY + 2; ++i)
		{
			for (int j = iCullX; j < iCullEndX + 2; ++j)
			{
				int iIndex = j + TileX * i;

				if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
					continue;

				D3DXMatrixScaling(&matScale, m_vecTile[iIndex]->vSize.x, m_vecTile[iIndex]->vSize.y, 0.f);
				D3DXMatrixTranslation(&matTrans,
					m_vecTile[iIndex]->vPos.x - CScrollMgr::GetScroll(0),
					m_vecTile[iIndex]->vPos.y - CScrollMgr::GetScroll(1),
					0.f);

				m_vecTile[iIndex]->matWorld = matScale * matTrans;

				const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
					L"Terrain", m_vecTile[iIndex]->byDrawID, L"Tile");
				NULL_CHECK(pTexInfo);

				float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
				float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

				CGraphicDev::GetInstance()->GetSprite()->SetTransform(&(m_vecTile[iIndex]->matWorld));
				CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
					nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
}

void CTerrain::LineRender()
{

}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	SafeDelete(m_pBackGround);
}

HRESULT CTerrain::LoadTile(const wstring & strFilePath)
{
	HANDLE hFile = CreateFile(strFilePath.c_str(), GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"LoadTile Failed");
		return E_FAIL;
	}

	DWORD dwBytes = 0;
	TILE tTile = {};
	float fScrollX = 0.f;
	float fScrollY = 0.f;

	if (!m_vecTile.empty())
		Release();

	if (nullptr == m_pBackGround)
		m_pBackGround = new TILE;

	m_pBackGround->vPos.x = 0.f;
	m_pBackGround->vPos.y = 0.f;
	m_pBackGround->vSize = { 1.f, 1.f, 0.f };
	D3DXMatrixIdentity(&(m_pBackGround->matWorld));
	m_pBackGround->byDrawID = 0;
	m_pBackGround->byOption = 0;

	ReadFile(hFile, &(fScrollX), sizeof(float), &dwBytes, nullptr);
	ReadFile(hFile, &(fScrollY), sizeof(float), &dwBytes, nullptr);

	ReadFile(hFile, &(m_pBackGround->byDrawID), sizeof(BYTE), &dwBytes, nullptr);
	ReadFile(hFile, &(m_fWidth), sizeof(float), &dwBytes, nullptr);
	ReadFile(hFile, &(m_fHeight), sizeof(float), &dwBytes, nullptr);
	// 뒷배경의 DrawID부터 가로 세로 길이를 먼저 읽는다.
	while (true)
	{
		ReadFile(hFile, &tTile, sizeof(TILE), &dwBytes, nullptr); // 해당 맵의 타일을 가져옴.

		if (0 == dwBytes)
			break;

		m_vecTile.emplace_back(new TILE(tTile));
	}

	CloseHandle(hFile);

	int TileX = int(m_fWidth / TILECX);
	int TileY = int(m_fHeight / (TILECY * 0.5f));

	for (int i = 0; i < TileY; ++i)
	{
		for (int j = 0; j < TileX; ++j)
		{
			int iIndex = j + i * TileX;
			m_vecTile[iIndex]->vPos.x = TILECX * j + (i % 2) * (TILECX * 0.5f);
			m_vecTile[iIndex]->vPos.y = (TILECY * 0.5f) * i;
			m_vecTile[iIndex]->vPos.z = 0.f;
			m_vecTile[iIndex]->vSize = { 1.f, 1.f, 0.f }; // 타일 재배치.

			if(2 == m_vecTile[iIndex]->byOption ||
				3 == m_vecTile[iIndex]->byOption || 
				4 == m_vecTile[iIndex]->byOption)
				CGate::Create(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y);

		}
	}
	return S_OK;
}

HRESULT CTerrain::MakeGraph()
{
	m_vecGraph.resize(m_vecTile.size());

	int TileX = int(m_fWidth / TILECX);
	int TileY = int(m_fHeight / (TILECY * 0.5f));

	for (size_t i = 0; i < TileY; ++i)
	{
		for (size_t j = 0; j < TileX; ++j)
		{
			size_t iIndex = j + TileX * i;

			if (iIndex >= m_vecGraph.size())
				continue;

			// 좌상
			if ((0 != i) && (0 != iIndex % (TileX * 2)))
			{
				if ((0 != i % 2) && (0 == m_vecTile[iIndex - TileX]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TileX]);
				else if ((0 == i % 2) && (0 == m_vecTile[iIndex - (TileX + 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TileX + 1)]);
			}

			// 우상
			if ((0 != i) && ((TileX * 2 - 1) != iIndex % (TileX * 2)))
			{
				if ((0 != i % 2) && (0 == m_vecTile[iIndex - (TileX - 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TileX - 1)]);
				else if ((0 == i % 2) && (0 == m_vecTile[iIndex - TileX]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TileX]);
			}

			// 좌하
			if ((TileY - 1 != i) && (0 != iIndex % (TileX * 2)))
			{
				if ((0 != i % 2) && (0 == m_vecTile[iIndex + TileX]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TileX]);
				else if ((0 == i % 2) && (0 == m_vecTile[iIndex + (TileX - 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TileX - 1)]);
			}

			// 우하
			if ((TileY - 1 != i) && ((TileX * 2 - 1) != iIndex % (TileX * 2)))
			{
				if ((0 != i % 2) && (0 == m_vecTile[iIndex + (TileX + 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TileX + 1)]);
				else if ((0 == i % 2) && (0 == m_vecTile[iIndex + TileX]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TileX]);
			}
		}
	}
	return S_OK;
}

void CTerrain::KeyCheck()
{
	if (m_pKeyMgr->KeyPressing(KEY_SPACE))
		m_bIsTileRender = true;
	else
		m_bIsTileRender = false;
}

CTerrain * CTerrain::Create(const wstring & strFilePath)
{
	CTerrain* pInstance = new CTerrain(strFilePath);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
