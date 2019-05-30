#include <math.h>
#include <GL/glut.h>
#define pi 3.142

int bicoeff[] = {1, 3, 3, 1};

int nCtrlPts = 4, nBezCurvePts = 250;
float theta, bezBlendFcn;

void bezier(int ctrlPts[][2])
{
    glBegin(GL_POINTS);
    
    for (int k = 0; k <= nBezCurvePts; k++)
    {
        float t = (float)k/nBezCurvePts;
        float bezPt[2] ={0,0};

        for (int k = 0; k < nCtrlPts; k++) //find bezier point
        {   
            bezBlendFcn = bicoeff[k] * pow(t, k) * pow(1 - t, (nCtrlPts -1) - k);
            for(int  j =0; j <= 1; j++)
                bezPt[j] += ctrlPts[k][j] * bezBlendFcn;
        }
        for (int i = 0; i < 40; i++) // plot the same point one below the other
            glVertex2f(bezPt[0],bezPt[1]-(0.5*i));
    }
    glEnd();
}

void display()
{   
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    int ctrlPts[][2] = {{20,100}, {30,120},{50,80},{60,100}};  

    for(int  i =1; i < nCtrlPts; i++) //move control points for animation
        for(int  j =0; j <= 1; j++)
            ctrlPts[i][j] += 5*sin(theta);

    theta += 0.0002;
    glPointSize(2);

    bezier(ctrlPts); //start 
        
    glLineWidth(5);
    glBegin(GL_LINES);
        glVertex2f(20, 100);
        glVertex2f(20, 40);
    glEnd();
    glFlush();
    glutPostRedisplay();
}

void menu(int op)
{
    if (op == 1) glColor3f(1, 0, 0);
    else if (op == 2) glColor3f(0, 1, 0);
    else if (op == 3) glColor3f(0, 0, 1);
    else exit(0);
    glutPostRedisplay();
}

int main(int argc, char *argv[]){

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("BEZIER CURVE");
    glOrtho(0, 130, 0, 130,-1,1);
    glutDisplayFunc(display);
    glutCreateMenu(menu);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
}
