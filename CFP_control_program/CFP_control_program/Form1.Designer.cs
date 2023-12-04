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
            System.Windows.Forms.DataVisualization.Charting.Title title5 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.Title title6 = new System.Windows.Forms.DataVisualization.Charting.Title();
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
            this.txtByte1 = new System.Windows.Forms.TextBox();
            this.txtByte2 = new System.Windows.Forms.TextBox();
            this.txtByte3 = new System.Windows.Forms.TextBox();
            this.txtByte4 = new System.Windows.Forms.TextBox();
            this.txtByte5 = new System.Windows.Forms.TextBox();
            this.chkByte1 = new System.Windows.Forms.CheckBox();
            this.chkByte2 = new System.Windows.Forms.CheckBox();
            this.chkByte3 = new System.Windows.Forms.CheckBox();
            this.chkByte4 = new System.Windows.Forms.CheckBox();
            this.chkByte5 = new System.Windows.Forms.CheckBox();
            this.btnTransmitToComPort = new System.Windows.Forms.Button();
            this.btnZeroPosition = new System.Windows.Forms.Button();
            this.btnSTOP = new System.Windows.Forms.Button();
            this.tbManualMove = new System.Windows.Forms.TrackBar();
            this.lblManualMovement = new System.Windows.Forms.Label();
            this.lblSetParameters = new System.Windows.Forms.Label();
            this.txtMembraneSize = new System.Windows.Forms.TextBox();
            this.btnSetMembraneSize = new System.Windows.Forms.Button();
            this.txtStrainTarget = new System.Windows.Forms.TextBox();
            this.btnSetStrainTarget = new System.Windows.Forms.Button();
            this.txtStrainRate = new System.Windows.Forms.TextBox();
            this.btnSetStrainRate = new System.Windows.Forms.Button();
            this.txtStrainIncrement = new System.Windows.Forms.TextBox();
            this.btnSetStrainIncrement = new System.Windows.Forms.Button();
            this.btnLockParams = new System.Windows.Forms.Button();
            this.btnReturntoZero = new System.Windows.Forms.Button();
            this.btnStretchtoMaxStrain = new System.Windows.Forms.Button();
            this.txtStrainCycles = new System.Windows.Forms.TextBox();
            this.btnSetStrainCycles = new System.Windows.Forms.Button();
            this.lblCyclicStretching = new System.Windows.Forms.Label();
            this.btnCyclicStretching = new System.Windows.Forms.Button();
            this.lblPositiveMove = new System.Windows.Forms.Label();
            this.lblNegativeMove = new System.Windows.Forms.Label();
            this.chartLoadCellForce = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chartCurrentStepPosition = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.txtFileName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSelectFileName = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.cbSaveFile = new System.Windows.Forms.CheckBox();
            this.btnResetAllCharts = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.tbManualMove)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartLoadCellForce)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartCurrentStepPosition)).BeginInit();
            this.SuspendLayout();
            // 
            // cmbComPorts
            // 
            this.cmbComPorts.FormattingEnabled = true;
            this.cmbComPorts.Location = new System.Drawing.Point(11, 10);
            this.cmbComPorts.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cmbComPorts.Name = "cmbComPorts";
            this.cmbComPorts.Size = new System.Drawing.Size(166, 24);
            this.cmbComPorts.TabIndex = 0;
            this.cmbComPorts.DropDown += new System.EventHandler(this.cmbComPorts_DropDown);
            // 
            // lblBaudRate
            // 
            this.lblBaudRate.AutoSize = true;
            this.lblBaudRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblBaudRate.Location = new System.Drawing.Point(181, 12);
            this.lblBaudRate.Name = "lblBaudRate";
            this.lblBaudRate.Size = new System.Drawing.Size(79, 17);
            this.lblBaudRate.TabIndex = 1;
            this.lblBaudRate.Text = "Baud Rate:";
            // 
            // txtBaudRate
            // 
            this.txtBaudRate.Location = new System.Drawing.Point(267, 10);
            this.txtBaudRate.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtBaudRate.Name = "txtBaudRate";
            this.txtBaudRate.Size = new System.Drawing.Size(130, 22);
            this.txtBaudRate.TabIndex = 2;
            this.txtBaudRate.Text = "38400";
            // 
            // btnComConnect
            // 
            this.btnComConnect.Location = new System.Drawing.Point(402, 10);
            this.btnComConnect.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnComConnect.Name = "btnComConnect";
            this.btnComConnect.Size = new System.Drawing.Size(117, 23);
            this.btnComConnect.TabIndex = 3;
            this.btnComConnect.Text = "Connect";
            this.btnComConnect.UseVisualStyleBackColor = true;
            this.btnComConnect.Click += new System.EventHandler(this.btnComConnect_Click);
            // 
            // txtKeyboardInput
            // 
            this.txtKeyboardInput.Location = new System.Drawing.Point(9, 590);
            this.txtKeyboardInput.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtKeyboardInput.Multiline = true;
            this.txtKeyboardInput.Name = "txtKeyboardInput";
            this.txtKeyboardInput.Size = new System.Drawing.Size(501, 65);
            this.txtKeyboardInput.TabIndex = 4;
            this.txtKeyboardInput.TextChanged += new System.EventHandler(this.txtKeyboardInput_TextChanged);
            // 
            // lblKeyboardInput
            // 
            this.lblKeyboardInput.AutoSize = true;
            this.lblKeyboardInput.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblKeyboardInput.Location = new System.Drawing.Point(9, 565);
            this.lblKeyboardInput.Name = "lblKeyboardInput";
            this.lblKeyboardInput.Size = new System.Drawing.Size(166, 25);
            this.lblKeyboardInput.TabIndex = 5;
            this.lblKeyboardInput.Text = "Keyboard Input:";
            // 
            // cbComResponse
            // 
            this.cbComResponse.AutoSize = true;
            this.cbComResponse.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbComResponse.Location = new System.Drawing.Point(9, 659);
            this.cbComResponse.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbComResponse.Name = "cbComResponse";
            this.cbComResponse.Size = new System.Drawing.Size(247, 28);
            this.cbComResponse.TabIndex = 6;
            this.cbComResponse.Text = "Response from COM Port";
            this.cbComResponse.UseVisualStyleBackColor = true;
            // 
            // cbAlphNumOutput
            // 
            this.cbAlphNumOutput.AutoSize = true;
            this.cbAlphNumOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbAlphNumOutput.Location = new System.Drawing.Point(271, 659);
            this.cbAlphNumOutput.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbAlphNumOutput.Name = "cbAlphNumOutput";
            this.cbAlphNumOutput.Size = new System.Drawing.Size(211, 28);
            this.cbAlphNumOutput.TabIndex = 7;
            this.cbAlphNumOutput.Text = "Alphanumeric Output";
            this.cbAlphNumOutput.UseVisualStyleBackColor = true;
            // 
            // txtComOutput
            // 
            this.txtComOutput.Location = new System.Drawing.Point(11, 688);
            this.txtComOutput.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtComOutput.Multiline = true;
            this.txtComOutput.Name = "txtComOutput";
            this.txtComOutput.Size = new System.Drawing.Size(501, 132);
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
            this.lblDataRate.Location = new System.Drawing.Point(11, 822);
            this.lblDataRate.Name = "lblDataRate";
            this.lblDataRate.Size = new System.Drawing.Size(354, 24);
            this.lblDataRate.TabIndex = 9;
            this.lblDataRate.Text = "Incoming Data Rate = 0 bytes per second";
            // 
            // cbAutoReconnect
            // 
            this.cbAutoReconnect.AutoSize = true;
            this.cbAutoReconnect.Location = new System.Drawing.Point(377, 37);
            this.cbAutoReconnect.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cbAutoReconnect.Name = "cbAutoReconnect";
            this.cbAutoReconnect.Size = new System.Drawing.Size(131, 21);
            this.cbAutoReconnect.TabIndex = 10;
            this.cbAutoReconnect.Text = "Auto Reconnect";
            this.cbAutoReconnect.UseVisualStyleBackColor = true;
            // 
            // txtByte1
            // 
            this.txtByte1.Location = new System.Drawing.Point(11, 87);
            this.txtByte1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtByte1.Name = "txtByte1";
            this.txtByte1.Size = new System.Drawing.Size(89, 22);
            this.txtByte1.TabIndex = 11;
            this.txtByte1.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // txtByte2
            // 
            this.txtByte2.Location = new System.Drawing.Point(105, 87);
            this.txtByte2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtByte2.Name = "txtByte2";
            this.txtByte2.Size = new System.Drawing.Size(89, 22);
            this.txtByte2.TabIndex = 12;
            // 
            // txtByte3
            // 
            this.txtByte3.Location = new System.Drawing.Point(199, 87);
            this.txtByte3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtByte3.Name = "txtByte3";
            this.txtByte3.Size = new System.Drawing.Size(89, 22);
            this.txtByte3.TabIndex = 13;
            // 
            // txtByte4
            // 
            this.txtByte4.Location = new System.Drawing.Point(293, 87);
            this.txtByte4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtByte4.Name = "txtByte4";
            this.txtByte4.Size = new System.Drawing.Size(89, 22);
            this.txtByte4.TabIndex = 14;
            // 
            // txtByte5
            // 
            this.txtByte5.Location = new System.Drawing.Point(388, 87);
            this.txtByte5.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtByte5.Name = "txtByte5";
            this.txtByte5.Size = new System.Drawing.Size(89, 22);
            this.txtByte5.TabIndex = 15;
            // 
            // chkByte1
            // 
            this.chkByte1.AutoSize = true;
            this.chkByte1.Location = new System.Drawing.Point(12, 63);
            this.chkByte1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkByte1.Name = "chkByte1";
            this.chkByte1.Size = new System.Drawing.Size(78, 21);
            this.chkByte1.TabIndex = 16;
            this.chkByte1.Text = "Byte #1";
            this.chkByte1.UseVisualStyleBackColor = true;
            this.chkByte1.CheckedChanged += new System.EventHandler(this.chkByte1_CheckedChanged);
            // 
            // chkByte2
            // 
            this.chkByte2.AutoSize = true;
            this.chkByte2.Location = new System.Drawing.Point(105, 63);
            this.chkByte2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkByte2.Name = "chkByte2";
            this.chkByte2.Size = new System.Drawing.Size(78, 21);
            this.chkByte2.TabIndex = 17;
            this.chkByte2.Text = "Byte #2";
            this.chkByte2.UseVisualStyleBackColor = true;
            this.chkByte2.CheckedChanged += new System.EventHandler(this.chkByte2_CheckedChanged);
            // 
            // chkByte3
            // 
            this.chkByte3.AutoSize = true;
            this.chkByte3.Location = new System.Drawing.Point(199, 62);
            this.chkByte3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkByte3.Name = "chkByte3";
            this.chkByte3.Size = new System.Drawing.Size(78, 21);
            this.chkByte3.TabIndex = 18;
            this.chkByte3.Text = "Byte #3";
            this.chkByte3.UseVisualStyleBackColor = true;
            this.chkByte3.CheckedChanged += new System.EventHandler(this.chkByte3_CheckedChanged);
            // 
            // chkByte4
            // 
            this.chkByte4.AutoSize = true;
            this.chkByte4.Location = new System.Drawing.Point(293, 62);
            this.chkByte4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkByte4.Name = "chkByte4";
            this.chkByte4.Size = new System.Drawing.Size(78, 21);
            this.chkByte4.TabIndex = 19;
            this.chkByte4.Text = "Byte #4";
            this.chkByte4.UseVisualStyleBackColor = true;
            this.chkByte4.CheckedChanged += new System.EventHandler(this.chkByte4_CheckedChanged);
            // 
            // chkByte5
            // 
            this.chkByte5.AutoSize = true;
            this.chkByte5.Location = new System.Drawing.Point(388, 63);
            this.chkByte5.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkByte5.Name = "chkByte5";
            this.chkByte5.Size = new System.Drawing.Size(78, 21);
            this.chkByte5.TabIndex = 20;
            this.chkByte5.Text = "Byte #5";
            this.chkByte5.UseVisualStyleBackColor = true;
            this.chkByte5.CheckedChanged += new System.EventHandler(this.chkByte5_CheckedChanged);
            // 
            // btnTransmitToComPort
            // 
            this.btnTransmitToComPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTransmitToComPort.Location = new System.Drawing.Point(11, 113);
            this.btnTransmitToComPort.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnTransmitToComPort.Name = "btnTransmitToComPort";
            this.btnTransmitToComPort.Size = new System.Drawing.Size(500, 42);
            this.btnTransmitToComPort.TabIndex = 21;
            this.btnTransmitToComPort.Text = "Transmit to COM Port";
            this.btnTransmitToComPort.UseVisualStyleBackColor = true;
            this.btnTransmitToComPort.Click += new System.EventHandler(this.btnTransmitToComPort_Click);
            // 
            // btnZeroPosition
            // 
            this.btnZeroPosition.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnZeroPosition.Location = new System.Drawing.Point(11, 161);
            this.btnZeroPosition.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnZeroPosition.Name = "btnZeroPosition";
            this.btnZeroPosition.Size = new System.Drawing.Size(132, 40);
            this.btnZeroPosition.TabIndex = 22;
            this.btnZeroPosition.Text = "Zero Position";
            this.btnZeroPosition.UseVisualStyleBackColor = true;
            this.btnZeroPosition.Click += new System.EventHandler(this.btnZeroPosition_Click);
            // 
            // btnSTOP
            // 
            this.btnSTOP.BackColor = System.Drawing.Color.Firebrick;
            this.btnSTOP.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSTOP.ForeColor = System.Drawing.SystemColors.Control;
            this.btnSTOP.Location = new System.Drawing.Point(11, 206);
            this.btnSTOP.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSTOP.Name = "btnSTOP";
            this.btnSTOP.Size = new System.Drawing.Size(132, 40);
            this.btnSTOP.TabIndex = 23;
            this.btnSTOP.Text = "STOP";
            this.btnSTOP.UseVisualStyleBackColor = false;
            this.btnSTOP.Click += new System.EventHandler(this.btnSTOP_Click);
            // 
            // tbManualMove
            // 
            this.tbManualMove.Location = new System.Drawing.Point(148, 190);
            this.tbManualMove.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tbManualMove.Name = "tbManualMove";
            this.tbManualMove.Size = new System.Drawing.Size(363, 56);
            this.tbManualMove.TabIndex = 24;
            this.tbManualMove.Value = 5;
            this.tbManualMove.ValueChanged += new System.EventHandler(this.tbManualMove_ValueChanged);
            // 
            // lblManualMovement
            // 
            this.lblManualMovement.AutoSize = true;
            this.lblManualMovement.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblManualMovement.Location = new System.Drawing.Point(144, 161);
            this.lblManualMovement.Name = "lblManualMovement";
            this.lblManualMovement.Size = new System.Drawing.Size(195, 20);
            this.lblManualMovement.TabIndex = 25;
            this.lblManualMovement.Text = "MANUAL MOVEMENT";
            this.lblManualMovement.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lblSetParameters
            // 
            this.lblSetParameters.AutoSize = true;
            this.lblSetParameters.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSetParameters.Location = new System.Drawing.Point(11, 248);
            this.lblSetParameters.Name = "lblSetParameters";
            this.lblSetParameters.Size = new System.Drawing.Size(174, 20);
            this.lblSetParameters.TabIndex = 26;
            this.lblSetParameters.Text = "SET PARAMETERS";
            // 
            // txtMembraneSize
            // 
            this.txtMembraneSize.Location = new System.Drawing.Point(11, 270);
            this.txtMembraneSize.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtMembraneSize.Multiline = true;
            this.txtMembraneSize.Name = "txtMembraneSize";
            this.txtMembraneSize.Size = new System.Drawing.Size(169, 32);
            this.txtMembraneSize.TabIndex = 27;
            this.txtMembraneSize.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetMembraneSize
            // 
            this.btnSetMembraneSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetMembraneSize.Location = new System.Drawing.Point(184, 270);
            this.btnSetMembraneSize.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSetMembraneSize.Name = "btnSetMembraneSize";
            this.btnSetMembraneSize.Size = new System.Drawing.Size(212, 31);
            this.btnSetMembraneSize.TabIndex = 28;
            this.btnSetMembraneSize.Text = "Set Membrane Size [mm]";
            this.btnSetMembraneSize.UseVisualStyleBackColor = true;
            this.btnSetMembraneSize.Click += new System.EventHandler(this.btnSetMembraneSize_Click);
            // 
            // txtStrainTarget
            // 
            this.txtStrainTarget.Location = new System.Drawing.Point(11, 306);
            this.txtStrainTarget.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtStrainTarget.Multiline = true;
            this.txtStrainTarget.Name = "txtStrainTarget";
            this.txtStrainTarget.Size = new System.Drawing.Size(169, 32);
            this.txtStrainTarget.TabIndex = 29;
            this.txtStrainTarget.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetStrainTarget
            // 
            this.btnSetStrainTarget.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainTarget.Location = new System.Drawing.Point(183, 306);
            this.btnSetStrainTarget.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSetStrainTarget.Name = "btnSetStrainTarget";
            this.btnSetStrainTarget.Size = new System.Drawing.Size(212, 31);
            this.btnSetStrainTarget.TabIndex = 30;
            this.btnSetStrainTarget.Text = "Set Strain Target [%]";
            this.btnSetStrainTarget.UseVisualStyleBackColor = true;
            this.btnSetStrainTarget.Click += new System.EventHandler(this.btnSetStrainTarget_Click);
            // 
            // txtStrainRate
            // 
            this.txtStrainRate.Location = new System.Drawing.Point(11, 342);
            this.txtStrainRate.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtStrainRate.Multiline = true;
            this.txtStrainRate.Name = "txtStrainRate";
            this.txtStrainRate.Size = new System.Drawing.Size(168, 32);
            this.txtStrainRate.TabIndex = 31;
            this.txtStrainRate.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetStrainRate
            // 
            this.btnSetStrainRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainRate.Location = new System.Drawing.Point(184, 342);
            this.btnSetStrainRate.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSetStrainRate.Name = "btnSetStrainRate";
            this.btnSetStrainRate.Size = new System.Drawing.Size(212, 31);
            this.btnSetStrainRate.TabIndex = 32;
            this.btnSetStrainRate.Text = "Set Strain Rate [%/s]";
            this.btnSetStrainRate.UseVisualStyleBackColor = true;
            this.btnSetStrainRate.Click += new System.EventHandler(this.btnSetStrainRate_Click);
            // 
            // txtStrainIncrement
            // 
            this.txtStrainIncrement.Location = new System.Drawing.Point(10, 378);
            this.txtStrainIncrement.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtStrainIncrement.Multiline = true;
            this.txtStrainIncrement.Name = "txtStrainIncrement";
            this.txtStrainIncrement.Size = new System.Drawing.Size(169, 32);
            this.txtStrainIncrement.TabIndex = 33;
            this.txtStrainIncrement.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetStrainIncrement
            // 
            this.btnSetStrainIncrement.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainIncrement.Location = new System.Drawing.Point(183, 378);
            this.btnSetStrainIncrement.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSetStrainIncrement.Name = "btnSetStrainIncrement";
            this.btnSetStrainIncrement.Size = new System.Drawing.Size(212, 31);
            this.btnSetStrainIncrement.TabIndex = 34;
            this.btnSetStrainIncrement.Text = "Set Strain Increment [%]";
            this.btnSetStrainIncrement.UseVisualStyleBackColor = true;
            this.btnSetStrainIncrement.Click += new System.EventHandler(this.btnSetStrainIncrement_Click);
            // 
            // btnLockParams
            // 
            this.btnLockParams.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLockParams.Location = new System.Drawing.Point(400, 270);
            this.btnLockParams.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnLockParams.Name = "btnLockParams";
            this.btnLockParams.Size = new System.Drawing.Size(109, 139);
            this.btnLockParams.TabIndex = 35;
            this.btnLockParams.Text = "Lock Params";
            this.btnLockParams.UseVisualStyleBackColor = true;
            this.btnLockParams.Click += new System.EventHandler(this.btnLockParams_Click);
            // 
            // btnReturntoZero
            // 
            this.btnReturntoZero.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnReturntoZero.Location = new System.Drawing.Point(10, 414);
            this.btnReturntoZero.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnReturntoZero.Name = "btnReturntoZero";
            this.btnReturntoZero.Size = new System.Drawing.Size(251, 38);
            this.btnReturntoZero.TabIndex = 36;
            this.btnReturntoZero.Text = "RETURN TO 0";
            this.btnReturntoZero.UseVisualStyleBackColor = true;
            this.btnReturntoZero.Click += new System.EventHandler(this.btnReturntoZero_Click);
            // 
            // btnStretchtoMaxStrain
            // 
            this.btnStretchtoMaxStrain.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnStretchtoMaxStrain.Location = new System.Drawing.Point(266, 414);
            this.btnStretchtoMaxStrain.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnStretchtoMaxStrain.Name = "btnStretchtoMaxStrain";
            this.btnStretchtoMaxStrain.Size = new System.Drawing.Size(244, 38);
            this.btnStretchtoMaxStrain.TabIndex = 37;
            this.btnStretchtoMaxStrain.Text = "STRETCH TO X%";
            this.btnStretchtoMaxStrain.UseVisualStyleBackColor = true;
            this.btnStretchtoMaxStrain.Click += new System.EventHandler(this.btnStretchtoMaxStrain_Click);
            // 
            // txtStrainCycles
            // 
            this.txtStrainCycles.Location = new System.Drawing.Point(9, 480);
            this.txtStrainCycles.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtStrainCycles.Multiline = true;
            this.txtStrainCycles.Name = "txtStrainCycles";
            this.txtStrainCycles.Size = new System.Drawing.Size(250, 30);
            this.txtStrainCycles.TabIndex = 38;
            this.txtStrainCycles.TextChanged += new System.EventHandler(this.txtStrainCycles_TextChanged);
            // 
            // btnSetStrainCycles
            // 
            this.btnSetStrainCycles.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainCycles.Location = new System.Drawing.Point(264, 480);
            this.btnSetStrainCycles.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSetStrainCycles.Name = "btnSetStrainCycles";
            this.btnSetStrainCycles.Size = new System.Drawing.Size(245, 30);
            this.btnSetStrainCycles.TabIndex = 39;
            this.btnSetStrainCycles.Text = "Set Number of Strain Cycles";
            this.btnSetStrainCycles.UseVisualStyleBackColor = true;
            this.btnSetStrainCycles.Click += new System.EventHandler(this.btnSetStrainCycles_Click);
            // 
            // lblCyclicStretching
            // 
            this.lblCyclicStretching.AutoSize = true;
            this.lblCyclicStretching.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCyclicStretching.Location = new System.Drawing.Point(9, 454);
            this.lblCyclicStretching.Name = "lblCyclicStretching";
            this.lblCyclicStretching.Size = new System.Drawing.Size(237, 25);
            this.lblCyclicStretching.TabIndex = 40;
            this.lblCyclicStretching.Text = "CYCLIC STRETCHING";
            // 
            // btnCyclicStretching
            // 
            this.btnCyclicStretching.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCyclicStretching.Location = new System.Drawing.Point(9, 514);
            this.btnCyclicStretching.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnCyclicStretching.Name = "btnCyclicStretching";
            this.btnCyclicStretching.Size = new System.Drawing.Size(500, 48);
            this.btnCyclicStretching.TabIndex = 41;
            this.btnCyclicStretching.Text = "CYCLIC STRETCHING";
            this.btnCyclicStretching.UseVisualStyleBackColor = true;
            this.btnCyclicStretching.Click += new System.EventHandler(this.btnCyclicStretching_Click);
            // 
            // lblPositiveMove
            // 
            this.lblPositiveMove.AutoSize = true;
            this.lblPositiveMove.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPositiveMove.Location = new System.Drawing.Point(472, 209);
            this.lblPositiveMove.Name = "lblPositiveMove";
            this.lblPositiveMove.Size = new System.Drawing.Size(38, 39);
            this.lblPositiveMove.TabIndex = 42;
            this.lblPositiveMove.Text = "+";
            // 
            // lblNegativeMove
            // 
            this.lblNegativeMove.AutoSize = true;
            this.lblNegativeMove.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblNegativeMove.Location = new System.Drawing.Point(154, 209);
            this.lblNegativeMove.Name = "lblNegativeMove";
            this.lblNegativeMove.Size = new System.Drawing.Size(29, 39);
            this.lblNegativeMove.TabIndex = 43;
            this.lblNegativeMove.Text = "-";
            // 
            // chartLoadCellForce
            // 
            this.chartLoadCellForce.Location = new System.Drawing.Point(524, 29);
            this.chartLoadCellForce.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chartLoadCellForce.Name = "chartLoadCellForce";
            this.chartLoadCellForce.Size = new System.Drawing.Size(897, 393);
            this.chartLoadCellForce.TabIndex = 44;
            this.chartLoadCellForce.Text = "chart1";
            title5.Name = "Load Cell Force";
            title5.Text = "Load Cell Force";
            this.chartLoadCellForce.Titles.Add(title5);
            // 
            // chartCurrentStepPosition
            // 
            this.chartCurrentStepPosition.Location = new System.Drawing.Point(524, 426);
            this.chartCurrentStepPosition.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chartCurrentStepPosition.Name = "chartCurrentStepPosition";
            this.chartCurrentStepPosition.Size = new System.Drawing.Size(897, 393);
            this.chartCurrentStepPosition.TabIndex = 45;
            this.chartCurrentStepPosition.Text = "chart2";
            title6.Name = "Current Position";
            title6.Text = "Current Position";
            this.chartCurrentStepPosition.Titles.Add(title6);
            // 
            // txtFileName
            // 
            this.txtFileName.Location = new System.Drawing.Point(963, 824);
            this.txtFileName.Name = "txtFileName";
            this.txtFileName.Size = new System.Drawing.Size(299, 22);
            this.txtFileName.TabIndex = 46;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(835, 827);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(122, 17);
            this.label1.TabIndex = 47;
            this.label1.Text = "Output File Name:";
            // 
            // btnSelectFileName
            // 
            this.btnSelectFileName.Location = new System.Drawing.Point(1268, 824);
            this.btnSelectFileName.Name = "btnSelectFileName";
            this.btnSelectFileName.Size = new System.Drawing.Size(153, 23);
            this.btnSelectFileName.TabIndex = 48;
            this.btnSelectFileName.Text = "Select File Name";
            this.btnSelectFileName.UseVisualStyleBackColor = true;
            this.btnSelectFileName.Click += new System.EventHandler(this.btnSelectFileName_Click);
            // 
            // cbSaveFile
            // 
            this.cbSaveFile.AutoSize = true;
            this.cbSaveFile.Location = new System.Drawing.Point(720, 826);
            this.cbSaveFile.Name = "cbSaveFile";
            this.cbSaveFile.Size = new System.Drawing.Size(109, 21);
            this.cbSaveFile.TabIndex = 49;
            this.cbSaveFile.Text = "Save To File";
            this.cbSaveFile.UseVisualStyleBackColor = true;
            this.cbSaveFile.CheckedChanged += new System.EventHandler(this.cbSaveFile_CheckedChanged);
            // 
            // btnResetAllCharts
            // 
            this.btnResetAllCharts.Location = new System.Drawing.Point(1177, 10);
            this.btnResetAllCharts.Name = "btnResetAllCharts";
            this.btnResetAllCharts.Size = new System.Drawing.Size(243, 23);
            this.btnResetAllCharts.TabIndex = 52;
            this.btnResetAllCharts.Text = "Reset All Charts";
            this.btnResetAllCharts.UseVisualStyleBackColor = true;
            this.btnResetAllCharts.Click += new System.EventHandler(this.btnResetAllCharts_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1432, 897);
            this.Controls.Add(this.btnResetAllCharts);
            this.Controls.Add(this.cbSaveFile);
            this.Controls.Add(this.btnSelectFileName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtFileName);
            this.Controls.Add(this.chartCurrentStepPosition);
            this.Controls.Add(this.chartLoadCellForce);
            this.Controls.Add(this.lblNegativeMove);
            this.Controls.Add(this.lblPositiveMove);
            this.Controls.Add(this.btnCyclicStretching);
            this.Controls.Add(this.lblCyclicStretching);
            this.Controls.Add(this.btnSetStrainCycles);
            this.Controls.Add(this.txtStrainCycles);
            this.Controls.Add(this.btnStretchtoMaxStrain);
            this.Controls.Add(this.btnReturntoZero);
            this.Controls.Add(this.btnLockParams);
            this.Controls.Add(this.btnSetStrainIncrement);
            this.Controls.Add(this.txtStrainIncrement);
            this.Controls.Add(this.btnSetStrainRate);
            this.Controls.Add(this.txtStrainRate);
            this.Controls.Add(this.btnSetStrainTarget);
            this.Controls.Add(this.txtStrainTarget);
            this.Controls.Add(this.btnSetMembraneSize);
            this.Controls.Add(this.txtMembraneSize);
            this.Controls.Add(this.lblSetParameters);
            this.Controls.Add(this.lblManualMovement);
            this.Controls.Add(this.tbManualMove);
            this.Controls.Add(this.btnSTOP);
            this.Controls.Add(this.btnZeroPosition);
            this.Controls.Add(this.btnTransmitToComPort);
            this.Controls.Add(this.chkByte5);
            this.Controls.Add(this.chkByte4);
            this.Controls.Add(this.chkByte3);
            this.Controls.Add(this.chkByte2);
            this.Controls.Add(this.chkByte1);
            this.Controls.Add(this.txtByte5);
            this.Controls.Add(this.txtByte4);
            this.Controls.Add(this.txtByte3);
            this.Controls.Add(this.txtByte2);
            this.Controls.Add(this.txtByte1);
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
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.tbManualMove)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartLoadCellForce)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartCurrentStepPosition)).EndInit();
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
        private System.Windows.Forms.TextBox txtByte1;
        private System.Windows.Forms.TextBox txtByte2;
        private System.Windows.Forms.TextBox txtByte3;
        private System.Windows.Forms.TextBox txtByte4;
        private System.Windows.Forms.TextBox txtByte5;
        private System.Windows.Forms.CheckBox chkByte1;
        private System.Windows.Forms.CheckBox chkByte2;
        private System.Windows.Forms.CheckBox chkByte3;
        private System.Windows.Forms.CheckBox chkByte4;
        private System.Windows.Forms.CheckBox chkByte5;
        private System.Windows.Forms.Button btnTransmitToComPort;
        private System.Windows.Forms.Button btnZeroPosition;
        private System.Windows.Forms.Button btnSTOP;
        private System.Windows.Forms.TrackBar tbManualMove;
        private System.Windows.Forms.Label lblManualMovement;
        private System.Windows.Forms.Label lblSetParameters;
        private System.Windows.Forms.TextBox txtMembraneSize;
        private System.Windows.Forms.Button btnSetMembraneSize;
        private System.Windows.Forms.TextBox txtStrainTarget;
        private System.Windows.Forms.Button btnSetStrainTarget;
        private System.Windows.Forms.TextBox txtStrainRate;
        private System.Windows.Forms.Button btnSetStrainRate;
        private System.Windows.Forms.TextBox txtStrainIncrement;
        private System.Windows.Forms.Button btnSetStrainIncrement;
        private System.Windows.Forms.Button btnLockParams;
        private System.Windows.Forms.Button btnReturntoZero;
        private System.Windows.Forms.Button btnStretchtoMaxStrain;
        private System.Windows.Forms.TextBox txtStrainCycles;
        private System.Windows.Forms.Button btnSetStrainCycles;
        private System.Windows.Forms.Label lblCyclicStretching;
        private System.Windows.Forms.Button btnCyclicStretching;
        private System.Windows.Forms.Label lblPositiveMove;
        private System.Windows.Forms.Label lblNegativeMove;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartLoadCellForce;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartCurrentStepPosition;
        private System.Windows.Forms.TextBox txtFileName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSelectFileName;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.CheckBox cbSaveFile;
        private System.Windows.Forms.Button btnResetAllCharts;
    }
}

