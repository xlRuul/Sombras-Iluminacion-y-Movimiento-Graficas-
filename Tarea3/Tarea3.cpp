#include "GL/glut.h"
#include <math.h>

#define GL_PI 3.1415f
#define YGROUND         10.f
#define LIGHT_X 1.5f
#define LIGHT_Y 40.f
#define LIGHT_Z 1.0f
#define LIGHT_A 1.0f
#define EX 0.0f
#define EY 10.0f
#define EZ 70.0f
#define CX 0.0f
#define CY YGROUND
#define CZ 0.0f
#define RADIUS          20.0f
#define EXTENT          70.0f

GLfloat ALFA = 0.5;
GLboolean bCull = glIsEnabled(GL_CULL_FACE),
bDepth = glIsEnabled(GL_DEPTH_TEST),
bOutline = (GLboolean)true;
GLenum shademode = GL_FLAT;
GLfloat xRot = 0.45f, yRot = 0.35f,
ex = EX, ey = EY + CY, ez = EZ, cx = CX, cy = CY, cz = CZ,
yground = YGROUND,
delta = 0.01f, deltaR = 0.01f,

piso[4] = { 0.f,1.f,0.f,  -yground },
luz[4] = { LIGHT_X,  LIGHT_Y, LIGHT_Z,  LIGHT_A };


GLfloat mat_s[16];

///////////////////////////////////////////////////////////
void gltMakeShadowMatrix(GLfloat vPlaneEquation[],
    GLfloat vLightPos[], GLfloat destMat[])
{
    GLfloat dot;


    // Dot product of plane and light position
    dot = vPlaneEquation[0] * vLightPos[0] + vPlaneEquation[1] * vLightPos[1] +
        vPlaneEquation[2] * vLightPos[2] + vPlaneEquation[3] * vLightPos[3];


    // Now do the projection
    // First column
    destMat[0] = dot - vLightPos[0] * vPlaneEquation[0];
    destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
    destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
    destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];


    // Second column
    destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
    destMat[5] = dot - vLightPos[1] * vPlaneEquation[1];
    destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
    destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];


    // Third Column
    destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
    destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
    destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
    destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];


    // Fourth Column
    destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
    destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
    destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
    destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
}


void Draw_Light(void) {

    glPushMatrix();
    GLfloat colv[4] = { 1.0f, 1.0f, 0.0f, 1.f };
    glColor4fv(colv);
    glTranslatef(luz[0], luz[1] + 20, luz[2]);
    glutSolidSphere(1.f, 10, 10);
    glPopMatrix();
}

void SetupRC()
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Set color shading model to flat
    glShadeModel(shademode);

    // Clockwise-wound polygons are front facing; this is reversed
    // because we are using triangle fans
    glFrontFace(GL_CCW);
}


// Called to draw scene

void piramide_s() {

    GLfloat x, y, z, angle;         // Storage for coordinates and angles
    glColor3f(0.5f, .5f, 0.5f); //color of the shadow      
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 10.75f, .0f);
    for (angle = 0.0f; angle < (2.0f * GL_PI) + 0.01; angle += (GL_PI / 8.0f))
    {
        x = RADIUS * sin(angle);
        z = RADIUS * cos(angle);
        glVertex3f(x, 0.f, z);
    }
    glEnd();

    // Begin a new triangle fan to cover the bottom
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (angle = 2.0f * GL_PI; angle >= 0; angle -= (GL_PI / 8.0f))
    {
        x = RADIUS * sin(angle);
        z = RADIUS * cos(angle);
        glVertex3f(x, 0.f, z);
    }
    glEnd();
}

void piramide() {

    GLfloat x, y, z, angle;  // Storage for coordinates and angles
    int iPivot = 1;
    // Begin a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 10.75f, .0f);
    for (angle = 0.0f; angle < (2.0f * GL_PI) + 0.01; angle += (GL_PI / 8.0f))
    {
        x = RADIUS * sin(angle);
        z = RADIUS * cos(angle);
        if ((iPivot % 2) == 0)glColor3f(0.0f, 1.0f, 0.0f);
        else                glColor3f(1.0f, 0.0f, 0.0f);
        iPivot++; // Increment pivot to change color next time
        glVertex3f(x, 0.f, z);
    }
    glEnd();

    // Begin a new triangle fan to cover the bottom
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (angle = 2.0f * GL_PI; angle >= 0; angle -= (GL_PI / 8.0f))
    {
        x = RADIUS * sin(angle);
        z = RADIUS * cos(angle);
        if ((iPivot % 2) == 0)  glColor3f(0.0f, 1.0f, 0.0f);
        else                  glColor3f(1.0f, 0.0f, 0.0f);
        iPivot++;// Increment pivot to change color next time
        glVertex3f(x, 0.f, z);
    }
    glEnd();
}

void piramide2() {

    GLfloat x, y, z, angle;  // Storage for coordinates and angles
    int iPivot = 1;
    // Begin a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 10.75f, .0f);
    for (angle = 0.0f; angle < (2.0f * GL_PI) + 0.01; angle += (GL_PI / 8.0f))
    {
        x = RADIUS * sin(angle);
        z = RADIUS * cos(angle);
        if ((iPivot % 2) == 0)glColor4f(0.0f, 1.0f, 0.0f, ALFA);
        else                glColor4f(1.0f, 0.0f, 0.0f, ALFA);
        iPivot++; // Increment pivot to change color next time
        glVertex3f(x, 0.f, z);
    }
    glEnd();

    // Begin a new triangle fan to cover the bottom
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (angle = 2.0f * GL_PI; angle >= 0; angle -= (GL_PI / 8.0f))
    {
        x = RADIUS * sin(angle);
        z = RADIUS * cos(angle);
        if ((iPivot % 2) == 0)  glColor4f(0.0f, 1.0f, 0.0f, ALFA);
        else                  glColor4f(1.0f, 0.0f, 0.0f, ALFA);
        iPivot++;// Increment pivot to change color next time
        glVertex3f(x, 0.f, z);
    }
    glEnd();
}

void Piso() {
    glColor4f(0.8f, 0.8f, 0.8f, ALFA);
    glBegin(GL_QUADS);
    glVertex3f(EXTENT, yground, -EXTENT);
    glVertex3f(EXTENT, yground, EXTENT);
    glVertex3f(-EXTENT, yground, EXTENT);
    glVertex3f(-EXTENT, yground, -EXTENT);
    glEnd();

}

void Objetos_simplificados(void)
{


    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    piramide_s();
    glPopMatrix();
}

void Objetos(void)
{

    glPushMatrix();
    glTranslatef(0.f, 40.f, 0.f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    piramide();
    glPopMatrix();
}

void Sombra_Objetos(GLfloat p_luz[], GLfloat e_plano[]) {

    glPushMatrix();
    gltMakeShadowMatrix(e_plano, p_luz, mat_s);
    glMultMatrixf(mat_s);
    Objetos_simplificados();
    glPopMatrix();
}

void Stencil_Config()
{
    // turning off writing to the color buffer and depth buffer so we only
    // write to stencil buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    //// enable stencil buffer
    //glEnable(GL_STENCIL_TEST);

    // write a one to the stencil buffer everywhere we are about to draw
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);

    // this is to always pass a one to the stencil buffer where we draw
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    // render the plane which the shadow will be on
    // color and depth buffer are disabled, only the stencil buffer
    // will be modified
    //DrawFloor(0,0,0);
    Piso();

    // turn the color and depth buffers back on
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // until stencil test is diabled, only write to areas where the
    // stencil buffer has a one. This is to draw the shadow only on
    // the floor.
    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);

    // don't modify the contents of the stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

}

void RenderScene(void)
{
    // Reset coordinate system

    glLoadIdentity();
    gluLookAt(ex, ey, ez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    GLfloat x, y, angle;         // Storage for coordinates and angles
    int iPivot = 1;            // Used to flag alternating colors

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Turn culling on if flag is set
    if (bCull)   glEnable(GL_CULL_FACE);
    else        glDisable(GL_CULL_FACE);

    // Enable depth testing if flag is set
    if (bDepth)  glEnable(GL_DEPTH_TEST);
    else        glDisable(GL_DEPTH_TEST);

    // Draw the back side as a wireframe only, if flag is set
    if (bOutline)glPolygonMode(GL_BACK, GL_LINE);
    else        glPolygonMode(GL_BACK, GL_FILL);

    // Save matrix state and do the rotation
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    /*mundo REFLEJADO*/
    // enable stencil buffer
    glEnable(GL_STENCIL_TEST);
    Stencil_Config();
    glFrontFace(GL_CCW);


    glPushMatrix();
    glTranslatef(0.0f, -piso[3]/*ALTURA_PISO*/, 0.0f);
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(0.0f, piso[3]/*- ALTURA_PISO*/, 0.0f);
    //Draw_Light();
    Objetos();
    glPopMatrix();

    glFrontFace(GL_CW);
    // enable stencil buffer
    glDisable(GL_STENCIL_TEST);
    /*FIN: mundo REFLEJADO*/

    /*mundo virtual*/

    /*foco*/
    Draw_Light();
    /*piso*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Piso();
    glDisable(GL_BLEND);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable stencil buffer
    glEnable(GL_STENCIL_TEST);
    Stencil_Config();
    glDisable(GL_DEPTH_TEST);
    Sombra_Objetos(/*GLfloat datosdeLuz[]*/luz, /*GLfloat datosdePlano[]*/piso);
    if (bDepth)  glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);

    /*cubo*/
    Objetos();

    glPopMatrix();

    // Flush drawing commands
    glutSwapBuffers();
}



// Called by GLUT library when the window has changed size

void ChangeSize(GLsizei w, GLsizei h)
{    // Set Viewport to window dimensions
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.5, 500.0);

    glMatrixMode(GL_MODELVIEW);
}

void SpecialKeys(int key, int x, int y)
{
    GLfloat dx, dz;


    if (key == GLUT_KEY_UP)
    {//increase distance from camera to origin
        ex *= (1.0f + deltaR);  ey *= (1.0f + deltaR);        ez *= (1.0f + deltaR);
    }

    if (key == GLUT_KEY_DOWN)
    {//reduce distance from camera to origin (close up)
        ex *= (1.0f - deltaR); ey *= (1.0f - deltaR);        ez *= (1.0f - deltaR);
    }

    if (key == GLUT_KEY_LEFT)
        //Rotate camera around origin in Oxz plane
    {
        dx = -ez;     dz = ex;
        GLfloat s = sqrtf(ex * ex + ey * ey + ez * ez);
        ex += delta * dx;       ez += delta * dz;
        GLfloat s1 = sqrtf(ex * ex + ey * ey + ez * ez) / s;
        ex /= s1;        ey /= s1;        ey /= s1;
    }

    if (key == GLUT_KEY_RIGHT)
        //Rotate camera around origin in Oxz plane
    {
        dx = -ez;     dz = ex;
        GLfloat s = sqrtf(ex * ex + ey * ey + ez * ez);
        ex -= delta * dx;              ez -= delta * dz;
        GLfloat s1 = sqrtf(ex * ex + ey * ey + ez * ez) / s;
        ex /= s1;        ey /= s1;        ey /= s1;
    }


    if (key == GLUT_KEY_F1) bCull = !bCull;
    if (key == GLUT_KEY_F2)bDepth = !bDepth;
    if (key == GLUT_KEY_F3)bOutline = !bOutline;
    if (key == GLUT_KEY_F4)
    {
        if (shademode == GL_FLAT) { shademode = GL_SMOOTH; }
        else { if (shademode == GL_SMOOTH) { shademode = GL_FLAT; } };
        glShadeModel(shademode);
    }
    if (key == GLUT_KEY_F5)
        luz[1] += 1;
    if (key == GLUT_KEY_F6)
        luz[1] -= 1;
    if (key == GLUT_KEY_F7)
        luz[0] += 1;
    if (key == GLUT_KEY_F8)
        luz[0] -= 1;
    if (key == GLUT_KEY_F9)
        ey += 1.5;
    if (key == GLUT_KEY_F10)
        ey -= 1.5;

    // Refresh the Window
    glutPostRedisplay();


    // Refresh the Window
    glutPostRedisplay();
}


///////////////////////////////////////////////////////////



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
    glutInitWindowSize(200, 200);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);

    SetupRC();
    glutMainLoop();
    return 0;
}
