/*
	Se desea encontrar la posici\'on de una part\'icula lanzada en la tierra considerando el roce del aire:
	mR''(t)=-Fo|R'(t)|R'(t)-mg y^
	
	Defino
	X1=(x11,x12)=R(t)
	X2=(x21,x22)=R'(t)
	
	Así,
	X1'=X2
	X2'=-Fo|V|V-mg y^
	
	Por componentes, se tiene
	x11'=x21
	x12'=x22
	x21'=-Fo|(x21,x22)|x21
	x22'=-Fo|(x21,x22)|x22-mg
	
	Para usar Runge Kutta de 4to orden, necesito 4 k-es para cada ecuación, que denotamos por
	
	
*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

double dt=1e-3;
double m=1;
double g=9.8;
double Fo=0;

double fx1(double x1,double x2,double y1,double y2){ return x2;}
double fy1(double x1,double x2,double y1,double y2){ return y2;}
double fx2(double x1,double x2,double y1,double y2){ return -(Fo/m)*sqrt(x2*x2+y2*y2)*x2;}
double fy2(double x1,double x2,double y1,double y2){ return -(Fo/m)*sqrt(x2*x2+y2*y2)*y2-g;}


int main(){
	// Posicion R(0)=(x1,y1)
	double x1=0, y1=0;
	// Velocidad R'(0)=(x2,y2)
	double x2=10, y2=10;
	double	k1x1, k2x1, k3x1, k4x1;
	double	k1x2, k2x2, k3x2, k4x2;
	double	k1y1, k2y1, k3y1, k4y1;
	double	k1y2, k2y2, k3y2, k4y2;
		
	ofstream data("rk4-num.dat");
	for (double t=0; t<3; t+=dt){
	/************ Fixing k1 ***************/
	data << x1 << "\t" << y1 << endl;
	k1x1=dt*fx1(x1,x2,y1,y2);
	k1y1=dt*fy1(x1,x2,y1,y2);
		
	k1x2=dt*fx2(x1,x2,y1,y2);
	k1y2=dt*fy2(x1,x2,y1,y2);
	
	/************ Fixing k2 ***************/
	k2x1=dt*fx1(x1+k1x1/2,x2+k1x2/2,y1+k1y1/2,y2+k1y2/2);
	k2y1=dt*fy1(x1+k1x1/2,x2+k1x2/2,y1+k1y1/2,y2+k1y2/2);
	
	k2x2=dt*fx2(x1+k1x1/2,x2+k1x2/2,y1+k1y1/2,y2+k1y2/2);
	k2y2=dt*fy2(x1+k1x1/2,x2+k1x2/2,y1+k1y1/2,y2+k1y2/2);
	
	/************ Fixing k3 ***************/
	k3x1=dt*fx1(x1+k2x1/2,x2+k2x2/2,y1+k2y1/2,y2+k2y2/2);
	k3y1=dt*fy1(x1+k2x1/2,x2+k2x2/2,y1+k2y1/2,y2+k2y2/2);

	k3x2=dt*fx2(x1+k2x1/2,x2+k2x2/2,y1+k2y1/2,y2+k2y2/2);
	k3y2=dt*fy2(x1+k2x1/2,x2+k2x2/2,y1+k2y1/2,y2+k2y2/2);

	/************ Fixing k4 ***************/
	k4x1=dt*fx1(x1+k3x1,x2+k3x2,y1+k3y1,y2+k3y2);
	k4y1=dt*fy1(x1+k3x1,x2+k3x2,y1+k3y1,y2+k3y2);

	k4x2=dt*fx2(x1+k3x1,x2+k3x2,y1+k3y1,y2+k3y2);
	k4y2=dt*fy2(x1+k3x1,x2+k3x2,y1+k3y1,y2+k3y2);

	x1=x1+(k1x1+2*k2x1+2*k3x1+k4x1)/6;
	y1=y1+(k1y1+2*k2y1+2*k3y1+k4y1)/6;
	x2=x2+(k1x2+2*k2x2+2*k3x2+k4x2)/6;
	y2=y2+(k1y2+2*k2y2+2*k3y2+k4y2)/6;
	}
	return 0;
}
