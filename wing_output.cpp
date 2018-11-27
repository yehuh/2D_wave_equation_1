#include<iostream>
#include<fstream>
#include <string>
#include<windows.h>
#include<gl\glut.h>
#include<gl\gl.h>
#include<cmath>
//#include "stlinput.H"
//#include "four_bar.H"
using namespace std;
   const GLfloat pi = 3.141592654;                //       no unit
   const GLfloat Gr = 980;                        //       cm/s^2
   const GLfloat omega = 1.2025;                  //       1/s
   const GLfloat line = 100;                      //       cm  length of string
   const GLfloat bar         = 50;                //       cm  length of outputbar
   const GLint y_interval    = 15;                //       total grids in x axis
   const GLint t_interval    = 10000;             //       total grids in t axis
   const GLint r_nodes       = 15;                //       total nodes in output bar

		 GLdouble ctrlPoints[y_interval][3] = {0.0};             //cm   
		 GLdouble u[y_interval][t_interval] = {0.0};             //u(y,r,t) displacement in x direction
		 GLdouble v[y_interval][t_interval] = {0.0};             //v(y,r,t) displacement in z direction
         GLdouble u1[y_interval][r_nodes][t_interval] = {0.0};
		 GLdouble v1[y_interval][r_nodes][t_interval] = {0.0};
		 GLfloat r[r_nodes] ={0};                 //       cm

         GLfloat time  =0;                        //       sec
		 GLfloat time2 =0;                        //       sec 
		 GLfloat step = 0.005;                    //       sec/frame

         GLfloat ob_angle_x =0;                   //       degree
         GLfloat ob_angle_y =0;                   //       degree
		 GLfloat ob_angle_z =0;                   //       degree
		  
		 

		 GLfloat ob_x =0;                         //       degree
         GLfloat ob_y =0;                         //       degree
		 GLfloat ob_z =0;                         //       degree

		 GLfloat length = 0;                      //       cm   =>length of shape function
		 GLfloat temp1  = 0;                      //      temperary caculate1
         GLfloat temp2  = 0;                      //      temperary caculate2
		 GLfloat temp3  = 0;                      //      temperary caculate3
		 GLfloat tencmpoint = 0;                  //      the point which length is equal to 10 cm

		 GLdouble delta_x = line/(y_interval);
		 int a = 0;                               //      temp storage


		 static GLfloat corners[]={      //static GLfloat corners[8][3]
			 0,   0,   0,  20,  0,   0,  20,   0,  60,   0,  0,  60,  //0123 0___0,   0,   0,
			 0,   0,   0,   0,  0,  60,   0,  20,  60,   0, 20,   0,  //0365 1___20,  0,   0,
			20,   0,  60,  20, 20,  60,   0,  20,  60,   0,  0,  60,  //2763 2___20,  0,  60,
			20,   0,   0,  20, 20,   0,  20,  20,  60,  20,  0,  60,  //1472 3___ 0,  0,  60,
			 0,   0,   0,   0, 20,   0,  20,  20,   0,  20,  0,   0,  //0541 4___20, 20,   0,
			20,  20,   0,   0, 20,   0,   0,  20,  60,  20, 20,  60   //4567 5___ 0, 20,   0,
			                                                          //     6___ 0, 20,  60,
			                                                          //     7___20, 20,  60,
		 };
		 
		 static GLfloat normalsve[]={  //static GLfloat normalsve[6][3]={  
			  0, -1,  0,  0, -1,  0,  0, -1,  0,  0, -1,  0,      //bottom
			 -1,  0,  0, -1,  0,  0, -1,  0,  0, -1,  0,  0,      //left 
			  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,      //front
			  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,      //right
			  0,  0, -1,  0,  0, -1,  0,  0, -1,  0,  0, -1,      //back
			  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,      //top
		 };    

		 static GLuint orders[] = {   0,  1,  2,  3,      //bottom
			                          4,  5,  6,  7,      //left
									  8,  9, 10, 11,      //front
			                         12, 13, 14, 15,      //right
									 16, 17, 18, 19,      //back
									 20, 21, 22, 23  };   //top

		 GLfloat wing_output_rad(GLfloat t1)
		 {
			 GLfloat temp = 0;
		    temp = pi/4*cos(4*t1);
			return temp;
		 };

		 GLfloat wing_output_deg(GLfloat t1)
		 {
			 GLfloat temp = 0;
		    temp = 45*cos(4*t1);
			return temp;
		 };

   GLdouble pde_result(GLdouble u11, GLdouble u01, GLdouble u21, GLdouble u10, GLint x)
   {
	   GLdouble temp = 0;
	   GLdouble temp1 = 0;
	   GLdouble temp2 = 0;
	   GLdouble temp3 = 0;
	   GLdouble u12 = 0;

	   temp  = 2*u11*(1/pow(step, 2) - Gr*(line-x*delta_x)/pow(delta_x, 2));
	   temp1 = Gr*u01*((line-x*delta_x)/pow(delta_x, 2) + 0.5/delta_x );
	   temp2 = Gr*u21*((line-x*delta_x)/pow(delta_x, 2) - 0.5/delta_x );
	   temp3 = u10/pow(step, 2);
	   u12 = (temp+temp1+temp2-temp3)*pow(step, 2);
	   return u12;
   }
		 



   void RenderScene(void)
   {
   
    GLfloat gray[] = {0.75f, 0.75f, 0.75f, 1.0f};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


    
	

	glPushMatrix();
	  
      glRotatef(ob_angle_x,1.0,0.0,0.0);     
	  glRotatef(-ob_angle_y,0.0,1.0,0.0);
	  glRotatef(ob_angle_z,0.0,0.0,1.0);
	  
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,gray);   
      
         glColor3f(1.0f,0.0f,0.0f);   //標示x軸為紅色
		 glLineWidth(3.0f);
		 glBegin(GL_LINES);
		 glVertex3f(0.0f,0.0f,0.0f);
         glVertex3f(200.0f,0.0f,0.0f);
		 glEnd();

		 glColor3f(0.0f,1.0f,0.0f);  //標示y軸為綠色
		 glLineWidth(3.0f);
		 glBegin(GL_LINES);
		 glVertex3f(0.0f,0.0f,0.0f);
         glVertex3f(0.0f,200.0f,0.0f);
		 glEnd();

		 glColor3f(0.0f,0.0f,1.0f);  //標示z軸為藍色
		 glLineWidth(3.0f);
		 glBegin(GL_LINES);
		 glVertex3f(0.0f,0.0f,0.0f);
         glVertex3f(0.0f,0.0f,200.0f);
		 glEnd();

		 glColor3f(1.0f,1.0f,1.0f) ;  //make cube white
		 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		 glColor3f(1.0f,1.0f,0.0f);
         glPushMatrix();
		 a = (time / step) - 1;

		 for(int i = 0; i < y_interval-1; i++)
		 {
		  for(int k = 0; k <r_nodes; k++)
		  {
		  glPointSize(5.0f);
			 glBegin(GL_POINTS);
			 glVertex3f(u1[i][k][a],-(i*line)/(y_interval-1),v1[i][k][a]);
			 //glVertex3f(u[i][a],-(i*line)/(y_interval-1),v[i][a]);
			 glEnd();
		  }
         }
		 /*glPointSize(5.0f);
		 glColor3f(1.0f,1.0f,0.0f);
		 glBegin(GL_POINTS);
			 glVertex3f(30*sin(pi/4*cos(4*time)),0.0f,30*cos(pi/4*cos(4*time)));
			 glEnd();*/
		 glPopMatrix();

		 glPushMatrix();
		 
		 //rotate along y axis
	     glRotatef(wing_output_deg(time),0.0,1.0,0.0);

		 //wave plotting
		 /*a = (time / step) - 1;
		 glColor3f(1.0f,1.0f,0.0f);
		 glPushMatrix();
		 glTranslatef(-25.0f,0.0f,45.0f);
		 for(int i = 0; i < y_interval-1; i++)
		 {
		 	 glPointSize(5.0f);
			 glBegin(GL_POINTS);
		     glVertex3f(u[i][a],-(i*line)/(y_interval-1),0);
			 glEnd();
		 }
		 glPopMatrix();*/

	     glColor3f(1.0f,1.0f,1.0f);
		 glTranslatef(ob_x,0.0f,0.0f);
	     glTranslatef(0.0f,ob_y,0.0f);
	     glTranslatef(0.0f,0.0f,ob_z);
		 //glTranslatef(-20*cos(time2)+30.0f,0.0f,0.0f);
		 glEnableClientState(GL_VERTEX_ARRAY);
		 glEnableClientState(GL_NORMAL_ARRAY);
		 glVertexPointer(3, GL_FLOAT, 0, corners);
		 glNormalPointer(GL_FLOAT, 0, normalsve);
		 glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, orders);
		 glPopMatrix();

		 

	glPopMatrix();

	     

         glutSwapBuffers();
		 glFlush();
   }


   void SetupRC(void)
   {

	GLfloat ambientLight[]={0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat diffuseLight[]={0.7f, 0.7f, 0.7f, 1.0f};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_CCW);

	glEnable(GL_LIGHTING);

	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //stlinput("base120x14.7x7mm.STL",base);
	
	

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   }

   void ChangeSize(GLsizei w,GLsizei h)
   { 
	GLfloat lightPos[]={100.0f, 100.0f, 100.0f, 1.0f};
	if(h==0)
		h=1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    if(w<=h)
      glOrtho(-200.0f*w/h,200.0f*w/h, -200.0f, 200.f, -200.0f, 200.0f); //顯示範圍
    else
	  glOrtho(-200.0f,200.0f, -200.0f*h/w, 200.f*h/w, -200.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   } 


   void SpecialKeyHandler(int key, int x, int y)
   {  


	if(key == GLUT_KEY_LEFT)
	{
		ob_angle_y = ob_angle_y+10.0f;

		if (ob_angle_y<-180.0f)
			ob_angle_y = ob_angle_y+360.0f;
	}

	if(key == GLUT_KEY_RIGHT)
	{
		ob_angle_y = ob_angle_y-10.0f;

		if (ob_angle_y>360.0f)
			ob_angle_y = ob_angle_y-360.0f;
	}
	     

	if(key == GLUT_KEY_UP)
	{
		ob_angle_x = ob_angle_x-10.0f;

		if (ob_angle_x>360.0f)
			ob_angle_x = ob_angle_x-360.0f;
	}

	if(key == GLUT_KEY_DOWN)
	{
		ob_angle_x = ob_angle_x+10.0f;

		if (ob_angle_y<-180.0f)
			ob_angle_y = ob_angle_y+360.0f;
	}




	if(key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		ob_x = ob_x-10.0f;
		ob_angle_y = ob_angle_y-10.0f;

		if (ob_angle_y<-180.0f)
			ob_angle_y = ob_angle_y+360.0f;

	}

	if(key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		ob_x = ob_x+10.0f;
		ob_angle_y = ob_angle_y+10.0f;

		if (ob_angle_y>360.0f)
			ob_angle_y = ob_angle_y-360.0f;
	}
	     

	if(key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		ob_y = ob_y+10.0f;
		ob_angle_x = ob_angle_x+10.0f;

		if (ob_angle_x>360.0f)
			ob_angle_x = ob_angle_x-360.0f;
	}

	if(key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		ob_y = ob_y-10.0f;
		ob_angle_x = ob_angle_x-10.0f;

		if (ob_angle_y<-180.0f)
			ob_angle_y = ob_angle_y+360.0f;
	}

	if(key == GLUT_KEY_PAGE_UP)
	{
		ob_angle_z = ob_angle_z+10.0f;

		while(ob_angle_z>360.0f)
			ob_angle_z = ob_angle_z - 360.0f;
	}

	if(key == GLUT_KEY_PAGE_DOWN)
	{
		ob_angle_z = ob_angle_z-10.0f;

		while(ob_angle_z < 0.0f)
			ob_angle_z = ob_angle_z + 360.0f;
	}


   }


   void TimerFunc(int value)
   {
	   
	   //if(time*omega*sqrt(Gr/line)>pi*2)
       //time = time-pi*2/(omega*sqrt(Gr/line));
		 
	   //if (time2>pi*2)
		//	 time2 = time2 -pi*2;

    glutPostRedisplay();
    glutTimerFunc(10, TimerFunc, 1);  // call timerfunc once per 0.005 sec glutTimerFunc(10, TimerFunc, 1)
    time = time+step;	

   }

   float vibrationtime(GLfloat t)
   {
	   GLfloat temp = 0;
	   temp = cos(omega*t*sqrt(Gr/line));
	   return temp;
   }

   GLfloat shapefunc(GLfloat y, GLfloat l)
   {
	  GLfloat temp = 0;
	  temp = y+0.64*pow(y,2)/l;
	  return temp;
   }

   void changesize2(GLsizei w1,GLsizei h1)
   {
	   if(h1==0)
		   h1=1;

		glViewport(0,0,w1,h1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(w1<=h1)
         glOrtho(-15.0f*w1/h1,15.0f*w1/h1,        -28.0f,       2.0f, -1.0, 1.0); //顯示範圍
        else
	     glOrtho(      -15.0f,      15.0f,  -28.0f*h1/w1, 2.0f*h1/w1, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
		glLoadIdentity;
   }

   void diagramdisplay()
   {
	   glClear(GL_COLOR_BUFFER_BIT);
	     glColor3f(0.0f,0.0f,0.0f);   //標示x軸為黑色
		 glLineWidth(1.0f);
		 glBegin(GL_LINES);
		 glVertex3f(0.0f,0.0f,0.0f);
         glVertex3f(2.0f,0.0f,0.0f);
		 glEnd();

		 /*glColor3f(0.0f,0.0f,0.0f);   //標示y軸為黑色
		 glLineWidth(1.0f);
		 glBegin(GL_LINES);
		 glVertex3f(0.0f,0.0f,0.0f);
         glVertex3f(0.0f,-10.0f,0.0f);
		 glEnd();*/
         
		 /*for(int j = 0; j < 100; j++)           //length calculate
		 {
			 temp1 = ((j)-(j-1))*line/100;                      
			 //delta y 
             temp2 = shapefunc((j)*line/100,line)*vibrationtime(time)-shapefunc((j-1)*line/100,line)*vibrationtime(time);
			 //delta x
			 temp3 = sqrt(powf(temp1,2)+powf(temp2,2));  
			 //delta length;
			 length = length + temp3;
			 if (length > 10 && length <12)
			 {
				 tencmpoint = j*line/100;
			 }
		 }*/

		 //if (time>2*pi)
		//	 time = time - (2*pi);
		 for(int i = 0; i < 10; i++)         // shape function
		 {
			 ctrlPoints[i][0] = shapefunc(i/line*tencmpoint,line)*vibrationtime(time);
             ctrlPoints[i][1] = -i/line*tencmpoint;
			 ctrlPoints[i][2] =  0;
		 }
		 length = 0;

		 //timevary = vibrationtime(time);


		 /*for(i = 0; i < 9; i++)
		 {
		 glLineWidth(1.0f);
			 glBegin(GL_LINES);
		     glVertex2f(ctrlPoints[i][0],ctrlPoints[i][1]);
			 glVertex2f(ctrlPoints[i+1][0],ctrlPoints[i+1][1]);
			 glEnd();
		 }*/
		 /*glMap1f(GL_MAP1_VERTEX_3,         //type of data
			     0.0f,                     //lower u range
				 100.0f,                     //Upper u range
				 3,                        //distance between points in the data
				 10,                        //number of contrl points
				 &ctrlPoints[0][0]);*/       //address of contrl points
         //glPointSize(4.0f);
		 //glColor3f(0.0f,0.0f,1.0f);
		 
		 //glVertex3f(time2,sin(time2),0);
		 glEnable(GL_MAP1_VERTEX_3);
		 glMapGrid1d(100,0.0,100.0);
		 glEvalMesh1(GL_LINE,0,100);


		 glEnd();

	

	     glutSwapBuffers();
		  glFlush();
   }

   void backgroundset()
   {
	   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   }


int main()
{
	cout<<" CTRL+ORIENT -> move   the object"<<endl;
	cout<<" ORIENT      -> evolve the object"<<endl;
	for(int i = 0;i<r_nodes;i++)
	{
		r[i] = i*bar/(r_nodes-1);
	}
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Simple Homonic Motion");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeyHandler);

	//wave equation solving x-direction starts here
	double b = 0;
		for(int j = 0; j < t_interval; j++)
		 {
          for(int k = 0; k < r_nodes; k++)
		  {			  
		  //u[0][j] = 30*sin(wing_output_rad(j*step));          //boundary condition u(0,t)=30*sin(w*t)
		  u1[0][k][j] = r[k]*sin(wing_output_rad(j*step));
		  //u[y_interval-1][j] = 0;             //boundary condition utt(L,t) = 0;
		  }
		 }

		 for(int i = 1; i < y_interval ; i++)              //initial condition u(y,r,0) = 0
		 {
			 for(int k = 0; k < r_nodes; k++)
			 {
             //u1[i][k][0] = 0;
			 u[i][0] = 0;
			 }
		 }

	     for(int j = 1; j < t_interval; j++)
		 {
		  for(int i = 1 ; i < y_interval; i++)              // the wave eqution solving
		  {
			  for(int k = 0; k < r_nodes; k++)
			  {
				 if(j == 1)
				 {
				 u1[i][k][j] = pde_result(u1[i][k][j-1], u1[i-1][k][j-1], u1[i][k][j-1], 0, i);
				 //u[i][j] = pde_result(u[i][j-1], u[i-1][j-1], u[i][j-1], 0, i);
				 }

				 else
				 {
				 u1[i][k][j] = pde_result(u1[i][k][j-1], u1[i-1][k][j-1], u1[i+1][k][j-1], u1[i][k][j-2], i);
         		 //u[i][j] = pde_result(u[i][j-1], u[i-1][j-1], u[i+1][j-1], u[i][j-2], i);
				 }
			  }
		  }
		 }
		 //wave equation solving x-direction ends here



		 //wave equation solving z-direction starts here
	double c = 0;
		for(int j = 0; j < t_interval; j++)
		 {
			for(int k = 0;k<r_nodes;k++)
			{
		     //v[0][j] = 30*cos(wing_output_rad(j*step));          //boundary condition u(0,t)=sin(w*t)
			 v1[0][k][j] = r[k]*cos(wing_output_rad(j*step));
            }
		  //u[y_interval-1][j] = 0;             //boundary condition utt(L,t) = 0;
		 }

		 for(int i = 1; i < y_interval ; i++)              //initial condition v(y,r,0) = 0
		 {
			 for(int k = 0; k < r_nodes; k++)
			 {
			 //v[i][0] = 0;
			 v1[i][k][0] = 0;
			 }
		 }

	     for(int j = 1; j < t_interval; j++)
		 {
		  for(int i = 1 ; i < y_interval; i++)              // the wave eqution solving
		  {
			  for(int k =0; k<r_nodes; k++)
			  {
				 if(j == 1)
				 {
				 //v[i][j] = pde_result(v[i][j-1], v[i-1][j-1], v[i][j-1], 0, i);
				 v1[i][k][j] = pde_result(v1[i][k][j-1], v1[i-1][k][j-1], v1[i][k][j-1], 0, i);
				 }

				 else
				 {
         		 //v[i][j] = pde_result(v[i][j-1], v[i-1][j-1], v[i+1][j-1], v[i][j-2], i);
				 v1[i][k][j] = pde_result(v1[i][k][j-1], v1[i-1][k][j-1], v1[i+1][k][j-1], v1[i][k][j-2], i);
				 }	  
			  }
		  }
		 }
         //wave equation solving z-direction ends here


	glutDisplayFunc(RenderScene);
	glutTimerFunc(500, TimerFunc, 1);
	SetupRC();

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//glutCreateWindow("Diagram of Accelation/Velacity versus time");
	//glutReshapeFunc(changesize2);
	//glutDisplayFunc(diagramdisplay);
	//glutTimerFunc(10, TimerFunc, 1);
	//backgroundset();


	glutMainLoop();
   return 0;
}