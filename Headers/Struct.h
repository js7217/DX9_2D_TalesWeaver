#pragma once

typedef struct tagUnit
{
#ifdef _AFX
	CString strObjKey; // ������Ʈ Ű -> ������Ʈ Ű
#else
	wstring strObjKey;
#endif
#ifdef _AFX
	CString strName; // ĳ���� �̸� -> ������ƮŰ
#else
	wstring strName;
#endif
	int		iAtt;	// ĳ���� ���ݷ�
	int		iHp;	// ĳ���� ü��
	int		iMaxHp; // ĳ���� ��ü��
	int		iMP;	// ĳ���� ����
}UNIT_DATA;

typedef struct tagTexInfo
{
	// �̹��� ������ �����ϱ� ���� Com��ü�� ������
	LPDIRECT3DTEXTURE9	pTexture;

	// �̹��� ������ ������ ������ ����ü
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
	CString strName; // ��ȭ�ϴ� ���� �̸�
#else
	wstring strName;
#endif
	int iDrawID; // �÷��̾� ���� ��ο� ���̵�.
#ifdef _AFX
	CString strDialog; // ��ȭ��
#else
	wstring strDialog;
#endif
}DIALOG;

typedef struct tagItem
{
#ifdef _AFX
	CString strName;
#else
	wstring strName; // ������ �̸�
#endif
	ITEM_TYPE eType; // ������ ����
	ITEM_LOCATION eLocation; // ������ ��ġ.
	int iValue; // ������ ��ġ
	int iCount = 1; // ������ ����
	int iPrice = 0;
}ITEM;