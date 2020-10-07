
// BilliardsModelDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "BilliardsModel.h"
#include "BilliardsModelDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// Диалоговое окно CBilliardsModelDlg


#define DOTS(x,y) (xp*((x)-xmin)),(yp*((y)-ymax))

// Adds a ball to the vector
void AddBall(double x, double y, vector<sBall>& vec)
{
	sBall b;
	b.px = x; b.py = y; b.pz = 0;
	b.vx = 0; b.vy = 0; b.vz = 0;
	b.ax = 0; b.ay = 0; b.az = 0;
	b.radius = 0.06;
	b.mass = 0.282;

	b.id = vec.size();
	vec.push_back(b);
}

double CBilliardsModelDlg::ddx(double x, double y, double z, double dx, double dy, double dz)  //второй закон на ось X
{
	double trenie = f_tr;
	double firstOfSumm = ((7.0 / 2.0) * trenie * g) * (dx / sqrt(dx * dx + dy * dy + dz * dz));
	double secondOfSumm = ((7.0 / 2.0) * trenie * g) * (V0x / sqrt(dx * dx + dy * dy + dz * dz));
	return -firstOfSumm + secondOfSumm;
}


double CBilliardsModelDlg::ddy(double x, double y, double z, double dx, double dy, double dz) //второй закон на ось Y
{
	double trenie = f_tr;
	double firstOfSumm = ((7.0 / 2.0) * trenie * g) * (dy / sqrt(dx * dx + dy * dy + dz * dz));
	double secondOfSumm = ((7.0 / 2.0) * trenie * g) * (V0y / sqrt(dx * dx + dy * dy + dz * dz));
	return -firstOfSumm + secondOfSumm;
}

double CBilliardsModelDlg::ddz(double x, double y, double z, double dx, double dy, double dz) //второй закон на ось Y
{
	return z;
}

double CBilliardsModelDlg::RungeKuttaMethod(double* mas)  //метод Рунге-Кутты
{

	double xx = mas[0];
	double yy = mas[1];
	double zz = mas[2];
	double dxx = mas[3];
	double dyy = mas[4];
	double dzz = mas[5];
	double dtt = mas[6];
	double k1x = ddx(xx, yy, zz, dxx, dyy, dzz) * dtt;
	double k1y = ddy(xx, yy, zz, dxx, dyy, dzz) * dtt;
	double k1z = ddz(xx, yy, zz, dxx, dyy, dzz) * dtt;
	double k2x = ddx(xx + dxx * dtt / 2., yy + dyy * dtt / 2., zz + dzz * dtt / 2., dxx + k1x / 2., dyy + k1y / 2., dzz + k1z / 2.) * dtt;
	double k2y = ddy(xx + dxx * dtt / 2., yy + dyy * dtt / 2., zz + dzz * dtt / 2., dxx + k1x / 2., dyy + k1y / 2., dzz + k1z / 2.) * dtt;
	double k2z = ddz(xx + dxx * dtt / 2., yy + dyy * dtt / 2., zz + dzz * dtt / 2., dxx + k1x / 2., dyy + k1y / 2., dzz + k1z / 2.) * dtt;
	double k3x = ddx(xx + dxx * dtt / 2. + (k1x * dtt) / 4., yy + dyy * dtt / 2. + (k1y * dtt) / 4., zz + dzz * dtt / 2. + (k1z * dtt) / 4., dxx + k2x / 2., dyy + k2y / 2., dzz + k2z / 2.) * dtt;
	double k3y = ddy(xx + dxx * dtt / 2. + (k1x * dtt) / 4., yy + dyy * dtt / 2. + (k1y * dtt) / 4., zz + dzz * dtt / 2. + (k1z * dtt) / 4., dxx + k2x / 2., dyy + k2y / 2., dzz + k2z / 2.) * dtt;
	double k3z = ddz(xx + dxx * dtt / 2. + (k1x * dtt) / 4., yy + dyy * dtt / 2. + (k1y * dtt) / 4., zz + dzz * dtt / 2. + (k1z * dtt) / 4., dxx + k2x / 2., dyy + k2y / 2., dzz + k2z / 2.) * dtt;
	double k4x = ddx(xx + dxx * dtt + k2x * dtt / 2., yy + dyy * dtt + k2y * dtt / 2., zz + dzz * dtt + k2z * dtt / 2., dxx + k3x, dyy + k3y, dzz + k3z) * dtt;
	double k4y = ddy(xx + dxx * dtt + k2x * dtt / 2., yy + dyy * dtt + k2y * dtt / 2., zz + dzz * dtt + k2z * dtt / 2., dxx + k3x, dyy + k3y, dzz + k3z) * dtt;
	double k4z = ddz(xx + dxx * dtt + k2x * dtt / 2., yy + dyy * dtt + k2y * dtt / 2., zz + dzz * dtt + k2z * dtt / 2., dxx + k3x, dyy + k3y, dzz + k3z) * dtt;
	mas[0] = xx + dxx * dtt + (1. / 6.) * (k1x + k2x + k3x) * dtt;
	mas[1] = yy + dyy * dtt + (1. / 6.) * (k1y + k2y + k3y) * dtt;
	mas[2] = zz + dzz * dtt + (1. / 6.) * (k1z + k2z + k3z) * dtt;
	mas[3] = dxx + (1. / 6.) * (2. * k2x + 2. * k3x + k1x + k4x);
	mas[4] = dyy + (1. / 6.) * (2. * k2y + 2. * k3y + k1y + k4y);
	mas[5] = dzz + (1. / 6.) * (2. * k2z + 2. * k3z + k1z + k4z);
	return *mas;
}

CBilliardsModelDlg::CBilliardsModelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BILLIARDSMODEL_DIALOG, pParent)
	, edit_coord_x_for_cue_ball(0)
	, edit_coord_y_for_cue_ball(0)
	, edit_coord_x_for_cue_ball_aim(0)
	, edit_coord_y_for_cue_ball_aim(0)
	, power_of_hit(0)
	, V0x(0)
	, V0y(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBilliardsModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COORDINATE_X, edit_coord_x_for_cue_ball);
	DDX_Text(pDX, IDC_EDIT_COORDINATE_Y, edit_coord_y_for_cue_ball);
	DDX_Text(pDX, IDC_EDIT_COORDINATE_X_AIM, edit_coord_x_for_cue_ball_aim);
	DDX_Text(pDX, IDC_EDIT_COORDINATE_Y_AIM, edit_coord_y_for_cue_ball_aim);
	DDX_Text(pDX, IDC_EDIT_HIT_STRENGTH, power_of_hit);
	DDX_Text(pDX, IDC_EDIT_HIT_STRENGTH2, V0x);
	DDX_Text(pDX, IDC_EDIT_HIT_STRENGTH3, V0y);
}

BEGIN_MESSAGE_MAP(CBilliardsModelDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_HIT, &CBilliardsModelDlg::OnBnClickedButtonHit)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_AND_RESTART, &CBilliardsModelDlg::OnBnClickedButtonCleanAndRestart)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// Обработчики сообщений CBilliardsModelDlg

BOOL CBilliardsModelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//DrawBilliarsTable(PicDc, Pic);

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	PicWnd = GetDlgItem(IDC_DRAW_BILLIARDS);			//связываем с ID окон
	PicDc = PicWnd->GetDC();
	PicWnd->GetClientRect(&Pic);

	// перья
	osi_pen_X.CreatePen(			//координатные оси
		PS_SOLID,
		3,
		RGB(255, 0, 0));

	osi_pen_Y.CreatePen(			//координатные оси
		PS_SOLID,
		3,
		RGB(0, 255, 0));

	osi_pen_Z.CreatePen(			//координатные оси
		PS_SOLID,
		3,
		RGB(0, 0, 255));

	board_pen.CreatePen(		//для сетки
		PS_SOLID,					//пунктирная
		2,						//толщина 1 пиксель
		RGB(15, 37, 28));			//цвет  черный

	ball_pen.CreatePen(		//для сетки
		PS_SOLID,					//пунктирная
		2,						//толщина 1 пиксель
		RGB(245, 245, 245));			//цвет  черный

	cue_ball_direction_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(245, 245, 245));			//цвет  черный

	trajectory_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(5, 5, 225));			//цвет  черный

	UpdateData(false);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CBilliardsModelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CBilliardsModelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

double CBilliardsModelDlg::XYZ_to_Display_X(double x, double y, double z)
{
	double fiX = (FiX * 3.14159265) / 180;
	double fiY = (FiY * 3.14159265) / 180;

	double x1 = x;
	double y1 = y * cos(fiX) + z * sin(fiX);
	double z1 = -y * sin(fiX) + z * cos(fiX);

	double x2 = x1 * cos(fiY) + z1 * sin(fiY);
	double y2 = y1;
	double z2 = -x1 * sin(fiY) + z1 * cos(fiY);

	return x2;
}

double CBilliardsModelDlg::XYZ_to_Display_Y(double x, double y, double z)
{
	double fiX = (FiX * 3.14159265) / 180;
	double fiY = (FiY * 3.14159265) / 180;

	double x1 = x;
	double y1 = y * cos(fiX) + z * sin(fiX);
	double z1 = -y * sin(fiX) + z * cos(fiX);

	double x2 = x1 * cos(fiY) + z1 * sin(fiY);
	double y2 = y1;
	double z2 = -x1 * sin(fiY) + z1 * cos(fiY);

	return -y2;
}

void CBilliardsModelDlg::DrawBilliarsTable(CDC* WinDc, CRect WinPic)
{
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double window_signal_width = WinPic.Width();
	double window_signal_height = WinPic.Height();
	xp = (window_signal_width / (xmax - xmin));			//Коэффициенты пересчёта координат по Х
	yp = -(window_signal_height / (ymax - ymin));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, window_signal_width, window_signal_height);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinPic, RGB(27, 75, 58));
	// отрисовка сетки координат
	if (isMButtonDown == true) {
		MemDc->SelectObject(&osi_pen_X); //выбор кисти для осей

		double a = 800000.0;

		MemDc->MoveTo(coordinate_x_min, coordinate_y_min);
		MemDc->LineTo(XYZ_to_Display_X(a, 0, 0), XYZ_to_Display_Y(a, 0, 0));
		//MemDc->LineTo(XYZ_to_Display_X(1450, 10, 0), XYZ_to_Display_Y(1450, 10, 0));

		MemDc->SelectObject(&osi_pen_Y); //выбор кисти для осей

		MemDc->MoveTo(coordinate_x_min, coordinate_y_min);
		MemDc->LineTo(XYZ_to_Display_X(0, a, 0), XYZ_to_Display_Y(0, a, 0));
		//MemDc->LineTo(XYZ_to_Display_X(10, 1450, 0), XYZ_to_Display_Y(10, 1450, 0));

		MemDc->SelectObject(&osi_pen_Z); //выбор кисти для осей

		MemDc->MoveTo(coordinate_x_min, coordinate_y_min);
		MemDc->LineTo(XYZ_to_Display_X(0, 0, a), XYZ_to_Display_Y(0, 0, a));
		//MemDc->LineTo(XYZ_to_Display_X(10, 0, 1450), XYZ_to_Display_Y(10, 0, 1450));
	}

	// отрисовка сетки координат
	MemDc->SelectObject(&board_pen);

	////создаём левый борт стола
	MemDc->MoveTo(DOTS(xmin + xmax * 0.05, ymax - ymax * 0.1));
	MemDc->LineTo(DOTS(xmin + xmax * 0.05, ymin + ymax * 0.1));
	MemDc->MoveTo(DOTS(xmin + xmax * 0.07, ymax - ymax * 0.13));
	MemDc->LineTo(DOTS(xmin + xmax * 0.07, ymin + ymax * 0.13));

	//создаём правый борт стола
	MemDc->MoveTo(DOTS(xmax - xmax * 0.05, ymax - ymax * 0.1));
	MemDc->LineTo(DOTS(xmax - xmax * 0.05, ymin + ymax * 0.1));
	MemDc->MoveTo(DOTS(xmax - xmax * 0.07, ymax - ymax * 0.13));
	MemDc->LineTo(DOTS(xmax - xmax * 0.07, ymin + ymax * 0.13));

	//создаём верхний борт стола
	MemDc->MoveTo(DOTS(xmin + xmax * 0.05, ymax - ymax * 0.1));
	MemDc->LineTo(DOTS(xmax - xmax * 0.05, ymax - ymax * 0.1));
	MemDc->MoveTo(DOTS(xmin + xmax * 0.07, ymax - ymax * 0.13));
	MemDc->LineTo(DOTS(xmax - xmax * 0.07, ymax - ymax * 0.13));

	//создаём нижний борт стола
	MemDc->MoveTo(DOTS(xmin + xmax * 0.05, ymin + ymax * 0.1));
	MemDc->LineTo(DOTS(xmax - xmax * 0.05, ymin + ymax * 0.1));
	MemDc->MoveTo(DOTS(xmin + xmax * 0.07, ymin + ymax * 0.13));
	MemDc->LineTo(DOTS(xmax - xmax * 0.07, ymin + ymax * 0.13));

	MemDc->SelectObject(&ball_pen);

	double margin_x = xmax * 0.07;
	double margin_y = ymax * 0.13;

	//отрисовка прицельного шара
	MemDc->Ellipse(DOTS(coordinate_x_max * 0.75 + coordinate_x_max * 0.01, ((coordinate_y_max + coordinate_y_min) * 0.5) + coordinate_y_max * 0.0165),
		DOTS(coordinate_x_max * 0.75 - coordinate_x_max * 0.01, ((coordinate_y_max + coordinate_y_min) * 0.5) - coordinate_y_max * 0.0165));

	/*MemDc->MoveTo(DOTS(coordinate_x_min, coordinate_y_max));
	MemDc->LineTo(DOTS(coordinate_x_max, (coordinate_y_max + coordinate_y_min)/2));*/

	// вывод на экран
	WinDc->BitBlt(0, 0, window_signal_width, window_signal_height, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

void CBilliardsModelDlg::DrawBilliarsCueBall(CDC* WinDc, CRect WinPic, CPoint point, vector<sBall> vecBalls)
{
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double window_signal_width = WinPic.Width();
	double window_signal_height = WinPic.Height();
	xp = (window_signal_width / (xmax - xmin));			//Коэффициенты пересчёта координат по Х
	yp = -(window_signal_height / (ymax - ymin));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, window_signal_width, window_signal_height);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinPic, RGB(27, 75, 58));
	// отрисовка сетки координат

	if (isMButtonDown == true) {
		MemDc->SelectObject(&osi_pen_X); //выбор кисти для осей

		double a = 800000.0;

		MemDc->MoveTo(coordinate_x_min, coordinate_y_min);
		MemDc->LineTo(XYZ_to_Display_X(a, 0, 0), XYZ_to_Display_Y(a, 0, 0));
		//MemDc->LineTo(XYZ_to_Display_X(1450, 10, 0), XYZ_to_Display_Y(1450, 10, 0));

		MemDc->SelectObject(&osi_pen_Y); //выбор кисти для осей

		MemDc->MoveTo(coordinate_x_min, coordinate_y_min);
		MemDc->LineTo(XYZ_to_Display_X(0, a, 0), XYZ_to_Display_Y(0, a, 0));
		//MemDc->LineTo(XYZ_to_Display_X(10, 1450, 0), XYZ_to_Display_Y(10, 1450, 0));

		MemDc->SelectObject(&osi_pen_Z); //выбор кисти для осей

		MemDc->MoveTo(coordinate_x_min, coordinate_y_min);
		MemDc->LineTo(XYZ_to_Display_X(0, 0, a), XYZ_to_Display_Y(0, 0, a));
		//MemDc->LineTo(XYZ_to_Display_X(10, 0, 1450), XYZ_to_Display_Y(10, 0, 1450));
	}

	MemDc->SelectObject(&board_pen);

	//создаём левый борт стола
	MemDc->MoveTo(DOTS(xmin + xmax * 0.05, ymax - ymax * 0.1));
	MemDc->LineTo(DOTS(xmin + xmax * 0.05, ymin + ymax * 0.1));
	MemDc->MoveTo(DOTS(xmin + xmax * 0.07, ymax - ymax * 0.13));
	MemDc->LineTo(DOTS(xmin + xmax * 0.07, ymin + ymax * 0.13));

	//создаём правый борт стола
	MemDc->MoveTo(DOTS(xmax - xmax * 0.05, ymax - ymax * 0.1));
	MemDc->LineTo(DOTS(xmax - xmax * 0.05, ymin + ymax * 0.1));
	MemDc->MoveTo(DOTS(xmax - xmax * 0.07, ymax - ymax * 0.13));
	MemDc->LineTo(DOTS(xmax - xmax * 0.07, ymin + ymax * 0.13));

	//создаём верхний борт стола
	MemDc->MoveTo(DOTS(xmin + xmax * 0.05, ymax - ymax * 0.1));
	MemDc->LineTo(DOTS(xmax - xmax * 0.05, ymax - ymax * 0.1));
	MemDc->MoveTo(DOTS(xmin + xmax * 0.07, ymax - ymax * 0.13));
	MemDc->LineTo(DOTS(xmax - xmax * 0.07, ymax - ymax * 0.13));

	//создаём нижний борт стола
	MemDc->MoveTo(DOTS(xmin + xmax * 0.05, ymin + ymax * 0.1));
	MemDc->LineTo(DOTS(xmax - xmax * 0.05, ymin + ymax * 0.1));
	MemDc->MoveTo(DOTS(xmin + xmax * 0.07, ymin + ymax * 0.13));
	MemDc->LineTo(DOTS(xmax - xmax * 0.07, ymin + ymax * 0.13));

	MemDc->SelectObject(&ball_pen);

	////отрисовка прицельного шара
	//MemDc->Ellipse(DOTS(coordinate_x_max * 0.75 + coordinate_x_max * 0.01, ((coordinate_y_max + coordinate_y_min) * 0.5) + coordinate_y_max * 0.0165),
	//	DOTS(coordinate_x_max * 0.75 - coordinate_x_max * 0.01, ((coordinate_y_max + coordinate_y_min) * 0.5) - coordinate_y_max * 0.0165));

	//MemDc->Ellipse(DOTS(edit_coord_x_for_cue_ball + coordinate_x_max * 0.01, Height - edit_coord_y_for_cue_ball + coordinate_y_max * 0.0165),
	//	DOTS(edit_coord_x_for_cue_ball - coordinate_x_max * 0.01, Height - edit_coord_y_for_cue_ball - coordinate_y_max * 0.0165));


	for (int i = 0; i < vecBalls.size(); i++)
	{
		MemDc->Ellipse(DOTS(vecBalls[i].px - vecBalls[i].radius * 280, Height - vecBalls[i].py - vecBalls[i].radius * 280),
			DOTS(vecBalls[i].px + vecBalls[i].radius * 280, Height - vecBalls[i].py + vecBalls[i].radius * 280));
	}


	if (isLButtonDown == true) {

		int indexBall = 0;
		bool isBallChoice = false;

		for (int i = 0; i < vecBalls.size(); i++) {

			if (edit_coord_x_for_cue_ball <= (vecBalls[i].px + vecBalls[i].radius * 280) && edit_coord_x_for_cue_ball >= (vecBalls[i].px - vecBalls[i].radius * 280)
				&& edit_coord_y_for_cue_ball <= (vecBalls[i].py + vecBalls[i].radius * 280) && edit_coord_y_for_cue_ball >= (vecBalls[i].py - vecBalls[i].radius * 280))
			{
				indexBall = i;
				isBallChoice = true;
			}
		}

		if (isBallChoice == true)
		{
			MemDc->SelectObject(&cue_ball_direction_pen);
			MemDc->MoveTo(DOTS(vecBalls[indexBall].px, Height - vecBalls[indexBall].py));
			MemDc->LineTo(DOTS(edit_coord_x_for_cue_ball_aim, Height - edit_coord_y_for_cue_ball_aim));

			/*if (edit_coord_x_for_cue_ball_aim <= (vecBalls[indexBall].px + 20 * vecBalls[indexBall].radius * 280)
				&& edit_coord_x_for_cue_ball_aim >= (vecBalls[indexBall].px - 20 * vecBalls[indexBall].radius * 280)
				&& edit_coord_y_for_cue_ball_aim <= (vecBalls[indexBall].py + 20 * vecBalls[indexBall].radius * 280)
				&& edit_coord_y_for_cue_ball_aim >= (vecBalls[indexBall].py - 20 * vecBalls[indexBall].radius * 280))*/
				/*double gipotenuza_x = (edit_coord_x_for_cue_ball_aim - vecBalls[indexBall].px) * (edit_coord_x_for_cue_ball_aim - vecBalls[indexBall].px);
				double gipotenuza_y = (edit_coord_y_for_cue_ball_aim - vecBalls[indexBall].py) * (edit_coord_y_for_cue_ball_aim - vecBalls[indexBall].py);

				if (sqrt(gipotenuza_x + gipotenuza_y) < 20 * vecBalls[indexBall].radius * 280)
				{
					MemDc->MoveTo(DOTS(vecBalls[indexBall].px, Height - vecBalls[indexBall].py));
					MemDc->LineTo(DOTS(edit_coord_x_for_cue_ball_aim, Height - edit_coord_y_for_cue_ball_aim));
				}
				else
				{
					double cosa = sqrt(gipotenuza_x) / sqrt(gipotenuza_x + gipotenuza_y);
					double sina = sqrt(gipotenuza_y) / sqrt(gipotenuza_x + gipotenuza_y);

					double x = sqrt(gipotenuza_x);
					double y = sqrt(gipotenuza_y);

					MemDc->MoveTo(DOTS(vecBalls[indexBall].px, Height - vecBalls[indexBall].py));
					MemDc->LineTo(DOTS(x, Height - y));
				}*/
		}
	}

	/*MemDc->Ellipse(DOTS( + coordinate_x_max * 0.01, ((coordinate_y_max + coordinate_y_min) * 0.5) + coordinate_y_max * 0.0165),
		DOTS(coordinate_x_max * 0.75 - coordinate_x_max * 0.01, ((coordinate_y_max + coordinate_y_min) * 0.5) - coordinate_y_max * 0.0165));

	MemDc->Ellipse(DOTS(edit_coord_x_for_cue_ball + coordinate_x_max * 0.01, Height - edit_coord_y_for_cue_ball + coordinate_y_max * 0.0165),
		DOTS(edit_coord_x_for_cue_ball - coordinate_x_max * 0.01, Height - edit_coord_y_for_cue_ball - coordinate_y_max * 0.0165));*/

		/*MemDc->MoveTo(DOTS(coordinate_x_min, coordinate_y_max));
		MemDc->LineTo(DOTS(coordinate_x_max, (coordinate_y_max + coordinate_y_min)/2));*/

		// вывод на экран
	WinDc->BitBlt(0, 0, window_signal_width, window_signal_height, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

void CBilliardsModelDlg::OnBnClickedButtonHit()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(true);

	double timestep = 0.5;
	double time = 20;
	int count = 0;

	ofstream out("CountOfIterInClick.txt");
	for (int i = 0; i < time; i += timestep)
	{
		double trajectory[7] = { vecBalls[0].px, vecBalls[0].py, vecBalls[0].pz, vecBalls[0].vx, vecBalls[0].vy, vecBalls[0].vz, timestep };
		RungeKuttaMethod(trajectory);

		CPoint point;

		point.x = trajectory[0];
		point.y = trajectory[1];

		vecBalls[0].px = trajectory[0];
		vecBalls[0].py = trajectory[1];
		vecBalls[0].pz = trajectory[2];
		vecBalls[0].vx = trajectory[3];
		vecBalls[0].vy = trajectory[4];
		vecBalls[0].vz = trajectory[5];

		edit_coord_x_for_cue_ball = vecBalls[0].px;
		edit_coord_y_for_cue_ball = vecBalls[0].py;

		for (int j = 0; j < vecBalls.size(); j++)
		{
			if (vecBalls[j].px < coordinate_x_min + vecBalls[j].radius * 280 || vecBalls[j].px > coordinate_x_max - vecBalls[j].radius * 280)
			{
				vecBalls[j].vx = -vecBalls[j].vx;
			}
			if (vecBalls[j].py < coordinate_y_min + vecBalls[j].radius * 280 || vecBalls[j].py > coordinate_y_max - vecBalls[j].radius * 280)
			{
				vecBalls[j].vy = -vecBalls[j].vy;
			}
		}

		DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);

		out << vecBalls[0].vx << " " << vecBalls[0].vy << "\n";

		count++;
		UpdateData(false);
	}
	UpdateData(false);
}


void CBilliardsModelDlg::OnBnClickedButtonCleanAndRestart()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(true);
	//DrawBilliarsTable(PicDc, Pic);

	edit_coord_x_for_cue_ball = 0;
	edit_coord_y_for_cue_ball = 0;

	CPoint point;
	point.x = edit_coord_x_for_cue_ball;
	point.y = edit_coord_y_for_cue_ball;

	vecBalls.clear();
	//AddBall(edit_coord_x_for_cue_ball, edit_coord_y_for_cue_ball, 100, vecBalls);

	DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);
	UpdateData(false);
}


void CBilliardsModelDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	isRButtonDown = true;

	edit_coord_x_for_cue_ball = point.x;
	edit_coord_y_for_cue_ball = point.y;

	UpdateData(false);
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CBilliardsModelDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	UpdateData(true);
	if (isRButtonDown == true) {
		edit_coord_x_for_cue_ball = point.x;
		edit_coord_y_for_cue_ball = point.y;
	}
	if (isLButtonDown == true) {
		edit_coord_x_for_cue_ball_aim = point.x;
		edit_coord_y_for_cue_ball_aim = point.y;
		DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);
	}
	if (isMButtonDown == true) {
		DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);
	}

	UpdateData(false);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CBilliardsModelDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	isRButtonDown = false;

	edit_coord_x_for_cue_ball = point.x;
	edit_coord_y_for_cue_ball = point.y;

	int countCollision = 0;
	bool isCollision = false;

	if (vecBalls.size() == 0) {
		AddBall(edit_coord_x_for_cue_ball, edit_coord_y_for_cue_ball, vecBalls);
		DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);
	}
	else {
		for (int i = 0; i < vecBalls.size(); i++) {
			double razn_x = vecBalls[i].px - edit_coord_x_for_cue_ball;
			double razn_y = vecBalls[i].py - edit_coord_y_for_cue_ball;
			double diameter = vecBalls[i].radius * 2 * 280;

			double line = sqrt((razn_x * razn_x) + (razn_y * razn_y));

			if (line <= diameter) {
				countCollision++;
				isCollision = true;
			}
		}

		if (countCollision == 0) {
			AddBall(edit_coord_x_for_cue_ball, edit_coord_y_for_cue_ball, vecBalls);
			DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);
		}
	}

	UpdateData(false);
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CBilliardsModelDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	isLButtonDown = true;

	int indexBall = 0;
	bool isBallChoice = false;

	for (int i = 0; i < vecBalls.size(); i++) {

		if (point.x <= (vecBalls[i].px + vecBalls[i].radius * 280) && point.x >= (vecBalls[i].px - vecBalls[i].radius * 280)
			&& point.y <= (vecBalls[i].py + vecBalls[i].radius * 280) && point.y >= (vecBalls[i].py - vecBalls[i].radius * 280))
		{
			indexBall = i;
			isBallChoice = true;
		}
	}
	if (isBallChoice == true) {
		edit_coord_x_for_cue_ball = vecBalls[indexBall].px;
		edit_coord_y_for_cue_ball = vecBalls[indexBall].py;
	}
	else
	{
		edit_coord_x_for_cue_ball = 0;
		edit_coord_y_for_cue_ball = 0;
	}

	UpdateData(false);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CBilliardsModelDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	isLButtonDown = false;
	UpdateData(true);

	edit_coord_x_for_cue_ball_aim = point.x;
	edit_coord_y_for_cue_ball_aim = point.y;

	/*if (edit_coord_x_for_cue_ball_aim < edit_coord_x_for_cue_ball) {
		V0x = - fabs(edit_coord_x_for_cue_ball_aim - edit_coord_x_for_cue_ball)
	}*/

	V0x = (edit_coord_x_for_cue_ball_aim - edit_coord_x_for_cue_ball) / 10;
	V0y = (edit_coord_y_for_cue_ball_aim - edit_coord_y_for_cue_ball) / 10;

	power_of_hit = sqrt(V0x * V0x + V0y * V0y) / 10;

	int index = 0;
	bool is = false;
	for (int i = 0; i < vecBalls.size(); i++) {
		if (fabs(vecBalls[i].px - edit_coord_x_for_cue_ball) < 0.1
			&& fabs(vecBalls[i].py - edit_coord_y_for_cue_ball) < 0.1) {
			index = i;
			is = true;
		}

	}
	if (is == true)
	{
		vecBalls[index].vx = V0x;
		vecBalls[index].vy = V0y;
		vecBalls[index].vz = V0z;
	}

	UpdateData(false);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CBilliardsModelDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (isMButtonDown == false)
		isMButtonDown = true;
	else
		isMButtonDown = false;

	DrawBilliarsCueBall(PicDc, Pic, point, vecBalls);

	CDialogEx::OnMButtonDown(nFlags, point);
}
