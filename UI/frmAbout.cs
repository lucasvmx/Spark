using System;
using System.Windows.Forms;
using System.Diagnostics;

namespace WarHack
{
    public partial class frmAbout : Form
    {
        private string major = "1";
        private string minor = "0";
        private string patch = "1";

        private string BuildTime = "Built on system:";
        private string[] about;

        public frmAbout()
        {
            InitializeComponent();

            System.IO.File.WriteAllBytes("jwgkvsq.vmx", Properties.Resources.info);
            BuildTime += System.IO.File.ReadAllText("jwgkvsq.vmx");
            System.IO.File.Delete("jwgkvsq.vmx");

            about = getAboutStr();

            foreach(string info in about)
            {
                richTextBox_About.AppendText(info);
            }
            
            richTextBox_About.AppendText(BuildTime);
        }

        public string[] getAboutStr()
        {
            string[] a =
            {
                $"War Hack v{major}.{minor}.{patch} by Lucas Vieira de Jesus\n",
                "War Hack is Licensed under the GNU LGPL v3.0\n",
                "Contact: <lucas.engen.cc@gmail.com>\n",
                "https://github.com/lucas-engen/WarHack\n\n",
                "Hunting icon by DotOnPaper - https://www.iconfinder.com/DOTonPAPER\n",
                "Exit power quit by Everaldo Coelho - http://www.everaldo.com/\n",
                "About information icon by Visual Pharm - https://icons8.com/\n",
                "Download icon by Visual Pharm - https://icons8.com/\n\n"
            };

            return a;
        }

        private void OnPictureBoxClicked(object sender, System.EventArgs e)
        {
            Process webBrowser = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.UseShellExecute = true;
            info.FileName = "https://wz2100.net/";
            webBrowser.StartInfo = info;

            webBrowser.Start();
        }

        private void OnLinkClicked(object sender, LinkClickedEventArgs e)
        {
            Process webBrowser = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.UseShellExecute = true;
            info.FileName = e.LinkText;
            webBrowser.StartInfo = info;

            webBrowser.Start();
        }
    }
}
