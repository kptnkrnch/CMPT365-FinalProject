
// CMPT365-A2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CMPT365-A2.h"
#include "CMPT365-A2Dlg.h"
#include "afxdialogex.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "SDL2\SDL.h"
#include "SDL2_mixer\SDL_mixer.h"
#include "resource.h"
#include <Windows.h>
#include <cstdint>
#include <string>
#include <math.h>

#define PI 3.141592653589793

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
// CAboutDlg dialog used for App About

UINT VideoToSoundThread(LPVOID pParam);

HBITMAP hBitmap = NULL;
CString file_path = NULL;
bool running = true;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCMPT365A2Dlg dialog



CCMPT365A2Dlg::CCMPT365A2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCMPT365A2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCMPT365A2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCMPT365A2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCMPT365A2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCMPT365A2Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CCMPT365A2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_STOPBTN, &CCMPT365A2Dlg::OnBnClickedStopbtn)
END_MESSAGE_MAP()


// CCMPT365A2Dlg message handlers

BOOL CCMPT365A2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCMPT365A2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCMPT365A2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCMPT365A2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCMPT365A2Dlg::OnBnClickedOk()
{
	running = true;
	AfxBeginThread(VideoToSoundThread, NULL);
	/*bool audio_opened = false;
	std::string s = "";
	if (file_path.GetLength() > 0) {
		CT2CA pszConvertedAnsiString (file_path);
		 s = std::string(pszConvertedAnsiString);
	}
	if (s.length() > 0) {
		VideoCapture vcap(s.c_str());
		if (SDL_Init(SDL_INIT_AUDIO) >= 0) {
			audio_opened = true;
		}
		if (vcap.isOpened() && audio_opened) {
			namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
			double count = vcap.get(CV_CAP_PROP_FRAME_COUNT);
			if( Mix_OpenAudio( 8000, AUDIO_F32, 2, 1024 ) < 0 ) {
				exit(0);
			}
			int counterer = 0;
			bool running = true;
			while (running)
			{
				//if (img.empty()) {
				//	break;
				//}
				Mat frame;
			
				bool foundFrame = vcap.read(frame);
				if (!foundFrame) {
					break;
				}
				Size size = frame.size();
				int t = frame.type();
				Mat output;
				Size out_size(64, 64);
				resize(frame, output, out_size);
				Mat frame_grey(out_size, CV_8UC1);
				uint8_t soundArray[64][64];
				for (int x = 0; x < out_size.width; x++) {
					for (int y = 0; y < out_size.height; y++) {
						Vec3b pixel;
						pixel = output.at<Vec3b>(y, x);
					
						float b = (float) pixel.val[0] / 255;
						float g = (float) pixel.val[1] / 255;
						float r = (float) pixel.val[2] / 255;
					
						float fgrey = 0.299f * r + 0.587f * g + 0.114 * b;
						uchar ugrey = (uchar)(255 * fgrey);
						soundArray[y][x] = (uint8_t)(fgrey * 16);
						Vec<uchar, 1> intensity;
						intensity.val[0] = ugrey;
						frame_grey.at<Vec<uchar, 1> >(y,x) = intensity;
						//std::cout << x << " " << y << endl;
					}
				}
				imshow("MyWindow", frame_grey);
				int c = cvWaitKey(10);
				if( (char)c == 27 ) { 
					running = false;
				}
				double Fs = 8000;
				double freq[64];
				for (int i = 0; i < 64; i++) {
					freq[i] = 0;
				}
				freq[31] = 440.0;
				for (int i = 32; i < 64; i++) {
					freq[i] = freq[i - 1] * pow(2.0, 1.0/12.0);
				}
				for (int i = 30; i >= 0; i--) {
					freq[i] = freq[i + 1] * pow(2.0, -1.0/12.0);
				}

				double N = 500;
				double tt_vector[500];
				for (int i = 0; i < 500; i++) {
					tt_vector[i] = (i + 1)/Fs;
				}
				double volume = 0.75;
				for (int col = 0; col < 64; col++) {
					float signal[1000];
					for (int i = 0; i < 1000; i++) {
						signal[i] = 0;
					}
					for (int row = 0; row < 64; row++) {
						int m = 64 - row - 1;
						double ss[500];
						for (int i = 0; i < 1000; i += 2) {
							ss[i / 2] = sin(2 * PI * freq[m] * tt_vector[i/2]);
							signal[i] = signal[i] + (float)(ss[i / 2] * soundArray[row][col] * pow(volume, (double)(16 - soundArray[row][col])));
							signal[i+1] = signal[i+1] + (float)(ss[i / 2] * soundArray[row][col] * pow(volume, (double)(16 - soundArray[row][col])));
						}
					}
				
					Mix_Chunk * chunk = Mix_QuickLoad_RAW((Uint8*)signal, 1000);
					chunk->allocated = true;
					int rc = Mix_PlayChannel(1, chunk, 0);
					Sleep(62);
					Mix_FreeChunk(chunk);
				}
			}
		}
		destroyWindow("MyWindow");
	}*/
	
	//CDialogEx::OnOK();
}


void CCMPT365A2Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CCMPT365A2Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog * filebrowser = new CFileDialog(true, 0, 0, 0, 0, this, 0, 1);
	if (filebrowser->DoModal() == IDOK) {
		file_path = filebrowser->GetPathName();
		CWnd * editbox = GetDlgItem(IDC_EDIT1);
		editbox->SetWindowTextW(filebrowser->GetPathName());
	}
	delete filebrowser;
}

UINT VideoToSoundThread(LPVOID pParam) {
	bool audio_opened = false;
	std::string s = "";
	if (file_path.GetLength() > 0) {
		CT2CA pszConvertedAnsiString (file_path);
		 s = std::string(pszConvertedAnsiString);
	}
	if (s.length() > 0) {
		VideoCapture vcap(s.c_str());
		if (SDL_Init(SDL_INIT_AUDIO) >= 0) {
			audio_opened = true;
		}
		if (vcap.isOpened() && audio_opened) {
			namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
			double count = vcap.get(CV_CAP_PROP_FRAME_COUNT);
			if( Mix_OpenAudio( 8000, AUDIO_F32, 2, 1024 ) < 0 ) {
				exit(0);
			}
			int counterer = 0;
			
			while (running)
			{
				//if (img.empty()) {
				//	break;
				//}
				Mat frame;
			
				bool foundFrame = vcap.read(frame);
				if (!foundFrame) {
					break;
				}
				Size size = frame.size();
				int t = frame.type();
				Mat output;
				Size out_size(64, 64);
				resize(frame, output, out_size);
				Mat frame_grey(out_size, CV_8UC1);
				uint8_t soundArray[64][64];
				for (int x = 0; x < out_size.width; x++) {
					for (int y = 0; y < out_size.height; y++) {
						Vec3b pixel;
						pixel = output.at<Vec3b>(y, x);
					
						float b = (float) pixel.val[0] / 255;
						float g = (float) pixel.val[1] / 255;
						float r = (float) pixel.val[2] / 255;
					
						float fgrey = 0.299f * r + 0.587f * g + 0.114 * b;
						uchar ugrey = (uchar)(255 * fgrey);
						soundArray[y][x] = (uint8_t)(fgrey * 16);
						Vec<uchar, 1> intensity;
						intensity.val[0] = ugrey;
						frame_grey.at<Vec<uchar, 1> >(y,x) = intensity;
						//std::cout << x << " " << y << endl;
					}
				}
				imshow("MyWindow", frame_grey);
				int c = cvWaitKey(10);
				if( (char)c == 27 ) { 
					running = false;
				}
				double Fs = 8000;
				double freq[64];
				for (int i = 0; i < 64; i++) {
					freq[i] = 0;
				}
				freq[31] = 440.0;
				for (int i = 32; i < 64; i++) {
					freq[i] = freq[i - 1] * pow(2.0, 1.0/12.0);
				}
				for (int i = 30; i >= 0; i--) {
					freq[i] = freq[i + 1] * pow(2.0, -1.0/12.0);
				}

				double N = 500;
				double tt_vector[500];
				for (int i = 0; i < 500; i++) {
					tt_vector[i] = (i + 1)/Fs;
				}
				double volume = 0.75;
				for (int col = 0; col < 64; col++) {
					float signal[1000];
					for (int i = 0; i < 1000; i++) {
						signal[i] = 0;
					}
					for (int row = 0; row < 64; row++) {
						int m = 64 - row - 1;
						double ss[500];
						for (int i = 0; i < 1000; i += 2) {
							ss[i / 2] = sin(2 * PI * freq[m] * tt_vector[i/2]);
							signal[i] = signal[i] + (float)(ss[i / 2] * soundArray[row][col] * pow(volume, (double)(16 - soundArray[row][col])));
							signal[i+1] = signal[i+1] + (float)(ss[i / 2] * soundArray[row][col] * pow(volume, (double)(16 - soundArray[row][col])));
						}
					}
				
					Mix_Chunk * chunk = Mix_QuickLoad_RAW((Uint8*)signal, 1000);
					chunk->allocated = true;
					int rc = Mix_PlayChannel(1, chunk, 0);
					Sleep(62);
					Mix_FreeChunk(chunk);
					if (!running) {
						break;
					}
				}
			}
			destroyWindow("MyWindow");
		}
	}
}


void CCMPT365A2Dlg::OnBnClickedStopbtn()
{
	// TODO: Add your control notification handler code here
	running = false;
}
