using System;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.IO;
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
        private Process WzHack;
        private ProcessStartInfo WzHack_StartInfo;

        public frmMain()
        {
            InitializeComponent();

            comboBox_PlayerId.SelectedIndex = id;
            labelDelay.Text = Convert.ToString(trackBarDelay.Value) + " sec.";
            labelEnergy.Text = Convert.ToString(trackBarEnergy.Value * 10000);
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
                
                WzHack.Kill();
                WzHack.Close();

                statusLabel.Text = "";

                return;
            }

            id = comboBox_PlayerId.SelectedIndex;

            if (checkBox_EasterEgg.Checked)
                with_easter_egg = true;
            else
                with_easter_egg = false;

            Arguments = "-d:" + (trackBarDelay.Value * 1000) + " -p:" + id;
            Arguments += " -e:" + (trackBarEnergy.Value * 10000);

            if (with_easter_egg)
                Arguments += " --easter-egg";

            statusLabel.Text = $"Command line: {WzHack_Filename} {Arguments}";

            try
            {
                WzHack_StartInfo = new ProcessStartInfo(WzHack_Filename, Arguments);
                WzHack_StartInfo.RedirectStandardOutput = true;
                WzHack_StartInfo.RedirectStandardError = true;
                WzHack_StartInfo.UseShellExecute = false;
                WzHack_StartInfo.CreateNoWindow = true;

                WzHack = new Process();
                WzHack.EnableRaisingEvents = true;
                WzHack.StartInfo = WzHack_StartInfo;
                WzHack.OutputDataReceived += WzHack_OutputDataReceived;
                WzHack.Start();

                WzHack.BeginOutputReadLine();
                WzHack.WaitForExit();

                running = true;

                ButtonStart.Text = "Stop";
                ButtonStart.ForeColor = Color.Red;

                statusLabel.Text = "Hack running ...";
            } catch(Exception Ex)
            {
                MessageBox.Show($"Failed to start thread: {Ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void WzHack_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            TextBoxOutput.AppendText(e.Data);
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                WzHack.Kill();
            } catch(Exception)
            {

            }
        }
    }
}
