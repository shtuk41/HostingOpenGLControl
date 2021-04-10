using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using GLClassLibrary;

namespace FormGLTest
{
    public partial class Form1 : UserControl
    {
        ClassGL c = new ClassGL();
        Timer timer = new Timer();

        public Form1()
        {
            InitializeComponent();

            c.StartGL(this);

            timer.Tick += new EventHandler(timer_Tick); // Everytime timer ticks, timer_Tick will be called
            timer.Interval = (100) * (1);              // Timer will tick evert second
            timer.Enabled = true;                       // Enable the timer
            timer.Start();                              // Start the timer
        }

        void timer_Tick(object sender, EventArgs e)
        {
            c.Draw();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
    }
}
