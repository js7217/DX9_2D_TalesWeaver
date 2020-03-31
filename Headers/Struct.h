#pragma once

typedef struct tagUnit
{
#ifdef _AFX
	CString strObjKey; // 오브젝트 키 -> 오브젝트 키
#else
	wstring strObjKey;
#endif
#ifdef _AFX
	CString strName; // 캐릭터 이름 -> 스테이트키
#else
	wstring strName;
#endif
	int		iAtt;	// 캐릭터 공격력
	int		iHp;	// 캐릭터 체력
	int		iMaxHp; // 캐릭터 총체력
	int		iMP;	// 캐릭터 마력
}UNIT_DATA;

typedef struct tagTexInfo
{
	// 이미지 한장을 제어하기 위한 Com객체의 포인터
	LPDIRECT3DTEXTURE9	pTexture;

	// 이미지 한장의 정보를 저장할 구조체
	D3DXIMAGE_INFO		tImgInfo;
}TEX_INFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXMATRIX	matWorld;
	BYTE		byDrawID;
	BYTE		byOption;
	int			iMyIndex;
	int			iParentIndex;
}TILE;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;
	D3DXMATRIX	matWorld;
	float		fCX;
	float		fCY;
}INFO;

typedef struct tagFrame
{
	float	fCurFrame;
	float	fMaxFrame;
}FRAME;

typedef struct tagImagePath
{
	int		iImgCount = 0;
	wstring wstrObjKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrRelativePath = L"";
	wstring wstrDirectionKey = L"";
}IMG_PATH;

typedef struct tagDialog
{
#ifdef _AFX
	CString strName; // 대화하는 자의 이름
#else
	wstring strName;
#endif
	int iDrawID; // 플레이어 전용 드로우 아이디.
#ifdef _AFX
	CString strDialog; // 대화글
#else
	wstring strDialog;
#endif
}DIALOG;

typedef struct tagItem
{
#ifdef _AFX
	CString strName;
#else
	wstring strName; // 아이템 이름
#endif
	ITEM_TYPE eType; // 아이템 종류
	ITEM_LOCATION eLocation; // 아이템 위치.
	int iValue; // 아이템 수치
	int iCount = 1; // 아이템 갯수
	int iPrice = 0;
}ITEM;