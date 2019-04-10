
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

class CFileAgentDoc;

class CFileAgentView : public CView
{
protected: // serialization에서만 만들어집니다.
	CFileAgentView() noexcept;
	DECLARE_DYNCREATE(CFileAgentView)

// 특성입니다.
public:
	CFileAgentDoc* GetDocument() const;
	void AddItem(WIN32_FIND_DATA& file);
	void DeleteItem(char * fileName);
	void ClearItem();
	int GetItemSize();
	int GetListSize();
	void SetListSize(int listSize);
	DWORD GenerateShowNumber();
	DWORD GetShowNumber();
	void SetItemCountEx(int count = -1);
	
	CListCtrl fileCListCtrl;
	
	CEdit iPAddressCEdit;
	CEdit portCEdit;
	CEdit dirCEdit;

	CStatic iPAddresscStatic;
	CStatic portStatic;
	CStatic dirStatic;

	CButton connectBtn;
	CButton exploreBtn;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CFileAgentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConeectBtnClicked();
	afx_msg void OnExploreBtnClicked();
	afx_msg void OnItemDblclked(NMHDR* pNMHDR, LRESULT* pResult);
	void DeleteFileRequest();
	void RenameFileRequest();
	afx_msg void OnListKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int listSize = 0;
	DWORD showNumber = 0;

	CString ip;
	CString port;
	CString dir;

	char pCharIp[15];
	u_short uShortPort;
	char pCharDir[MAX_PATH];
	char pFileName[MAX_PATH];
	
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