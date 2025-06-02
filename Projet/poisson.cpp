#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>



#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif
using namespace std;

/*class Point*/
class Point{
    public :
	//coordonnées x, y et z du point
	double x;
	double y;
	double z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};
//Tableau pour stocker les sommets du cube et leur couleur
Point pCube[8]={
    {-0.5,-0.5, 0.5,1,1,1},
    { 0.5, -0.5, 0.5,1,1,1},
    { 0.5, -0.5, -0.5,1,1,1},
    { -0.5, -0.5, -0.5,11,1,1},
    { -0.5,0.5, 0.5,1,1,1},
    { 0.5, 0.5, 0.5,1,1,1},
    { 0.5, 0.5, -0.5,1,1,1},
    { -0.5, 0.5, -0.5,1,1,1}};

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4]={
  {0,3,2,1},
  {0,1,5,4},
  {1,2,6,5},
  {2,3,7,6},
  {0,4,7,3},
  {4,5,6,7}};

char presse;
int anglex,angley,x,y,xold,yold;
Point sp[8*2];
float zoomf =1.0;
const int largimg = 256;
const int hautimg = 256;
unsigned char texture[largimg][hautimg][3];
const int maxR = 10;
float lightAngle = 0.0f;
bool anim = false;
float finAxeY = 1.0f;
float finAxeZ = 1.0f;
int finDirection = -1;
float angleNageoiresL = 0.0f;
int directionNageoiresL = 1; // Direction de la rotation
const float maxAngleNageoiresL = 30.0f; // Angle maximum de rotation
const float vitesseRotationNageoiresL = 2.0f; // Vitesse de rotation



GLuint tex1;
GLuint tex2;
GLuint tex3;
GLuint tex4;


/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void Lumieres();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void Ellipsoide(float x,float y,float z,int n);
void Cube();


void Corps();
void Yeux();
void Nageoire();
void NageoiresL();
void Environement();
void loadJpegImage(char * fichier);
void Manu(int key,int x,int y);
void animer();

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
      glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
      glutInitWindowPosition(200,200);
      glutInitWindowSize(500,500);
      glutCreateWindow("Poisson");
/* Initialisation d'OpenGL */
  glClearColor(0,0,0,0.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);
  /*Géneration des id pour les textures*/
glGenTextures(1, &tex1);
glBindTexture(GL_TEXTURE_2D, tex1);
loadJpegImage("./Clownfish.jpeg");
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hautimg, largimg, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

glGenTextures(1, &tex2);
glBindTexture(GL_TEXTURE_2D, tex2);
loadJpegImage("./TextureNageoire.jpeg");
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hautimg, largimg, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

glGenTextures(1, &tex3);
glBindTexture(GL_TEXTURE_2D, tex3);
loadJpegImage("./Eau.jpeg");
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hautimg, largimg, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);



  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutIdleFunc(Lumieres);
  glutSpecialFunc(Manu);
  glutReshapeFunc(reshape);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


    void Lumieres() {

    finAxeY += finDirection * 0.05f; //
    finAxeZ += finDirection * 0.05f;


    if (finAxeY >= 1.0f || finAxeY <= -1.0f)
    {
        finDirection *= -1;
    }
       lightAngle += 0.06f;  // Ajustez la vitesse de rotation si nécessaire
    GLfloat lightX = 1.0f * cos(lightAngle);  // Ajustez la distance si nécessaire
    GLfloat lightZ = -1.0f * sin(lightAngle);  // Ajustez la distance si nécessaire

    GLfloat lightPosition[] = {lightX, 0.0f, lightZ, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Configuration de la lumière directionnelle
    GLfloat lightDirection[] = {lightX, 0.0f, lightZ, 0.0f};  // Direction de la lumière (vers le bas)
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Couleur de la lumière (blanche)
    GLfloat lightAmbient[] = {1.0f, 0.8f, 0.0f, 1.0f};  // Couleur ambiante dorée


    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);


    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);  // Angle d'éclairage de 360 degrés pour une lumière directionnelle


    glEnable(GL_LIGHT0);
    glutPostRedisplay();
    }

    void animer() {
    if (anim == true) {
        // Mise à jour de l'angle des nageoires
        angleNageoiresL += directionNageoiresL * vitesseRotationNageoiresL;

        // Inversion de la direction à l'atteinte des limites
        if (angleNageoiresL >= maxAngleNageoiresL || angleNageoiresL <= -maxAngleNageoiresL) {
            directionNageoiresL *= -1;
        }
    }
}

    void Cube()
    {
        glBindTexture(GL_TEXTURE_2D, tex3);

        for (int i = 0; i < 6; i++) {
            glBegin(GL_POLYGON);
            int vertexIndex = fCube[i][0];

    glTexCoord2f(0,0);
    glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

    vertexIndex = fCube[i][1];

    glTexCoord2f(0,1);
    glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

    vertexIndex = fCube[i][2];

    glTexCoord2f(1, 1);
    glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);

    vertexIndex = fCube[i][3];

    glTexCoord2f(0, 1);
    glVertex3f(pCube[vertexIndex].x, pCube[vertexIndex].y, pCube[vertexIndex].z);
            glEnd();
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

void Ellipsoide(int NP, int NM, float r,float a,float b,float c)
{


    Point * points = new Point[NP*NM];
    int faces[NM*(NP-1)][4];

    for(int j=0;j<NP;j++)
    {
        for(int i=0;i<NM;i++)
        {
            points[NM*j+i].x=a*r*cos(i*2*M_PI/NM)*cos((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].y=b*r*sin(i*2*M_PI/NM)*cos((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].z=c*r*sin((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].r= 0.5;
            points[NM*j+i].g= 0.5;
            points[NM*j+i].b= 0.5;
        }
    }

    for(int j=0;j<NP-1;j++)
    {
        for(int i=0;i<NM;i++)
        {
            faces[j*NM+i][0]=j*NM+i;
            faces[j*NM+i][1]=NM*j+(i+1)%NM;
            faces[j*NM+i][2]=NM*(j+1)+(i+1)%NM;
            faces[j*NM+i][3]=NM*(j+1)+i;
        }
    }
    glBindTexture(GL_TEXTURE_2D,tex1);

    for(int j=0;j<NP-1;j++)
    {
        for(int i=0;i<NM;i++)
        {
            glBegin(GL_POLYGON);
            // Calcul de la normale pour la face courante
            float normalX = points[faces[j*NM+i][0]].x;
            float normalY = points[faces[j*NM+i][0]].y;
            float normalZ = points[faces[j*NM+i][0]].z;

            // Calcul de la longueur de la normale
            float length = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);

            // Normalisation de la normale
            if (length > 0)
            {
                normalX /= length;
                normalY /= length;
                normalZ /= length;
            }

            glNormal3f(normalX, normalY, normalZ);

            glTexCoord2f(double(i)/NM,1-double(j)/(NP-1));   glVertex3f(points[faces[j*NM+i][0]].x,points[faces[j*NM+i][0]].y,points[faces[j*NM+i][0]].z);
            glTexCoord2f(double(i+1)/NM,1-double(j)/(NP-1));   glVertex3f(points[faces[j*NM+i][1]].x,points[faces[j*NM+i][1]].y,points[faces[j*NM+i][1]].z);
            glTexCoord2f(double(i+1)/NM,1-double(j+1)/(NP-1));   glVertex3f(points[faces[j*NM+i][2]].x,points[faces[j*NM+i][2]].y,points[faces[j*NM+i][2]].z);
            glTexCoord2f(double(i)/NM,1-double(j+1)/(NP-1));   glVertex3f(points[faces[j*NM+i][3]].x,points[faces[j*NM+i][3]].y,points[faces[j*NM+i][3]].z);
            glEnd();
        }
    }
    glBindTexture(GL_TEXTURE_2D,0);



}
void Ellipsoide2(int NP, int NM, float r,float a,float b,float c)
{


    Point * points = new Point[NP*NM];
    int faces[NM*(NP-1)][4];

    for(int j=0;j<NP;j++)
    {
        for(int i=0;i<NM;i++)
        {
            points[NM*j+i].x=a*r*cos(i*2*M_PI/NM)*cos((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].y=b*r*sin(i*2*M_PI/NM)*cos((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].z=c*r*sin((j*M_PI)/(NP-1)-(M_PI/2));
            points[NM*j+i].r= 0.5;
            points[NM*j+i].g= 0.5;
            points[NM*j+i].b= 0.5;
        }
    }

    for(int j=0;j<NP-1;j++)
    {
        for(int i=0;i<NM;i++)
        {
            faces[j*NM+i][0]=j*NM+i;
            faces[j*NM+i][1]=NM*j+(i+1)%NM;
            faces[j*NM+i][2]=NM*(j+1)+(i+1)%NM;
            faces[j*NM+i][3]=NM*(j+1)+i;
        }
    }
    glBindTexture(GL_TEXTURE_2D,tex2);

    for(int j=0;j<NP-1;j++)
    {
        for(int i=0;i<NM;i++)
        {
            glBegin(GL_POLYGON);
            // Calcul de la normale pour la face courante
            float normalX = points[faces[j*NM+i][0]].x;
            float normalY = points[faces[j*NM+i][0]].y;
            float normalZ = points[faces[j*NM+i][0]].z;

            // Calcul de la longueur de la normale
            float length = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);

            // Normalisation de la normale
            if (length > 0)
            {
                normalX /= length;
                normalY /= length;
                normalZ /= length;
            }

            glNormal3f(normalX, normalY, normalZ);

            glTexCoord2f(double(i)/NM,1-double(j)/(NP-1));   glVertex3f(points[faces[j*NM+i][0]].x,points[faces[j*NM+i][0]].y,points[faces[j*NM+i][0]].z);
            glTexCoord2f(double(i+1)/NM,1-double(j)/(NP-1));   glVertex3f(points[faces[j*NM+i][1]].x,points[faces[j*NM+i][1]].y,points[faces[j*NM+i][1]].z);
            glTexCoord2f(double(i+1)/NM,1-double(j+1)/(NP-1));   glVertex3f(points[faces[j*NM+i][2]].x,points[faces[j*NM+i][2]].y,points[faces[j*NM+i][2]].z);
            glTexCoord2f(double(i)/NM,1-double(j+1)/(NP-1));   glVertex3f(points[faces[j*NM+i][3]].x,points[faces[j*NM+i][3]].y,points[faces[j*NM+i][3]].z);
            glEnd();
        }
    }
    glBindTexture(GL_TEXTURE_2D,0);



}
void Corps()
{

    glPushMatrix();
    glColor3f(0.0, 0.5, 1.0);
    glScalef(2.0, 0.6, 0.6);
    Ellipsoide(40, 500, 1, 1.5, 2, 0.8);
    glPopMatrix();
}
void Yeux()
{
 glPushMatrix();
      glColor3f(0,0,0);
      glTranslatef(2,0,-0.4);
      glRotatef(45,0,0,1);
      glutSolidSphere(0.2,16,16);
    glPopMatrix();
     glPushMatrix();
      glColor3f(0,0,0);
      glTranslatef(2,0,0.4);
      glRotatef(45,0,0,1);
      glutSolidSphere(0.2,16,16);
    glPopMatrix();
}
void Nageoire()
{

    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(-3.5, 0.5, 0);
    glScalef(1.5, 0.6, 0.6);
    glRotatef(-30, 0.1, finAxeY, finAxeZ);
    Ellipsoide2(20, 20, 0.3, 3, 3, 0.3);

    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(-3.5, -0.5, 0);
    glScalef(1.5, 0.6, 0.6);
    glRotatef(-30, 0.1, finAxeY, finAxeZ);
    Ellipsoide2(20, 20, 0.3, 3, 3, 0.3);

    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0, 1.1, 0);
    glScalef(1.5, 0.6, 0.6);
    Ellipsoide2(20, 20, 0.3, 3, 3, 0.3);

    glPopMatrix();
}
void Environement()
{

 glPushMatrix();
 glTranslatef(0,-1.1,0);
 glScalef(10,0.5,10);
 Cube();
 glPopMatrix();

 glPushMatrix();
 glTranslatef(0,4,-5);
 glRotatef(90,1, 0, 0);
 glScalef(10,0.5,10);
 Cube();
 glPopMatrix();

  glPushMatrix();
 glTranslatef(-5,4,0);
 glRotatef(-90,0, 0, -1);
 glScalef(10,0.5,10);
 Cube();
 glPopMatrix();
}

void NageoiresL()
{
    glPushMatrix();
    glColor3f(255, 0, 0);
    glTranslatef(0.5, -0.3, 0.7);
    glRotatef(45 + angleNageoiresL, 0, 1, 0);
    glScalef(5,1,5);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.2, 0.5, 0.0);
        glVertex3f(-0.2, 0.5, 0.0);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(255, 0, 0);
    glTranslatef(0.5, -0.3, -0.7);
    glRotatef(-45 + angleNageoiresL, 0, 1, 0);
    glScalef(5,1,5);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.2, 0.5, 0.0);
        glVertex3f(-0.2, 0.5, 0.0);
    glEnd();
    glPopMatrix();
}
void affichage()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_TEXTURE_2D);
   glLoadIdentity();



  glOrtho(-5,5,-5,5,-5,5);
  glScalef(zoomf,zoomf,1);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);



    Corps();
    NageoiresL();
    Nageoire();
    Yeux();
    Environement();
    animer();


  glFlush();
  glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p':
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f':
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' :
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'z':
        zoomf-=0.2f;
        glutPostRedisplay();
        break;
    case 'Z':
        zoomf+=0.2f;
        glutPostRedisplay();
        break;
    case 'i':
         anim = true;
    break;
    case 'I':
         anim = false;
    break;
    case 'q' :
      exit(0);
    }
}
void Manu(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:
        angley+=5;
        glutPostRedisplay();
        break;
        case GLUT_KEY_UP:
        angley-=5;
        glutPostRedisplay();
        break;
        case GLUT_KEY_LEFT:
        anglex-=5;
        glutPostRedisplay();
        break;
        case GLUT_KEY_RIGHT:
        anglex+=5;
        glutPostRedisplay();
        break;
        default:
	  break;
    }
}
void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}
void mouse(int button, int state,int x,int y)
{

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1;
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }

  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0;
}
void mousemotion(int x,int y)
  {
    if (presse)
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }
void loadJpegImage(char *fichier)
{
    unsigned char* image = (unsigned char*) (malloc(sizeof(char) * 3 * 256 * 256));

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *file;
    unsigned char *ligne;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    #ifdef _WIN32
    if (fopen_s(&file, fichier, "rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
    #elif __GNUC__
    if ((file = fopen(fichier,"rb")) == 0)
    {
        fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
        exit(1);
    }
    #endif
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);


    if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
        fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
        exit(1);
    }

    jpeg_start_decompress(&cinfo);
    ligne=image;
    while (cinfo.output_scanline<cinfo.output_height)
    {
        ligne=image+3*256*cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo,&ligne,1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    //boucle qui permet de faire la texture
    for(int i = 0; i<256 ; i++)
    {
        for(int j = 0; j<256; j++)
        {
            texture[i][j][0] = image[i*256*3+j*3];
            texture[i][j][1] = image[i*256*3+j*3+1];
            texture[i][j][2] = image[i*256*3+j*3+2];
        }
    }

    free(image);
}
