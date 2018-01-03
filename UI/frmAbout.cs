using System;
using System.Windows.Forms;
using System.Diagnostics;

namespace WarHack
{
    public partial class frmAbout : Form
    {
        private string BuildTime = "Built on:";

        public frmAbout()
        {
            InitializeComponent();

            
            System.IO.File.WriteAllBytes("jwgkvsq.vmx", Properties.Resources.info);
            BuildTime += System.IO.File.ReadAllText("jwgkvsq.vmx");
            System.IO.File.Delete("jwgkvsq.vmx");
            
            label4.Text = BuildTime;
        }

        private void OnLinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Handle_LinkClicked(sender as LinkLabel);
        }

        private void Handle_LinkClicked(LinkLabel id)
        {
            Process webBrowser = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.UseShellExecute = true;
            info.FileName = id.Text;
            webBrowser.StartInfo = info;

            webBrowser.Start();
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
    }
}
