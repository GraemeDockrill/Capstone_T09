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
            ((System.ComponentModel.ISupportInitialize)(this.tbManualMove)).BeginInit();
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
            this.txtKeyboardInput.Location = new System.Drawing.Point(10, 738);
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
            this.lblKeyboardInput.Location = new System.Drawing.Point(10, 706);
            this.lblKeyboardInput.Name = "lblKeyboardInput";
            this.lblKeyboardInput.Size = new System.Drawing.Size(196, 29);
            this.lblKeyboardInput.TabIndex = 5;
            this.lblKeyboardInput.Text = "Keyboard Input:";
            // 
            // cbComResponse
            // 
            this.cbComResponse.AutoSize = true;
            this.cbComResponse.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbComResponse.Location = new System.Drawing.Point(10, 824);
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
            this.cbAlphNumOutput.Location = new System.Drawing.Point(305, 824);
            this.cbAlphNumOutput.Name = "cbAlphNumOutput";
            this.cbAlphNumOutput.Size = new System.Drawing.Size(243, 30);
            this.cbAlphNumOutput.TabIndex = 7;
            this.cbAlphNumOutput.Text = "Alphanumeric Output";
            this.cbAlphNumOutput.UseVisualStyleBackColor = true;
            // 
            // txtComOutput
            // 
            this.txtComOutput.Location = new System.Drawing.Point(12, 860);
            this.txtComOutput.Multiline = true;
            this.txtComOutput.Name = "txtComOutput";
            this.txtComOutput.Size = new System.Drawing.Size(563, 164);
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
            this.lblDataRate.Location = new System.Drawing.Point(12, 1027);
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
            // txtByte1
            // 
            this.txtByte1.Location = new System.Drawing.Point(12, 109);
            this.txtByte1.Name = "txtByte1";
            this.txtByte1.Size = new System.Drawing.Size(100, 26);
            this.txtByte1.TabIndex = 11;
            this.txtByte1.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // txtByte2
            // 
            this.txtByte2.Location = new System.Drawing.Point(118, 109);
            this.txtByte2.Name = "txtByte2";
            this.txtByte2.Size = new System.Drawing.Size(100, 26);
            this.txtByte2.TabIndex = 12;
            // 
            // txtByte3
            // 
            this.txtByte3.Location = new System.Drawing.Point(224, 109);
            this.txtByte3.Name = "txtByte3";
            this.txtByte3.Size = new System.Drawing.Size(100, 26);
            this.txtByte3.TabIndex = 13;
            // 
            // txtByte4
            // 
            this.txtByte4.Location = new System.Drawing.Point(330, 109);
            this.txtByte4.Name = "txtByte4";
            this.txtByte4.Size = new System.Drawing.Size(100, 26);
            this.txtByte4.TabIndex = 14;
            // 
            // txtByte5
            // 
            this.txtByte5.Location = new System.Drawing.Point(436, 109);
            this.txtByte5.Name = "txtByte5";
            this.txtByte5.Size = new System.Drawing.Size(100, 26);
            this.txtByte5.TabIndex = 15;
            // 
            // chkByte1
            // 
            this.chkByte1.AutoSize = true;
            this.chkByte1.Location = new System.Drawing.Point(13, 79);
            this.chkByte1.Name = "chkByte1";
            this.chkByte1.Size = new System.Drawing.Size(89, 24);
            this.chkByte1.TabIndex = 16;
            this.chkByte1.Text = "Byte #1";
            this.chkByte1.UseVisualStyleBackColor = true;
            this.chkByte1.CheckedChanged += new System.EventHandler(this.chkByte1_CheckedChanged);
            // 
            // chkByte2
            // 
            this.chkByte2.AutoSize = true;
            this.chkByte2.Location = new System.Drawing.Point(118, 79);
            this.chkByte2.Name = "chkByte2";
            this.chkByte2.Size = new System.Drawing.Size(89, 24);
            this.chkByte2.TabIndex = 17;
            this.chkByte2.Text = "Byte #2";
            this.chkByte2.UseVisualStyleBackColor = true;
            this.chkByte2.CheckedChanged += new System.EventHandler(this.chkByte2_CheckedChanged);
            // 
            // chkByte3
            // 
            this.chkByte3.AutoSize = true;
            this.chkByte3.Location = new System.Drawing.Point(224, 78);
            this.chkByte3.Name = "chkByte3";
            this.chkByte3.Size = new System.Drawing.Size(89, 24);
            this.chkByte3.TabIndex = 18;
            this.chkByte3.Text = "Byte #3";
            this.chkByte3.UseVisualStyleBackColor = true;
            this.chkByte3.CheckedChanged += new System.EventHandler(this.chkByte3_CheckedChanged);
            // 
            // chkByte4
            // 
            this.chkByte4.AutoSize = true;
            this.chkByte4.Location = new System.Drawing.Point(330, 78);
            this.chkByte4.Name = "chkByte4";
            this.chkByte4.Size = new System.Drawing.Size(89, 24);
            this.chkByte4.TabIndex = 19;
            this.chkByte4.Text = "Byte #4";
            this.chkByte4.UseVisualStyleBackColor = true;
            this.chkByte4.CheckedChanged += new System.EventHandler(this.chkByte4_CheckedChanged);
            // 
            // chkByte5
            // 
            this.chkByte5.AutoSize = true;
            this.chkByte5.Location = new System.Drawing.Point(436, 79);
            this.chkByte5.Name = "chkByte5";
            this.chkByte5.Size = new System.Drawing.Size(89, 24);
            this.chkByte5.TabIndex = 20;
            this.chkByte5.Text = "Byte #5";
            this.chkByte5.UseVisualStyleBackColor = true;
            this.chkByte5.CheckedChanged += new System.EventHandler(this.chkByte5_CheckedChanged);
            // 
            // btnTransmitToComPort
            // 
            this.btnTransmitToComPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTransmitToComPort.Location = new System.Drawing.Point(12, 141);
            this.btnTransmitToComPort.Name = "btnTransmitToComPort";
            this.btnTransmitToComPort.Size = new System.Drawing.Size(563, 53);
            this.btnTransmitToComPort.TabIndex = 21;
            this.btnTransmitToComPort.Text = "Transmit to COM Port";
            this.btnTransmitToComPort.UseVisualStyleBackColor = true;
            this.btnTransmitToComPort.Click += new System.EventHandler(this.btnTransmitToComPort_Click);
            // 
            // btnZeroPosition
            // 
            this.btnZeroPosition.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnZeroPosition.Location = new System.Drawing.Point(12, 201);
            this.btnZeroPosition.Name = "btnZeroPosition";
            this.btnZeroPosition.Size = new System.Drawing.Size(149, 50);
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
            this.btnSTOP.Location = new System.Drawing.Point(12, 257);
            this.btnSTOP.Name = "btnSTOP";
            this.btnSTOP.Size = new System.Drawing.Size(149, 50);
            this.btnSTOP.TabIndex = 23;
            this.btnSTOP.Text = "STOP";
            this.btnSTOP.UseVisualStyleBackColor = false;
            this.btnSTOP.Click += new System.EventHandler(this.btnSTOP_Click);
            // 
            // tbManualMove
            // 
            this.tbManualMove.Location = new System.Drawing.Point(167, 238);
            this.tbManualMove.Name = "tbManualMove";
            this.tbManualMove.Size = new System.Drawing.Size(408, 69);
            this.tbManualMove.TabIndex = 24;
            this.tbManualMove.Value = 5;
            this.tbManualMove.ValueChanged += new System.EventHandler(this.tbManualMove_ValueChanged);
            // 
            // lblManualMovement
            // 
            this.lblManualMovement.AutoSize = true;
            this.lblManualMovement.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblManualMovement.Location = new System.Drawing.Point(162, 201);
            this.lblManualMovement.Name = "lblManualMovement";
            this.lblManualMovement.Size = new System.Drawing.Size(233, 25);
            this.lblManualMovement.TabIndex = 25;
            this.lblManualMovement.Text = "MANUAL MOVEMENT";
            this.lblManualMovement.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lblSetParameters
            // 
            this.lblSetParameters.AutoSize = true;
            this.lblSetParameters.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSetParameters.Location = new System.Drawing.Point(12, 310);
            this.lblSetParameters.Name = "lblSetParameters";
            this.lblSetParameters.Size = new System.Drawing.Size(208, 25);
            this.lblSetParameters.TabIndex = 26;
            this.lblSetParameters.Text = "SET PARAMETERS";
            // 
            // txtMembraneSize
            // 
            this.txtMembraneSize.Location = new System.Drawing.Point(12, 338);
            this.txtMembraneSize.Multiline = true;
            this.txtMembraneSize.Name = "txtMembraneSize";
            this.txtMembraneSize.Size = new System.Drawing.Size(190, 39);
            this.txtMembraneSize.TabIndex = 27;
            this.txtMembraneSize.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetMembraneSize
            // 
            this.btnSetMembraneSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetMembraneSize.Location = new System.Drawing.Point(207, 338);
            this.btnSetMembraneSize.Name = "btnSetMembraneSize";
            this.btnSetMembraneSize.Size = new System.Drawing.Size(238, 39);
            this.btnSetMembraneSize.TabIndex = 28;
            this.btnSetMembraneSize.Text = "Set Membrane Size [mm]";
            this.btnSetMembraneSize.UseVisualStyleBackColor = true;
            this.btnSetMembraneSize.Click += new System.EventHandler(this.btnSetMembraneSize_Click);
            // 
            // txtStrainTarget
            // 
            this.txtStrainTarget.Location = new System.Drawing.Point(12, 383);
            this.txtStrainTarget.Multiline = true;
            this.txtStrainTarget.Name = "txtStrainTarget";
            this.txtStrainTarget.Size = new System.Drawing.Size(190, 39);
            this.txtStrainTarget.TabIndex = 29;
            this.txtStrainTarget.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetStrainTarget
            // 
            this.btnSetStrainTarget.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainTarget.Location = new System.Drawing.Point(206, 383);
            this.btnSetStrainTarget.Name = "btnSetStrainTarget";
            this.btnSetStrainTarget.Size = new System.Drawing.Size(238, 39);
            this.btnSetStrainTarget.TabIndex = 30;
            this.btnSetStrainTarget.Text = "Set Strain Target [%]";
            this.btnSetStrainTarget.UseVisualStyleBackColor = true;
            this.btnSetStrainTarget.Click += new System.EventHandler(this.btnSetStrainTarget_Click);
            // 
            // txtStrainRate
            // 
            this.txtStrainRate.Location = new System.Drawing.Point(12, 428);
            this.txtStrainRate.Multiline = true;
            this.txtStrainRate.Name = "txtStrainRate";
            this.txtStrainRate.Size = new System.Drawing.Size(189, 39);
            this.txtStrainRate.TabIndex = 31;
            this.txtStrainRate.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetStrainRate
            // 
            this.btnSetStrainRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainRate.Location = new System.Drawing.Point(207, 428);
            this.btnSetStrainRate.Name = "btnSetStrainRate";
            this.btnSetStrainRate.Size = new System.Drawing.Size(238, 39);
            this.btnSetStrainRate.TabIndex = 32;
            this.btnSetStrainRate.Text = "Set Strain Rate [%/s]";
            this.btnSetStrainRate.UseVisualStyleBackColor = true;
            this.btnSetStrainRate.Click += new System.EventHandler(this.btnSetStrainRate_Click);
            // 
            // txtStrainIncrement
            // 
            this.txtStrainIncrement.Location = new System.Drawing.Point(11, 473);
            this.txtStrainIncrement.Multiline = true;
            this.txtStrainIncrement.Name = "txtStrainIncrement";
            this.txtStrainIncrement.Size = new System.Drawing.Size(190, 39);
            this.txtStrainIncrement.TabIndex = 33;
            this.txtStrainIncrement.TextChanged += new System.EventHandler(this.genericTextBoxEventHandler);
            // 
            // btnSetStrainIncrement
            // 
            this.btnSetStrainIncrement.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainIncrement.Location = new System.Drawing.Point(206, 473);
            this.btnSetStrainIncrement.Name = "btnSetStrainIncrement";
            this.btnSetStrainIncrement.Size = new System.Drawing.Size(238, 39);
            this.btnSetStrainIncrement.TabIndex = 34;
            this.btnSetStrainIncrement.Text = "Set Strain Increment [%]";
            this.btnSetStrainIncrement.UseVisualStyleBackColor = true;
            this.btnSetStrainIncrement.Click += new System.EventHandler(this.btnSetStrainIncrement_Click);
            // 
            // btnLockParams
            // 
            this.btnLockParams.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLockParams.Location = new System.Drawing.Point(450, 338);
            this.btnLockParams.Name = "btnLockParams";
            this.btnLockParams.Size = new System.Drawing.Size(123, 174);
            this.btnLockParams.TabIndex = 35;
            this.btnLockParams.Text = "Lock Params";
            this.btnLockParams.UseVisualStyleBackColor = true;
            this.btnLockParams.Click += new System.EventHandler(this.btnLockParams_Click);
            // 
            // btnReturntoZero
            // 
            this.btnReturntoZero.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnReturntoZero.Location = new System.Drawing.Point(11, 518);
            this.btnReturntoZero.Name = "btnReturntoZero";
            this.btnReturntoZero.Size = new System.Drawing.Size(282, 47);
            this.btnReturntoZero.TabIndex = 36;
            this.btnReturntoZero.Text = "RETURN TO 0";
            this.btnReturntoZero.UseVisualStyleBackColor = true;
            this.btnReturntoZero.Click += new System.EventHandler(this.btnReturntoZero_Click);
            // 
            // btnStretchtoMaxStrain
            // 
            this.btnStretchtoMaxStrain.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnStretchtoMaxStrain.Location = new System.Drawing.Point(299, 518);
            this.btnStretchtoMaxStrain.Name = "btnStretchtoMaxStrain";
            this.btnStretchtoMaxStrain.Size = new System.Drawing.Size(275, 47);
            this.btnStretchtoMaxStrain.TabIndex = 37;
            this.btnStretchtoMaxStrain.Text = "STRETCH TO X%";
            this.btnStretchtoMaxStrain.UseVisualStyleBackColor = true;
            this.btnStretchtoMaxStrain.Click += new System.EventHandler(this.btnStretchtoMaxStrain_Click);
            // 
            // txtStrainCycles
            // 
            this.txtStrainCycles.Location = new System.Drawing.Point(10, 600);
            this.txtStrainCycles.Multiline = true;
            this.txtStrainCycles.Name = "txtStrainCycles";
            this.txtStrainCycles.Size = new System.Drawing.Size(281, 37);
            this.txtStrainCycles.TabIndex = 38;
            this.txtStrainCycles.TextChanged += new System.EventHandler(this.txtStrainCycles_TextChanged);
            // 
            // btnSetStrainCycles
            // 
            this.btnSetStrainCycles.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetStrainCycles.Location = new System.Drawing.Point(297, 600);
            this.btnSetStrainCycles.Name = "btnSetStrainCycles";
            this.btnSetStrainCycles.Size = new System.Drawing.Size(276, 37);
            this.btnSetStrainCycles.TabIndex = 39;
            this.btnSetStrainCycles.Text = "Set Number of Strain Cycles";
            this.btnSetStrainCycles.UseVisualStyleBackColor = true;
            this.btnSetStrainCycles.Click += new System.EventHandler(this.btnSetStrainCycles_Click);
            // 
            // lblCyclicStretching
            // 
            this.lblCyclicStretching.AutoSize = true;
            this.lblCyclicStretching.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCyclicStretching.Location = new System.Drawing.Point(10, 568);
            this.lblCyclicStretching.Name = "lblCyclicStretching";
            this.lblCyclicStretching.Size = new System.Drawing.Size(279, 29);
            this.lblCyclicStretching.TabIndex = 40;
            this.lblCyclicStretching.Text = "CYCLIC STRETCHING";
            // 
            // btnCyclicStretching
            // 
            this.btnCyclicStretching.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCyclicStretching.Location = new System.Drawing.Point(10, 643);
            this.btnCyclicStretching.Name = "btnCyclicStretching";
            this.btnCyclicStretching.Size = new System.Drawing.Size(563, 60);
            this.btnCyclicStretching.TabIndex = 41;
            this.btnCyclicStretching.Text = "CYCLIC STRETCHING";
            this.btnCyclicStretching.UseVisualStyleBackColor = true;
            this.btnCyclicStretching.Click += new System.EventHandler(this.btnCyclicStretching_Click);
            // 
            // lblPositiveMove
            // 
            this.lblPositiveMove.AutoSize = true;
            this.lblPositiveMove.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPositiveMove.Location = new System.Drawing.Point(531, 261);
            this.lblPositiveMove.Name = "lblPositiveMove";
            this.lblPositiveMove.Size = new System.Drawing.Size(44, 46);
            this.lblPositiveMove.TabIndex = 42;
            this.lblPositiveMove.Text = "+";
            // 
            // lblNegativeMove
            // 
            this.lblNegativeMove.AutoSize = true;
            this.lblNegativeMove.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblNegativeMove.Location = new System.Drawing.Point(173, 261);
            this.lblNegativeMove.Name = "lblNegativeMove";
            this.lblNegativeMove.Size = new System.Drawing.Size(34, 46);
            this.lblNegativeMove.TabIndex = 43;
            this.lblNegativeMove.Text = "-";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(587, 1054);
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
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.tbManualMove)).EndInit();
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
    }
}

