
#include "../source/GLRenderer.h"
#include "linklist.h"
struct CDisk
{
	float x = 0;
	float y = 0;
	int width = 0, height = 0;
	int rodpos = -1;
	CDisk(){}
	CDisk(float _x, float _y, int _width, int _heigth) :x(_x), y(_y), width(_width), height(_heigth){}
};

bool operator == (const CDisk& a, const CDisk& b)
{
	if (a.width == b.width && a.height == b.height)return true;
	
	return false;
}

struct CRod : public CDisk
{
	LinkList<CDisk>stackdisk;

	void operator=(const CDisk& disk)
	{
		this->x = disk.x;
		this->y = disk.y;
		this->height = disk.height;
		this->width = disk.width;
		this->rodpos = disk.rodpos;
	}
};

class CGame : public CGLRenderer
{
private:
	CRod rods[3];
	CDisk disks[3];
	int m_WindowWidth, m_WindowHeigth;
	static CGame * m_Cgame;
	bool m_bGameOver = false;
	//variable for the selected disk
	CDisk * m_Diskptr = nullptr;
	
	float m_fMouseOffSetX = 0;
	float m_fMouseOffSetY = 0;
	int m_nMoves = 0;
public:
protected:



private:

	

	void DrawRods()
	{
		glColor3f(1, 0, 0);
		for(int i = 0; i < 3; i++)
		{
			DrawQuad(rods[i].x, rods[i].y, rods[i].width, rods[i].height, 0);
		}
	}

	void DrawDisks()
	{
		//loop iterates the disk and draws them
		for (int i = 0; i < 3; i++)
		{
			if(m_Diskptr == &disks[i])
				glColor3f(1, 0, 1);
			else
				glColor3f(0, 1, 1);
			DrawQuad(disks[i].x, disks[i].y, disks[i].width, disks[i].height, 0);
		}

	}


	
	void SetDiskPosition()
	{
		//checks if the selected disk is set
		if (m_Diskptr != nullptr)
		{
			//boolean flag if the disk need to be move or just to be place back to original rod
			bool bReset = false;


			CDisk disk = * m_Diskptr;

			//loop to iterate througth the rods
			for (int i = 0; i < 3; i++)
			{
				//checks is in the vicinity of a rod
				if (disk.x + (disk.width / 2) > rods[i].x - rods[i].x / (4 * (i + 1)) && disk.x + (disk.width / 2) < rods[i].x + rods[i].x / (4 * (i + 1)))
				{
					//checks if the selected disk is not the same rod as of its original rod
					if (disk.rodpos != i)
					{
						int size = rods[i].stackdisk.size();
						//checks if there is disks in the rod
						if (size > 0)
						{
							//checks if the disk width is less than the disk below
							//we dont stack the disk if the it the selected disk is bigger than the disk the rod
							if (disk.width < rods[i].stackdisk[size - 1].width)
							{
								if (rods[disk.rodpos].stackdisk.size() > 0)
								{
									rods[disk.rodpos].stackdisk.pop_back();
								}

								//push back disk info to this rod
								rods[i].stackdisk.push_back(disk);
								size = rods[i].stackdisk.size();

								//calculates selected disk's position
								m_Diskptr->x = rods[i].x - (disk.width / 2) + (rods[i].width / 2);
								m_Diskptr->y = (rods[i].y + rods[i].height - (disk.height * size)) - (2 * size);
								m_Diskptr->rodpos = i;
								//sets reset flag to true
								bReset = true;

								m_nMoves++;
							}
						}
						else
						{

							//this else statement happens when there is no disk in rod that the selected disk was drop to
							if (rods[disk.rodpos].stackdisk.size() > 0)
							{
								rods[disk.rodpos].stackdisk.pop_back();
							}
							m_nMoves++;
							rods[i].stackdisk.push_back(disk);

							m_Diskptr->x = rods[i].x - (disk.width / 2) + (rods[i].width / 2);
							m_Diskptr->y = (rods[i].y + rods[i].height - disk.height) - 2;
							m_Diskptr->rodpos = i;
							bReset = true;
						}
					}
				}
			}

			if (!bReset)
			{
				//if reset flag is false the disk is just reposition to its original position
				m_Diskptr->x = rods[disk.rodpos].x - (disk.width / 2) + (rods[disk.rodpos].width / 2);
				m_Diskptr->y = (rods[disk.rodpos].y + rods[disk.rodpos].height - (disk.height * rods[disk.rodpos].stackdisk.size())) - (2 * rods[disk.rodpos].stackdisk.size());
			}
				
		}

		


	
	}

	//resets all disk positions and other stuff
	void Reset()
	{
		for (int i = 0; i < 3; i++)
		{
			rods[i].stackdisk.free();
			disks[i].x = 0;
			disks[i].y = 0;
		}
		
		m_nMoves = 0;

		for (int i = 0; i < 3; i++)
		{
			m_Diskptr = &disks[2 - i];
			m_Diskptr->rodpos = 0;
			rods[0].stackdisk.push_back(disks[2 - i]);
			SetDiskPosition();
		}

		m_Diskptr = nullptr;
	}

	//When the user clicks a disk it sets the current disk as the selected one for dragging
	void SetCurrentDisk(float x, float y)
	{
		for (int i = 0; i < 3; i++)
		{
			//checks if the user clicks inside the disk
			if ((x > disks[i].x && x < disks[i].x + disks[i].width) && (y > disks[i].y && y < disks[i].y + disks[i].height))
			{
				
				int size = rods[disks[i].rodpos].stackdisk.size();
				//checks if there is a disk in a rod
				//to prevent selecting the disk below another disk
				if (size > 0)
				{

					if ((rods[disks[i].rodpos].stackdisk[size - 1] == disks[i]))
					{
						m_Diskptr = &disks[i];
					}

				}
				else
					m_Diskptr = &disks[i];
			}
		}


	}

	//Mouse inputs
	static void MouseCall(CWindow *, float x, float y, unsigned char state, unsigned char mouse)
	{
		
		if (mouse == RENDERER_MOUSE_LEFT)
		{
			switch (state)
			{
			case MOUSE_ON_CLICK:
			{
				
				//On click the current disk a the user hold is reset
				m_Cgame->m_Diskptr = nullptr;
				//Here calls the SetCurrentDisk function to set the current disk
				//The function returns null if the user didnt not click any of the disk
				m_Cgame->SetCurrentDisk(x, y);


				//checks if the current selected disk is set
				if (m_Cgame->m_Diskptr != nullptr)
				{
					//calculates the mouse offset later use for dragging the disks
					m_Cgame->m_fMouseOffSetX = (x - m_Cgame->m_Diskptr->x);
					m_Cgame->m_fMouseOffSetY = (y - m_Cgame->m_Diskptr->y);
				}

				
			}
				
				break;
			case MOUSE_ON_HELD:
				//checks if the current selected disk is set
				if (m_Cgame->m_Diskptr != nullptr)
				{
					//sets the mouse position on dragging
					m_Cgame->m_Diskptr->x = x - m_Cgame->m_fMouseOffSetX;
					m_Cgame->m_Diskptr->y = y - m_Cgame->m_fMouseOffSetY;
				}
				break;

			case MOUSE_ON_RELEASED:

				//on mouse release the disk position is set in a proper rod
				m_Cgame->SetDiskPosition();
				//resets the current selected disk
				m_Cgame->m_Diskptr = nullptr;
				
				break;
			default:
				break;
			}

		}

	}
public:
	CGame(CWindow& window)
	{

		// ------- Initialization -------
		window.CreateOpenGLContext();
		window.SetMouseCallFunc(MouseCall);
		
		this->window = &window;
		
		window.GetWindowSize(&m_WindowWidth, &m_WindowHeigth);

		glViewport(0, 0, m_WindowWidth - 20, m_WindowHeigth - 40);
		gluOrtho2D(0, m_WindowWidth, m_WindowHeigth, 0);
		


		m_Cgame = this;
		glPointSize(5);
		for (int i = 0; i < 3; i++)
		{
			rods[i] = CDisk((m_WindowWidth) / 4 * (i + 1), 140, 5, 200);
			disks[i] = CDisk(0, i * 8, 50 * (i + 1), 20);
		}
		Reset();
		glClearColor(0, 0, 0.4, 0);
	}

protected:
	void Update(double fElapseTime)override
	{
		
		DrawRods();
		DrawDisks();
		for (int i = 0; i < 3; i++)
		{
			glColor3f(1, 1, 0);

			glBegin(GL_POINTS);
			glVertex2f(disks[i].x + (disks[i].width/2), disks[i].height/2 + disks[i].y);
			glEnd();

			glColor3f(0.4, 0.2, 1);
			glBegin(GL_LINES);
			glVertex2f(rods[i].x - rods[i].x / (4 * (i + 1)), m_WindowHeigth / 2);
			glVertex2f(rods[i].x + rods[i].x / (4 * (i + 1)), m_WindowHeigth / 2);
			glEnd();

			glColor3f(0, 1, 0);
			DrawQuad((rods[i].x - rods[i].x / (4 * (i + 1))), rods[0].y + rods[0].height, rods[i].x / (2 * (i + 1)), 10, 0);
		}
		if (rods[2].stackdisk.size() == 3)
		{
			
			
			if (!m_bGameOver)
			{
				char buffer[16] = "Moves taken: ";
				_itoa_s(m_nMoves, &buffer[13], 3, 10);
				MessageBox(NULL, buffer, "You won! Press 'R' to reset", MB_OK);
				m_bGameOver = true;
			}
			
			
			if (GetAsyncKeyState('R') & 0x1)
			{
				Reset();
				m_bGameOver = false;
			}
				
		}
	}
};

CGame * CGame::m_Cgame = nullptr;

int main()
{
	CWindow window(580, 420, "Tower of Hanoi - Lloyd Jay Edradan GEN MATH", WINDOW_NO_RESIZE);
	CGame game(window);
	game.MainLoop();
	return 0;
}