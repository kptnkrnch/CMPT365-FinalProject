
// CMPT365-A3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CMPT365-A3.h"
#include "CMPT365-A3Dlg.h"
#include "afxdialogex.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "resource.h"
#include <Windows.h>
#include <cstdint>
#include <string>
#include <math.h>
#include <vector>
#include <stdint.h>
#include <sstream>

#define PI 3.141592653589793

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
// CAboutDlg dialog used for App About

UINT STIByCopingCenterRowsThread(LPVOID pParam);
UINT STIByCopingCenterColumnsThread(LPVOID pParam);
UINT STIByHistogramsThread(LPVOID pParam);

Mat DetectEdge(Mat img);
Point FindTransition(Mat img);

HBITMAP hBitmap = NULL;
CString file_path = NULL;
bool running = false;
int STI_MODE = 1;
bool find_transition_frame = false;

#define STI_MODE_ROWS 1
#define STI_MODE_COLUMNS 2
#define STI_MODE_HISTOGRAM 3


// CAboutDlg dialog used for App About

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


// CCMPT365A3Dlg dialog



CCMPT365A3Dlg::CCMPT365A3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCMPT365A3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCMPT365A3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCMPT365A3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCMPT365A3Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCMPT365A3Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CCMPT365A3Dlg::OnBnClickedOpenVideo)
	ON_BN_CLICKED(IDC_STOPBTN, &CCMPT365A3Dlg::OnBnClickedStopbtn)
	ON_BN_CLICKED(IDC_RADIO1, &CCMPT365A3Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCMPT365A3Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CCMPT365A3Dlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_CHECK1, &CCMPT365A3Dlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CCMPT365A3Dlg message handlers

BOOL CCMPT365A3Dlg::OnInitDialog()
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
	CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton1->SetCheck(true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCMPT365A3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCMPT365A3Dlg::OnPaint()
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
HCURSOR CCMPT365A3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCMPT365A3Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (running == false) {
		running = true;
		switch(STI_MODE) {
		case STI_MODE_ROWS:
			AfxBeginThread(STIByCopingCenterRowsThread, NULL);
			break;
		case STI_MODE_COLUMNS:
			AfxBeginThread(STIByCopingCenterColumnsThread, NULL);
			break;
		case STI_MODE_HISTOGRAM:
			AfxBeginThread(STIByHistogramsThread, NULL);
			break;
		}
	}
	//CDialogEx::OnOK();
}


void CCMPT365A3Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CCMPT365A3Dlg::OnBnClickedOpenVideo()
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

typedef struct Pixel {
	uint8_t red;
	uint8_t blue;
	uint8_t green;
} Pixel;

UINT STIByCopingCenterRowsThread(LPVOID pParam) {
	std::string s = "";
	if (file_path.GetLength() > 0) {
		CT2CA pszConvertedAnsiString (file_path);
		 s = std::string(pszConvertedAnsiString);
	}
	if (s.length() > 0) {
		VideoCapture vcap(s.c_str());
		if (vcap.isOpened()) {
			double count = vcap.get(CV_CAP_PROP_FRAME_COUNT);

			int counterer = 0;
			std::vector<std::vector<Pixel> > STI;
			int current_frame = 0;
			Size size;
			while (running)
			{
				Mat frame;
			
				bool foundFrame = vcap.read(frame);
				if (!foundFrame) {
					break;
				}
				size = frame.size();
				int t = frame.type();

				std::vector<Pixel> column;

				int y = size.height / 2;
				for (int x = 0; x < size.width; x++) {
					
					Vec3b pixel;
					pixel = frame.at<Vec3b>(y, x);

					Pixel temp;
					
					temp.blue = pixel.val[0];
					temp.green = pixel.val[1];
					temp.red = pixel.val[2];
					column.push_back(temp);
						
				}
				STI.push_back(column);
			}
			
			if (running) {
				Size stiImageSize(STI.size(), size.width);
				Mat stiImage(stiImageSize, CV_8UC3);
				for (int y = 0; y < STI.size() && running; y++) {
					std::vector<Pixel> column = STI.at(y);
					for (int x = 0; x < size.width && running; x++) {
						Vec<uchar, 3> pixel;
						pixel.val[0] = (uchar)column.at(x).blue;
						pixel.val[1] = (uchar)column.at(x).green;
						pixel.val[2] = (uchar)column.at(x).red;
					
						stiImage.at<Vec<uchar, 3> >(x, y) = pixel;
					}
				}
				if (running) {
					namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
					
					imshow("MyWindow", stiImage);
					int c = cvWaitKey(5000);
					if( (char)c == 27 ) { 
						running = false;
					}
					//std::vector<int> params;
					//params.push_back(CV_IMWRITE_PNG_COMPRESSION);
					//params.push_back(3);
					//Mat edges = DetectEdge(stiImage);
					
					//imshow("MyWindow", edges);
					//c = cvWaitKey(5000);
					//if( (char)c == 27 ) { 
					//	running = false;
					//}
					if (find_transition_frame) {
						Mat edges = DetectEdge(stiImage);
						Point p = FindTransition(edges);
					
						CString msg;
						msg.Format(L"The transition begins at approximately frame #: %d", (p.x + 1));
						AfxMessageBox(msg);
					}

					destroyWindow("MyWindow");
				}
			}
		}
	}
	running = false;
	return 0;
}

UINT STIByCopingCenterColumnsThread(LPVOID pParam) {
	std::string s = "";
	if (file_path.GetLength() > 0) {
		CT2CA pszConvertedAnsiString (file_path);
		 s = std::string(pszConvertedAnsiString);
	}
	if (s.length() > 0) {
		VideoCapture vcap(s.c_str());
		if (vcap.isOpened()) {
			//namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
			double count = vcap.get(CV_CAP_PROP_FRAME_COUNT);

			int counterer = 0;
			std::vector<std::vector<Pixel> > STI;
			int current_frame = 0;
			Size size;
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
				size = frame.size();
				int t = frame.type();

				std::vector<Pixel> column;
				//Pixel * column = 0;
				//column = (Pixel *)malloc(sizeof(Pixel) * size.width);
				int x = size.width / 2;
				for (int y = 0; y < size.height; y++) {
					//for (int y = 0; y < size.height; y++) {
						Vec3b pixel;
						pixel = frame.at<Vec3b>(y, x);

						Pixel temp;
					//}
						temp.blue = pixel.val[0];
						temp.green = pixel.val[1];
						temp.red = pixel.val[2];
						column.push_back(temp);
						
				}
				STI.push_back(column);

				//imshow("MyWindow", frame);
				
				//Sleep(1000);
			}
			if (running) {
				Size stiImageSize(STI.size(), size.height);
				Mat stiImage(stiImageSize, CV_8UC3);
				for (int y = 0; y < STI.size() && running; y++) {
					std::vector<Pixel> column = STI.at(y);
					for (int x = 0; x < size.height && running; x++) {
						Vec<uchar, 3> pixel;
						pixel.val[0] = (uchar)column.at(x).blue;
						pixel.val[1] = (uchar)column.at(x).green;
						pixel.val[2] = (uchar)column.at(x).red;
					
						stiImage.at<Vec<uchar, 3> >(x, y) = pixel;
						//stiImage.at<Vec<uchar, 1> >(y, x)[0] = 255;
						//stiImage.at<Vec<uchar, 3> >(y, x)[0] = 0;
						//stiImage.at<Vec<uchar, 3> >(y, x)[0] = 0;
					}
				}
				if (running) {
					namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
					//for (int i = 0; i < 100000; i++) {
					imshow("MyWindow", stiImage);
					int c = cvWaitKey(5000);
					if( (char)c == 27 ) { 
						running = false;
					}

					if (find_transition_frame) {
						Mat edges = DetectEdge(stiImage);
						Point p = FindTransition(edges);
					
						CString msg;
						msg.Format(L"The transition begins at approximately frame #: %d", (p.x + 1));
						AfxMessageBox(msg);
					}
			
					//}
					//Sleep(10000);
					destroyWindow("MyWindow");
				}
			}
		}
	}
	running = false;
	return 0;
}

Mat DetectEdge(Mat img) {
	Mat grey, dest;
	dest.create(img.size(), img.type());
	cvtColor(img, grey, CV_BGR2GRAY);
	Mat detectedEdges;
	blur(grey, detectedEdges, Size(3,3));
	double threshold = 0.1;
	Canny( detectedEdges, detectedEdges, threshold, threshold*3,3);
	return detectedEdges;
}

Point FindTransition(Mat img) {
	std::vector<Point> longestPath;
	for (int y = 0; y < img.size().height - 1; y++) {
		for (int x = 0; x < img.size().width - 1; x++) {
			Vec<uchar, 1> pixel;
			pixel = img.at<Vec<uchar, 1> >(y, x);

			int intensity = pixel.val[0];
			if (intensity > 230) {
				std::vector<Point> tempLongestPath;
				Point p(x, y);
				tempLongestPath.push_back(p);
				int xDist = 0;
				int yDist = 0;
				while (true) {
					bool found = false;

					if (!found && y < img.size().height - 1) {
						pixel = img.at<Vec<uchar, 1> >(y + 1, x);
						intensity = pixel.val[0];
						if (intensity > 230) {
							found = true;
							y += 1;
							yDist++;
						}
					}

					if (!found && y < img.size().height - 1 && x < img.size().width - 1) {
						pixel = img.at<Vec<uchar, 1> >(y + 1, x + 1);
						intensity = pixel.val[0];
						if (intensity > 230) {
							found = true;
							y += 1;
							x += 1;
							yDist++;
							xDist++;
						} 
					}
					if (!found && x < img.size().width - 1) {
						pixel = img.at<Vec<uchar, 1> >(y, x + 1);
						intensity = pixel.val[0];
						if (intensity > 230) {
							found = true;
							x += 1;
							xDist++;
						}
					}

					if (!found) {
						if (tempLongestPath.size() > longestPath.size()) {
							longestPath = tempLongestPath;
						}
						break;
					}

					if (xDist > 5 || yDist > 5 && xDist > 0) {
						double angle = atan((double)yDist / (double)xDist);
						if (angle < 0.5) {
							if (tempLongestPath.size() > longestPath.size()) {
								longestPath = tempLongestPath;
							}
							break;
						}
					}
					Point tempP(x, y);
					tempLongestPath.push_back(tempP);
				}
			}
		}
						
	}
	if (longestPath.size() > 0) {
		int startX = longestPath.at(0).x;
		int startY = longestPath.at(0).y;
		int endX = longestPath.at(longestPath.size() - 1).x;
		int endY = longestPath.at(longestPath.size() - 1).y;
		int x = endX - startX;
		int y = endY - startY;
		double angle = atan((double)x/(double)y);
		int newX = startX - (int)(tan(angle) * startY);
		Point transition(newX, 0);
		return transition;
		//return longestPath.at(0);
	} else {
		Point error(-1, -1);
		return error;
	}
}

typedef struct ChromaPixel {
	float r;
	float g;
} ChromaPixel;

class ChromaticityFrame {
private:
	bool imageAllocated;
public:
	int width;
	int height;
	long bins;
	ChromaPixel ** image;
	int ** histogram;//in format [r][g]
	//std::vector<float **> colhistogram;
	std::vector<std::vector<std::vector<float> > * > colhistogram;

	ChromaticityFrame(const int _width, const int _height) {
		this->width = _width;
		this->height = _height;
		this->bins = 0;
		image = (ChromaPixel **)malloc(sizeof(ChromaPixel *) * _height);
		for (int i = 0; i < _height; i++) {
			image[i] = (ChromaPixel *)malloc(sizeof(ChromaPixel) * _width);
		}
		histogram = 0;
		imageAllocated = true;
	}

	void DeleteImage() {
		if (imageAllocated) {
			for (int i = 0; i < height; i++) {
				free(image[i]);
			}
			free(image);
			imageAllocated = false;
		}
	}

	void CreateHistogram(long numberOfBins) {
		this->bins = numberOfBins;
		histogram = (int **)malloc(sizeof(int *) * numberOfBins);
		for (int i = 0; i < numberOfBins; i++) {
			histogram[i] = (int *)malloc(sizeof(int) * numberOfBins);
			for (int n = 0; n < numberOfBins; n++) {
				histogram[i][n] = 0;
			}
		}
	}

	void SetPixel(int x, int y, ChromaPixel value) {
		image[y][x].r = value.r;
		image[y][x].g = value.g;
	}

	ChromaPixel GetPixel(int x, int y) {
		return image[y][x];
	}
};

UINT STIByHistogramsThread(LPVOID pParam) {
	std::string s = "";
	if (file_path.GetLength() > 0) {
		CT2CA pszConvertedAnsiString (file_path);
		 s = std::string(pszConvertedAnsiString);
	}
	if (s.length() > 0) {
		VideoCapture vcap(s.c_str());
		if (vcap.isOpened()) {
			double count = vcap.get(CV_CAP_PROP_FRAME_COUNT);

			int counterer = 0;
			std::vector<ChromaticityFrame> STI;
			std::vector<float **> columnHistograms;
			int current_frame = 0;
			Size size;
			while (running)
			{
				Mat frame;
			
				bool foundFrame = vcap.read(frame);
				if (!foundFrame) {
					break;
				}
				size = frame.size();
				int t = frame.type();

				//calculating chromaticity frames and storing them in a vector for easy access
				ChromaticityFrame cimage(size.width, size.height);
				for (int y = 0; y < size.height; y++) {
					for (int x = 0; x < size.width; x++) {
						Vec3b pixel;
						pixel = frame.at<Vec3b>(y, x);

						ChromaPixel temp;
						int red = pixel.val[0];
						int green = pixel.val[1];
						int blue = pixel.val[2];
						if ((red + green + blue) > 0) {
							temp.r = (float)red/(red + green + blue);
							temp.g = (float)green/(red + green + blue);
						} else {
							temp.r = 0;
							temp.g = 0;
						}
						cimage.SetPixel(x, y, temp);
					}
						
				}
				long bins = 1 + (long)(log10(size.height) / log10(2));
				cimage.bins = bins;
				STI.push_back(cimage);
			}
			if (running) {
				Size stiImageSize(STI.size(), size.width);
				Mat stiImage(stiImageSize, CV_8UC1);
				
				
				for (int frame = 0; frame < STI.size(); frame++) {
					if (frame == 0) {
						for (int row = 0; row < size.width; row++) {
							Vec<uchar, 1> pixel;
							pixel.val[0] = 0;
							stiImage.at<Vec<uchar, 1> >(row, 0) = pixel;
						}
					} else {
						ChromaticityFrame cimage1 = STI.at(frame);
						ChromaticityFrame cimage2 = STI.at(frame - 1);

						const int bins = cimage1.bins;
						for (int column = 0; column < cimage1.width; column++) {
							float ** histogram1 = new float*[bins];
							float ** histogram2 = new float*[bins];
							for (int temp = 0; temp < bins; temp++) {
								histogram1[temp] = new float[bins];
								histogram2[temp] = new float[bins];
								for (int x = 0; x < bins; x++) {
									histogram1[temp][x] = 0;
									histogram2[temp][x] = 0;
								}
							}
							float sum = cimage1.height;
							for (int row = 0; row < cimage1.height; row++) {
								int r1 = (int)floor((bins - 1) * cimage1.GetPixel(column, row).r);
								int g1 = (int)floor((bins - 1) * cimage1.GetPixel(column, row).g);
								histogram1[r1][g1] += 1.0;
								int r2 = (int)floor((bins - 1) * cimage2.GetPixel(column, row).r);
								int g2 = (int)floor((bins - 1) * cimage2.GetPixel(column, row).g);
								histogram2[r2][g2] += 1.0;
							}
							float I = 0;
							for (int r = 0; r < bins; r++) {
								for (int g = 0; g < bins; g++) {
									histogram1[r][g] = histogram1[r][g] / sum;
									histogram2[r][g] = histogram2[r][g] / sum;

									if (histogram1[r][g] < histogram2[r][g]) {
										I += histogram1[r][g];
									} else {
										I += histogram2[r][g];
									}
								}
							}
							Vec<uchar, 1> pixel;
							if (I > 0.7) {
								pixel.val[0] = 0;
							} else {
								pixel.val[0] = 255;
							}
							stiImage.at<Vec<uchar, 1> >(column, frame) = pixel;

							for (int y = 0; y < bins; y++) {
								delete[] histogram1[y];
								delete[] histogram2[y];
							}
							delete[] histogram1;
							delete[] histogram2;
						}
					}
				}
				if (running) {
					namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
					imshow("MyWindow", stiImage);
					int c = cvWaitKey(5000);
					if( (char)c == 27 ) { 
						running = false;
					}
					destroyWindow("MyWindow");
					if (find_transition_frame) {
						Point p = FindTransition(stiImage);
					
						CString msg;
						msg.Format(L"The transition begins at approximately frame #: %d", (p.x + 1));
						AfxMessageBox(msg);
					}
				}
			}
			for (int i = 0; i < STI.size(); i++) {
				ChromaticityFrame frame = STI.at(i);
				frame.DeleteImage();
			}
		}
	}
	running = false;
	return 0;
}

void CCMPT365A3Dlg::OnBnClickedStopbtn()
{
	// TODO: Add your control notification handler code here
	running = false;
}

//Radio button handler for Center Rows STI
void CCMPT365A3Dlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton2->SetCheck(false);
	CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton3->SetCheck(false);
	STI_MODE = STI_MODE_ROWS;
}

//Radio button handler for Center Columns STI
void CCMPT365A3Dlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton1->SetCheck(false);
	CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton3->SetCheck(false);
	STI_MODE = STI_MODE_COLUMNS;
}

//Radio button handler for Histogram STI
void CCMPT365A3Dlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton1->SetCheck(false);
	CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton2->SetCheck(false);
	STI_MODE = STI_MODE_HISTOGRAM;
}


void CCMPT365A3Dlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	if (find_transition_frame) {
		CButton* checkBtn = (CButton*)GetDlgItem(IDC_CHECK1);
		checkBtn->SetCheck(false);
		find_transition_frame = false;
	} else {
		CButton* checkBtn = (CButton*)GetDlgItem(IDC_CHECK1);
		checkBtn->SetCheck(true);
		find_transition_frame = true;
	}
}
