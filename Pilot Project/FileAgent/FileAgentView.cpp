
// FileAgentView.cpp: CFileAgentView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "FileAgent.h"
#endif

#include <algorithm>

#include "FileAgentDoc.h"
#include "FileAgentView.h"
#include "FileAgentSocket.h"

#include "resource.h"  
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
	ON_NOTIFY(LVN_BEGINDRAG, FILECLISTCTRL_ID, &CFileAgentView::OnBeginDrag)
	ON_NOTIFY(TVN_SELCHANGED, FILECTREECTRL_ID, &CFileAgentView::OnBeginDrag)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void makeTime(char *buf, FILETIME& fileTime);

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
			swprintf_s((wchar_t *)file.cFileName, MAX_PATH, L"%s", T2W(CString(newName).GetBuffer()));
			SetItemCountEx(listSize);	
			break;
		}
	}
}

void CFileAgentView::ClearItem()
{
	files.clear();
}

struct SortByNameAndAttribute
{
	inline bool operator() (const WIN32_FIND_DATA& struct1, const WIN32_FIND_DATA& struct2)
	{
		if (struct1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
			&& (struct2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}
		else if (struct2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
			&& (struct1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			return false;
		}
		else
		{
			if (StrCmpW(struct1.cFileName, struct2.cFileName) == 1)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
};

void CFileAgentView::SortItemsByNameAndAttribute()
{
	std::sort(files.begin(), files.end(), SortByNameAndAttribute());
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

char* CFileAgentView::GetPCharDir()
{
	return pCharDir;
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

void CFileAgentView::AddDrives(wchar_t* pLogicalDriveStrings, int type)
{
	fileCTreeCtrl.InsertItem(pLogicalDriveStrings, 0, 0, TVI_ROOT);
}

// CFileAgentView 메시지 처리기
int CFileAgentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	fileCListCtrl.Create( WS_VISIBLE | WS_BORDER | LVS_OWNERDATA | LVS_REPORT | LVS_EDITLABELS | LVS_ICON, CRect(180, 30, 180, 180), this, FILECLISTCTRL_ID);
	fileCTreeCtrl.Create(WS_VISIBLE | WS_BORDER, CRect(0, 30, 180, 180), this, FILECTREECTRL_ID);
	
	imgList.Create(16, 16, ILC_COLOR32, 2, 0);
	imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON1));
	imgList.Add(AfxGetApp()->LoadIconW(IDI_ICON2));
	fileCListCtrl.SetImageList(&imgList, LVSIL_SMALL);
	fileCTreeCtrl.SetImageList(&imgList, LVSIL_NORMAL);

	// @temp 
	/*HTREEITEM hItem = nullptr;
	hItem = fileCTreeCtrl.InsertItem(TEXT("바탕 화면"), 0, 0, TVI_ROOT);
	hItem = fileCTreeCtrl.InsertItem(TEXT("내 문서"), 0, 5, hItem);
	fileCTreeCtrl.InsertItem(TEXT("내 그림"), 2, 5, hItem);
	fileCTreeCtrl.InsertItem(TEXT("내 비디오"), 3, 5, hItem);
	hItem = fileCTreeCtrl.InsertItem(TEXT("Sample folder"), 4, 5, hItem);
	hItem = fileCTreeCtrl.InsertItem(TEXT("Sub folder"), 4, 5, hItem);
	
	fileCTreeCtrl.EnsureVisible(hItem);*/

	// setting column name
	stringTableValue.LoadStringW(FILE_CLIENT_COLUMN_NAME);
	fileCListCtrl.InsertColumn(nameColumnPos, stringTableValue, LVCF_TEXT, 200);

	stringTableValue.LoadStringW(FILE_CLIENT_COLUMN_FIXED_DATE);
	fileCListCtrl.InsertColumn(fixedDateColumnPos, stringTableValue, LVCF_TEXT, 130);

	stringTableValue.LoadStringW(FILE_CLIENT_COLUMN_MADE_DATE);
	fileCListCtrl.InsertColumn(madeDateColumnPos, stringTableValue, LVCF_TEXT, 130);

	stringTableValue.LoadStringW(FILE_CLIENT_COLUMN_ATTRIBUTE);
	fileCListCtrl.InsertColumn(attributeDateColumnPos, stringTableValue, LVCF_TEXT, 130);

	stringTableValue.LoadStringW(FILE_CLIENT_COLUMN_FILE_SIZE);
	fileCListCtrl.InsertColumn(fileSizeColumnPos, stringTableValue, LVCF_TEXT | LVCFMT_RIGHT, 130);

	stringTableValue.LoadStringW(FILE_CLIENT_IP_LABEL);
	iPAddresscStatic.Create(stringTableValue, WS_VISIBLE, CRect(0, 0, 30, 20), this, IPADDRESSCEDIT_ID);
	iPAddressCEdit.Create(WS_BORDER | WS_VISIBLE, CRect(30, 0, 150, 20), this, IPADDRESSSTATIC_ID);

	stringTableValue.LoadStringW(FILE_CLIENT_PORT_LABEL);
	portStatic.Create(stringTableValue, WS_VISIBLE, CRect(150, 0, 200, 20), this, PORTSTATIC_ID);
	portCEdit.Create(WS_BORDER | WS_VISIBLE, CRect(205, 0, 255, 20), this, PORTCEDIT_ID);

	stringTableValue.LoadStringW(FILE_CLIENT_DIR_LABEL);
	dirStatic.Create(stringTableValue, WS_VISIBLE, CRect(310, 0, 340, 20), this, DIRSTATIC_ID);
	dirCEdit.Create(WS_BORDER | WS_VISIBLE, CRect(340, 0, 1000, 20), this, DIRCEDIT_ID);

	// setting ip
	stringTableValue.LoadStringW(FILE_CLIENT_DEFAULT_IP);
	iPAddressCEdit.SetWindowTextW(stringTableValue);

	// setting port
	stringTableValue.LoadStringW(FILE_CLIENT_DEFAULT_PORT);
	portCEdit.SetWindowTextW(stringTableValue);

	// setting dir
	stringTableValue.LoadStringW(FILE_CLIENT_DEFAULT_DIR);
	dirCEdit.SetWindowTextW(stringTableValue);

	// setting button
	stringTableValue.LoadStringW(FILE_CLIENT_CONNECT_BUTTON);
	connectBtn.Create(stringTableValue, WS_VISIBLE, CRect(255, 0, 300, 20), this, CONNECTBTN_ID);

	stringTableValue.LoadStringW(FILE_CLIENT_EXPLORE_BUTTON);
	exploreBtn.Create(stringTableValue, WS_VISIBLE, CRect(1000, 0, 1050, 20), this, EXPLOREBTN_ID);

	return 0;
}

// Connect button handler
afx_msg void CFileAgentView::OnConeectBtnClicked()
{
	SetItemCountEx(0);
	files.clear();
	fileCTreeCtrl.DeleteAllItems();

	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	
	iPAddressCEdit.GetWindowTextW(ip);
	
	portCEdit.GetWindowTextW(port);

	strcpy_s(pCharIp, CT2A(ip));
	_stscanf_s(port, _T("%hu"), &uShortPort);

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
		stringTableValue.LoadStringW(FILE_CLIENT_CONNECT_FAIL);
		AfxMessageBox(stringTableValue);
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

// #delete file
void CFileAgentView::DeleteFileRequest()
{
	POSITION pos = fileCListCtrl.GetFirstSelectedItemPosition();
	int nItem = fileCListCtrl.GetNextSelectedItem(pos);

	strcpy_s(pFileName, CT2A(fileCListCtrl.GetItemText(nItem, 0)));

	char attribute = _ttoi(fileCListCtrl.GetItemText(nItem, 3)) & 16;

	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	fileAgentSocket->Delete(pCharDir, pFileName, attribute);
}

// #rename file
void CFileAgentView::RenameFileRequest()
{
	POSITION pos = fileCListCtrl.GetFirstSelectedItemPosition();
	int nItem = fileCListCtrl.GetNextSelectedItem(pos);

	fileCListCtrl.EditLabel(nItem);
}

void CFileAgentView::OnBeginDrag(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;

	if (0 >= fileCListCtrl.GetSelectedCount())
	{
		return;
	}
	nSource = pNMLV->iItem;

	POINT ptImg;

	pDragImage = fileCListCtrl.CreateDragImage(nSource, &ptImg);

	if (pDragImage == nullptr)
	{
		return;
	}

	// standard point of image 
	CPoint ptSpot;

	ptSpot.x = pNMLV->ptAction.x - ptImg.x;
	ptSpot.y = pNMLV->ptAction.y - ptImg.y;
	pDragImage->BeginDrag(0, ptSpot);

	ptImg.x = pNMLV->ptAction.x;
	ptImg.y = pNMLV->ptAction.y;

	ClientToScreen(&ptImg);

	pDragImage->DragEnter(NULL, ptImg);

	nOldTarget = nSource;

	bDrag = true;

	SetCapture();

}

void CFileAgentView::OnTvnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);


}

int CFileAgentView::GetHitIndex(CPoint point)
{
	CRect rcList;

	fileCListCtrl.GetWindowRect(&rcList);
	ScreenToClient(reinterpret_cast<LPPOINT>(&rcList));

	LVHITTESTINFO HitInfo;

	HitInfo.pt.x = point.x - rcList.left;
	HitInfo.pt.y = point.y - rcList.top;

	return fileCListCtrl.HitTest(&HitInfo);
}

void CFileAgentView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (bDrag)
	{
		if (pDragImage == nullptr)
		{
			return;
		}

		// move drag image
		CPoint ptCursor(point);

		ClientToScreen(&ptCursor);
		pDragImage->DragMove(ptCursor);

		int nTarget = GetHitIndex(point);

		if (nTarget != -1)
		{
			pDragImage->DragLeave(NULL);
			fileCListCtrl.SetItemState(nTarget, LVIS_DROPHILITED, LVIS_DROPHILITED);

			if (nTarget != nOldTarget)
			{
				fileCListCtrl.SetItemState(nOldTarget, 0, LVIS_DROPHILITED);
				nOldTarget = nTarget;
				fileCListCtrl.UpdateWindow();
			}

			pDragImage->DragEnter(NULL, ptCursor);
		}
	}
	CView::OnMouseMove(nFlags, point);

}

void CFileAgentView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (bDrag)
	{
		if (pDragImage == nullptr)
		{
			return;
		}
		fileCListCtrl.SetItemState(nOldTarget, 0, LVIS_DROPHILITED);

		// end drag   
		pDragImage->DragLeave(NULL);
		ReleaseCapture();
		pDragImage->EndDrag();

		bDrag = false;
		delete pDragImage;
		pDragImage = nullptr;

		int nDest = GetHitIndex(point);
		if (nDest != -1) {
			CString cAttribute = fileCListCtrl.GetItemText(nDest, attributeDateColumnPos);
			DWORD Attribute = (DWORD)_ttoi((LPCTSTR)cAttribute);
			// #move file
			if (Attribute & FILE_ATTRIBUTE_DIRECTORY)
			{
				FileAgentSocket* fileAgentSocket = FileAgentSocket::GetInstance();

				strcpy_s(pFileName, CT2A(fileCListCtrl.GetItemText(nSource, nameColumnPos)));
				strcpy_s(pGoalDir, CT2A(fileCListCtrl.GetItemText(nDest, nameColumnPos)));
				fileAgentSocket->Move(pCharDir, pFileName, pGoalDir, files.at(nSource));
			}
		}
	}

	CView::OnLButtonUp(nFlags, point);
}

void CFileAgentView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (bDrag && nChar == VK_ESCAPE)
	{
		fileCListCtrl.SetItemState(nOldTarget, 0, LVIS_DROPHILITED);
		pDragImage->DragLeave(NULL);
		ReleaseCapture();
		pDragImage->EndDrag();

		bDrag = false;
		delete pDragImage;
		pDragImage = nullptr;
	}
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

	FileAgentSocket *fileAgentSocket = FileAgentSocket::GetInstance();
	fileAgentSocket->Rename(pCharDir, pFileOldName, pFileNewName);

	*pResult = 0;
}

LRESULT CFileAgentView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// @issue

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
		cx - rectCtl.left,            // w. Grow to fill horizontally
		cy - rectCtl.top, // h. Grow to fill vertically
		TRUE)
		;
	
	fileCTreeCtrl.GetWindowRect(&rectCtl);
	ScreenToClient(&rectCtl);

	fileCTreeCtrl.MoveWindow(
		rectCtl.left,                               // x. remains unchanged
		rectCtl.top,                                // y. remains unchanged
		rectCtl.right,            // w. Grow to fill horizontally
		cy - rectCtl.top, // h. Grow to fill vertically
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
#include <iostream>
void CFileAgentView::OnLvnGetdispinfoList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;

	int itemid = pItem->iItem;

	if (pItem->mask & LVIF_TEXT)
	{
		CString text;

		if (pItem->iSubItem == nameColumnPos)
		{
			text = CString(files[itemid].cFileName);
		}
		else if (pItem->iSubItem == fixedDateColumnPos)
		{
			char buffer[100];
			makeTime(buffer, files[itemid].ftLastWriteTime);
			text = CString(buffer);
		}
		else if (pItem->iSubItem == madeDateColumnPos)
		{
			char buffer[100];
			makeTime(buffer, files[itemid].ftCreationTime);
			text = CString(buffer);
		}
		else if (pItem->iSubItem == attributeDateColumnPos)
		{
			if (files[itemid].dwFileAttributes & 16)
			{ // 폴더
				text.Format(TEXT("%d"), files[itemid].dwFileAttributes);
			}
			else
			{
				text.Format(TEXT("%u"), files[itemid].dwFileAttributes);
			}

		}
		else if (pItem->iSubItem == fileSizeColumnPos)
		{
			ULONGLONG fileSize = (static_cast<ULONGLONG>(files[itemid].nFileSizeHigh) <<
				sizeof(files[itemid].nFileSizeLow) * 8) |
				files[itemid].nFileSizeLow;

			if(fileSize)
			{
				text.Format(TEXT("%lluKB"), fileSize /1024);
			}
			
		}

		//Copy the text to the LV_ITEM structure
		//Maximum number of characters is in pItem->cchTextMax
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
	}
	if (pItem->mask & LVIF_IMAGE)
	{
		if (files[itemid].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			pItem->iImage = 0;
		}
		else
		{
			pItem->iImage = 1;
		}
		
	}
}
