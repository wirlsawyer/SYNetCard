// SYNetCardDemoDlg.h : header file
//

#pragma once


// CSYNetCardDemoDlg dialog
class CSYNetCardDemoDlg : public CDialog
{
// Construction
public:
	CSYNetCardDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SYNETCARDDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListBox *_listBox1;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
