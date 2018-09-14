
// Image_cutDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <string>
//#include "cv.h"
//#include "highgui.h"

// CImage_cutDlg 对话框
class CImage_cutDlg : public CDialogEx
{
// 构造
public:
	CImage_cutDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMAGE_CUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//CRect rect_pic;
	CRect rect_img;
	CRect rect_img_all;//duan
	CRect rect_window_ori;
	CRect rect_IMG_ori;
	CRect rect_PIC_ori;
	CImage image;
	CImage image_all;//duan
	bool PIC_empty,IMG_load;
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedCut();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CImage_cutDlg::ChangeSize(CWnd *pWnd, int cx, int cy);
	//void CImage_cutDlg::ChangeSizeOthers(CWnd *pWnd, int cx, int xy)
	void CImage_cutDlg::ShowImage(CImage img, CRect rect_img, int id);
    void CImage_cutDlg::ImgPatch(int n);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_hScr;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_vScr;

	CPoint m_StartPoint;//开始点  
	CPoint m_EndPoint;//结束点 
	CPoint point_ori;
	BOOL m_bLBDown,m_bRBDown;//鼠标是否按下 
	CPoint CurrentLoc;//duan


	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	CString SavePath;
	int max_size;
	int **inf;
	int img_width;
	int img_height;
	double num_patch;
	int count;
	int num_crop;
	//int flag_save;
	int show_index;
	int status; //duan 0--整张图，1--每组图

	int **line;
	int line_index;
	int *line_ind;
	int line_ind_index;
	int *label_index;
	int label_num;
	int flag_label;
	std::string *pos;
	int *pos_r,*pos_c;
	int H,W;
	void CImage_cutDlg::LabelSave();
	bool save;
	
	//afx_msg void OnBnClickedForward();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedBack();
	CStatic m_proc;
	CFont m_font,m_font1;
	//afx_msg void OnEnChangeEdit1();
	CEdit m_edit;
//	afx_msg void OnChangeEdit();
	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	int CImage_cutDlg::findmax(int a[],int len);
	int CImage_cutDlg::findmin(int a[],int len);
	int cx_max,cx_min,cy_max,cy_min;
//	CEdit m_edit_text;
	CEdit m_edit_text;
	afx_msg void OnStnClickedImg();
	afx_msg void OnStnClickedProc();
};
