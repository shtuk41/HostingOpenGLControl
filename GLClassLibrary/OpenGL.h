#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

//#define GLEW_STATIC 1

#include <windows.h>
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "common_header.h"
#include "Shaders.h"
#include "SOIL/SOIL.h"

#using <System.Windows.Forms.dll>


glm::vec4 fTriangle[3]; // Data to render triangle (3 vertices, each has 3 floats)
float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
glm::vec4 fTriangleColor[3];
float fQuadColor[12];
float fQuadTexturePos[8];

UINT uiVBO[4];
UINT uiVAO[2];

GLuint texture;

CShader shVertex, shFragment;
CShaderProgram spMain;


using namespace System::Windows::Forms;


namespace OpenGLWindow 
{
	public ref class COpenGL: public System::Windows::Forms::NativeWindow
	{
	public:
		COpenGL(System::Windows::Forms::UserControl ^ parentForm, GLsizei iWidth, GLsizei iHeight)
		{
			CreateParams^ cp = gcnew CreateParams;

			// Set the position on the form
			cp->X = 0;
			cp->Y = 0;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window
			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());

			if(m_hDC)
			{
				MySetPixelFormat(m_hDC);
				ReSizeGLScene(iWidth, iHeight);
				InitScene();
			}

			rtri = 0.0f;
			rquad = 0.0f;
		}


		System::Void Render(System::Void)
		{
			// We just clear color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			glEnable(GL_TEXTURE_2D);
			glBindTexture( GL_TEXTURE_2D, texture );

			glBindVertexArray(uiVAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glBindVertexArray(uiVAO[1]);
			glDrawArrays(GL_QUADS, 0, 4);

			
			

		}

		System::Void SwapOpenGLBuffers(System::Void)
		{
			SwapBuffers(m_hDC) ;
		}

	private:
		HDC m_hDC;
		HGLRC m_hglrc;
		GLfloat	rtri;				// Angle for the triangle
		GLfloat	rquad;				// Angle for the quad

	protected:
		~COpenGL(System::Void)
		{
			this->DestroyHandle();
		}

		GLint MySetPixelFormat(HDC hdc)
		{
			static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
				{
					sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
					1,											// Version Number
					PFD_DRAW_TO_WINDOW |						// Format Must Support Window
					PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
					PFD_DOUBLEBUFFER,							// Must Support Double Buffering
					PFD_TYPE_RGBA,								// Request An RGBA Format
					16,										// Select Our Color Depth
					0, 0, 0, 0, 0, 0,							// Color Bits Ignored
					0,											// No Alpha Buffer
					0,											// Shift Bit Ignored
					0,											// No Accumulation Buffer
					0, 0, 0, 0,									// Accumulation Bits Ignored
					16,											// 16Bit Z-Buffer (Depth Buffer)  
					0,											// No Stencil Buffer
					0,											// No Auxiliary Buffer
					PFD_MAIN_PLANE,								// Main Drawing Layer
					0,											// Reserved
					0, 0, 0										// Layer Masks Ignored
				};
			
			GLint  iPixelFormat; 
		 
			// get the device context's best, available pixel format match 
			if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}
			 
			// make that match the device context's current pixel format 
			if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}

			if(glewInit() != GLEW_OK)
			{
					int a = 0;
					a = 1;
			}

			InitScene();


			return 1;
		}

		void InitScene()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glFrontFace(GL_CCW);
			//glCullFace(GL_BACK);

			// Setup triangle vertices
			fTriangle[0] = glm::vec4(-4.0f,1.0f,-3.0f,1.0f);
			fTriangle[1] = glm::vec4(4.0f,1.0f,-3.0f,1.0f);
			fTriangle[2] = glm::vec4(0.0f,7.0f,-3.0f,1.0f);
			

			// Setup triangle color
			fTriangleColor[0] = glm::vec4(1.0f,0.0f,0.0f,1.0f);
			fTriangleColor[1] = glm::vec4(0.0f,1.0f,0.0f,1.0f);
			fTriangleColor[2] = glm::vec4(0.0f,0.0f,1.0f,1.0f);
 
			// Setup quad vertices
 
			fQuad[0] = -1.0f; fQuad[1] = 1.0f; fQuad[2] = -2.0f;
			fQuad[3] = 1.0f; fQuad[4] = 1.0f; fQuad[5] = -2.0f;
			fQuad[6] = 1.0f; fQuad[7] = -1.0f; fQuad[8] = -2.0f;
			fQuad[9] = -1.0f; fQuad[10] = -1.0f; fQuad[11] = -2.0f;

			// Setup quad color

			fQuadColor[0] = 0.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;		
			fQuadColor[3] = 0.0f; fQuadColor[4] = 0.0f; fQuadColor[8] = 0.0f;
			fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 0.0f;
			fQuadColor[9] = 0.0f; fQuadColor[10] = 0.0f; fQuadColor[11] = 0.0f;

			fQuadTexturePos[0] = 0;	fQuadTexturePos[1] = 0;
			fQuadTexturePos[2] = 1;	fQuadTexturePos[3] = 0;
			fQuadTexturePos[4] = 1;	fQuadTexturePos[5] = 1;
			fQuadTexturePos[6] = 0;	fQuadTexturePos[7] = 1;

			glGenVertexArrays(2, uiVAO); // Generate two VAOs, one for triangle and one for quad
			glGenBuffers(5, uiVBO); // And four VBOs

			// Setup whole triangle
			glBindVertexArray(uiVAO[0]);

			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
			glBufferData(GL_ARRAY_BUFFER, 3* sizeof(glm::vec4), fTriangle, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
			glBufferData(GL_ARRAY_BUFFER, 3*sizeof(glm::vec4), fTriangleColor, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

			// Setup whole quad
			glBindVertexArray(uiVAO[1]);

			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
			glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
			glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), fQuadTexturePos, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

			shVertex.LoadShader("shader.vert", GL_VERTEX_SHADER);
			shFragment.LoadShader("shader.frag", GL_FRAGMENT_SHADER);

			spMain.CreateProgram();
			spMain.AddShaderToProgram(&shVertex);
			spMain.AddShaderToProgram(&shFragment);

			spMain.LinkProgram();
			spMain.UseProgram();

			GLint model_view = glGetUniformLocation(spMain.GetProgramID(),"mv");
			GLint projection = glGetUniformLocation(spMain.GetProgramID(),"p");

			glm::vec3 Pos(0.0f, 0.0f, 3.0f);
			glm::vec3 Target(0.0f, 0.0f, 0.0f);
			glm::vec3 Up(0.0, 1.0f, 0.0f);


			glm::mat4 mv = glm::lookAt(Pos,Target,Up);
			glm::mat4 p = glm::mat4(1.0f);

			p = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.01f, 10.0f);

			glUniformMatrix4fv(model_view,1,GL_FALSE,glm::value_ptr(mv));
			glUniformMatrix4fv(projection,1,GL_FALSE,glm::value_ptr(p));

			texture = SOIL_load_OGL_texture("img_test2.png", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
							   SOIL_FLAG_INVERT_Y |
                               //SOIL_FLAG_POWER_OF_TWO | 
                               SOIL_FLAG_MIPMAPS 
                               //SOIL_FLAG_COMPRESS_TO_DXT
							   );

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

		}

		GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
		{
			if (height==0)										// Prevent A Divide By Zero By
			{
				height=1;										// Making Height Equal One
			}

			glViewport(0,0,width,height);						// Reset The Current Viewport

			//glMatrixMode(GL_PROJECTION);

			glEnable(GL_DEPTH_TEST);

			//int texture_units;

			//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);

			//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			//glLoadIdentity();									// Reset The Projection Matrix

			// Calculate The Aspect Ratio Of The Window
			//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

			//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			//glLoadIdentity();									// Reset The Modelview Matrix
		}
	};
}

CShader::CShader()
{
	bLoaded = false;
}

/*-----------------------------------------------

Name:	loadShader

Params:	sFile - path to a file
		a_iType - type of shader (fragment, vertex, geometry)

Result:	Loads and compiles shader.

/*---------------------------------------------*/

bool CShader::LoadShader(string sFile, int a_iType)
{
	FILE* fp = fopen(sFile.c_str(), "rt");
	if(!fp)
		
	{
		MessageBox::Show("File Not Found");
		return false;
	}

	// Get all lines from a file

	vector<string> sLines;
	char sLine[255];
	while(fgets(sLine, 255, fp))sLines.push_back(sLine);
	fclose(fp);

	const char** sProgram = new const char*[ESZ(sLines)];
	FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();
	
	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader, ESZ(sLines), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)return false;
	iType = a_iType;
	bLoaded = true;

	return 1;


}

/*-----------------------------------------------

Name:	IsLoaded

Params:	none

Result:	True if shader was loaded and compiled.

/*---------------------------------------------*/

bool CShader::IsLoaded()
{
	return bLoaded;
}

/*-----------------------------------------------

Name:	GetShaderID

Params:	none

Result:	Returns ID of a generated shader.

/*---------------------------------------------*/

UINT CShader::GetShaderID()
{
	return uiShader;
}

/*-----------------------------------------------

Name:	DeleteShader

Params:	none

Result:	Deletes shader and frees memory in GPU.

/*---------------------------------------------*/

void CShader::DeleteShader()
{
	if(!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}

CShaderProgram::CShaderProgram()
{
	bLinked = false;
}

/*-----------------------------------------------

Name:	CreateProgram

Params:	none

Result:	Creates a new program.

/*---------------------------------------------*/

void CShaderProgram::CreateProgram()
{
	uiProgram = glCreateProgram();
}

/*-----------------------------------------------

Name:	AddShaderToProgram

Params:	sShader - shader to add

Result:	Adds a shader (like source file) to
		a program, but only compiled one.

/*---------------------------------------------*/

bool CShaderProgram::AddShaderToProgram(CShader* shShader)
{
	if(!shShader->IsLoaded())return false;

	glAttachShader(uiProgram, shShader->GetShaderID());

	return true;
}

/*-----------------------------------------------

Name:	LinkProgram

Params:	none

Result:	Performs final linkage of OpenGL program.

/*---------------------------------------------*/

bool CShaderProgram::LinkProgram()
{
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}

/*-----------------------------------------------

Name:	DeleteProgram

Params:	none

Result:	Deletes program and frees memory on GPU.

/*---------------------------------------------*/

void CShaderProgram::DeleteProgram()
{
	if(!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}

/*-----------------------------------------------

Name:	UseProgram

Params:	none

Result:	Tells OpenGL to use this program.

/*---------------------------------------------*/

void CShaderProgram::UseProgram()
{
	if(bLinked)glUseProgram(uiProgram);
}