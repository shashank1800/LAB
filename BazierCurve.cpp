#include<stdio.h>
#include<math.h>
#include<GL\glut.h>

#define PI 3.1416

GLint nControlPoints = 4, nBezierCurvePoints = 20;
GLint *C;

struct Point3d
{
	GLfloat x, y, z;
};

void bino(GLint C[])
{
	GLint j, n=nControlPoints-1;
	for (GLint k = 0; k <=n; k++)
	{
		C[k] = 1;
		for (j = n; j > k ; j--)
			C[k] *= j;
		for (j = n-k; j > 1; j--)
			C[k] /= j;
	}
}

void computeBezierPoint(GLfloat u, Point3d *bezierPoint, Point3d controlPoints[], GLint C[])
{
	GLint n = nControlPoints - 1;
	GLfloat bezBlendFcn;
	bezierPoint->x = bezierPoint->y = bezierPoint->z = 0.0;
	for (GLint k = 0; k <= n; k++)
	{
		bezBlendFcn = C[k] * pow(u, k) * pow(1 - u, n - k);
		bezierPoint->x += controlPoints[k].x * bezBlendFcn;
		bezierPoint->y += controlPoints[k].y * bezBlendFcn;
		bezierPoint->z += controlPoints[k].z * bezBlendFcn;
	}
}

void bezier(Point3d controlPoints[])
{
	Point3d bezierCurvePoint;
	GLfloat u;
	C = new GLint[nControlPoints];

	bino(C);

	glBegin(GL_LINE_STRIP);
	for (GLint k = 0; k <= nBezierCurvePoints; k++)
	{
		u = GLfloat(k) / GLfloat(nBezierCurvePoints);
		computeBezierPoint(u, &bezierCurvePoint, controlPoints, C);
		glVertex2f(bezierCurvePoint.x, bezierCurvePoint.y);
	}
	glEnd();
	delete[] C;
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5);
	glLineWidth(5);

	static float theta = 0;
	Point3d ctrlPts[4] = {{ 20, 100, 0 },{ 30, 110, 0 },{ 50, 90, 0 },{ 60, 100, 0 } };

	ctrlPts[1].x += 10 * sin(theta * PI / 180.0);
	ctrlPts[1].y += 10 * sin(theta * PI / 180.0);
	ctrlPts[2].x -= 10 * sin((theta + 30) * PI / 180.0);
	ctrlPts[2].y -= 10 * sin((theta + 30) * PI / 180.0);
	ctrlPts[3].x -= 10 * sin((theta)* PI / 180.0);
	ctrlPts[3].y += sin((theta - 30) * PI / 180.0);
	theta += 1;

	if (theta == 360)
		theta = 0;

	glPushMatrix();
	for (int i = 0; i < 24; i++)
	{
		glTranslatef(0,-1, 0);
		bezier(ctrlPts);
	}
	glPopMatrix();

	glBegin(GL_LINES);
	glVertex2f(20, 100);
	glVertex2f(20, 40);
	glEnd();

	glFlush();
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 110, 0, 110);
	glClear(GL_COLOR_BUFFER_BIT);
}

void menu(int op)
{
	if (op == 1)
		glColor3f(1.0, 0.0, 0.0);
	else if (op == 2)
		glColor3f(0.0, 1.0, 0.0);
	else if (op == 3)
		glColor3f(0.0, 0.0, 1.0);
	else if (op == 4)
		exit(0);
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Bezier Curve");

	glutCreateMenu(menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Quit", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}
