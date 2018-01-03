using System;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.IO;
using System.ComponentModel;

namespace WarHack
{
    public partial class frmMain : Form
    {
        private string Arguments = "";
        private string WzHack_Filename = "WzHack.exe";
        private int delay = -1;
        private int id = 0;
        private int energy = -1;
        private bool running = false;
        private bool with_easter_egg = false;
        private Process WzHack_Process;
        private ProcessStartInfo WzHack_StartInfo;
        private BackgroundWorker worker;

        public frmMain()
        {
            InitializeComponent();

            /* Initialize values */

            worker = new BackgroundWorker();
            worker.DoWork += OnWzHack_Start;
            worker.WorkerSupportsCancellation = true;

            comboBox_PlayerId.SelectedIndex = id;
            labelDelay.Text = Convert.ToString(trackBarDelay.Value) + " sec.";
            labelEnergy.Text = Convert.ToString(trackBarEnergy.Value * 10000);
            statusLabel.Text = "Welcome to WarHack v1.0 by [GhostRider]";
        }

        private void OnWzHack_Start(object sender, DoWorkEventArgs e)
        {
            int pid;

            WzHack_Process = new Process();
            WzHack_StartInfo = new ProcessStartInfo();
            WzHack_StartInfo.UseShellExecute = false;
            WzHack_StartInfo.RedirectStandardOutput = true;
            WzHack_StartInfo.RedirectStandardError = true;
            WzHack_StartInfo.Arguments = Arguments;
            WzHack_StartInfo.CreateNoWindow = true;
            WzHack_StartInfo.FileName = WzHack_Filename;

            WzHack_Process.StartInfo = WzHack_StartInfo;

            WzHack_Process.Start();
            pid = WzHack_Process.Id;

            statusLabel.Text = "Hack running ...";
            WzHack_Process.WaitForExit();
        }

        private void OnTrackBarDelay_Scroll(object sender, EventArgs e)
        {
            labelDelay.Text = trackBarDelay.Value + " sec.";
            delay = trackBarDelay.Value * 1000;
        }

        private void OnAboutWarHack_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmAbout about = new frmAbout();
            about.ShowDialog();
        }

        private void OnTrackBarEnery_Scroll(object sender, EventArgs e)
        {
            labelEnergy.Text = Convert.ToString(trackBarEnergy.Value * 10000);
            energy = trackBarEnergy.Value * 10000;
        }

        private void OnButtonStart_Click(object sender, EventArgs e)
        {
            if(running)
            {
                ButtonStart.Text = "Start";
                ButtonStart.ForeColor = Color.Green;
                running = false;

                try
                {
                    worker.CancelAsync();

                    if(WzHack_Process != null)
                    {
                        if (!WzHack_Process.HasExited)
                            WzHack_Process.Kill();
                    }
                } catch(Exception)
                {
                    // Add error msg here
                }

                statusLabel.Text = "";

                return;
            }

            // Get player id
            id = comboBox_PlayerId.SelectedIndex;

            // Check for easter egg parameter
            if (checkBox_EasterEgg.Checked)
                with_easter_egg = true;
            else
                with_easter_egg = false;

            // Build command line
            Arguments = "-d:" + (trackBarDelay.Value * 1000) + " -p:" + id;
            Arguments += " -e:" + (trackBarEnergy.Value * 10000);

            if (with_easter_egg)
                Arguments += " --easter-egg";

            if (String.IsNullOrEmpty(Arguments))
            {
                statusLabel.Text = "Empty arguments ...";
                running = false;
                return;
            }

            running = true;

            ButtonStart.Text = "Stop";
            ButtonStart.ForeColor = Color.Red;
            
            worker.RunWorkerAsync();
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            statusLabel.Text = "Closing ...";

            try
            {
                if(WzHack_Process != null)
                {
                    WzHack_Process.Kill();
                }
            } catch(Exception)
            {

            }
        }

        private void OnDownloadWZ_ToolstripClicked(object sender, EventArgs e)
        {
            Process webBrowser = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.UseShellExecute = true;
            info.FileName = "https://sourceforge.net/projects/warzone2100/files/releases/";
            webBrowser.StartInfo = info;

            webBrowser.Start();
        }

        private void OnQuitToolstrip_Clicked(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
