﻿
// FileAgentView.h: CFileAgentView 클래스의 인터페이스
//

#ifndef FILEAGENTVIEW_H
#define FILEAGENTVIEW_H

#include <vector>

#define CONNECTBTN_ID 26001
#define PORTCEDIT_ID 26002
#define PORTSTATIC_ID 26003
#define IPADDRESSSTATIC_ID 26004
#define IPADDRESSCEDIT_ID 26005
#define FILECLISTCTRL_ID 26006
#define DIRSTATIC_ID 26007
#define DIRCEDIT_ID 26008
#define EXPLOREBTN_ID 26009
#define FILECTREECTRL_ID 26010

#define DIRECTORY_FILE_IMAGE_INDEX 0
#define NORMAL_FILE_IMAGE_INDEX 1

class CFileAgentDoc;

class CFileAgentView : public CView
{
public:
	CFileAgentDoc* GetDocument() const;
	void AddItem(WIN32_FIND_DATA& file);
	void DeleteItem(char * fileName);
	void RenameItem(char * oldName, char * newName);
	void ClearItem();
	void SortItemsByNameAndAttribute();
	int GetItemSize();
	int GetListSize();
	void SetListSize(int listSize);
	DWORD GenerateShowNumber();
	DWORD GetShowNumber();
	char* GetPCharDir();
	void SetItemCountEx(int count = -1);
	void AddDrives(wchar_t * pLogicalDriveStrings, int type);
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CFileAgentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConeectBtnClicked();
	afx_msg void OnExploreBtnClicked();
	afx_msg void OnItemDblclked(NMHDR* pNMHDR, LRESULT* pResult);
	int CountTreeItems(HTREEITEM hItem, BOOL Recurse);
	void DelTreeItems(HTREEITEM hItem);
	void OnTreeItemDblclked(NMHDR * pNMHDR, LRESULT * pResult);
	void DeleteFileRequest();
	void RenameFileRequest();
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelChangingTree(NMHDR * pNMHDR, LRESULT * pResult);
	int GetHitIndex(CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnListKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult);

protected: 
	CFileAgentView() noexcept;
	DECLARE_DYNCREATE(CFileAgentView)
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl fileCListCtrl;
	CTreeCtrl fileCTreeCtrl;
	CImageList imgList;
	CImageList *pDragImage;

	CEdit iPAddressCEdit;
	CEdit portCEdit;
	CEdit dirCEdit;

	CStatic iPAddresscStatic;
	CStatic portStatic;
	CStatic dirStatic;

	CButton connectBtn;
	CButton exploreBtn;

	bool bDrag = false;        
	int         nOldTarget = -1;   
	int         nSource = -1;      

	int listSize = -1;
	DWORD showNumber = 0;
	
	int nameColumnPos = 0;
	int fixedDateColumnPos = 1;
	int madeDateColumnPos = 2;
	int attributeDateColumnPos = 3;
	int fileSizeColumnPos = 4;

	CString ip;
	CString port;
	CString dir;
	CString stringTableValue;

	char pCharIp[15];
	u_short uShortPort;
	char pCharDir[MAX_PATH];
	char pFileName[MAX_PATH];
	char pFileOldName[MAX_PATH];
	char pFileNewName[MAX_PATH];
	char pGoalDir[MAX_PATH];

	char buffer[1024];
	
	int sendBytes;

	std::vector<WIN32_FIND_DATA> files;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // FileAgentView.cpp의 디버그 버전
inline CFileAgentDoc* CFileAgentView::GetDocument() const
   { return reinterpret_cast<CFileAgentDoc*>(m_pDocument); }
#endif

#endif