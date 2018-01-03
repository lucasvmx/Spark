namespace WarHack
{
    partial class frmMain
    {
        /// <summary>
        /// Variável de designer necessária.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpar os recursos que estão sendo usados.
        /// </summary>
        /// <param name="disposing">true se for necessário descartar os recursos gerenciados; caso contrário, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código gerado pelo Windows Form Designer

        /// <summary>
        /// Método necessário para suporte ao Designer - não modifique 
        /// o conteúdo deste método com o editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.squitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.downloadWarzoneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutWarHackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBox_EasterEgg = new System.Windows.Forms.CheckBox();
            this.labelEnergy = new System.Windows.Forms.Label();
            this.labelDelay = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.trackBarEnergy = new System.Windows.Forms.TrackBar();
            this.trackBarDelay = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBox_PlayerId = new System.Windows.Forms.ComboBox();
            this.ButtonStart = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarEnergy)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarDelay)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(306, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.squitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // squitToolStripMenuItem
            // 
            this.squitToolStripMenuItem.Name = "squitToolStripMenuItem";
            this.squitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.squitToolStripMenuItem.Text = "Quit";
            this.squitToolStripMenuItem.Click += new System.EventHandler(this.OnQuitToolstrip_Clicked);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.downloadWarzoneToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // downloadWarzoneToolStripMenuItem
            // 
            this.downloadWarzoneToolStripMenuItem.Name = "downloadWarzoneToolStripMenuItem";
            this.downloadWarzoneToolStripMenuItem.Size = new System.Drawing.Size(177, 22);
            this.downloadWarzoneToolStripMenuItem.Text = "Download Warzone";
            this.downloadWarzoneToolStripMenuItem.Click += new System.EventHandler(this.OnDownloadWZ_ToolstripClicked);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutWarHackToolStripMenuItem});
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
            this.aboutToolStripMenuItem.Text = "About";
            // 
            // aboutWarHackToolStripMenuItem
            // 
            this.aboutWarHackToolStripMenuItem.Name = "aboutWarHackToolStripMenuItem";
            this.aboutWarHackToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.aboutWarHackToolStripMenuItem.Text = "About War Hack";
            this.aboutWarHackToolStripMenuItem.Click += new System.EventHandler(this.OnAboutWarHack_ToolStripMenuItem_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBox_EasterEgg);
            this.groupBox1.Controls.Add(this.labelEnergy);
            this.groupBox1.Controls.Add(this.labelDelay);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.trackBarEnergy);
            this.groupBox1.Controls.Add(this.trackBarDelay);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.comboBox_PlayerId);
            this.groupBox1.ForeColor = System.Drawing.Color.Red;
            this.groupBox1.Location = new System.Drawing.Point(12, 42);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(280, 245);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Settings";
            // 
            // checkBox_EasterEgg
            // 
            this.checkBox_EasterEgg.AutoSize = true;
            this.checkBox_EasterEgg.Location = new System.Drawing.Point(9, 211);
            this.checkBox_EasterEgg.Name = "checkBox_EasterEgg";
            this.checkBox_EasterEgg.Size = new System.Drawing.Size(112, 17);
            this.checkBox_EasterEgg.TabIndex = 8;
            this.checkBox_EasterEgg.Text = "Enable easter egg";
            this.checkBox_EasterEgg.UseVisualStyleBackColor = true;
            // 
            // labelEnergy
            // 
            this.labelEnergy.AutoSize = true;
            this.labelEnergy.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelEnergy.Location = new System.Drawing.Point(204, 118);
            this.labelEnergy.Name = "labelEnergy";
            this.labelEnergy.Size = new System.Drawing.Size(0, 24);
            this.labelEnergy.TabIndex = 7;
            // 
            // labelDelay
            // 
            this.labelDelay.AutoSize = true;
            this.labelDelay.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelDelay.Location = new System.Drawing.Point(214, 52);
            this.labelDelay.Name = "labelDelay";
            this.labelDelay.Size = new System.Drawing.Size(0, 24);
            this.labelDelay.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(3, 100);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(48, 15);
            this.label3.TabIndex = 5;
            this.label3.Text = "Energy:";
            // 
            // trackBarEnergy
            // 
            this.trackBarEnergy.Location = new System.Drawing.Point(6, 118);
            this.trackBarEnergy.Minimum = 1;
            this.trackBarEnergy.Name = "trackBarEnergy";
            this.trackBarEnergy.Size = new System.Drawing.Size(198, 45);
            this.trackBarEnergy.TabIndex = 4;
            this.trackBarEnergy.Value = 1;
            this.trackBarEnergy.Scroll += new System.EventHandler(this.OnTrackBarEnery_Scroll);
            // 
            // trackBarDelay
            // 
            this.trackBarDelay.Location = new System.Drawing.Point(6, 52);
            this.trackBarDelay.Maximum = 60;
            this.trackBarDelay.Minimum = 1;
            this.trackBarDelay.Name = "trackBarDelay";
            this.trackBarDelay.Size = new System.Drawing.Size(198, 45);
            this.trackBarDelay.TabIndex = 3;
            this.trackBarDelay.Value = 1;
            this.trackBarDelay.Scroll += new System.EventHandler(this.OnTrackBarDelay_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(6, 27);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(134, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "Delay to use (seconds):";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(3, 166);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 15);
            this.label1.TabIndex = 1;
            this.label1.Text = "Your player id:";
            // 
            // comboBox_PlayerId
            // 
            this.comboBox_PlayerId.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBox_PlayerId.FormattingEnabled = true;
            this.comboBox_PlayerId.Items.AddRange(new object[] {
            "Player 0",
            "Player 1",
            "Player 2",
            "Player 3",
            "Player 4",
            "Player 5",
            "Player 6",
            "Player 7",
            "Player 8",
            "Player 9",
            "Player 10"});
            this.comboBox_PlayerId.Location = new System.Drawing.Point(93, 169);
            this.comboBox_PlayerId.Name = "comboBox_PlayerId";
            this.comboBox_PlayerId.Size = new System.Drawing.Size(111, 23);
            this.comboBox_PlayerId.TabIndex = 0;
            // 
            // ButtonStart
            // 
            this.ButtonStart.FlatAppearance.MouseDownBackColor = System.Drawing.Color.LightGray;
            this.ButtonStart.FlatAppearance.MouseOverBackColor = System.Drawing.Color.LightGray;
            this.ButtonStart.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ButtonStart.ForeColor = System.Drawing.Color.Green;
            this.ButtonStart.Location = new System.Drawing.Point(12, 293);
            this.ButtonStart.Name = "ButtonStart";
            this.ButtonStart.Size = new System.Drawing.Size(280, 42);
            this.ButtonStart.TabIndex = 3;
            this.ButtonStart.Text = "Start";
            this.ButtonStart.UseVisualStyleBackColor = true;
            this.ButtonStart.Click += new System.EventHandler(this.OnButtonStart_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 353);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(306, 22);
            this.statusStrip1.TabIndex = 6;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // statusLabel
            // 
            this.statusLabel.ForeColor = System.Drawing.Color.Blue;
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(306, 375);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.ButtonStart);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "frmMain";
            this.Text = "WarHack";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarEnergy)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarDelay)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem squitToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar trackBarEnergy;
        private System.Windows.Forms.TrackBar trackBarDelay;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox_PlayerId;
        private System.Windows.Forms.Label labelEnergy;
        private System.Windows.Forms.Label labelDelay;
        private System.Windows.Forms.Button ButtonStart;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutWarHackToolStripMenuItem;
        private System.Windows.Forms.CheckBox checkBox_EasterEgg;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem downloadWarzoneToolStripMenuItem;
    }
}

