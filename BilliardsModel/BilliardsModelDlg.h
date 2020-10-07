
// BilliardsModelDlg.h: файл заголовка
//

#include <vector>

#pragma once

struct sBall
{
	double px, py, pz;
	double vx, vy, vz;
	double ax, ay, az;
	double radius;
	double mass;

	int id;
};

// Диалоговое окно CBilliardsModelDlg
class CBilliardsModelDlg : public CDialogEx
{
// Создание
public:
	CBilliardsModelDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BILLIARDSMODEL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CWnd* PicWnd;				//области рисования
	CDC* PicDc;
	CRect Pic;

	CPen board_pen;				//ручки
	CPen ball_pen;
	CPen cue_ball_direction_pen;
	CPen trajectory_pen;
	CPen osi_pen_X;
	CPen osi_pen_Y;
	CPen osi_pen_Z;

	double FiX = 180.0;
	double FiY = 0.0;

	/*int Weight = 1450;
	int Height = 938;*/

	int Weight = 1450;
	int Height = 938;

	double xp = 0, yp = 0,				//коэфициенты пересчета
		xmin = 0, xmax = Weight,				//максисимальное и минимальное значение х 
		ymin = 0, ymax = Height;				//максисимальное и минимальное значение y

	double coordinate_xp = 0;
	double coordinate_yp = 0;
	double coordinate_x_min = xmin + xmax * 0.07;
	double coordinate_x_max = xmax - xmax * 0.07;
	double coordinate_y_max = ymax - ymax * 0.13;
	double coordinate_y_min = ymin + ymax * 0.13;

	bool isRButtonDown = false;
	bool isLButtonDown = false;
	bool isMButtonDown = false;

	double f_tr = 0.45;
	double g = 9.8066;

	std::vector<sBall> vecBalls;

	void DrawBilliarsTable(CDC*, CRect);
	void DrawBilliarsCueBall(CDC*, CRect, CPoint, std::vector<sBall>);
	double RungeKuttaMethod(double* mas);

	double ddx(double x, double y, double z, double dx, double dy, double dz);  //второй закон на ось X
	double ddy(double x, double y, double z, double dx, double dy, double dz); //второй закон на ось Y
	double ddz(double x, double y, double z, double dx, double dy, double dz); //второй закон на ось Y

	double XYZ_to_Display_X(double x, double y, double z);
	double XYZ_to_Display_Y(double x, double y, double z);

	afx_msg void OnBnClickedButtonHit();
	afx_msg void OnBnClickedButtonCleanAndRestart();
	double edit_coord_x_for_cue_ball;
	double edit_coord_y_for_cue_ball;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	double edit_coord_x_for_cue_ball_aim;
	double edit_coord_y_for_cue_ball_aim;
	double power_of_hit;
	double V0x;
	double V0y;
	double V0z = 0;
};
