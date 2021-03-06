#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

//#define pi (2*acos(0.0))
#define pi 3.1416

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double tx=0;
double rC=8;
float bll=0;
float movx=0;

float posx=100;
float posy=100;
float posz=0;

float lx=-1/sqrt(2);
float ly=-1/sqrt(2);
float lz=0;

float rx=-1/sqrt(2);
float ry=-1/sqrt(2);
float rz=0;

float ux=0;
float uy=0;
float uz=1;


boolean cheakUp=true;
boolean isThrow=true;
struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}
//draws a cube of side length 2a
void drawCube(double a)
{
    glBegin(GL_QUADS);{
		glVertex3f( a, a, a);
		glVertex3f( a,-a, a);
		glVertex3f(-a,-a, a);
		glVertex3f(-a, a, a);

		glVertex3f( a, a,-a);
		glVertex3f( a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a, a,-a);

		glVertex3f( a, a, a);
		glVertex3f( a, a,-a);
		glVertex3f( a,-a,-a);
		glVertex3f( a,-a, a);

		glVertex3f(-a, a, a);
		glVertex3f(-a, a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a, a);

		glVertex3f( a, a, a);
		glVertex3f( a, a,-a);
		glVertex3f(-a, a,-a);
		glVertex3f(-a, a, a);

		glVertex3f( a,-a, a);
		glVertex3f( a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a, a);
	}glEnd();
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
       // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

//draws a cylinder of given radius and height
void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,height/2);
			glVertex3f(points[i+1].x,points[i+1].y,height/2);
			glVertex3f(points[i+1].x,points[i+1].y,-height/2);
			glVertex3f(points[i].x,points[i].y,-height/2);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

double ti=0;
double tj=0;
double tl=0;
double tm=0;
double tt=0;
void drawSS()
{
    glPushMatrix();///border
    {
        glScalef(4.8,.005,6);
        glColor3f(1,1,1);
        drawCube(30);
    }glPopMatrix();


    glPushMatrix();///Field
    {
        glTranslatef(0,0.50,0);
        glScalef(4.6,.009,5.8);
        glColor3f(0,0.9,0);
        drawCube(30);
    }glPopMatrix();


    glPushMatrix();///line
    {
        glTranslatef(0,2,0);
        glScalef(4.7,.009,.05);
        glColor3f(01,01,01);
        drawCube(30);
    }glPopMatrix();


    ///Goalbar 1
    glPushMatrix();
    {

            glPushMatrix();///bar1
            {
                glTranslatef(28,22,172);
                glScalef(.2,.98,.2);
                glRotatef(90,1,0,0);
                glColor3f(1,1,1);
                drawCylinder(rC,40,40);
            }glPopMatrix();

            glPushMatrix();///bar2
            {
                glTranslatef(0,44,172);
                glScalef(1.2,.2,.2);
                glRotatef(90,0,1,0);
                glColor3f(1,1,1);
                drawCylinder(rC,50,10);
            }glPopMatrix();

            glPushMatrix();///bar3
            {
                glTranslatef(-28,22,172);
                glScalef(.2,.98,.2);
                glRotatef(90,1,0,0);
                glColor3f(1,1,1);
                drawCylinder(rC,40,40);
            }glPopMatrix();

    }

    ///goalbar 2
    glPushMatrix();
    {
            glPushMatrix();///bar1
            {
                glTranslatef(28,22,-172);
                glScalef(.2,.98,.2);
                glRotatef(90,1,0,0);
                glColor3f(1,1,1);
                drawCylinder(rC,40,40);
            }glPopMatrix();

            glPushMatrix();///bar2
            {
                glTranslatef(0,44,-172);
                glScalef(1.2,.2,.2);
                glRotatef(90,0,1,0);
                glColor3f(1,1,1);
                drawCylinder(rC,50,10);
            }glPopMatrix();

            glPushMatrix();///bar3
            {
                glTranslatef(-28,22,-172);
                glScalef(.2,.98,.2);
                glRotatef(90,1,0,0);
                glColor3f(1,1,1);
                drawCylinder(rC,40,40);
            }glPopMatrix();
    }

    ///mid circle
    glPushMatrix();
    {
        glTranslatef(0,5,0);
        glScalef(4.5,.2,4.5);
        glRotatef(90,1,0,0);
        drawCylinder(15,10,50);
    }glPopMatrix();

    ///obstacles
    glPushMatrix();
    {
        glTranslatef(0,20,-160); ///ob 1
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1);
        drawCylinder(rC,40,40);

    }glPopMatrix();


   /* glPushMatrix();
    {
        glTranslatef(0,5,120);///ob 2
        glColor3f(1.1,0,1.1);
        glScalef(.5,.2,.5);
        drawCube(20);

    }glPopMatrix(); */


    glPushMatrix();
    {
        glTranslatef(0,20,120);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 3
        drawCylinder(rC,40,40);

    }glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-48,20,-60);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 4
        drawCylinder(rC,40,40);
    }glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(48,20,-60);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 5
        drawCylinder(rC,40,40);
    }glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-90,20,0);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 6
        drawCylinder(rC,40,40);

    }glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(90,20,0);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 7
        drawCylinder(rC,40,40);

    }glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(48,20,60);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 8
        drawCylinder(rC,40,40);

    }glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-48,20,60);
        glScalef(1,.98,1);
        glRotatef(90,1,0,0);
        glColor3f(1.1,0,1.1); ///ob 9
        drawCylinder(rC,40,40);

    }glPopMatrix();

}


void Ball()
{


    glPushMatrix();///ball
    {


        glTranslatef(0,0,-tl);
        glTranslatef(tm,0,0);
        glTranslatef(-ti,0,0);
        glTranslatef(0,0,tj);
        glColor3f(1,0,0);
        glTranslatef(0,10,10);
        drawSphere(5,20,20);
    }glPopMatrix();
}
void human()
{
    ///Player
    glPushMatrix();///body
        {
            glTranslatef(0,0,0);
            glTranslatef(0,35,0);
            glScalef(.9,1.5,.5);
            glColor3f(1,.5,0);
            drawCube(10);
        }glPopMatrix();

        glPushMatrix(); ///leg
        {
            glTranslatef(0,0,0);
            glScalef(.25,.98,.65);
            glTranslatef(-30,11,0);
            glRotatef(90,1,0,0);
            drawCylinder(rC,20,20);
        }glPopMatrix();

        glPushMatrix(); ///leg
        {
            glTranslatef(0,0,0);
            glScalef(.25,.98,.65);
            glTranslatef(30,11,0);
            glRotatef(90,1,0,0);
            drawCylinder(rC,20,20);
        }glPopMatrix();

        glPushMatrix(); ///hand
        {
            glTranslatef(0,0,0);
            glRotatef(-50,0,1,0);
            glTranslatef(12,45,-7);
            glScalef(.75,.25,.2);
            glRotatef(90,0,1,0);
            drawCylinder(rC,20,20);
        }glPopMatrix();

        glPushMatrix(); ///hand
        {
            glTranslatef(0,0,0);
            glRotatef(-120,0,1,0);
            glTranslatef(12,45,7);
            glScalef(.75,.25,.2);
            glRotatef(90,0,1,0);
            drawCylinder(rC,20,20);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0,0,0);
            glColor3f(.5,1,.5);  ///head
            glTranslatef(0,56,0);
            drawSphere(7,20,20);
        }glPopMatrix();
}

void game()
{

    glPushMatrix();
        {

            drawSS();

        }glPopMatrix();
  glPushMatrix();
        {
           glTranslatef(0,0,tt);
            Ball();

        }glPopMatrix();


    glPushMatrix();
        {


        glTranslatef(0,0,-tl);
        glTranslatef(tm,0,0);
        glTranslatef(-ti,0,0);
        glTranslatef(0,0,tj);
            human();

        }glPopMatrix();

}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

        case 'w':
            posx+=lx;
            posy+=ly;
            posz+=lz;
            break;

        case 's':
            posx-=lx;
            posy-=ly;
            posz-=lz;
            break;

         case 'h':
            posx+=rx;
            posy+=ry;
            posz+=rz;
            break;

        case 'f':
            posx-=rx;
            posy-=ry;
            posz-=rz;
            break;

        case 'a':
            posx+=ux;
            posy+=uy;
            posz+=uz;
            break;

        case 'd':
            posx-=ux;
            posy-=uy;
            posz-=uz;
            break;

        case 'i':
            ti+=0.8;
            break;
        case 'j':
            tj+=0.8;
            break;
        case 'l':
            tl+=0.8;
            break;
        case 'm':
            tm+=0.8;
            break;
        case 't':
            tt=1;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(00,200,200,	0,0,0,	0,1,0);
	//gluLookAt(00,70,170,	0,0,0,	0,1,0);
	gluLookAt(posx,posy,posz,	posx+lx,posy+ly,posz+lz,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    //drawSS();
      //human();
     game();





	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

int flag=0;
void animate()
{
   // bll=bll+.04;
   // if(bll>90)
    //{
      //  bll=0;
    //}


    if(tt>=1)
    {
        tt+=0.5;


    }
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
