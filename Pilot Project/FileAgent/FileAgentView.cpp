
// FileAgentView.cpp: CFileAgentView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "FileAgent.h"
#endif

#include "FileAgentDoc.h"
#include "FileAgentView.h"
#include "FileAgentSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFileAgentView

IMPLEMENT_DYNCREATE(CFileAgentView, CView)

BEGIN_MESSAGE_MAP(CFileAgentView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(CONNECTBTN_ID, &CFileAgentView::OnConeectBtnClicked)
	ON_BN_CLICKED(EXPLOREBTN_ID, &CFileAgentView::OnExploreBtnClicked)
	ON_NOTIFY(LVN_GETDISPINFO, FILECLISTCTRL_ID, &CFileAgentView::OnLvnGetdispinfoList)
	ON_NOTIFY(NM_DBLCLK, FILECLISTCTRL_ID, &CFileAgentView::OnItemDblclked)
	ON_NOTIFY(LVN_KEYDOWN, FILECLISTCTRL_ID, &CFileAgentView::OnListKeyDown)
	ON_NOTIFY(LVN_ENDLABELEDIT, FILECLISTCTRL_ID, &CFileAgentView::OnEndLabelEdit)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CFileAgentView 생성/소멸

CFileAgentView::CFileAgentView() noexcept
{
	
}

CFileAgentView::~CFileAgentView()
{
}

BOOL CFileAgentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CFileAgentView 그리기

void CFileAgentView::OnDraw(CDC* /*pDC*/)
{
	CFileAgentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CFileAgentView 인쇄

BOOL CFileAgentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CFileAgentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CFileAgentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CFileAgentView 진단

#ifdef _DEBUG
void CFileAgentView::AssertValid() const
{
	CView::AssertValid();
}

void CFileAgentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFileAgentDoc* CFileAgentView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileAgentDoc)));
	return (CFileAgentDoc*)m_pDocument;
}
#endif //_DEBUG

void CFileAgentView::AddItem(WIN32_FIND_DATA& file)
{
	files.push_back(file);
}
#include <iostream>


void CFileAgentView::DeleteItem(char* fileName)
{
	int index = 0;
	char temp[MAX_PATH];
	for (WIN32_FIND_DATA& file : files) 
	{
		strcpy_s(temp, CT2A(file.cFileName));
		if (!strcmp(temp, fileName))
		{
			files.erase(files.begin() + index);
			break;
		}
		index++;
	}
}

void CFileAgentView::RenameItem(char* oldName, char* newName)
{
	char temp[MAX_PATH];

	for (WIN32_FIND_DATA& file : files)
	{
		strcpy_s(temp, CT2A(file.cFileName));
		if (!strcmp(temp, oldName))
		{	
			swprintf_s((wchar_t *)file.cFileName, MAX_PATH, L"%s", CString(newName));
			SetItemCountEx(listSize);	
			break;
		}
	}
}

void CFileAgentView::ClearItem()
{
	files.clear();
}

int CFileAgentView::GetItemSize()
{
	return files.size();
}

int CFileAgentView::GetListSize()
{
	return listSize;
}

void CFileAgentView::SetListSize(int listSize)
{
	this->listSize = listSize;
}

DWORD CFileAgentView::GenerateShowNumber()
{
	return ++showNumber;
}

DWORD CFileAgentView::GetShowNumber()
{
	return showNumber;
}

void CFileAgentView::SetItemCountEx(int count)
{
	if (count == -1) {
		fileCListCtrl.SetItemCountEx(files.size());
		fileCListCtrl.Invalidate();
	}
	else 
	{
		fileCListCtrl.SetItemCountEx(count);
	}
}
// CFileAgentView 메시지 처리기
int CFileAgentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	fileCListCtrl.Create(WS_BORDER | WS_VISIBLE | LVS_OWNERDATA | LVS_REPORT | LVS_EDITLABELS, CRect(30, 30, 180, 180), this, FILECLISTCTRL_ID);
	fileCListCtrl.InsertColumn(0, TEXT("이름"), LVCF_TEXT, 150);
	fileCListCtrl.InsertColumn(1, TEXT("수정한 날짜"), LVCF_TEXT, 130);
	fileCListCtrl.InsertColumn(1, TEXT("만든 날짜"), LVCF_TEXT, 130);
	fileCListCtrl.InsertColumn(1, TEXT("유형"), LVCF_TEXT, 130);
	/*fileCListCtrl.SetExtendedStyle(fileCListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);*/

	iPAddresscStatic.Create(TEXT("IP"), WS_VISIBLE, CRect(0, 0, 30, 20), this, IPADDRESSCEDIT_ID);
	iPAddressCEdit.Create(WS_BORDER | WS_VISIBLE, CRect(30, 0, 150, 20), this, IPADDRESSSTATIC_ID);

	portStatic.Create(TEXT("PORT"), WS_VISIBLE, CRect(150, 0, 200, 20), this, PORTSTATIC_ID);
	portCEdit.Create(WS_BORDER | WS_VISIBLE, CRect(205, 0, 255, 20), this, PORTCEDIT_ID);

	connectBtn.Create(TEXT("접속"), WS_VISIBLE, CRect(255, 0, 300, 20), this, CONNECTBTN_ID);

	dirStatic.Create(TEXT("DIR"), WS_VISIBLE, CRect(310, 0, 340, 20), this, DIRSTATIC_ID);
	dirCEdit.Create(WS_BORDER | WS_VISIBLE, CRect(340, 0, 1000, 20), this, DIRCEDIT_ID);

	iPAddressCEdit.SetWindowTextW(TEXT("127.0.0.1"));
	portCEdit.SetWindowTextW(TEXT("9030"));
	dirCEdit.SetWindowTextW(TEXT("C:\\"));
	//dirCEdit.SetWindowTextW(TEXT("C:\\Windows\\WinSxS\\"));

	exploreBtn.Create(TEXT("탐색"), WS_VISIBLE, CRect(1000, 0, 1050, 20), this, EXPLOREBTN_ID);

	return 0;
}

// Connect button handler
afx_msg void CFileAgentView::OnConeectBtnClicked()
{
	SetItemCountEx(0);
	files.clear();

	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	
	iPAddressCEdit.GetWindowTextW(ip);
	
	portCEdit.GetWindowTextW(port);

	strcpy_s(pCharIp, CT2A(ip));
	_stscanf_s(port, _T("%u"), &uShortPort);

	//WSAAsyncSelect(fileAgentSocket.GetSocket(), m_hWnd, 27001, FD_CONNECT);
	fileAgentSocket->Connect(pCharIp, uShortPort);

	dirCEdit.GetWindowTextW(dir);
	strcpy_s(pCharDir, CT2A(dir));

	fileAgentSocket->Subscribe(pCharDir);
}

afx_msg void CFileAgentView::OnExploreBtnClicked()
{
	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	if (fileAgentSocket->GetSocket() == INVALID_SOCKET)
	{
		AfxMessageBox(TEXT("서버와 연결되어 있지 않습니다."));
		return;
	}
	fileAgentSocket->UnSubscribe(pCharDir);
	SetItemCountEx(0);
	files.clear();

	dirCEdit.GetWindowTextW(dir);
	strcpy_s(pCharDir, CT2A(dir));

	fileAgentSocket->Subscribe(pCharDir);
}
// @issue
// List item dbclick handler
void CFileAgentView::OnItemDblclked(NMHDR * pNMHDR, LRESULT * pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Create a pointer to the item
	LV_ITEM* pItem = &(pDispInfo)->item;

	// @issue
	int itemid = pItem->iGroup;// iItem;

	CString sIndexValue;
	CString attr;
	sIndexValue = fileCListCtrl.GetItemText(itemid, 0);
	attr = fileCListCtrl.GetItemText(itemid, 3);
	std::wcout << (const wchar_t*)sIndexValue << std::endl;
	if ((DWORD)_ttoi((LPCTSTR)attr) & FILE_ATTRIBUTE_DIRECTORY && sIndexValue != CString("."))
	{
		FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
		fileAgentSocket->UnSubscribe(pCharDir);
		SetItemCountEx(0);
		files.clear();
		
		if (!sIndexValue.Compare(TEXT("..")))
		{
			int i = dir.GetLength() - 2;
			for (; i > 0; i--)
			{
				if (dir.GetAt(i) == CString("\\"))
					break;
			}
			if (dir.GetAt(i) != CString("\\"))
			{
				return;
			}
			dir = dir.Left(i+1);
			std::wcout << (const wchar_t*)dir << std::endl;
		}
		else
		{
			dir += sIndexValue + CString("\\");
		}
		
		dirCEdit.SetWindowTextW(dir);
		strcpy_s(pCharDir, CT2A(dir));
		fileAgentSocket->Subscribe(pCharDir);
	}
}

// key down
// select menu
void CFileAgentView::DeleteFileRequest()
{
	POSITION pos = fileCListCtrl.GetFirstSelectedItemPosition();
	int nItem = fileCListCtrl.GetNextSelectedItem(pos);

	strcpy_s(pFileName, CT2A(fileCListCtrl.GetItemText(nItem, 0)));

	char attribute = _ttoi(fileCListCtrl.GetItemText(nItem, 3)) & 16;

	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	fileAgentSocket->Delete(pCharDir, pFileName, attribute);
}

// key down
// select menu
void CFileAgentView::RenameFileRequest()
{
	POSITION pos = fileCListCtrl.GetFirstSelectedItemPosition();
	int nItem = fileCListCtrl.GetNextSelectedItem(pos);

	fileCListCtrl.EditLabel(nItem);
}

void CFileAgentView::OnListKeyDown(NMHDR * pNMHDR, LRESULT * pResult)
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	int key = pLVKeyDow->wVKey;
	if (key == VK_DELETE)
	{
		DeleteFileRequest();
	}
	else if(key == VK_F2)
	{
		RenameFileRequest();
	}
	
}

void CFileAgentView::OnEndLabelEdit(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	CString str;

	CEdit* pEdit = fileCListCtrl.GetEditControl();
	pEdit->GetWindowTextW(str);

	strcpy_s(pFileOldName, CT2A(fileCListCtrl.GetItemText(pDispInfo->item.iItem, 0)));
	strcpy_s(pFileNewName, CT2A(str));

	std::wcout << (const wchar_t *)str << std::endl;
	std::wcout << (const wchar_t *)fileCListCtrl.GetItemText(pDispInfo->item.iItem,0) << std::endl;

	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	fileAgentSocket->Rename(pCharDir, pFileOldName, pFileNewName);

	*pResult = 0;
}

LRESULT CFileAgentView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(message);
	if (message == 27001) // FD_CONNECT 
	{
		if (WSAGETSELECTERROR(lParam) == 0) 
		{ // 에러가 없다면 => 접속 성공을 의미
			//WSAAsyncSelect(fa_socket, m_hWnd, 27002, FD_READ | FD_CLOSE);
			dirCEdit.GetWindowTextW(dir);
			strcpy_s(pCharDir, CT2A(dir));

			//FileAgentSocket::GetInstance().Subscribe(pCharDir, *this);

		}
		else 
		{ 
			/*closesocket(fa_socket);
			fa_socket = INVALID_SOCKET;
			AddItem(TEXT("서버에 접속할 수 없습니다!"));*/
		}
	}
	else if( message == 27002)
	{
		if (WSAGETSELECTEVENT(lParam) == FD_READ) 
		{
			//AddItem(TEXT("전달"));
		}
		else if (WSAGETSELECTEVENT(lParam))
		{
			//AddItem(TEXT("서버와의 연결 해제"));
		}
		
	}
	else if ( message == 20000)
	{
		std::cout << "??" << lParam << std::endl;
		std::cout << (char *)wParam << std::endl;
		//fileCListCtrl.SetItemText(lParam, 0, TEXT("!!!"));
	}

	return CView::WindowProc(message, wParam, lParam);
}


void CFileAgentView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rectCtl;
	fileCListCtrl.GetWindowRect(&rectCtl);
	ScreenToClient(&rectCtl);


	fileCListCtrl.MoveWindow(
		rectCtl.left,                               // x. remains unchanged
		rectCtl.top,                                // y. remains unchanged
		cx - 2 * rectCtl.left,            // w. Grow to fill horizontally
		cy - rectCtl.top - rectCtl.left, // h. Grow to fill vertically
		TRUE)
		;
}

void makeTime(char *buf, FILETIME& fileTime)
{
	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&fileTime, &systemTime);
	sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
}

void CFileAgentView::OnLvnGetdispinfoList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Create a pointer to the item
	LV_ITEM* pItem = &(pDispInfo)->item;

	//Which item number?
	int itemid = pItem->iItem;

	//Do the list need text information?
	if (pItem->mask & LVIF_TEXT)
	{
		CString text;
		
		//Which column?
		if (pItem->iSubItem == 0)
		{
			//Text is name
			//std::wcout << (const wchar_t *)files[itemid].cFileName << std::endl;
			text = CString(files[itemid].cFileName);
		}
		else if (pItem->iSubItem == 1)
		{
			//Text is slogan
			char buffer[100];
			makeTime(buffer, files[itemid].ftLastWriteTime);
			text = CString(buffer);
		}
		else if (pItem->iSubItem == 2)
		{
			//Text is slogan
			char buffer[100];
			makeTime(buffer, files[itemid].ftCreationTime);
			text = CString(buffer);
		}
		else if (pItem->iSubItem == 3)
		{
			//Text is slogan
			files[itemid].dwFileAttributes;
			text.Format(TEXT("%u"),files[itemid].dwFileAttributes);
			
		}

		//Copy the text to the LV_ITEM structure
		//Maximum number of characters is in pItem->cchTextMax
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
	}
}
