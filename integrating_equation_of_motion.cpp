#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
#include<sstream> // int to string
using namespace std;

const double w=1;
const double g=9.8;
const double drag=2.000;
const double xo=0,yo=0, vox=1,voy=40, tau=0.01, tmax=10;

double acceleration_x(double x, double y=0, double vx=0, double vy=0)
{
 //return -w*w*sin(x); // simple pendulum
 //return 0.0; // free fall 
 return -drag*vx; // free fall + air drag
}
double acceleration_y(double x, double y=0, double vx=0, double vy=0)
{
 //return -w*w*sin(x); // simple pendulum
 //return -g; // free fall 
 return -g -drag*vy; // free fall + air drag
}



void Euler(ofstream &file, double &x, double &y, double &vx, double &vy,  double &t, double tmax)
{
 file << t << " " << x << " " << y << " " << vx << " " << vy << endl;
 double xtmp=x,   ytmp=y;
 double vxtmp=vx, vytmp=vy;
 x += tau*vx;
 y += tau*vy;
 vx+= tau*acceleration_x(xtmp,ytmp,vxtmp,vytmp);
 vy+= tau*acceleration_y(xtmp,ytmp,vxtmp,vytmp);
 t += tau;
 if (t<=tmax) Euler(file,x,y,vx,vy,t,tmax);
}

void EulerCromer(ofstream &file, double &x, double &y, double &vx, double &vy,  double &t, double tmax)
{
 file << t << " " << x << " " << y << " " << vx << " " << vy << endl;
 double xtmp=x,   ytmp=y;
 double vxtmp=vx, vytmp=vy;
 vx += tau*acceleration_x(xtmp,ytmp,vxtmp,vytmp);
 vy += tau*acceleration_y(xtmp,ytmp,vxtmp,vytmp);
 x += tau*vx;
 y += tau*vy;
 t += tau;
 if (t<=tmax) EulerCromer(file,x,y,vx,vy,t,tmax);
}


void VelocityVerlet(ofstream &file, double &x, double &y, double &vx, double &vy,  double &t, double tmax)
{
 file << t << " " << x << " " << y << " " << vx << " " << vy << endl;
 double xtmp=x,   ytmp=y;
 double vxtmp=vx, vytmp=vy;
 x = x + tau*vx + 0.5*tau*tau*acceleration_x(xtmp,ytmp,vxtmp,vytmp);
 y = y + tau*vy + 0.5*tau*tau*acceleration_y(xtmp,ytmp,vxtmp,vytmp);
 double  v1=vx, v2=vy;
 vx += 0.5*tau*( acceleration_x(xtmp,ytmp,vxtmp,vytmp) + acceleration_x(x,y,v1,v2) );
 vy += 0.5*tau*( acceleration_y(xtmp,ytmp,vxtmp,vytmp) + acceleration_y(x,y,v1,v2) );
 t += tau;
 if (t<=tmax) VelocityVerlet(file,x,y,vx,vy,t,tmax);
}


int main()
{
 double x=xo, vx=vox;
 double y=yo, vy=voy;
 double t=0;
 ofstream arch;

 cout << "########### INTEGRATING EQUATIONS OF MOTION ###########"   << endl;
 cout << "Initial conditions: r(0)= (" << xo << ", " << yo << "),  v(0)= (" << vox << ", " << voy << ")\n"    << endl;
 cout << " ---> Using the Euler algorithm... (Euler.dat)"  << endl;
 arch.open("Euler.dat");
 arch << "#t   x   y   vx   vy "  << endl;
 Euler(arch,x,y,vx,vy,t,tmax);
 arch.close();
 
 x=xo;y=yo; vx=vox; vy=voy; t=0;
 cout << " ---> Using the Euler-Cromer algorithm... (EulerCromer.dat)"  << endl;
 arch.open("EulerCromer.dat");
 arch << "#t   x   y   vx   vy "  << endl;
 EulerCromer(arch,x,y,vx,vy,t,tmax);
 arch.close();
 
 x=xo;y=yo; vx=vox; vy=voy; t=0;
 cout << " ---> Using Velocity Verlet algorithm... (VelocityVerlet.dat)"  << endl;
 arch.open("VelocityVerlet.dat");
 arch << "#t   x   y   vx   vy "  << endl;
 VelocityVerlet(arch,x,y,vx,vy,t,tmax);
 arch.close();

 x=xo;y=yo; vx=vox; vy=voy; t=0;
 cout << " ---> Analytical solution with no drag... (No-drag_trajectory.dat)"  << endl;
 arch.open("No-drag_trajectory.dat");
 arch << "#t   x   y   vx   vy "  << endl;
 for (t=0; t<tmax; t+=tau)  arch << t  << " "  << xo+vox*t << " "  << yo+voy*t-g*t*t*0.5 << " "  << vox << " "  << voy-g*t  << endl;
 arch.close();



 return 0;
}
