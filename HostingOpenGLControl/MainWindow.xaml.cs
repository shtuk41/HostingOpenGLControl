using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using FormGLTest;

namespace HostingOpenGLControl
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Create the interop host control.
            System.Windows.Forms.Integration.WindowsFormsHost host =
                new System.Windows.Forms.Integration.WindowsFormsHost();

            // Create the MaskedTextBox control.
            Form1 mtbDate = new Form1();

            // Assign the MaskedTextBox control as the host control's child.
            host.Child = mtbDate;

            // Add the interop host control to the Grid
            // control's collection of child controls.
            this.grid2.Children.Add(host);
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            foreach (var i in this.grid2.Children)
            {
                var b = (System.Windows.Forms.Integration.WindowsFormsHost)i;

                if (b.Visibility == Visibility.Collapsed)
                {
                    b.Visibility = Visibility.Visible;
                }
                else
                {
                    b.Visibility = Visibility.Collapsed;
                }
            }
        }
    }
}
