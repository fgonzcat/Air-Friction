///////////////////////////
// AIR.CC: Air friction  //
///////////////////////////
// Creado: 25-Agosto-2008.
// Ultima modificacion: 25-Agosto-2008.
// Autor: Felipe Gonzalez.

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
 #include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

/***************************************************************/
/************************* WINDOW ******************************/
const double ancho=1300.0;
const double alto=700.0;
const double profundidad=700.0;
unsigned char Buttons[3] = {0};
double xmax=ancho/2, ymax=alto/2, xmin=-ancho/2, ymin=-alto/2, zmin=-profundidad/2, zmax=profundidad/2;
double zoom = 3.1*ymax/(2*tan(M_PI/8));
double rotx = -70, roty = 0, rotz = 0, tx = 0, ty = 0;
int lastx=0, lasty=0;
double t=0, dt=2e-2;
bool pausa=true;
/***************************************************************/
/***************************************************************/

//------------------------------------------GLUT FUNCTIONS-----------------------------------------------------------//
void Inicializa(void){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

   GLfloat light_ambient[] = {0.0, 0, 0, 1};
   GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1};
   GLfloat light_specular[] ={1, 1, 1, 1};
   GLfloat light_position[] ={1.0, 0.0, 0.0, 0}; 	// {x,y,z,1}: Sun-like light in the position (x,y,z)
   																// {x,y,z,0}: Directional light
   glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   
   
//   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glClearColor(0,0,0,0);
   glEnable(GL_DEPTH_TEST);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45,1,1,2*ancho);
    
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0,0,zoom,0,0,0,0,1,0); 
}

void Time(void){ if (pausa==false){t+=dt;}	glutPostRedisplay();}

void Mouse(int b,int s,int x,int y){
	lastx=x;
	lasty=y;
	switch(b){
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}

void Motion(int x,int y){
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[1] ){	zoom -= (double) 10.1f * diffx;}
	else{
		if( Buttons[0] ){
			rotx += (double) 0.5f * diffy;
			roty += (double) 0.5f * diffx;
		}
		else{
			if( Buttons[2] ){
				tx += (double) 0.8f * diffx;
				ty -= (double) 0.8f * diffy;
			}
		}
	}
	glutPostRedisplay();
}

void reshape3D(int w, int h){
	// prevent divide by 0 error when minimised
	if(w==0) 
		h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(double)w/h,0.1,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshape(int w,int h){
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w<=h)
		glOrtho(-1.5,1.5,-1.5*(GLfloat)h/(GLfloat)w,1.5*(GLfloat)h/(GLfloat)w,-10.0,10.0);
	else
		glOrtho(-1.5*(GLfloat)w/(GLfloat)h,1.5*(GLfloat)w/(GLfloat)h,-1.5,1.5,-10.0,10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y){
	switch (key){
		case 27: exit(0);
		break;
		case 'q': exit(0);
		break;
		case 'p': pausa=!pausa;
		break;
	}
}

void drawString(char *s){
     unsigned int i;
     for (i = 0; i < strlen(s); i++){	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, s[i]);}
}


void drawStringBig(char *s){
     unsigned int i;
     for (i = 0; i < strlen(s); i++){	glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]);}
}
//-------------------------------------------------------------------------------------------------------------------//

//---------------- BOUNDARY CONDITIONS ------------------------//
double R=20;
// Posicion R(0)=(X1,Y1)
const double xo=xmin, yo=100.1+R;
double X1=xo, Y1=yo;
// Velocidad R'(0)=(X2,Y2)
const double vox=70, voy=80;
double X2=vox, Y2=voy;
//------------------------------------------------------------//
double X_teo=xo, Y_teo=yo, place_teo=0, place_real=0;
double	k1X1, k2X1, k3X1, k4X1;
double	k1X2, k2X2, k3X2, k4X2;
double	k1Y1, k2Y1, k3Y1, k4Y1;
double	k1Y2, k2Y2, k3Y2, k4Y2;

double m=1;
double g=9.8;
double Fo=0.001;

double fX1(double X1,double X2,double Y1,double Y2){ return X2;}
double fY1(double X1,double X2,double Y1,double Y2){ return Y2;}
double fX2(double X1,double X2,double Y1,double Y2){ return -(Fo/m)*sqrt(X2*X2+Y2*Y2)*X2;}
double fY2(double X1,double X2,double Y1,double Y2){ return -(Fo/m)*sqrt(X2*X2+Y2*Y2)*Y2-g;}

void Solver(){
	if (place_teo==0){
		X_teo=xo+vox*t;
		Y_teo=yo+voy*t-0.5*g*t*t;
	}
	/************ Fixing k1 ***************/
	k1X1=dt*fX1(X1,X2,Y1,Y2);
	k1Y1=dt*fY1(X1,X2,Y1,Y2);
		
	k1X2=dt*fX2(X1,X2,Y1,Y2);
	k1Y2=dt*fY2(X1,X2,Y1,Y2);
	
	/************ Fixing k2 ***************/
	k2X1=dt*fX1(X1+k1X1/2,X2+k1X2/2,Y1+k1Y1/2,Y2+k1Y2/2);
	k2Y1=dt*fY1(X1+k1X1/2,X2+k1X2/2,Y1+k1Y1/2,Y2+k1Y2/2);
	
	k2X2=dt*fX2(X1+k1X1/2,X2+k1X2/2,Y1+k1Y1/2,Y2+k1Y2/2);
	k2Y2=dt*fY2(X1+k1X1/2,X2+k1X2/2,Y1+k1Y1/2,Y2+k1Y2/2);
	
	/************ Fixing k3 ***************/
	k3X1=dt*fX1(X1+k2X1/2,X2+k2X2/2,Y1+k2Y1/2,Y2+k2Y2/2);
	k3Y1=dt*fY1(X1+k2X1/2,X2+k2X2/2,Y1+k2Y1/2,Y2+k2Y2/2);

	k3X2=dt*fX2(X1+k2X1/2,X2+k2X2/2,Y1+k2Y1/2,Y2+k2Y2/2);
	k3Y2=dt*fY2(X1+k2X1/2,X2+k2X2/2,Y1+k2Y1/2,Y2+k2Y2/2);

	/************ Fixing k4 ***************/
	k4X1=dt*fX1(X1+k3X1,X2+k3X2,Y1+k3Y1,Y2+k3Y2);
	k4Y1=dt*fY1(X1+k3X1,X2+k3X2,Y1+k3Y1,Y2+k3Y2);

	k4X2=dt*fX2(X1+k3X1,X2+k3X2,Y1+k3Y1,Y2+k3Y2);
	k4Y2=dt*fY2(X1+k3X1,X2+k3X2,Y1+k3Y1,Y2+k3Y2);

	if (place_real==0){
		X1=X1+(k1X1+2*k2X1+2*k3X1+k4X1)/6;
		Y1=Y1+(k1Y1+2*k2Y1+2*k3Y1+k4Y1)/6;
		X2=X2+(k1X2+2*k2X2+2*k3X2+k4X2)/6;
		Y2=Y2+(k1Y2+2*k2Y2+2*k3Y2+k4Y2)/6;
	}
	
	if (Y1<=R && place_real==0){ Y1=R; place_real=1; cout << "dmax_real=" << X1-xo << endl;}
	if (X1>=xmax){ X2=-X2;}

	if (Y_teo<=R && place_teo==0){ Y_teo=R; place_teo=1; cout << "dmax_teo=" << X_teo-xo << endl;}
//	if (X1<=xmin){ X2=-X2;}
}

void Dibuja(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glShadeModel(GL_FLAT);
	
	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,0,1);

	if (pausa==false){Solver();}
//	cout << X1 << "\t" << Y1 << endl;
	
	glPushMatrix();
	glTranslatef(X1,0,Y1);
//	glRotatef(120, 0.0, 1.0, 0.0);
//	glRotatef(100*t, 0.0, 0.0, 1.0);
	glColor3d(1,0,0); glutWireSphere(R, 20, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(X_teo,0,Y_teo);
	glColor3d(1,0,1); glutWireSphere(R, 20, 16);
	glPopMatrix();
	
	glColor3d(0,1,0);
	glBegin(GL_LINES);
	for(double i=xmin;i<=xmax;i+=50){	glVertex3f(i,ymin,0);	glVertex3f(i,ymax,0);}
	for(double i=ymin;i<=ymax;i+=50){	glVertex3f(xmin,i,0);	glVertex3f(xmax,i,0);}

	glColor3d(1,1,0);
	/* BUILDING */
	glVertex3f(xmin,-100,0);	glVertex3f(xmin,-100,zmax);
	glVertex3f(xmin,100,0);	glVertex3f(xmin,100,zmax);
	glVertex3f(xmin-100,-100,0);	glVertex3f(xmin-100,-100,zmax);
	glVertex3f(xmin-100,100,0);	glVertex3f(xmin-100,100,zmax);

	glVertex3f(xmin,-100,zmax);	glVertex3f(xmin,100,zmax);
	glVertex3f(xmin-100,-100,zmax);	glVertex3f(xmin-100,100,zmax);
	glVertex3f(xmin,-100,0);	glVertex3f(xmin,100,0);
	glVertex3f(xmin-100,-100,0);	glVertex3f(xmin-100,100,0);

	glVertex3f(xmin-100,100,zmax);	glVertex3f(xmin,100,zmax);
	glVertex3f(xmin-100,-100,zmax);	glVertex3f(xmin,-100,zmax);
	glVertex3f(xmin-100,100,0);	glVertex3f(xmin,100,0);
	glVertex3f(xmin-100,-100,0);	glVertex3f(xmin,-100,0);
	
	// Window
	glVertex3f(xmin,-50,300);	glVertex3f(xmin,50,300);
	glVertex3f(xmin,-50,200);	glVertex3f(xmin,50,200);
	glVertex3f(xmin,-50,200);	glVertex3f(xmin,-50,300);
	glVertex3f(xmin,50,200);	glVertex3f(xmin,50,300);
	
	
	glEnd();	

	glutSwapBuffers();
}
//------------------------------------------------------------------------------------------------//

//************************************MAIN****************************************//
int main(int argc, char** argv){
	// Principal Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(int(ancho),int(alto));
	glutInitWindowPosition(0,0);
	glutCreateWindow("AIR FRICTION");
	// Program's Functions:
	Inicializa();

	glutIdleFunc(Time);
	glutDisplayFunc(Dibuja);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutReshapeFunc(reshape3D);
	glutMotionFunc(Motion);

	glutMainLoop();
	return 0;
}
//********************************************************************************//
