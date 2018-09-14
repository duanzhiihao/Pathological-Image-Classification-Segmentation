
// Image_cutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_cut.h"
#include "Image_cutDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <direct.h>  
#include <windows.h>
#include <fstream>
#include <string>

using namespace std;
//using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImage_cutDlg 对话框




CImage_cutDlg::CImage_cutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImage_cutDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage_cutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLL_H, m_hScr);
	DDX_Control(pDX, IDC_SCROLL_V, m_vScr);
	DDX_Control(pDX, IDC_PROC, m_proc);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	//  DDX_Control(pDX, IDC_TEXT, m_edit_text);
	DDX_Control(pDX, IDC_TEXT, m_edit_text);
}

BEGIN_MESSAGE_MAP(CImage_cutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CImage_cutDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_CUT, &CImage_cutDlg::OnBnClickedCut)
	ON_WM_SIZE()
//	ON_STN_CLICKED(IDC_IMG, &CImage_cutDlg::OnStnClickedImg)
//ON_BN_CLICKED(IDOK, &CImage_cutDlg::OnBnClickedOk)
ON_WM_WINDOWPOSCHANGING()
//ON_WM_MOUSEHWHEEL()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEWHEEL()
//ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CImage_cutDlg::OnNMThemeChangedScrollbar1)
//ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &CImage_cutDlg::OnNMThemeChangedScrollbar2)
ON_WM_HSCROLL()
ON_WM_VSCROLL()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()

ON_BN_CLICKED(IDC_NEXT, &CImage_cutDlg::OnBnClickedNext)
ON_BN_CLICKED(IDC_BACK, &CImage_cutDlg::OnBnClickedBack)
//ON_EN_CHANGE(IDC_EDIT1, &CImage_cutDlg::OnEnChangeEdit1)
//ON_EN_CHANGE(IDC_EDIT, &CImage_cutDlg::OnChangeEdit)
ON_BN_CLICKED(IDC_HELP, &CImage_cutDlg::OnBnClickedHelp)
ON_BN_CLICKED(IDC_BUTTON1, &CImage_cutDlg::OnBnClickedButton1)
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_STN_CLICKED(IDC_IMG, &CImage_cutDlg::OnStnClickedImg)
ON_STN_CLICKED(IDC_PROC, &CImage_cutDlg::OnStnClickedProc)
END_MESSAGE_MAP()


// CImage_cutDlg 消息处理程序

BOOL CImage_cutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetClientRect(&rect_window_ori);
	CWnd *pWnd_IMG;   
    pWnd_IMG = GetDlgItem(IDC_IMG);
	pWnd_IMG->GetWindowRect(&rect_PIC_ori);
	ScreenToClient(&rect_PIC_ori);
	PIC_empty=0;
	IMG_load=0;
	m_bLBDown=0;
	m_bRBDown=0;
	SavePath="";
	GetDlgItem(IDC_SCROLL_H)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_SCROLL_V)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_IMG)->ShowWindow(SW_HIDE);
	max_size=16000*16000;
	count=1;
	flag_label=0;
	line_index=0;
	line_ind_index=0;
	save=0;

    m_font.CreatePointFont(100, "Arial", NULL);  
    m_proc.SetFont(&m_font,true);  
    m_proc.SetWindowText("进度");  
	m_font1.CreatePointFont(130, "Arial", NULL);  
    m_edit.SetFont(&m_font1,true);  
    m_edit.SetWindowText(""); 
	m_edit_text.SetFont(&m_font,true);
	m_edit_text.SetWindowText("");
	//m_font1.CreatePointFont(300,_T("华文行楷"));

	//PIC_empty=1;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImage_cutDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImage_cutDlg::OnPaint()
{	/*	
	if (m_bLBDown)  
    {   
		ShowImage(image,rect_img,IDC_IMG);
		CPaintDC dc(this);
        CRect rect(m_StartPoint, m_EndPoint);  
  
        rect.NormalizeRect();//规范化矩形   
  
        CBrush bruPen(RGB(255, 0, 0));//矩形边框颜色  
  
        dc.FrameRect(&rect, &bruPen);  
    }
	*/
	if (m_bLBDown)  
    {     
		CWnd *pWnd = GetDlgItem(IDC_IMG);
        CDC* pDC = pWnd->GetDC();
		CRect rect;
		CPoint s,e;

		//HDC hDC = GetDC(hwnd);
		//CDC *pDC = GetDC();
		//ShowImage(image,rect_img,IDC_IMG);

		//ScreenToClient(&rect);
		//pWnd->GetClientRect(&rect);
		cout<<"s "<<m_StartPoint.x<<" "<<m_StartPoint.y<<endl;
		cout<<"e "<<m_EndPoint.x<<" "<<m_EndPoint.y<<endl;
		pWnd->GetWindowRect(&rect);//获取句柄指向控件区域的大小
		ScreenToClient(&rect);
		cout<<"s "<<m_StartPoint.x<<" "<<m_StartPoint.y<<endl;
		cout<<"e "<<m_EndPoint.x<<" "<<m_EndPoint.y<<endl;

		cout<<"l "<<rect.left<<" "<<rect.top<<endl;
		if (line_index!=label_index[label_num])
		{
			s.x=(double)(line[line_index-1][0]-rect_img.left)/rect_img.Width()*rect.Width();
			s.y=(double)(line[line_index-1][1]-rect_img.top)/rect_img.Height()*rect.Height();
			//s.x=m_StartPoint.x-rect.left;
			//s.y=m_StartPoint.y-rect.top;
			e.x=m_EndPoint.x-rect.left;
			e.y=m_EndPoint.y-rect.top;
			cout<<"ss "<<s.x<<" "<<s.y<<endl;
			cout<<"ee "<<e.x<<" "<<e.y<<endl;
			CPen redpen;
			redpen.CreatePen(PS_SOLID,2,RGB(255,0,0));
			pDC->SelectObject(redpen);
			pDC->MoveTo(s);
			pDC->LineTo(e);
			//pDC->DeleteDC();
			pWnd->ReleaseDC(pDC);
			redpen.DeleteObject();
			cout<<"s "<<m_StartPoint.x<<" "<<m_StartPoint.y<<endl;
			cout<<"e "<<m_EndPoint.x<<" "<<m_EndPoint.y<<endl;
		}
		return;
    }  

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//{ShowImage(image,rect_img,IDC_IMG);}
	}
	if (PIC_empty!=1||IMG_load!=1)
		return;
	CRect rect,rect_resize;
	int p_H,p_W;
	CWnd *pWnd = NULL;  
	pWnd = GetDlgItem(IDC_IMG);
	pWnd->GetWindowRect(&rect);//获取句柄指向控件区域的大小
	ScreenToClient(&rect);
	double img_height=rect_img.Height();
	double img_width=rect_img.Width();
	double PIC_height=rect.Height();
	double PIC_width=rect.Width();

	//if (img_height<PIC_height&&img_width<PIC_height)
	//{
	//	return;
	//}
	//else
	
	cout<<img_width/img_height<<endl;
	cout<<PIC_width/PIC_height<<endl;
	if (img_width/img_height>=PIC_width/PIC_height)
	{
		rect_resize.left=rect.left;
		rect_resize.right=rect.right;
		p_H=PIC_width/img_width*img_height;
		p_W=PIC_width;
		cout<<p_H<<endl;
		rect_resize.top=(PIC_height-p_H)/2+rect.top;
		rect_resize.bottom=rect_resize.top+p_H-1;
	}

	else
	{
		rect_resize.top=rect.top;
		rect_resize.bottom=rect.bottom;
		p_W=PIC_height/img_height*img_width;
		p_H=PIC_height;
		rect_resize.left=(PIC_width-p_W)/2+rect.left;
		rect_resize.right=rect_resize.left+p_W-1;
	}

	CWnd *pWnd_hscr;
	CRect rect_hscr;
	pWnd_hscr=GetDlgItem(IDC_SCROLL_H);
	rect_hscr.left=rect_resize.left;
	rect_hscr.right=rect_resize.right;
	rect_hscr.top=rect_resize.bottom+1;
	rect_hscr.bottom=rect_resize.bottom+26;
	pWnd_hscr->MoveWindow(rect_hscr);
	CWnd *pWnd_vscr;
	CRect rect_vscr;
	pWnd_vscr=GetDlgItem(IDC_SCROLL_V);
	rect_vscr.left=rect_resize.right+1;
	rect_vscr.right=rect_resize.right+26;
	rect_vscr.top=rect_resize.top;
	rect_vscr.bottom=rect_resize.bottom;
	pWnd_vscr->MoveWindow(rect_vscr);
	pWnd->MoveWindow(rect_resize);
	pWnd->MoveWindow(rect_resize);
	//ShowImage(image,rect_img,IDC_IMG); //duan

	//pWnd->Detach();
}

HCURSOR CImage_cutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImage_cutDlg::OnBnClickedOpen()
{
	//对image初始化
	if (!image.IsNull())
	{
		//image.Detach();
		image.~CImage();
		cout<<"image destroy  "<<image.IsNull()<<endl;
		SavePath="";
	}
	//duan
	if (!image_all.IsNull())
	{
		//image.Detach();
		image_all.~CImage();
		cout << "image_all destroy  " << image_all.IsNull() << endl;
		SavePath = "";
	}

	count=1;
	PIC_empty=0;
	IMG_load=0;
	if (flag_label!=0)
	{		
		line_index=0;
		line_ind_index=0;
		label_num=0;
		delete []line_ind;
		delete []line;
		delete []label_index;
		save=0;
		flag_label=0;
	}
	GetDlgItem(IDC_IMG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SCROLL_H)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_SCROLL_V)->ShowWindow(SW_HIDE);
	m_edit.SetWindowText(""); 
	m_edit_text.SetWindowText("");
	//num_crop=1;
	//OnInitDialog();
	AllocConsole();
	freopen("CONOUT$","w+t",stdout);
	CString Img_path,Img_folder,File_path,file_name,f_SavePath;
	//cout<<image.IsNull()<<endl;

	//CRect rect,rect_resize;
	
	//浏览文件打开图片对话框
	CFileDialog dlg(TRUE, _T("*.jpg"), NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,"Image Files(*.jpg)|*.jpg", NULL);
	dlg.m_ofn.lpstrTitle=_T("打开图片");
	if(dlg.DoModal()!=IDOK) return;
	EnableWindow(FALSE);
	Img_folder=dlg.GetFolderPath();
	SavePath=Img_folder;
	Img_path = dlg.GetPathName();
	file_name=Img_path.Right(Img_path.GetLength()-1-Img_path.ReverseFind('\\'));
	f_SavePath=Img_folder.Left(Img_folder.ReverseFind('\\'));
	
	//把scan.txt扫描一遍，记录有n_line行
	ifstream readfile;
	string temp;
	int n_line=0;
	readfile.open(f_SavePath+"\\scan.txt",ios::in);
	while(getline(readfile,temp))
	{
		//cout<<temp<<endl;
		n_line++;
	}
	readfile.close();
	//cout<<n_line<<endl;


	cout<<f_SavePath<<endl;
	cout<<file_name<<endl;
	int num1=atoi(file_name.Mid(3,3));
	int num2=atoi(file_name.Mid(7,3));
	//cout<<atoi("a")<<endl;
	//cout<<num1<<" "<<num2<<endl;
	//cout<<file_name.Mid(3,3)<<endl;
	if (num1==0||num2==0||file_name.Left(3)!="IMG"||file_name[6]!='x')
	{
		MessageBox("请打开正确的图片!","错误",MB_OK );
		EnableWindow(TRUE);
		return;
	}
	IMG_load=1;
    PIC_empty=1;
	
	
	//duan
	CString Img_path_all,folder_name;
	folder_name = f_SavePath.Right(f_SavePath.GetLength() - 1 - f_SavePath.ReverseFind('\\'));
	cout<<folder_name<<endl;
	Img_path_all = f_SavePath + "\\Thumbs\\Result-" + folder_name + ".JPG";
	cout<< Img_path_all<<endl;
	image_all.Load(_T(Img_path_all));

	//根据路径载入图片  
	CImage img;
	img.Load(_T(Img_path));

	img_width  = img.GetWidth();  
	img_height  = img.GetHeight(); 

	/*
    CImage testimg,testimg_1;
	cout<<"testimg  "<<testimg.IsNull()<<testimg_1.IsNull()<<endl;
	testimg.Create(16600,16860,24);
	cout<<"testimg  "<<testimg.IsNull()<<endl;
	testimg.Destroy();
	testimg_1.Create(16000,16000,24);
	cout<<"testimg_1  "<<testimg_1.IsNull()<<endl;
	//testimg.Destroy();
	*/
	
	//把images目录下的文件名打印一遍，并记录共有H*W个图片
	CFileFind ff=NULL;
	bool bf=ff.FindFile(Img_folder+"\\*.jpg");
	while(bf)
	{
		bf=ff.FindNextFileA();
		File_path=ff.GetFilePath();
		//cout<<File_path<<endl;
	}
	ff.Close();
	int len=File_path.GetLength();
	H=atoi(File_path.Mid(len-11,3));
	W=atoi(File_path.Mid(len-7,3));
	//rect_img.left=0;
	//rect_img.right=img_width*W-1;
	//rect_img.top=0;
	//rect_img.bottom=img_height*H-1;

	//把scan.txt中的[images]部分遍历了一遍，暂时没看懂是什么意思
	pos=new string[n_line];
	int n=0,n_pos=0;
	readfile.open(f_SavePath+"\\scan.txt",ios::in);
	CString str;
	int str_pos;
	pos_r=new int[W*H];
	pos_c=new int[W*H];
	while(getline(readfile,temp))
	{
		//pos[n]=temp;
		//cout<<pos[n]<<endl;
		
		if (n>27&&n<=27+W*H*2)
		{
			str=temp.c_str();
			if (n%2==0) pos_r[n_pos/2]=atoi(str.Right(str.GetLength()-str.ReverseFind('=')-1));
			else pos_c[n_pos/2]=atoi(str.Right(str.GetLength()-str.ReverseFind('=')-1));
			//cout<<pos_r[n_pos/2]<<" "<<pos_c[n_pos/2]<<endl;
			n_pos++;
		}
		n++;
	}
	readfile.close();

	//计算所有小图分为几组查看
	//long double size=img_width*W*img_height*H;
	cout<<"img_width="<<img_width<<" W="<<W<<" img_height="<<img_height<<" H="<<H<<endl;
	//size=10;
	//max_size=3;
	num_patch=(double)img_width*img_height/max_size*W*H;
	cout<<"num_patch="<<num_patch<<endl;
	int s=ceil(sqrt(num_patch)); //ceil -- 向上取整
	//所有小图分为s*s组合成查看
	num_patch=s*s;
	cout<<"s="<<s<<endl;
	//cout<<size<<endl;
	cout<<"num_patch="<<num_patch<<endl;
	int patch_H=ceil((double)H/s);
	int patch_W=ceil((double)W/s);
	int r_H=s*patch_H-H;
	int r_W=s*patch_W-W;
	int *pos_H=new int[s];
	int *pos_W=new int[s];
	pos_H[0]=1;
	pos_W[0]=1;
	for (int i=1;i<s;i++)
	{
		if (r_H>0)
		{
			pos_H[i]=pos_H[i-1]+patch_H-1;
			cout<<pos_H[i]<<endl;
			r_H--;
		}
		else
		{
			pos_H[i]=pos_H[i-1]+patch_H;
			cout<<pos_H[i]<<endl;
		}
		if (r_W>0)
		{
			pos_W[i]=pos_W[i-1]+patch_W-1;
			cout<<pos_W[i]<<endl;
			r_W--;
		}
		else
		{
			pos_W[i]=pos_W[i-1]+patch_W;
			cout<<pos_W[i]<<endl;
		}
	}

	//inf二维数组 记录了每组大图分别用哪些小图合成
	//inf的第一个索引i代表第i组合成图像
	//inf的第二个索引有0 1 2 3四种取值，分别代表第i组纵向由inf[i][0]到inf[i][1]，横向由inf[i][2]到inf[i][3]
	inf=new int*[(int)num_patch];
	for(int i=0;i<num_patch;i++)
	{	
		//cout<<i-(i/s)*s<<endl;
		inf[i]=new int[4];
		inf[i][0]=pos_H[i/s];
		inf[i][1]=inf[i][0]+patch_H-1;
		inf[i][2]=pos_W[i-(i/s)*s];
		inf[i][3]=inf[i][2]+patch_W-1;
		//cout<<inf[i][0]<<" "<<inf[i][1]<<" "<<inf[i][2]<<" "<<inf[i][3]<<endl;
	}
	delete []pos_H;
	delete []pos_W;
	
	/*
	cout<<"image  "<<image.IsNull()<<endl;
	//int N=
	image.Create(img_width*W,img_height*H,img.GetBPP());
	cout<<"image  "<<image.IsNull()<<endl;
	cout<<H<<"*"<<W<<endl;
	cout<<image.GetWidth()<<endl;
	cout<<image.GetHeight()<<endl;

	CFileFind f2=NULL;
	bool b2=f2.FindFile(Img_folder+"\\*.jpg");
	int num=0;
	while(b2)
	{
		num++;
		b2=f2.FindNextFileA();
		File_path=f2.GetFilePath();
		cout<<File_path<<endl;
		
		//file_path=LPCSTR(File_path);
		CImage img;
		img.Load(_T(File_path));
		int h=img.GetHeight();
		int w=img.GetWidth();
		int d=img.GetBPP();
		cout<<h<<endl;
		cout<<w<<endl;
		cout<<d<<endl;
		cout<<(num-1-((num-1)/W)*W)*w<<"   "<<((num-1)/W)*h<<endl;
		img.BitBlt(image.GetDC(),(num-1-((num-1)/W)*W)*w,((num-1)/W)*h,w,h,0,0,SRCCOPY);
		image.ReleaseDC();	
		//image.Detach();
		
		img.Destroy();
	}  
	f2.Close();
	*/
	//cout<<image.GetWidth()<<"  "<<image.GetHeight()<<endl;
	
	//打包第count组
	ImgPatch(count);
	//image.Save("C://test//patch1.png");
	
	//设置rect_img矩形参数，用来将image放进去
	rect_img.left=0;
	rect_img.right=image.GetWidth()-1;
	rect_img.top=0;
	rect_img.bottom=image.GetHeight()-1;
	cout<<rect_img.left<<" "<<rect_img.right<<" "<<rect_img.top<<" "<<rect_img.bottom<<endl;
	//duan
	rect_img_all.left = 0;
	rect_img_all.right = image_all.GetWidth() - 1;
	rect_img_all.top = 0;
	rect_img_all.bottom = image_all.GetHeight() - 1;
	//cout << rect_img.left << " " << rect_img.right << " " << rect_img.top << " " << rect_img.bottom << endl;

	//显示image or image_all
	CRect rect;
	GetClientRect(rect);
	int c_height=rect.Height();
	int c_width=rect.Width();
	CWnd *pWnd;
    pWnd = GetDlgItem(IDC_IMG);  
    ChangeSize(pWnd, c_width, c_height); 
	GetDlgItem(IDC_IMG)->ShowWindow(SW_SHOW);
    OnPaint();
	ShowImage(image_all, rect_img_all, IDC_IMG);//duan
	status = 0;

	//显示顶部进度编辑框
	CString c1,c2;
	c1.Format("%d",count);
	c2.Format("%d",(int)num_patch);
	CString text;
	text=c1+"/"+c2;
	//cout<<num_patch<<" "<<c2<<endl;
    //m_edit.SetWindowTextA(text);

	EnableWindow(TRUE);
	return;
}

//打包，n代表第n组合成图,此函数执行完后会生成第n组的合成图，存在image中
void CImage_cutDlg::ImgPatch(int n)
{
	cout<<"------------ImgPatch------------"<<endl;
	if (!image.IsNull())
	{
		image.Destroy();
	}
	int x1=inf[n-1][0];
	int x2=inf[n-1][1];
	int x3=inf[n-1][2];
	int x4=inf[n-1][3];
	int x=x2-x1+1;
	int y=x4-x3+1;
	//y*x个小图像合为一组，竖x个，横y个
	cout<<"x="<<x<<" y="<<y<<endl;
	//CImage img_patch;
	//cout<<image.IsNull()<<endl;
	//image.Create(img_width*y,img_height*x,24);
	//cout<<image.IsNull()<<endl;
	//cout<<image.GetWidth()<<" "<<image.GetHeight()<<endl;
	int num=0;
	CImage img1;
	int *coord_x,*coord_y;
	coord_x=new int [x*y];
	coord_y=new int [x*y];
	int n_line;
	while(num<x*y)
	{
		int pos_x=num-(num/y)*y+x3;
		int pos_y=num/y+x1;
		cout<<pos_y<<" "<<pos_x<<endl;
		n_line=(pos_y-1)*W+pos_x-1;
		coord_x[num]=pos_c[n_line];
		coord_y[num]=pos_r[n_line];
		cout<<coord_x[num]<<" "<<coord_y[num]<<endl;
		num++;
	}
	//cout<<findmax(coord_x,x*y)<<endl;
	//cout<<findmin(coord_x,x*y)<<endl;
	cx_max=findmax(coord_x,x*y);
	cx_min=findmin(coord_x,x*y);
	cy_max=findmax(coord_y,x*y);
	cy_min=findmin(coord_y,x*y);
	cout<<cx_min<<" "<<cx_max<<" "<<cy_min<<" "<<cy_max<<endl;
	cout<<img_width<<" "<<img_height<<endl;
	image.Create(cx_max+img_width-cx_min,cy_max+img_height-cy_min,24);
	cout<<image.GetHeight()<<" "<<image.GetWidth()<<endl;
	/*
	int aa[15];
	for (int i=-5;i<10;i++)
	{
		aa[i]=i;
		cout<<aa[i]<<endl;
	}
	cout<<findmax(aa,15);
	*/
	//把小图像(img1)一块一块拼到合成图像(image)上，每组执行x*y次
	num=0;
	while(num<x*y)
	{
		int pos_x=num-(num/y)*y+x3;
		int pos_y=num/y+x1;
		CString sx,sy,path;

		sx.Format("%.3d",pos_x);
		sy.Format("%.3d",pos_y);
		path=SavePath+"//IMG"+sy+"x"+sx+".JPG";
		//cout<<path<<endl;
		//CImage img1;
		img1.Load(_T(path));
		//cout<<img1.IsNull()<<endl;
		//img1.Save("C://test//patch_.png");
		int h=img1.GetHeight();
		int w=img1.GetWidth();
		int d=img1.GetBPP();
		//img1.BitBlt(image.GetDC(),(num-((num)/y)*y)*w,((num)/y)*h,w,h,0,0,SRCCOPY);
		img1.BitBlt(image.GetDC(),coord_x[num]-cx_min,coord_y[num]-cy_min,w,h,0,0,SRCCOPY);
		image.ReleaseDC();
		img1.Destroy();
		num++;
	}  
	//image=img_patch;
	cout<<image.GetWidth()<<" "<<image.GetHeight()<<endl;
	cout << "------------ImgPatch end------------" << endl;
	//image.Save("C://test//patch.png");
	//OnPaint();
	//return img_patch;
}


void CImage_cutDlg::ShowImage(CImage img, CRect rect_I, int id)
{			
	PIC_empty=0;
	CRect rect,rect_resize;
	int p_H,p_W;
	CWnd *pWnd = NULL;  
	pWnd = GetDlgItem(id);
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
	//ScreenToClient(&rect);
	double img_height=rect_I.Height();
	double img_width=rect_I.Width();
	//double PIC_height=rect.Height();
	//double PIC_width=rect.Width();
	//cout<<img_height<<" "<<img_width<<endl;
	//cout<<PIC_height<<" "<<PIC_width<<endl;
	/*
	//if (img_height<PIC_height&&img_width<PIC_height)
	//{
	//	return;
	//}
	//else
	{
		cout<<img_width/img_height<<endl;
		cout<<PIC_width/PIC_height<<endl;
		if (img_width/img_height>=PIC_width/PIC_height)
		{
			rect_resize.left=rect.left;
			rect_resize.right=rect.right;
			p_H=PIC_width/img_width*img_height;
			p_W=PIC_width;
			cout<<p_H<<endl;
			rect_resize.top=(PIC_height-p_H)/2+rect.top;
			rect_resize.bottom=rect_resize.top+p_H;
		}

		else
		{
			rect_resize.top=rect.top;
			rect_resize.bottom=rect.bottom;
			p_W=PIC_height/img_height*img_width;
			p_H=PIC_height;
			rect_resize.left=(PIC_width-p_W)/2+rect.left;
			rect_resize.right=rect_resize.left+p_W;
		}
		pWnd->MoveWindow(rect_resize);
		*/
		CDC *pDc = NULL;  
		pDc = pWnd->GetDC();//获取picture的DC 
		pDc->SetStretchBltMode(COLORONCOLOR);
		img.StretchBlt(pDc->m_hDC, rect, rect_I);//将图片绘制到picture表示的区域内  
		img.Detach();
		img.Destroy();
		ReleaseDC(pDc);  
		if (line_index==0) return;
		int nn=1;
		cout<<label_index[label_num]<<" "<<line_index<<endl;
		while(nn<=label_num&&label_index[nn]<=line_index)
		{
			for (int i=label_index[nn-1]+1;i<label_index[nn];i++)
			{
				CRect rect_for;
				CWnd *pWnd = GetDlgItem(IDC_IMG);
				pWnd->GetWindowRect(&rect_for);//获取句柄指向控件区域的大小
				ScreenToClient(&rect_for);
				int x1=line[i][0],x2=line[i+1][0];
				int y1=line[i][1],y2=line[i+1][1];
				if (x1<rect_img.left||x1>rect_img.right||x2<rect_img.left||x2>rect_img.right||y1<rect_img.top||y1>rect_img.bottom||y2<rect_img.top||y2>rect_img.bottom) continue;
				int x1_convert=(double)(x1-rect_img.left)/rect_img.Width()*rect_for.Width();
				int x2_convert=(double)(x2-rect_img.left)/rect_img.Width()*rect_for.Width();
				int y1_convert=(double)(y1-rect_img.top)/rect_img.Height()*rect_for.Height();
				int y2_convert=(double)(y2-rect_img.top)/rect_img.Height()*rect_for.Height();

				CDC* pDC = pWnd->GetDC();
				CPen redpen;
				redpen.CreatePen(PS_SOLID,2,RGB(255,0,0));
				pDC->SelectObject(redpen);
				pDC->MoveTo(x1_convert,y1_convert);
				pDC->LineTo(x2_convert,y2_convert);
				//pDC->DeleteDC();
				pWnd->ReleaseDC(pDC);
				redpen.DeleteObject();
			}
			nn++;
		}
}


void CImage_cutDlg::OnBnClickedCut()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!IMG_load)
	{
		MessageBox("先打开要处理的图片!","错误",MB_OK );
		return;
	}
	if (save==0) return;
	//EnableWindow(FALSE);
	CRect rect;
	int d=image.GetBPP();

	CImage img_save;
	//img_save.Create(100,100,d);
	m_StartPoint;//开始点  
	m_EndPoint;//结束点   
	//cout<<m_StartPoint.x<<"%%%"<<m_StartPoint.y<<endl;
	//cout<<m_EndPoint.x<<"%%%"<<m_EndPoint.y<<endl;
	CWnd *pWnd = NULL;  
	pWnd = GetDlgItem(IDC_IMG);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	//cout<<abs(-1)<<endl;
	/*
	int width=(double)abs(m_StartPoint.x-m_EndPoint.x)/rect.Width()*rect_img.Width();
	int height=(double)abs(m_StartPoint.y-m_EndPoint.y)/rect.Height()*rect_img.Height();
	//cout<<width<<"  "<<height<<endl;
	//cout<<rect.Width()<<"  "<<rect.Height()<<endl;
	//cout<<rect_img.Width()<<"  "<<rect_img.Height()<<endl;
	img_save.Create(width,height,d);
	if (img_save.IsNull())
	{
		MessageBox("截取区域太大，请分次截取!","错误",MB_OK );
		EnableWindow(TRUE);
		return;
	}

	CString sp,name,n_crop;
	sp=SavePath+"_crop";
	if(GetFileAttributes("D:\\MyProjects\\临时程序")==INVALID_FILE_ATTRIBUTES) mkdir(sp);
	CFileFind find=NULL;
	bool b=find.FindFile(sp+"\\*.png");
	int c=0;
	while(b)
	{
		b=find.FindNextFile();
		if(find.IsDots()||find.IsDirectory())   continue; 
        c++;       
	}
	//cout<<c<<endl;
	//cout<<sp<<endl;
	//cout<<sp+"_crop"<<endl;
	num_crop=c+1;
	n_crop.Format("%.3d",num_crop);
	name="Crop_"+n_crop+".png";
	//cout<<name<<endl;
	//cout<<sp<<endl;
	
	//int left=(double)(m_StartPoint.x-rect.left)/rect.Width()*rect_img.Width()+rect_img.left;
	//int top=(double)(m_StartPoint.y-rect.top)/rect.Height()*rect_img.Height()+rect_img.top;
	//cout<<left<<"    "<<top<<endl;
	image.BitBlt(img_save.GetDC(),0,0,width,height,(double)(m_StartPoint.x-rect.left)/rect.Width()*rect_img.Width()+rect_img.left,(double)(m_StartPoint.y-rect.top)/rect.Height()*rect_img.Height()+rect_img.top,SRCCOPY);
	cout<<sp+"//"+name<<endl;
	HRESULT hResult=img_save.Save(sp+"//"+name);
	cout<<hResult<<endl;
	if (FAILED(hResult))
	{
		MessageBox("截取区域太大，请分次截取!","错误",MB_OK );
		EnableWindow(TRUE);
		return;
	}
	img_save.ReleaseDC();
	//num_crop++;
	//img_save.Detach();
	//img_save.Destroy();
	EnableWindow(TRUE);
	*/
	CString sp,num;
	sp=SavePath+"_crop";
	if(GetFileAttributes(sp)==INVALID_FILE_ATTRIBUTES) mkdir(sp);
	CFileFind find=NULL;
	bool b=find.FindFile(sp+"\\*.txt");
	int c=0;
	while(b)
	{
		b=find.FindNextFile();
		if(find.IsDots()||find.IsDirectory())   continue; 
        c++;       
	}
	cout<<c<<endl;
	//cout<<sp<<endl;
	//cout<<sp+"_crop"<<endl;
	CString text;
	GetDlgItem(IDC_TEXT)->GetWindowTextA(text);
	cout<<text<<endl;
	num_crop=c/2+1;
	num.Format("%.3d",num_crop);
	ofstream fout,fout1;
	fout.open(sp+"//Crop_"+num+".txt");
	for (int i=label_index[label_num-1]+1;i<=label_index[label_num];i++)
	{
		fout<<line[i][0]+cx_min<<" "<<line[i][1]+cy_min<<endl;
		//delete []line[i];
	}
	fout.close();
	fout1.open(sp+"//Text_"+num+".txt");
	fout1<<text<<endl;
	fout1.close();
	//flag_label=0;
	cout<<cx_min<<" "<<cy_min<<endl;
	label_num++;
	label_index[label_num]=label_index[label_num-1]+1;
	m_edit_text.SetWindowTextA("");
	save=0;
	//num_crop++;
	return;
}


void CImage_cutDlg::OnSize(UINT nType, int cx, int cy)
{
	
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if(nType==1) return;//最小化则什么都不做  
    CWnd *pWnd;   
    pWnd = GetDlgItem(IDC_IMG);  
    ChangeSize(pWnd, cx, cy); 
	//GetClientRect(&rect_ori);
}

void CImage_cutDlg::ChangeSize(CWnd *pWnd, int cx, int cy)  
{  
	PIC_empty=1;
    if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
    {  
        CRect rect,r;   //获取控件变化前的大小   
		GetClientRect(r);
        rect.left=rect_PIC_ori.left*cx/rect_window_ori.Width();//调整控件大小  
        rect.right=rect_PIC_ori.right*cx/rect_window_ori.Width();  
        //rect.top=rect_PIC_ori.top*cy/rect_window_ori.Height();  
        //rect.bottom=rect_PIC_ori.bottom*cy/rect_window_ori.Height();  
		rect.top=rect.top+105;
		rect.bottom=rect.top+rect_PIC_ori.Height()*cy/rect_window_ori.Height()-1;
		if (rect.bottom>r.Height()-5)
		{
			rect.bottom=r.Height()-5;
		}
        pWnd->MoveWindow(rect);//设置控件大小  
			
    }  
} 





//void CImage_cutDlg::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
//}


void CImage_cutDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

      
    // TODO: Add your message handler code here  
  
    // 获取屏幕宽度  
    int nMaxX = GetSystemMetrics(SM_CXSCREEN);  
    // 获取屏幕高度  
    int nMaxY = GetSystemMetrics(SM_CYSCREEN);  
      
    // 判断窗口X坐标有无超过左边桌面。  
    if (lpwndpos->x < 0)  
    {  
		PIC_empty=1;
        //lpwndpos->x = 0;  
    }  
      
    // 判断窗口X坐标有无超过右边桌面。  
    if (lpwndpos->x + lpwndpos->cx > nMaxX)  
    {  
		PIC_empty=1;
       // lpwndpos->x = nMaxX - lpwndpos->cx;  
    }  
      
    // 判断窗口Y坐标有无超过顶部桌面。  
    if (lpwndpos->y < 0)  
    {  
		PIC_empty=1;
      //  lpwndpos->y = 0;  
    }  
      
    // 判断窗口Y坐标有无超过底部桌面。  
    if (lpwndpos->y + lpwndpos->cy > nMaxY)  
    {  
		PIC_empty=1;
       // lpwndpos->y = nMaxY - lpwndpos->cy;  
    }

}

void CImage_cutDlg::LabelSave()
{
	
}

BOOL CImage_cutDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//duan
	GetCursorPos(&CurrentLoc);
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMG);
	pWnd->GetClientRect(&rect);
	pWnd->ScreenToClient(&CurrentLoc);
	cout<<"---------------------"<<endl;
	int cx,cy,rr,rb;
	cx = CurrentLoc.x;
	cy = CurrentLoc.y;
	rr = rect.right;
	rb = rect.bottom;
	int s = ceil(sqrt(num_patch));
	//cout<<"s="<<s<<endl;
	cout<<"x="<<cx<<" y="<<cy<<endl;
	//cout<<"rect: "<<rect.left<<" "<<rr<<" "<<rect.top<<" "<<rb<<endl;
	if (cx<0 || cy<0 || cx>rr || cy>rb) {
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	}
	int blockx, blocky, numx, numy, num;
	if (status==0) {
		if (zDelta<0) {
			return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
		}
		blockx = rr/s + 1;
		blocky = rb/s + 1;
		numx = cx/blockx;
		numy = cy/blocky;
		num = numy*s + numx + 1;
		cout<<"count= "<<num<<endl;
		count = num;

		EnableWindow(FALSE);
		PIC_empty = 1;
		ImgPatch(count);
		rect_img.left = 0;
		rect_img.right = image.GetWidth()-1;
		rect_img.top = 0;
		rect_img.bottom = image.GetHeight()-1;
		cout<<rect_img.left<<" "<<rect_img.right<<" "<<rect_img.top<<" "<<rect_img.bottom<<endl;
		OnPaint();
		ShowImage(image, rect_img, IDC_IMG);
		CString c1, c2;
		c1.Format("%d", count);
		c2.Format("%d", (int)num_patch);
		CString text;
		text = c1+"/"+c2;
		m_edit.SetWindowTextA(text);
		m_edit_text.SetWindowTextA("");
		EnableWindow(TRUE);

		status = 1;

		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	}

	PIC_empty=1;
	int w=rect_img.Width();
	int h=rect_img.Height();
	int l=rect_img.left;
	int r=rect_img.right;
	int t=rect_img.top;
	int b=rect_img.bottom;
	int img_w=image.GetWidth();
	int img_h=image.GetHeight();
	
	//duan
	if (zDelta<0) {
		if (l==0 && t==0) {
			ShowImage(image_all, rect_img_all, IDC_IMG);
			status = 0;
			return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
		}
	}

	if (zDelta>0)
	{
		l=l+w*0.1;
		r=r-w*0.1;
		t=t+h*0.1;
		b=b-h*0.1;
		w=r-l+1;
		h=b-t+1;
	}
	else
	{
		l=l-w*0.2;
		r=r+w*0.2;
		t=t-h*0.2;
		b=b+h*0.2;
		if (l<0)
		{
			l=0;r=l+w*1.4-1;
			if (r>=img_w)
				r=img_w-1;
		}
		if (r>=img_w)
		{
			r=img_w-1;l=r-w*1.4+1;
			if (l<0)
				r=0;
		}
		if (t<0)
		{
			t=0;b=t+h*1.4-1;
			if (b>=img_h)
				b=img_h-1;
		}
		if (b>=img_h)
		{
			b=img_h-1;t=b-h*1.4+1;
			if (t<0)
				t=0;
		}
		w=r-l+1;
		h=b-t+1;
	}
	rect_img.left=l;
	rect_img.right=r;
	rect_img.top=t;
	rect_img.bottom=b;

	//rect_img.left=100;
	//rect_img.right=200;
	//rect_img.top=100;
	//rect_img.bottom=200;
	cout << l << endl;
	cout << r << endl;
	cout << t << endl;
	cout << b << endl;
	cout<<w<<"/"<<img_w<<endl;
	cout<<h<<"/"<<img_h<<endl;

	ShowImage(image,rect_img,IDC_IMG);
	if (img_w>w)
	{
		GetDlgItem(IDC_SCROLL_H)->ShowWindow(SW_SHOW);
    
		//cout<<img_w-w<<endl;
		m_hScr.SetScrollRange(0,img_w-w,0);
		m_hScr.SetScrollPos(l,1);
	}
	else
		{GetDlgItem(IDC_SCROLL_H)->ShowWindow(SW_HIDE);}
	if (img_h>h)
	{
		GetDlgItem(IDC_SCROLL_V)->ShowWindow(SW_SHOW);
		m_vScr.SetScrollRange(0,img_h-h,0);
		m_vScr.SetScrollPos(t,1);
	}
	else
		{GetDlgItem(IDC_SCROLL_V)->ShowWindow(SW_HIDE);}
	//if (img_h>h)
	//{m_ImgVScr.SetScrollRange(0,img_h-h,TRUE);}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CImage_cutDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos = m_hScr.GetScrollPos();    // 获?取¨?水?平?滚?动¡¥条¬?当Ì¡À前¡ã位?置?  
	  switch (nSBCode)    
	    {    
	        // 如¨?果?向¨°左Á¨®滚?动¡¥一°?列¢D，ê?则¨°pos减?1   
	       case SB_LINEUP:    
	            pos -= 100;    
	            break;    
	        // 如¨?果?向¨°右®¨°滚?动¡¥一°?列¢D，ê?则¨°pos加¨®1   
	       case SB_LINEDOWN:    
	            pos  += 100;    
	            break;    
	        // 如¨?果?向¨°左Á¨®滚?动¡¥一°?页°3，ê?则¨°pos减?10   
	        case SB_PAGEUP:    
	            pos -= 500;    
	            break;    
	        // 如¨?果?向¨°右®¨°滚?动¡¥一°?页°3，ê?则¨°pos加¨®10   
	        case SB_PAGEDOWN:    
	            pos  += 500;    
	            break;    
	        // 如¨?果?滚?动¡¥到Ì?最Á?左Á¨®端?，ê?则¨°pos为a1   
	        //case SB_TOP:    
	          //  pos = 1;    
	            //break;    
	        // 如¨?果?滚?动¡¥到Ì?最Á?右®¨°端?，ê?则¨°pos为a100   
	        //case SB_BOTTOM:    
	          //  pos = 100;    
	            //break;      
	        // 如¨?果?拖ª?动¡¥滚?动¡¥块¨¦滚?动¡¥到Ì?指?定¡§位?置?，ê?则¨°pos赋3值¦Ì为anPos的Ì?值¦Ì   
	        case SB_THUMBPOSITION:    
	            pos = nPos;    
	        break; 
			default:
				
			return;
	  }
	     
	  m_hScr.SetScrollPos(pos);   
	  //cout<<"pos "<<pos<<endl;

	  PIC_empty=1;
	int w=rect_img.Width();
	int h=rect_img.Height();
	int l=rect_img.left;
	int r=rect_img.right;
	int t=rect_img.top;
	int b=rect_img.bottom;
	int img_w=image.GetWidth();
	int img_h=image.GetHeight();

	l=pos;
	r=pos+w-1;
	//t=t;
	//b=b;
    if(r>=img_w)
	{
		r=img_w-1; l=r-w;
		if (l<0)
			l=0;
	}
	if (l<0)
	{
		l=0; r=l+w-1;
		if (r>=img_w)
			r=img_w-1;
	}
	//cout<<l<<"##"<<r<<endl;
	
	rect_img.left=l;
	rect_img.right=r;
	rect_img.top=t;
	rect_img.bottom=b;
	ShowImage(image,rect_img,IDC_IMG);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CImage_cutDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		int pos = m_vScr.GetScrollPos();    // 获?取¨?水?平?滚?动¡¥条¬?当Ì¡À前¡ã位?置?  
	  switch (nSBCode)    
	    {    
	        // 如¨?果?向¨°左Á¨®滚?动¡¥一°?列¢D，ê?则¨°pos减?1   
	       case SB_LINEUP:    
	            pos -= 100;    
	            break;    
	        // 如¨?果?向¨°右®¨°滚?动¡¥一°?列¢D，ê?则¨°pos加¨®1   
	       case SB_LINEDOWN:    
	            pos  += 100;    
	            break;    
	        // 如¨?果?向¨°左Á¨®滚?动¡¥一°?页°3，ê?则¨°pos减?10   
	        case SB_PAGEUP:    
	            pos -= 500;    
	            break;    
	        // 如¨?果?向¨°右®¨°滚?动¡¥一°?页°3，ê?则¨°pos加¨®10   
	        case SB_PAGEDOWN:    
	            pos  += 500;    
	            break;    
	        // 如¨?果?滚?动¡¥到Ì?最Á?左Á¨®端?，ê?则¨°pos为a1   
	        //case SB_TOP:    
	          //  pos = 1;    
	            //break;    
	        // 如¨?果?滚?动¡¥到Ì?最Á?右®¨°端?，ê?则¨°pos为a100   
	        //case SB_BOTTOM:    
	          //  pos = 100;    
	            //break;      
	        // 如¨?果?拖ª?动¡¥滚?动¡¥块¨¦滚?动¡¥到Ì?指?定¡§位?置?，ê?则¨°pos赋3值¦Ì为anPos的Ì?值¦Ì   
	        case SB_THUMBPOSITION:    
	            pos = nPos;    
	        break; 
			default:
				
			return;
	  }
	     
	  m_vScr.SetScrollPos(pos);   
	  //cout<<"pos "<<pos<<endl;

	  PIC_empty=1;
	int w=rect_img.Width();
	int h=rect_img.Height();
	int l=rect_img.left;
	int r=rect_img.right;
	int t=rect_img.top;
	int b=rect_img.bottom;
	int img_w=image.GetWidth();
	int img_h=image.GetHeight();

	//l=l;
	//r=r;
	t=pos;
	b=pos+h-1;
    if(b>=img_h)
	{
		b=img_h-1; t=b-h;
		if (t<0)
			t=0;
	}
	if (t<0)
	{
		t=0; b=t+h-1;
		if (b>=img_h)
			b=img_h-1;
	}
	//cout<<l<<"##"<<r<<endl;
	
	//rect_img.left=l;
	//rect_img.right=r;
	rect_img.top=t;
	rect_img.bottom=b;
	ShowImage(image,rect_img,IDC_IMG);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImage_cutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	if (!IMG_load) return;
	CRect rect;
	CWnd *pWnd = NULL;  
	pWnd = GetDlgItem(IDC_IMG);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if(point.x>rect.left&&point.x<rect.right&&point.y>rect.top&&point.y<rect.bottom)
	{	
		m_StartPoint = point;  
		m_EndPoint = point;  
		m_bLBDown = TRUE;  
		SetCapture();//设置鼠标捕获   
		CDialogEx::OnLButtonDown(nFlags, point);
		//cout<<point.x<<"*"<<point.y<<endl;
	}*/
	if (!IMG_load) return;
	//line_index=0;
	CRect rect;
	CWnd *pWnd = NULL;  
	pWnd = GetDlgItem(IDC_IMG);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (flag_label==0)
	{
		if(point.x>rect.left&&point.x<rect.right&&point.y>rect.top&&point.y<rect.bottom)
		{	
			line_index=0;
			line=new int*[100000];
			line_ind=new int [100];
			line_ind_index=0;
			line_ind[0]=0;
			label_index=new int [100];
			label_num=1;
			label_index[0]=-1;
			line[line_index]=new int[2];
			int x=(double)(point.x-rect.left)/rect.Width()*rect_img.Width()+rect_img.left;
			int y=(double)(point.y-rect.top)/rect.Height()*rect_img.Height()+rect_img.top;
			line[line_index][0]=x;
			line[line_index][1]=y;
			m_StartPoint = point;  
			m_EndPoint = point;  
			m_bLBDown = TRUE;  
			SetCapture();//设置鼠标捕获   
			CDialogEx::OnLButtonDown(nFlags, point);
			//cout<<point.x<<"*"<<point.y<<endl;
			flag_label=1;
		}
		
	}
	else
	{
		if(point.x>rect.left&&point.x<rect.right&&point.y>rect.top&&point.y<rect.bottom)
		{	
			line_index++;
			line[line_index]=new int[2];
			
			int x=(double)(point.x-rect.left)/rect.Width()*rect_img.Width()+rect_img.left;
			int y=(double)(point.y-rect.top)/rect.Height()*rect_img.Height()+rect_img.top;
			line[line_index][0]=x;
			line[line_index][1]=y;
			m_StartPoint = m_EndPoint; 
			m_EndPoint = point;  
			m_bLBDown = TRUE;  
			SetCapture();//设置鼠标捕获 
			Invalidate(FALSE);//更新界面  
			CDialogEx::OnLButtonDown(nFlags, point);
			//cout<<point.x<<"*"<<point.y<<endl;
		}
	}
	save=1;
}




void CImage_cutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	 if (m_bLBDown)  
    {  
		//PIC_empty=1;
		CRect rect;
		CWnd *pWnd = NULL;  
		pWnd = GetDlgItem(IDC_IMG);
		pWnd->GetWindowRect(&rect);
		//ClientToScreen(rect);
		ClipCursor(&rect);
        m_EndPoint = point; 

		//cout<<point.x<<"#"<<point.y<<endl;
  
        Invalidate(FALSE);//更新界面  
    }  
	CDialogEx::OnMouseMove(nFlags, point);
	*/
	if (m_bLBDown)  
    {  
		//PIC_empty=1;
		CRect rect,rect_clip;
		CWnd *pWnd = NULL,*pWnd1=NULL;  
		//pWnd = GetDlgItem(IDC_IMG);
		//pWnd->GetWindowRect(&rect_clip);
		//ClipCursor(&rect_clip);
		pWnd = GetDlgItem(IDC_IMG);
		pWnd->GetWindowRect(&rect);
		ClipCursor(&rect);
		ScreenToClient(&rect);
		m_StartPoint = m_EndPoint; 
        m_EndPoint = point; 
		line_index++;
		line[line_index]=new int[2];
		int x=(double)(point.x-rect.left)/rect.Width()*rect_img.Width()+rect_img.left;
		int y=(double)(point.y-rect.top)/rect.Height()*rect_img.Height()+rect_img.top;
		line[line_index][0]=x;
		line[line_index][1]=y;

		//cout<<point.x<<"#"<<point.y<<endl;
  
        Invalidate(FALSE);//更新界面  
    }  
	if (m_bRBDown)
	{
		CRect rect;
		CWnd *pWnd = NULL;  
		//pWnd = GetDlgItem(IDC_IMG);
		//pWnd->GetWindowRect(&rect_clip);
		//ClipCursor(&rect_clip);
		pWnd = GetDlgItem(IDC_IMG);
		pWnd->GetWindowRect(&rect);
		ClipCursor(&rect);
		ScreenToClient(&rect);
		int w=rect_img.Width(),h=rect_img.Height();
		cout<<rect_img.left<<" "<<rect_img.right<<" "<<rect_img.top<<" "<<rect_img.bottom<<endl;
		cout<<point.x-point_ori.x<<endl;
		rect_img.left=rect_img.left-(double)(point.x-point_ori.x)/rect.Width()*rect_img.Width();
		if (rect_img.left<0)
		{
			rect_img.left=0;
			rect_img.right=w;
		}
		else
		{
			//rect_img.right=rect_img.right-(double)(point.x-point_ori.x)/rect.Width()*rect_img.Width();
			rect_img.right=rect_img.left+w;
			if (rect_img.right>image.GetWidth())
			{
				rect_img.right=image.GetWidth();
				rect_img.left=rect_img.right-w;
			}
		}
		rect_img.top=rect_img.top-(double)(point.y-point_ori.y)/rect.Height()*rect_img.Height();
		if (rect_img.top<0)
		{
			rect_img.top=0;
			rect_img.bottom=h;
		}
		else
		{
			//rect_img.bottom=rect_img.bottom-(double)(point.y-point_ori.y)/rect.Height()*rect_img.Height();
			rect_img.bottom=rect_img.top+h;
			if (rect_img.bottom>image.GetHeight())
			{
				rect_img.bottom=image.GetHeight();
				rect_img.top=rect_img.bottom-h;
			}
		}
		cout<<rect_img.left<<" "<<rect_img.right<<" "<<rect_img.top<<" "<<rect_img.bottom<<endl;
		cout<<rect_img.Height()<<" "<<rect_img.Width()<<endl;
		ShowImage(image,rect_img,IDC_IMG);
		point_ori=point;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}




void CImage_cutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码/和/或调用默认值
	if(this == GetCapture())  
    {  
    ReleaseCapture();  
    }  
    if(m_bLBDown == TRUE) 
	{
		line_ind_index++;
		line_ind[line_ind_index]=line_index;
		label_index[label_num]=line_index;
	}
	//cout<<line_ind[0]<<endl;

	m_bLBDown = FALSE;  
    Invalidate(FALSE);//更新界面   
	ClipCursor(NULL);
	CDialogEx::OnLButtonUp(nFlags, point);
}



void CImage_cutDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	//duan
	if (status==0) {
		MessageBox("请先对图片进行放大", "错误", MB_OK);
		return;
	}

	if (!IMG_load)
	{
		MessageBox("先打开要处理的图片!","错误",MB_OK );
		return;
	}
	if (count>=num_patch) return;


	if (flag_label!=0)
	{		
		line_index=0;
		line_ind_index=0;
		label_num=0;
		delete []line_ind;
		delete []line;
		delete []label_index;
		save=0;
		flag_label=0;
	}
	EnableWindow(FALSE);
	count++;
	PIC_empty=1;
	ImgPatch(count);
	rect_img.left=0;
	rect_img.right=image.GetWidth()-1;
	rect_img.top=0;
	rect_img.bottom=image.GetHeight()-1;
	cout<<rect_img.left<<" "<<rect_img.right<<" "<<rect_img.top<<" "<<rect_img.bottom<<endl;
	OnPaint();
	ShowImage(image, rect_img, IDC_IMG);//duan
	CString c1,c2;
	c1.Format("%d",count);
	c2.Format("%d",(int)num_patch);
	CString text;
	text=c1+"/"+c2;
    m_edit.SetWindowTextA(text);
	m_edit_text.SetWindowTextA("");
	EnableWindow(TRUE);

}


void CImage_cutDlg::OnBnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	//duan
	if (status==0) {
		MessageBox("请先对图片进行放大", "错误", MB_OK);
		return;
	}

	if (!IMG_load)
	{
		MessageBox("先打开要处理的图片!","错误",MB_OK );
		return;
	}
	if (count<=1) return;
	if (flag_label!=0)
	{		
		line_index=0;
		line_ind_index=0;
		label_num=0;
		delete []line_ind;
		delete []line;
		delete []label_index;
		save=0;
		flag_label=0;
	}
	EnableWindow(FALSE);
	count--;
	PIC_empty=1;
	ImgPatch(count);
	rect_img.left=0;
	rect_img.right=image.GetWidth()-1;
	rect_img.top=0;
	rect_img.bottom=image.GetHeight()-1;
	cout<<rect_img.left<<" "<<rect_img.right<<" "<<rect_img.top<<" "<<rect_img.bottom<<endl;
    OnPaint();
	ShowImage(image, rect_img, IDC_IMG);//duan
	CString c1,c2;
	c1.Format("%d",count);
	c2.Format("%d",(int)num_patch);
	CString text;
	text=c1+"/"+c2;
    m_edit.SetWindowTextA(text);
	m_edit_text.SetWindowTextA("");
	EnableWindow(TRUE);
}



//void CImage_cutDlg::OnChangeEdit()
//{
//	
//}



void CImage_cutDlg::OnBnClickedHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	//cout << "123" << endl;
	MessageBox(_T(" 1.点击“打开图片”打开需要标注的病理图像 \r\n 2.使用滚轮放大缩小图片，鼠标左键标注病变区域，右键拖动图像 \r\n 3.点击“撤销”可撤销当前标注痕迹 \r\n 4.可在文本框内输入需要记录的文本信息，点击“保存”保存当前标注信息 \r\n 5.完成本组标注后，点击“下一组”载入图像的另一部分，进度框显示处理进度 \r\n 6.点击“退出”关闭软件窗口"),_T("帮助"),MB_OK);
}


void CImage_cutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!IMG_load) return;
	if (save==0) return;
	if (line_ind[line_ind_index]>label_index[label_num-1]) 
	{
		line_ind_index--;
		line_index=line_ind[line_ind_index];
		label_index[label_num]=line_index;
		cout<<line_index<<endl;
		ShowImage(image,rect_img,IDC_IMG);
	}
	
	if (line_ind[line_ind_index]==label_index[label_num-1]) 
	{
		label_index[label_num]=label_index[label_num-1]+1;
		save=0;
	}
	
	if (line_ind_index==0) 
	{
		flag_label=0;
		save=0;
		//delete []line_ind;
	}
}


void CImage_cutDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!IMG_load) return;
	CRect rect;
	CWnd *pWnd = NULL;  
	pWnd = GetDlgItem(IDC_IMG);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if(point.x>rect.left&&point.x<rect.right&&point.y>rect.top&&point.y<rect.bottom)
	{	
		point_ori=point;
		m_bRBDown=TRUE;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CImage_cutDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bRBDown=FALSE;
	ClipCursor(NULL);
	CDialogEx::OnRButtonUp(nFlags, point);
}

int CImage_cutDlg::findmax(int a[],int len)
{
	int m=a[0];
	//cout<<a<<endl;
	for (int i=0;i<len;i++)
	{
		//cout<<a<<endl;
		m=max(m,a[i]);
		cout<<m<<endl;
	}
	return m;
}

int CImage_cutDlg::findmin(int a[],int len)
{
	int m=a[0];
	for (int i=0;i<len;i++)
	{
		m=min(m,a[i]);
	}
	return m;
}

void CImage_cutDlg::OnStnClickedImg()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CImage_cutDlg::OnStnClickedProc()
{
	// TODO: 在此添加控件通知处理程序代码
}
