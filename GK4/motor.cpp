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
    glColor3f(1.0f,1.0f,0.0f);

    glPushMatrix();

    glPushMatrix();

    glColor3f(1.0f,1.0f,0.0f); //warna gear


    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.06f);
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    gear(0.08f,0.3f,0.03f,30,0.03f);
    glPopMatrix();

    glColor3f(1.0f,1.0f,0.0f); //rangka depan v
    glTranslatef(0.0f,0.0f,-0.2f);
    ZCylinder(0.08f,0.32f);
    glPopMatrix();

    glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,RIGHT_ROD);


    glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,MIDDLE_ROD);

    glColor3f(1.0f,1.0f,0.0f); //tidak tau
    glTranslatef(MIDDLE_ROD,0.0f,0.0f);
    glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f);
    glScalef(0.3f,ROD_RADIUS,0.25f);
    drawSeat();

    glColor3f(1.0f,1.0f,0.0f); //warna rangka bawah
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
    drawTyre(); //Ban belakang
    glColor3f(1.0f,1.0f,0.0f); //bulatan tengah belakang ban
    gear(0.03f,0.15f,0.03f,20,0.03f);
    glColor3f(1.0f,1.0f,0.0f);  //rangka belakang
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
    glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f); //angkat ban
    XCylinder(0.0000000001,TOP_LEN);

    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f); // warna slinder atas
    glTranslatef(-0.6,-0.6f,0.0f);
    XCylinder(GAS_TANK,1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);//warna tabung senter
    glTranslatef(0.6,0.5f,0.0f);
    XCylinder(GAS_TANK,0.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glTranslatef(-3.2,-2.1f,0.3f);
    XCylinder(ROD_RADIUS+0.1f,1.5f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f); //bagian belakang kanalpot bagian atas
    glTranslatef(-3.2,-2.1f,0.3f);
    glRotatef(120.0,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS+0.1f,0.5f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f); //warna knalpot
    glTranslatef(-3.2,-2.1f,-0.3f);
    XCylinder(ROD_RADIUS+0.1f,1.5f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f); //warna knalpot
    glTranslatef(-3.2,-2.1f,-0.3f);
    glRotatef(120.0,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS+0.1f,0.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f); //warna knalpot
    glTranslatef(0.4,-0.8f,0.2f);
    glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glTranslatef(0.2,-0.8f,0.2f);
    glRotatef(LEFT_ANGLE-50.0,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glPopMatrix();

    glPushMatrix(); //berpengaruh pada bagian rangka depan objek (stir motor)
    glColor3f(1.0f,1.0f,0.0f);
    glTranslatef(0.4,-0.8f,-0.2f);
    glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glTranslatef(0.2,-0.8f,-0.2f);
    glRotatef(LEFT_ANGLE-50.0,0.0f,0.0f,1.0f);
    XCylinder(GAS_TANK,1.0f);
    glPopMatrix();

    glPushMatrix();//berpengaruh pada bagian rangka depan objek (stir motor)
    glColor3f(1.0,1.0,0.0);
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
    glColor3f(1.0f,1.0f,0.0f); //warna rangka depan
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
    drawTyre(); //Ban depan
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0,1.0,0.0);     //warna lampu
    glRotatef(360.0,1.0,0.0,0.0);
    glTranslatef(1.0,2.6,0.0);                  //lampu motor
    glutSolidSphere(0.2,160.0,180.0);
    glPopMatrix();
}

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
    glColor3f(1.0f,1.0f,0.0f); //warna rantai
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

    glColor3f(1.0f,1.0f,0.0f);
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


void drawPedals()
{
    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.105f);
    glTranslatef(0.25f,0.0f,0.0f);

    glPushMatrix();
    glScalef(0.5f,0.1f,0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25f,0.0f,0.15f);
    glScalef(0.2f,0.02f,0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f,0.0f,-0.105f);
    glTranslatef(0.25f,0.0f,0.0f);

    glPushMatrix();
    glScalef(0.5f,0.1f,0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25f,0.0f,-0.15f);
    glScalef(0.2f,0.02f,0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();
    glColor3f(1.0f,0.0f,0.0f);
}

void drawTyre(void)
{
    int i;

    glColor3f(1.0f,1.0f,0.0f); //warna knalpot juga
    glutSolidTorus(-0.17,0.29,100,100);

    glColor3f(1.0f,1.0f,0.0f); //warna ban dalam
    glutSolidTorus(-0.05f,-0.75f,100,100);

    glColor3f(1.0f,1.0f,0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-0.06f);
    ZCylinder(0.02f,0.12f);
    glPopMatrix();
    glutSolidTorus(0.02f,0.02f,3,20);

    glColor3f(1.0f,1.0f,0.0f); // warna felek ban
    for(i=0;i<NUM_SPOKES;++i)
    {
        glPushMatrix();
        glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f,0.02f,0.0f);
        glVertex3f(0.0f,0.86f,0.0f);
        glEnd();
        glPopMatrix();
    }

    glColor3f(1.0f,1.0f,0.0f);                      // ubah warna ban
    glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30);
    glColor3f(1.0f,1.0f,0.0f);                      // ubah warna ban
}


void lighting() // FUNCTION FOR LIGHTING
{
    GLfloat light_directional[]={1.0,1.0,1.0,1.0};
    GLfloat light_positional[]={1.0,1.0,1.0,1.0};
    GLfloat light_diffuse[]={1.0,1.0,1.0};
    GLfloat light_ambient[]={1.0f,1.0f,1.0f,1.0f};
    if(light_value == 0){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient) ;
        glLightfv(GL_LIGHT0,GL_POSITION,light_positional);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    }
    else if(light_value == 1){
        glLightfv(GL_LIGHT0,GL_POSITION,light_positional);
    }
    else if(light_value == 2){
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    }
    else if(light_value==3){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient) ;
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


void shading() // FUNCTION FOR SHADING BIKE
{
    GLfloat mat_spec[]={1.0,1.0,1.0,1.0};
    GLfloat mat_shine[]={100.0};
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shine);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
    glColorMaterial(GL_FRONT,GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

GLuint loadimage(const char *fileName)
{
	FILE *file;
	unsigned char header[54],*data;
	unsigned int dataPos,size,width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);
	dataPos		= *(int*)&(header[0x0A]);
	size		= *(int*)&(header[0x22]);
	width		= *(int*)&(header[0x12]);
	height		= *(int*)&(header[0x16]);
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void init_texture()
{
    grass = loadimage("grass_1.bmp") ;
}

void init() // INITIALIZING SCENE
{
    reset();
    //glClearColor(1.0,0.0,0.0,0.0);
}
void draw_ground()
{
    glClearColor(1.0,1.0,1.0,0.0);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,grass);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f,-1.5f,100.0f);;
	//glTexCoord2f(100.0f, 0.0f); glVertex3f(-100.0f,-1.5f,-100.0f);;
	//glTexCoord2f(100.0f, 100.0f); glVertex3f(100.0f,-1.5f,-100.0f);
	//glTexCoord2f(0.0f, 100.0f); glVertex3f(100.0f,-1.5f,100.0f);
	//glEnd();
	glDisable(GL_TEXTURE_2D);
	//glLineWidth(5.0);
	//glTranslatef(0.0, -2, 0.0);
	//glTranslatef(0.0, 2, 0.0);
}

void display_bike(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);

    // drawscore(200);
    char speedString[100];
    sprintf(speedString, "Speed: %f", speed*10);
    writeText(-0.23, 2, 1, speedString);

    lighting();
    shading();
    glPushMatrix();

    glRotatef(angley,1.0f,0.0f,0.0f);
    glRotatef(anglex,0.0f,1.0f,0.0f);
    glRotatef(anglez,0.0f,0.0f,1.0f);

    draw_ground() ;

    glPushMatrix();
    glTranslatef(xpos,0.0f,zpos);
    glRotatef(direction,0.0f,1.0f,0.0f);
    drawFrame();
    drawChain();
    glPopMatrix();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camx,camy,camz, camx,1.0,0.0,0.0,1.0,1.0); // pengaturan kamera
    glutSwapBuffers(); // melakukan pertukaran buffer dari 1 jendela ke jendela lain

}

void idle(void) // FUNCTION WHEN NO OPERATIONS PERFORMED
{
    updateScene(); // untuk mengapdet setiap scene (kalo dihilangkan dia tidak bergerak)
    glutPostRedisplay(); // menandai jendela saat ini sebagai perlu ditampilkan ulang
}


void special(int key,int x,int y) // FUNCTION FOR CAM ZOOMING
{
    switch(key) //percabangan kode program dimana kita membandingkan isi sebuah variabel dengan beberapa nilai. Jika proses perbandingan tersebut menghasilkan true, maka block kode program akan di proses.
    {
        case GLUT_KEY_UP: // ini semua pengaturan untuk mengatur kamera melalui keyboard
            camz -= 0.1f;
        break;
        case GLUT_KEY_DOWN:
            camz += 0.1f;
        break;
        case GLUT_KEY_LEFT:
            camx -= 0.1f;
        break;
        case GLUT_KEY_RIGHT:
            camx += 0.1f;
        break;
    }
    glutPostRedisplay(); // menandai jendela saat ini sebagai perlu ditampilkan ulang
}

void reset() // RESETTING SCENE
{
    Mouse=GLUT_UP;
    pedalAngle=speed=steering=0.0f;
    camx=camy=0.0f;
    camz=5.0f;
    xpos=zpos=0.0f;
    direction=0.0f;
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glRotatef(100.0,0.0,0.0,1.0);
    glTranslatef(-1.0,0.0,-0.6);
    XCylinder(GAS_TANK-0.1,2.7);
    glPopMatrix();
}


void keyboard(unsigned char key,int x,int y)
{
    GLfloat r=0.0f,g=0.0f;
    switch(key)
    {
        case 'r':
        case 'R':
            reset();
        break;
        case 's':
        case 'S':
            glutDisplayFunc(operations_window);
        break;
        case 'c':
        case 'C':
            glutDisplayFunc(display_bike);
        break;
        case '1':
            if(steering < HANDLE_LIMIT)
            steering += INC_STEERING;
        break;
        case '2':
            if(steering > -HANDLE_LIMIT)
            steering -= INC_STEERING;
        break;
        case '+':
            speed += INC_SPEED;
            if (flag == 0)
            {
                flag = 1 ;
                // to kill the idle sound while idle time
                system("kill `ps | grep 'aplay'|awk '{print $1}'`") ;
                system("aplay motorcycle-ride-01.wav&") ;
            }
        break;
        case '-':
            speed -= INC_SPEED;
            if (flag == 1)
            {
                flag = 0 ;
                system("kill `ps | grep 'aplay'|awk '{print $1}'`") ;
                system("aplay motorcycle-idle-01.wav&") ;
            }
        break;
        case 'p':
            light_value = 0;
            lighting();
            break;
        case 'o':
            light_value = 1;
            lighting();
            break;
        case 'i':
            light_value = 2;
            lighting();
            break;
        case 'u':
            light_value = 3;
            lighting();
            break;
        case 'q':
            system("kill `ps | grep 'aplay'|awk '{print $1}'`") ;
            // exit(1);
    }

    pedalAngle += speed;
    if(speed < 0.0f)
        speed = 0.0f;
    if(pedalAngle < 0.0f)
        pedalAngle = 0.0f;
    if(pedalAngle >= 360.0f)
        pedalAngle -= 360.0f;

    glutPostRedisplay();
}

void mouse(int button,int state,int x,int y) // MOUSE FUNCTIONALITY
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN)
        {
            Mouse=GLUT_DOWN;
            prevx=x;
            prevy=y;
        }
        if(state==GLUT_UP)
        {
            Mouse=GLUT_UP;
        }
        break;
        case GLUT_RIGHT_BUTTON:
        break;
    }
    glutPostRedisplay();
}


void passive(int x,int y) // PASSIVE FUNCTION WHEN NO MOUSE KEYS ARE PRESSED
{

}


void motion(int x,int y) // MOTION FUNCTION FOR CURRENT WINDOW WHEN MOUSE KEYS ARE PRESSED
{
    if(Mouse==GLUT_DOWN)
    {
        int deltax,deltay;
        deltax=prevx-x;
        deltay=prevy-y;
        anglex += 0.5*deltax;
        angley += 0.5*deltay;
        if(deltax!=0 && deltay!=0)
            anglez += 0.5*sqrt(deltax*deltax + deltay*deltay);
        if(anglex < 0)
            anglex+=360.0;
        if(angley < 0)
            angley+=360.0;
        if(anglez < 0)
            anglez += 360.0;
        if(anglex > 360.0)
            anglex-=360.0;
        if(angley > 360.0)
            angley-=360.0;
        if(anglez > 360.0)
            anglez-=360.0;
    }
    else
    {
        Mouse=GLUT_UP;
    }

    prevx=x;
    prevy=y;
    glutPostRedisplay();
}

void reshape(int w,int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLfloat)w/(GLfloat)h,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camx,camy,camz, 0.0,0.0,0.0, 0.0,1.0,0.0);
}

void welcome_window() // WELCOME WINDOW
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0); // untuk mewarnai window bagian bike
    glColor3f(1.0,1.0,1.0); // warnai tulisan di window bike
    bitmap_output(-0.7,1.8,0.50,"Universitas Halu Oleo"); //tulisan di window bike
    bitmap_output(-0.6,1.6,0.50,"Teknik Informatika"); // tulisan di window bike
    bitmap_output(-0.6,0.70,0.50,"Projek Kelompok 6");
    bitmap_output(-0.85,0.50,0.50,"Simulasi 3D Sepeda Motor");
    bitmap_output(-0.95,-1.5,0.50,"Silahkan Tekan S Untuk Mulai");
    glutSwapBuffers(); // pergantian window
    glFlush(); // mengosongkan semua buffer ini, menyebabkan semua perintah yang dikeluarkan dieksekusi secepat mereka diterima oleh mesin rendering yang sebenarnya. Meskipun eksekusi ini mungkin tidak selesai dalam jangka waktu tertentu, itu selesai dalam jumlah waktu yang terbatas.
}

void operations_window() // OPERATION WINDOW FOR DESCRIPTION OF OPERATIONS TO PERFORM
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);
    glColor3f(0.0,1.0,1.0);
    bitmap_output(-2.25,1.7,0.50,"OPERASI YANG DAPAT DILAKUKAN DENGAN SEPEDA");
    bitmap_output(-2.25,1.2,0.50,"1. RESET KAMERA - GUNAKAN 'R' ATAU 'r'");
    bitmap_output(-2.25,1.0,0.50,"2. MENAMBAH KECEPATAN MOTOR - USE '+'");
    bitmap_output(-2.25,0.8,0.50,"3. MENGURANGI KECEPATAN MOTOR - USE '-'");
    bitmap_output(-2.25,0.6,0.50,"4. BELOK KANAN - USE '2'");
    bitmap_output(-2.25,0.4,0.50,"5. BELOK KIRI - USE '1'");
    bitmap_output(-2.25,0.2,0.50,"6. ZOOM IN - USE 'UPWARD ARROW'");
    bitmap_output(-2.25,0.0,0.50,"7. ZOOM OUT - USE 'DOWNWARD ARROW'");
    bitmap_output(-2.25,-0.2,0.50,"8. MOVE LEFT - USE 'LEFT ARROW'");
    bitmap_output(-2.25,-0.4,0.50,"9. MOVE RIGHT - USE 'RIGHT ARROW'");
    bitmap_output(-2.25,-0.6,0.50,"10. GUNAKAN MOUSE UNTUK MENGUBAH ARAH PENGLIHATAN KAMERA");
    bitmap_output(-2.25,-1.0,0.50,"SILAHKAN TEKAN C UNTUK MELANJUTKAN");
    glutSwapBuffers();
    glFlush();
}

int main(int argc,char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(WIN_WIDTH,WIN_HEIGHT);
    glutCreateWindow("Bike");
    init();
    glutDisplayFunc(welcome_window);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutMainLoop();
}
