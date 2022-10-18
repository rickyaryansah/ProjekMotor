// Muhammad Ricky Aryansah | E1E121033
#include<GL/glut.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define PI 3.14159
#define WIN_WIDTH 1366
#define WIN_HEIGHT 768
#define BIKE_LENGTH 3.3f
#define ROD_RADIUS 0.05f
#define GAS_TANK 0.3f
#define NUM_SPOKES 5
#define SPOKE_ANGLE 72
#define RADIUS_WHEEL 1.0f
#define TUBE_WIDTH 0.20f
#define RIGHT_ROD 2.35f
#define RIGHT_ANGLE 33.0f
#define MIDDLE_ROD 1.8f
#define MIDDLE_ANGLE 125.0f
#define BACK_CONNECTOR 0.5f
#define LEFT_ANGLE 50.0f
#define WHEEL_OFFSET 0.11f
#define WHEEL_LEN 0.8f
#define TOP_LEN 1.0f
#define FRONT_ROD 2.5f
#define CRANK_ROD 1.9f
#define CRANK_RODS 2.5f
#define CRANK_ANGLE 0.0f
#define HANDLE_ROD 1.2f
#define FRONT_INCLINE 38.0f
#define HANDLE_LIMIT 30.0f
#define INC_STEERING 2.0f
#define INC_SPEED 0.05f
int light_value = 0; //Mencerahkan seluruh objek

GLfloat pedalAngle, speed, steering; //Deklarasi posisi pedal, kecepatan, setir

GLfloat camx,camy,camz; //Deklarasi kamera berdasarkan sumbu x, y dan z
GLfloat anglex,angley,anglez; //Deklarasi sudut berdasarkan sumbu x, y dan z

GLint skybox[6], grass, background=1000 ;
const int SKY_FRONT=0,SKY_RIGHT=1,SKY_LEFT=2,SKY_BACK=3,SKY_UP=4,SKY_DOWN=5 ;

int prevx,prevy;
GLenum Mouse; //Mengatur sudut kamera pakai mouse

int flag = 0 ;
GLfloat xpos,zpos,direction; //Deklarasi posisi dan arah sepeda
void welcome_window(); //Jendela Selamat Datang
void operations_window(); //Jendela instruksi
void ZCylinder(GLfloat radius,GLfloat length); //Untuk membuat objek silinder sumbu z
void XCylinder(GLfloat radius,GLfloat length); //Untuk membuat objek silinder sumbu x
void drawFrame(void);
void gear( GLfloat inner_radius, GLfloat outer_radius,GLfloat width,GLint teeth, GLfloat tooth_depth ); //Untuk membuat gear motor
void drawTyre(void); //Deklarasi membuat ban
void drawSeat(void); //Deklarasi membuat tempat duduk
void init(void);
void reset(void); //Mengembalikan posisi awal
GLfloat radians(GLfloat);
GLfloat angleSum(GLfloat, GLfloat);

GLfloat degrees(GLfloat a) //Membuat objek belok dengan derajat
{
    return a*180.0f/PI;
}

GLfloat radians(GLfloat a) //Menjalankan Motor
{
    return a*PI/180.0f;
}

GLfloat angleSum(GLfloat a, GLfloat b) //Membuat motor belok
{
    a += b;
    if (a < 0) return a+2*PI;
    else if (a > 2*PI) return a-2*PI;
    else return a;
}

void writeText(float x,float y, float z, char *string) //Menampilkan kecepatan motor
{

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glColor3f(1,0,0); //Untuk memberi warna tulisan
  glClear(GL_COLOR_BUFFER_BIT ); //Untuk membersihkan layar latar belakang

  glRasterPos3f(x, y, z); //Menentukan posisi raster untuk operasi piksel
  for(int i = 0; i< (int) strlen(string); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]); //Menentukan jenis dan ukuran font
  }
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
}

void bitmap_output(float x, float y, float z, char *string)
{
    int len, i;
    glRasterPos3f(x,y,z);
    len = (int) strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}


void ZCylinder(GLfloat radius,GLfloat length)
{
    GLUquadricObj *cylinder;
    cylinder=gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.0f);
    gluCylinder(cylinder,radius,radius,length,15,5);
    glPopMatrix();
}

void XCylinder(GLfloat radius,GLfloat length)
{
    glPushMatrix();
    glRotatef(90.0f,0.0f,1.0f,0.0f);
    ZCylinder(radius,length);
    glPopMatrix();
}

//Elvi Saktiawati Salemaku |E1E121001
void updateScene()
{
    GLfloat xDelta, zDelta;
    GLfloat rotation;
    GLfloat sin_steering, cos_steering;
    if (-INC_SPEED < speed && speed < INC_SPEED)
        return;
    if(speed < 0.0f)
        pedalAngle = speed = 0.0f;

    xDelta = speed*cos(radians(direction + steering));
    zDelta = speed*sin(radians(direction + steering));
    xpos += xDelta;
    zpos -= zDelta;
    pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL));
    sin_steering = sin(radians(steering));
    cos_steering = cos(radians(steering));

    rotation = atan2(speed * sin_steering, BIKE_LENGTH + speed * cos_steering);
    direction = degrees(angleSum(radians(direction),rotation));
}

void drawFrame()
{
    glColor3f(1.0f,0.0f,0.0f);

    glPushMatrix();

    glPushMatrix();

    glColor3f(0.0f,1.0f,0.0f);


    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.06f);
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    gear(0.08f,0.3f,0.03f,30,0.03f);
    glPopMatrix();

    glColor3f(1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,-0.2f);
    ZCylinder(0.08f,0.32f);
    glPopMatrix();

    glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,RIGHT_ROD);


    glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,MIDDLE_ROD);

    glColor3f(1.0f,1.0f,1.0f);
    glTranslatef(MIDDLE_ROD,0.0f,0.0f);
    glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f);
    glScalef(0.3f,ROD_RADIUS,0.25f);
    drawSeat();

    glColor3f(1.0f,0.0f,0.0f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-180.0f,0.0f,1.0f,0.0f);
    XCylinder(ROD_RADIUS,BACK_CONNECTOR);


    glPushMatrix();
    glTranslatef(0.5f,0.0f,WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.5f,0.0f,-WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f);

    glPushMatrix();
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    drawTyre();
    glColor3f(0.0f,1.0f,0.0f);
    gear(0.03f,0.15f,0.03f,20,0.03f);
    glColor3f(1.0f,0.0f,0.0f);
    glPopMatrix();
    glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);


    glPushMatrix();
    glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,WHEEL_LEN);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,WHEEL_LEN);
    glPopMatrix();


    glTranslatef(WHEEL_LEN,0.0f,0.0f);
    XCylinder(ROD_RADIUS,CRANK_ROD-1.0);
    glTranslatef(CRANK_ROD,0.0f,0.0f);
    glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(0.0000000001,TOP_LEN);

    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(-0.6,-0.6f,0.0f);
    XCylinder(GAS_TANK,1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(0.6,0.5f,0.0f);
    XCylinder(GAS_TANK,0.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(-3.2,-2.1f,0.3f);
    XCylinder(ROD_RADIUS+0.1f,1.5f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(-3.2,-2.1f,0.3f);
    glRotatef(120.0,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS+0.1f,0.5f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(-3.2,-2.1f,-0.3f);
    XCylinder(ROD_RADIUS+0.1f,1.5f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(-3.2,-2.1f,-0.3f);
    glRotatef(120.0,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS+0.1f,0.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(0.4,-0.8f,0.2f);
    glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glTranslatef(0.2,-0.8f,0.2f);
    glRotatef(LEFT_ANGLE-50.0,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glPopMatrix();

    glPushMatrix(); //berpengaruh pada bagian rangka depan objek (stir motor)
    glColor3f(0.5f,0.5f,0.5f);
    glTranslatef(0.4,-0.8f,-0.2f);
    glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glTranslatef(0.2,-0.8f,-0.2f);
    glRotatef(LEFT_ANGLE-50.0,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glPopMatrix();

    glPushMatrix();//berpengaruh pada bagian rangka depan objek (stir motor)
    glColor3f(0.1,0.1,0.1);
    glTranslatef(-0.4f,-1.2f,0.0f);
    XCylinder(GAS_TANK,1.3f);
    glPopMatrix();

    glTranslatef(TOP_LEN,0.0f,0.0f); //Fungsi glTranslatef mengalikan matriks saat ini dengan matriks terjemahan (stir atas)
    glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f);


    glPushMatrix();//berhubungan dengan pergerakan objek orang, rantai dan lampu motor
    glRotatef(-steering/2.0,1.0f,0.0f,0.0f);

    glTranslatef(-0.3f,0.0f,0.0f);

    glPushMatrix();//berhubungan dengan pergerakan objek orang, rantai dan lampu motor
    glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f);
    glPushMatrix();//berhubungan dengan pergerakan objek orang, rantai dan lampu motor
    glTranslatef(-0.6f,0.5f,-HANDLE_ROD/2);
    ZCylinder(ROD_RADIUS,HANDLE_ROD);
    glPopMatrix();//mengatur panjang batang sebelah kiri bagian depan motor
    glPushMatrix();//berhubungan dengan pergerakan objek orang, rantai dan lampu motor
    glColor3f(1.0f,1.0f,0.0f);//warna setir
    glTranslatef(-0.6f,0.5f,-HANDLE_ROD/2);//mengatur posisi setir
    ZCylinder(0.07f,HANDLE_ROD/4);
    glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4);
    ZCylinder(0.07f,HANDLE_ROD/4);
    glColor3f(1.0f,0.0f,0.0f);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();

    glTranslatef(-0.75,0.0,0.0);//mengatur panjang batang depan motor
    XCylinder(ROD_RADIUS,FRONT_ROD);//panjang batang atas

    glTranslatef(CRANK_ROD,0.0f,0.0f);//memendekkan bagian depan motor
    glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f);//mengantur panjang depan motor

    glPushMatrix();//berhubungan dengan pergerakan objek orang, rantai dan lampu motor
    glTranslatef(0.0f,0.0f,WHEEL_OFFSET-0.35);//mengatur ukuran panjang batang depan
    XCylinder(ROD_RADIUS,CRANK_RODS);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-WHEEL_OFFSET+0.35);
    XCylinder(ROD_RADIUS,CRANK_RODS);
    glPopMatrix();

    glTranslatef(CRANK_RODS,0.0f,0.0f);
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    drawTyre();                             //kepala
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0,1.0,0.0);
    glRotatef(360.0,1.0,0.0,0.0);
    glTranslatef(1.0,2.6,0.0);                  //lampu motor
    glutSolidSphere(0.2,160.0,180.0);
    glPopMatrix();
}

//Salsabila Qodriyah_E1E121091
void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,GLint teeth, GLfloat tooth_depth )
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;
    const double pi = 3.14159264;
    r0 = inner_radius;
    r1 = outer_radius - tooth_depth/2.0;
    r2 = outer_radius + tooth_depth/2.0;
    da = 2.0*pi / teeth / 4.0;
    glShadeModel( GL_FLAT );
    glNormal3f( 0.0, 0.0, 1.0 );

    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++)
    {
        angle = i * 2.0*pi / teeth;
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();

    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++)
    {
        angle = i * 2.0*pi / teeth;
        glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
        glVertex3f( r2*cos(angle+da), r2*sin(angle+da), width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();
    glNormal3f( 0.0, 0.0, -1.0 );

    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++)
    {
        angle = i * 2.0*pi / teeth;
        glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
    }
    glEnd();

    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++)
    {
        angle = i * 2.0*pi / teeth;
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
        glVertex3f( r2*cos(angle+da), r2*sin(angle+da), -width*0.5 );
        glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
    }
    glEnd();

    glBegin( GL_QUAD_STRIP );
    for (i=0;i<teeth;i++)
    {
        angle = i * 2.0*pi / teeth;
        glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
        u = r2*cos(angle+da) - r1*cos(angle);
        v = r2*sin(angle+da) - r1*sin(angle);
        len = sqrt( u*u + v*v );
        u /= len;
        v /= len;
        glNormal3f( v, -u, 0.0 );
        glVertex3f( r2*cos(angle+da), r2*sin(angle+da), width*0.5 );
        glVertex3f( r2*cos(angle+da), r2*sin(angle+da), -width*0.5 );
        glNormal3f( cos(angle), sin(angle), 0.0 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
        u = r1*cos(angle+3*da) - r2*cos(angle+2*da);
        v = r1*sin(angle+3*da) - r2*sin(angle+2*da);
        glNormal3f( v, -u, 0.0 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glNormal3f( cos(angle), sin(angle), 0.0 );
    }
        glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 );
        glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 );
        glEnd();
        glShadeModel( GL_SMOOTH );

        glBegin( GL_QUAD_STRIP );
        for (i=0;i<=teeth;i++)
        {
            angle = i * 2.0*pi / teeth;
            glNormal3f( -cos(angle), -sin(angle), 0.0 );
            glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
            glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        }
        glEnd();
}

void drawChain()
{
    GLfloat depth;
    static int mode=0;
    glColor3f(0.0f,1.0f,0.0f);
    glEnable(GL_LINE_STIPPLE);
    mode=(mode+1)%2;

    if(mode==0 && speed>0)
    glLineStipple(1,0x1c47);
    else if(mode==1 && speed>0)
    glLineStipple(1,0x00FF);
    glBegin(GL_LINES);
    for(depth=0.06f;depth<=0.12f;depth+=0.01f)
    {
        glVertex3f(-1.6f,0.15f,ROD_RADIUS);
        glVertex3f(0.0f,0.3f,depth);

        glVertex3f(-1.6f,-0.15f,ROD_RADIUS);
        glVertex3f(0.0f,-0.3f,depth);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}


void drawSeat()
{

    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, 1.0f, -0.5f);
    glVertex3f( 1.0f, 1.0f, -0.3f);
    glVertex3f( 1.0f, 1.0f, 0.3f);
    glVertex3f(-0.1f, 1.0f, 0.5f);
    glVertex3f(-0.5f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-0.5f, 1.0f, -1.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, -1.0f, -0.5f);
    glVertex3f( 1.0f, -1.0f, -0.3f);
    glVertex3f( 1.0f, -1.0f, 0.3f);
    glVertex3f(-0.1f, -1.0f, 0.5f);
    glVertex3f(-0.5f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-0.5f, -1.0f, -1.0f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(1.0f,1.0f,-0.3f);
    glVertex3f(1.0f,1.0f,0.3f);
    glVertex3f(1.0f,-1.0f,0.3f);
    glVertex3f(1.0f,-1.0f,-0.3f);
    glVertex3f(1.0f,1.0f,0.3f);
    glVertex3f(-0.1f,1.0f,0.5f);
    glVertex3f(-0.1f,-1.0f,0.5f);
    glVertex3f(1.0f,-1.0f,0.3f);
    glVertex3f(1.0f,1.0f,-0.3f);
    glVertex3f(-0.1f,1.0f,-0.5f);
    glVertex3f(-0.1f,-1.0f,-0.5f);
    glVertex3f(1.0f,-1.0f,-0.3f);
    glVertex3f(-0.1f,1.0f,0.5f);
    glVertex3f(-0.5f,1.0f,1.0f);
    glVertex3f(-0.5f,-1.0f,1.0f);
    glVertex3f(-0.1f,-1.0f,0.5f);
    glVertex3f(-0.1f,1.0f,-0.5f);
    glVertex3f(-0.5f,1.0f,-1.0f);
    glVertex3f(-0.5f,-1.0f,-1.0f);
    glVertex3f(-0.1f,-1.0f,-0.5f);
    glVertex3f(-0.5f,1.0f,1.0f);
    glVertex3f(-1.0f,1.0f,1.0f);
    glVertex3f(-1.0f,-1.0f,1.0f);
    glVertex3f(-0.5f,-1.0f,1.0f);
    glVertex3f(-0.5f,1.0f,-1.0f);
    glVertex3f(-1.0f,1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-0.5f,-1.0f,-1.0f);
    glVertex3f(-1.0f,1.0f,1.0f);
    glVertex3f(-1.0f,1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,1.0f);
    glEnd();
}
