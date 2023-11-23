namespace CFP_control_program
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.cmbComPorts = new System.Windows.Forms.ComboBox();
            this.lblBaudRate = new System.Windows.Forms.Label();
            this.txtBaudRate = new System.Windows.Forms.TextBox();
            this.btnComConnect = new System.Windows.Forms.Button();
            this.txtKeyboardInput = new System.Windows.Forms.TextBox();
            this.lblKeyboardInput = new System.Windows.Forms.Label();
            this.cbComResponse = new System.Windows.Forms.CheckBox();
            this.cbAlphNumOutput = new System.Windows.Forms.CheckBox();
            this.txtComOutput = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.lblDataRate = new System.Windows.Forms.Label();
            this.cbAutoReconnect = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // cmbComPorts
            // 
            this.cmbComPorts.FormattingEnabled = true;
            this.cmbComPorts.Location = new System.Drawing.Point(12, 12);
            this.cmbComPorts.Name = "cmbComPorts";
            this.cmbComPorts.Size = new System.Drawing.Size(186, 28);
            this.cmbComPorts.TabIndex = 0;
            this.cmbComPorts.DropDown += new System.EventHandler(this.cmbComPorts_DropDown);
            // 
            // lblBaudRate
            // 
            this.lblBaudRate.AutoSize = true;
            this.lblBaudRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblBaudRate.Location = new System.Drawing.Point(204, 15);
            this.lblBaudRate.Name = "lblBaudRate";
            this.lblBaudRate.Size = new System.Drawing.Size(90, 20);
            this.lblBaudRate.TabIndex = 1;
            this.lblBaudRate.Text = "Baud Rate:";
            // 
            // txtBaudRate
            // 
            this.txtBaudRate.Location = new System.Drawing.Point(300, 12);
            this.txtBaudRate.Name = "txtBaudRate";
            this.txtBaudRate.Size = new System.Drawing.Size(146, 26);
            this.txtBaudRate.TabIndex = 2;
            this.txtBaudRate.Text = "9600";
            // 
            // btnComConnect
            // 
            this.btnComConnect.Location = new System.Drawing.Point(452, 12);
            this.btnComConnect.Name = "btnComConnect";
            this.btnComConnect.Size = new System.Drawing.Size(132, 28);
            this.btnComConnect.TabIndex = 3;
            this.btnComConnect.Text = "Connect";
            this.btnComConnect.UseVisualStyleBackColor = true;
            this.btnComConnect.Click += new System.EventHandler(this.btnComConnect_Click);
            // 
            // txtKeyboardInput
            // 
            this.txtKeyboardInput.Location = new System.Drawing.Point(12, 266);
            this.txtKeyboardInput.Multiline = true;
            this.txtKeyboardInput.Name = "txtKeyboardInput";
            this.txtKeyboardInput.Size = new System.Drawing.Size(563, 80);
            this.txtKeyboardInput.TabIndex = 4;
            this.txtKeyboardInput.TextChanged += new System.EventHandler(this.txtKeyboardInput_TextChanged);
            // 
            // lblKeyboardInput
            // 
            this.lblKeyboardInput.AutoSize = true;
            this.lblKeyboardInput.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblKeyboardInput.Location = new System.Drawing.Point(12, 234);
            this.lblKeyboardInput.Name = "lblKeyboardInput";
            this.lblKeyboardInput.Size = new System.Drawing.Size(196, 29);
            this.lblKeyboardInput.TabIndex = 5;
            this.lblKeyboardInput.Text = "Keyboard Input:";
            // 
            // cbComResponse
            // 
            this.cbComResponse.AutoSize = true;
            this.cbComResponse.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbComResponse.Location = new System.Drawing.Point(12, 352);
            this.cbComResponse.Name = "cbComResponse";
            this.cbComResponse.Size = new System.Drawing.Size(289, 30);
            this.cbComResponse.TabIndex = 6;
            this.cbComResponse.Text = "Response from COM Port";
            this.cbComResponse.UseVisualStyleBackColor = true;
            // 
            // cbAlphNumOutput
            // 
            this.cbAlphNumOutput.AutoSize = true;
            this.cbAlphNumOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbAlphNumOutput.Location = new System.Drawing.Point(307, 352);
            this.cbAlphNumOutput.Name = "cbAlphNumOutput";
            this.cbAlphNumOutput.Size = new System.Drawing.Size(243, 30);
            this.cbAlphNumOutput.TabIndex = 7;
            this.cbAlphNumOutput.Text = "Alphanumeric Output";
            this.cbAlphNumOutput.UseVisualStyleBackColor = true;
            // 
            // txtComOutput
            // 
            this.txtComOutput.Location = new System.Drawing.Point(12, 388);
            this.txtComOutput.Multiline = true;
            this.txtComOutput.Name = "txtComOutput";
            this.txtComOutput.Size = new System.Drawing.Size(563, 269);
            this.txtComOutput.TabIndex = 8;
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // lblDataRate
            // 
            this.lblDataRate.AutoSize = true;
            this.lblDataRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDataRate.Location = new System.Drawing.Point(12, 660);
            this.lblDataRate.Name = "lblDataRate";
            this.lblDataRate.Size = new System.Drawing.Size(413, 26);
            this.lblDataRate.TabIndex = 9;
            this.lblDataRate.Text = "Incoming Data Rate = 0 bytes per second";
            // 
            // cbAutoReconnect
            // 
            this.cbAutoReconnect.AutoSize = true;
            this.cbAutoReconnect.Location = new System.Drawing.Point(424, 46);
            this.cbAutoReconnect.Name = "cbAutoReconnect";
            this.cbAutoReconnect.Size = new System.Drawing.Size(151, 24);
            this.cbAutoReconnect.TabIndex = 10;
            this.cbAutoReconnect.Text = "Auto Reconnect";
            this.cbAutoReconnect.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(587, 715);
            this.Controls.Add(this.cbAutoReconnect);
            this.Controls.Add(this.lblDataRate);
            this.Controls.Add(this.txtComOutput);
            this.Controls.Add(this.cbAlphNumOutput);
            this.Controls.Add(this.cbComResponse);
            this.Controls.Add(this.lblKeyboardInput);
            this.Controls.Add(this.txtKeyboardInput);
            this.Controls.Add(this.btnComConnect);
            this.Controls.Add(this.txtBaudRate);
            this.Controls.Add(this.lblBaudRate);
            this.Controls.Add(this.cmbComPorts);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmbComPorts;
        private System.Windows.Forms.Label lblBaudRate;
        private System.Windows.Forms.TextBox txtBaudRate;
        private System.Windows.Forms.Button btnComConnect;
        private System.Windows.Forms.TextBox txtKeyboardInput;
        private System.Windows.Forms.Label lblKeyboardInput;
        private System.Windows.Forms.CheckBox cbComResponse;
        private System.Windows.Forms.CheckBox cbAlphNumOutput;
        private System.Windows.Forms.TextBox txtComOutput;
        private System.Windows.Forms.Timer timer1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Label lblDataRate;
        private System.Windows.Forms.CheckBox cbAutoReconnect;
    }
}

