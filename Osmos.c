#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
#define MAX 30

struct timeval begin, end, diff;

GLfloat size = 40;
GLfloat sizeOther[MAX];
//Player square position. Order : LEFT TOP RIGHT BOTTOM
GLint player[4];
//All other squares position AND direction. Yet to be initialised.
GLint others[MAX][5];

GLint flag = 0;

void GameOver(int x, int y, char *string);
void InitialiseOthers();

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void CollisionHandler(int i)
{
    //sleep(1);

    if(sizeOther[i] > size)
    {
        size = size - (0.02*sizeOther[i]);
        player[2] = player[0]+size;
        player[1] = player[3]+size;


        sizeOther[i] = sizeOther[i] + (0.02*sizeOther[i]);

        others[i][2] = others[i][0] + sizeOther[i];
        others[i][3] = others[i][1] - sizeOther[i];
    }
    else
    {
        size = size + (0.02*sizeOther[i]);
        player[2] = player[0]+size;
        player[1] = player[3]+size;

        sizeOther[i] = sizeOther[i] - (0.2*sizeOther[i]);

        others[i][2] = others[i][0] + sizeOther[i];
        others[i][3] = others[i][1] - sizeOther[i];
	}

}

void CollisionDetection()
{
    int i, flag = 0;
    for(i = 0; i < MAX; i++)
    {
        if(((others[i][0] >= player[0]) && (others[i][0] <= player[2])) && (((others[i][3] > player[3]) && (others[i][3]) < player[1]) || ((others[i][1] < player[1]) && (others[i][1] > player[3]))))
        {
            CollisionHandler(i);
        }
        else if(((others[i][3] <= player[1]) && (others[i][3] >= player[3])) && (((others[i][0] > player[0]) && (others[i][0]) < player[2]) || ((others[i][2] < player[2]) && (others[i][2] > player[0]))))
        {
            CollisionHandler(i);
        }
        else if(((others[i][0] <= player[2]) && (others[i][0] >= player[0])) && (((others[i][1] > player[3]) && (others[i][1]) < player[1]) || ((others[i][3] < player[1]) && (others[i][3] > player[3]))))
        {
            CollisionHandler(i);
        }
        else if(((others[i][1] >= player[3]) && (others[i][1] <= player[1])) && (((others[i][0] > player[0]) && (others[i][0]) < player[2]) || ((others[i][2] < player[2]) && (others[i][2] > player[0]))))
        {
            CollisionHandler(i);
        }
    }
}

void myinit()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(1.0,0.0,0.0);

    gettimeofday(&begin, NULL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1366,0,768);
	glMatrixMode(GL_MODELVIEW);
	InitialiseOthers();
}

//Draws player
void DrawPlayer()
{
	glPointSize(2.0);
	glBegin(GL_QUADS);
        glColor3f(0.5,1.0,0.5);
        glVertex2f(player[0], player[3]);
        glVertex2f(player[0], player[1]);
        glVertex2f(player[2], player[1]);
        glVertex2f(player[2], player[3]);
    glEnd();
}

void ModifyPlayer(int x, int y)
{
	player[0] = x;
	player[2] = x+size;
	player[1] = y+size;
	player[3] = y;
}
void MovePlayer(int x, int y)
{
    y=728-abs(y);
    ModifyPlayer(x,y);
}
void InitialiseOthers()
{
	int i, sizebox;
	for(i = 0; i < MAX; i++)
	{
		sizebox = (rand() % 30) + 30;
		others[i][0] = rand() % 750;
		others[i][1] = rand() % 750;
		others[i][2] = others[i][0] + sizebox;
		others[i][3] = others[i][1] - sizebox;
		others[i][4] = rand() % 8;
		sizeOther[i] = (float)sizebox;
	}
}

void DrawOthers()
{
	int i;
	glPointSize(2.0);
	//glColor3f(0.258824,0.258824,0.435294);

	for(i = 0; i < MAX; i++)
	{
        if(sizeOther[i] > size+1)
            glColor3f(1.0,0.0,0.0);
        else if(sizeOther[i] <= size-1)
            glColor3f(0.0,1.0,0.0);
		glBegin(GL_QUADS);
			glVertex2f(others[i][0], others[i][3]);
			glVertex2f(others[i][0], others[i][1]);
			glVertex2f(others[i][2], others[i][1]);
			glVertex2f(others[i][2], others[i][3]);
		glEnd();
	}
}

void ModifyOthers()
{
	float x, y, speed = 5;
	int i;
	for(i = 0; i < MAX; i++)
	{
		switch(others[i][4])
		{
			case 0:
				x = speed;
				y = 0;
				break;
			case 1:
				x = speed;
				y = -speed;
				break;
			case 2:
				x = 0;
				y = -speed;
				break;
			case 3:
				x = -speed;
				y = -speed;
				break;
			case 4:
				x = -speed;
				y = 0;
				break;
			case 5:
				x = -speed;
				y = speed;
				break;
			case 6:
				x = 0;
				y = speed;
				break;
			case 7:
				x = speed;
				y = speed;
				break;
			default:
				break;
		}
		others[i][0] += x;
		others[i][1] += y;
		others[i][2] += x;
		others[i][3] += y;

//Bounce off LEFT wall
		if(others[i][0] < 0)
		{
			switch(others[i][4])
			{
				case 3:
					others[i][4] = 1;
					break;
				case 4:
					others[i][4] = 0;
					break;
				case 5:
					others[i][4] = 7;
					break;
				default:
					break;
			}
		}
//Bounce off TOP wall
		if(others[i][1] > 768)
		{
			switch(others[i][4])
			{
				case 5:
					others[i][4] = 3;
					break;
				case 6:
					others[i][4] = 2;
					break;
				case 7:
					others[i][4] = 1;
					break;
				default:
					break;
			}
		}
//Bounce off RIGHT wall
		if(others[i][2] > 1366)
		{
			switch(others[i][4])
			{
				case 0:
					others[i][4] = 4;
					break;
				case 1:
					others[i][4] = 3;
					break;
				case 7:
					others[i][4] = 5;
					break;
				default:
					break;
			}
		}
//Bounce off BOTTOM wall
		if(others[i][3] < 0)
		{
			switch(others[i][4])
			{
				case 1:
					others[i][4] = 7;
					break;
				case 2:
					others[i][4] = 6;
					break;
				case 3:
					others[i][4] = 5;
					break;
				default:
					break;
			}
		}
	}
//	display();
}

void GameWinDetection()
{
    int i, temp = 0;
    for(i = 0; i < MAX; i++)
    {
        if(sizeOther[i] < size)
            temp++;
    }
    if(temp == MAX)
    {
        flag = 1;
        GameOver(500, 400, "CONGRATULATIONS! YOU WIN!");
    }
}

void GameOver(int x, int y, char *string)
{
    glClear(GL_DEPTH_BUFFER_BIT);
  glColor3f(0,0,0);
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  gettimeofday(&end, NULL);
  timeval_subtract(&diff, &end, &begin);
  if(flag == 1)
  {
    //printf("You score: %ld.%02ld!\n", diff.tv_sec, diff.tv_usec);
     printf("You score: %d!\n", 100-(int)diff.tv_sec);
    }
    else
    {
        printf("You Lose!\n");
    }
  glutIdleFunc(NULL);
}

void GameLoseDetection()
{
    int i, temp = 0;
    for(i = 0; i < MAX; i++)
    {
        if(size < sizeOther[i])
            temp++;
    }
    if(temp == MAX || size < 5)
    {
        flag = 0;
        GameOver(500, 400, "GAME OVER! BETTER LUCK NEXT TIME");
    }
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawPlayer();
	DrawOthers();
	glFlush();
	ModifyOthers();
	CollisionDetection();
	GameWinDetection();
	GameLoseDetection();
	glutSwapBuffers();
}



int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(1366,768);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Osmos");
	glutDisplayFunc(display);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(MovePlayer);
	//glutKeyboardFunc(MovePlayer);
	myinit();
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}
