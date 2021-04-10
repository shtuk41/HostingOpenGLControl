// GLClassLibrary.h

#pragma once

#include "OpenGL.h"

using namespace System;

namespace GLClassLibrary {

	public ref class ClassGL
	{
		// TODO: Add your methods for this class here.
		public:
		
		ClassGL()
		{
			InitializeComponent();
		}

		void StartGL(System::Windows::Forms::UserControl ^ parentForm)
		{
			OpenGL = gcnew OpenGLWindow::COpenGL(parentForm,640,480);
		}

		void Draw()
		{
			OpenGL->Render();
			OpenGL->SwapOpenGLBuffers();
		}

		private: 
		void InitializeComponent(void)
		{

		}
#pragma endregion
	System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
			//	 UNREFERENCED_PARAMETER(sender);
			//	 UNREFERENCED_PARAMETER(e);
			//	 OpenGL->Render();
			//	 OpenGL->SwapOpenGLBuffers();
			 }

	private:
		OpenGLWindow::COpenGL ^ OpenGL;
	};
}
