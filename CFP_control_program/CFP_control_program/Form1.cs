using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace CFP_control_program
{
    public partial class Form1 : Form
    {
        // constants
        public double stepPerRev = 200;
        public double leadPerRev = 2;
        public double pulsePerStep = 2;
        public double pulsePerSec = 2000000;
        public int maxManualStepperSpeed = 5;       // [mm/s]

        // communication bytes
        public int numberOfDataPoints = 0;
        public bool sendData = false;
        public int startByte = 255;
        public int cmdByte0 = 0;                    // 0 = zero pos, 1 = move +ve dir, 2 = movoe -ve dir, 3 = set membrane size, 4 = set strain target, 5 = set strain rate, 6 = set number of strain cycles, 7 = set strain increment, 8 = stretch to max strain, 9 = return to zero position, 10 = cyclic stretching protocol, 11 = STOP STEPPER
        public int dataByte0 = 0;
        public int dataByte1 = 0;
        public int ESCByte = 0;

        // load cell bytes
        public byte currentStepByte0;
        public byte currentStepByte1;
        public byte loadCellByte0;
        public byte loadCellByte1;
        public byte loadCellByte2;
        public byte loadCellByte3;
        public byte loadCellESCByte;

        // enabling/disabling parameter editing
        public bool parameterStatus = true;

        // data variables
        public int dataInt = 0;
        public float loadCellFloat = 0;
        public double manualSpeed;
        public int membraneSize_mm;
        public int membraneSize_steps;
        public int strainTarget_percent;
        public int strainTarget_steps;
        public int strainRate_percentPerSec;
        public double strainRate_mmPerSec;
        public int strainIncrement_percent;
        public int strainIncrement_steps;
        public int strainCycles;
        public int currentPosSteps;
        public double timeSinceDataReceived = 0;

        // define bit constants
        public int BIT0 = 0x0001;
        public int BIT1 = 0x0002;
        public int BIT2 = 0x0004;
        public int BIT3 = 0x0008;

        public enum loadCellByte
        {
            startByte,
            currentStepByte0,
            currentStepByte1,
            loadCellByte0,
            loadCellByte1,
            loadCellByte2,
            loadCellByte3,
            loadCellESCByte
        }

        public loadCellByte parsingState = loadCellByte.startByte;

        public int maxXValueLoadCellForce;
        public int maxXValueCurrentStepPosition;

        private Series seriesLoadCellForce, seriesCurrentStepPosition;

        public Form1()
        {
            InitializeComponent();
            InitializeLoadCellForceChart();
            InitializeCurrentStepPositionChart();
            cbComResponse.Checked = true;
            chkByte1.Checked = false;
            chkByte2.Checked = false;
            chkByte3.Checked = false;
            chkByte4.Checked = false;
            chkByte5.Checked = false;
            txtByte1.Enabled = false;
            txtByte2.Enabled = false;
            txtByte3.Enabled = false;
            txtByte4.Enabled = false;
            txtByte5.Enabled = false;
            ComPortUpdate();
        }

        // function for creating position chart
        public void InitializeLoadCellForceChart()
        {
            ChartArea chartLoadCellForce = new ChartArea();
            chartLoadCellForce.AxisX.Title = "Time [s]";
            chartLoadCellForce.AxisY.Title = "Load Cell Force [kg]";
            this.chartLoadCellForce.ChartAreas.Add(chartLoadCellForce);

            seriesLoadCellForce = new Series();
            seriesLoadCellForce.ChartType = SeriesChartType.Line;

            this.chartLoadCellForce.Series.Add(seriesLoadCellForce);

            this.chartLoadCellForce.ChartAreas[0].AxisX.Interval = 5;      // set X-axis interval to 10
        }

        // function for creating velocity chart
        public void InitializeCurrentStepPositionChart()
        {
            ChartArea chartAreaDCVelocity = new ChartArea();
            chartAreaDCVelocity.AxisX.Title = "Time [s]";
            chartAreaDCVelocity.AxisY.Title = "Current Step Position [steps]";
            this.chartCurrentStepPosition.ChartAreas.Add(chartAreaDCVelocity);

            seriesCurrentStepPosition = new Series();
            seriesCurrentStepPosition.ChartType = SeriesChartType.Line;

            this.chartCurrentStepPosition.Series.Add(seriesCurrentStepPosition);

            this.chartCurrentStepPosition.ChartAreas[0].AxisX.Interval = 5;      // set X-axis interval to 10
        }

        private void cmbComPorts_DropDown(object sender, EventArgs e)
        {
            ComPortUpdate();
        }

        public void ComPortUpdate()
        {
            cmbComPorts.Items.Clear();
            string[] comPortArray = System.IO.Ports.SerialPort.GetPortNames();
            Array.Reverse(comPortArray);
            cmbComPorts.Items.AddRange(comPortArray);
            if (cmbComPorts.Items.Count != 0)
                cmbComPorts.SelectedIndex = 0;
            else
                cmbComPorts.Text = "No Ports Found!";
        }

        private void btnComConnect_Click(object sender, EventArgs e)
        {
            if (btnComConnect.Text == "Connect")
            {
                if (cmbComPorts.Items.Count > 0)
                {
                    try
                    {
                        serialPort1.BaudRate = Convert.ToInt16(txtBaudRate.Text);
                        serialPort1.PortName = cmbComPorts.SelectedItem.ToString();
                        serialPort1.Open();
                        btnComConnect.Text = "Disconnect";
                        timer1.Enabled = true;
                        lblDataRate.Visible = true;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }
            }
            else
            {
                try
                {
                    serialPort1.Close();
                    btnComConnect.Text = "Connect";
                    timer1.Enabled = false;
                    lblDataRate.Visible = false;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (serialPort1.IsOpen && serialPort1.BytesToRead != 0)
            {
                int newByte = serialPort1.ReadByte();
                byte currentByte = (byte) newByte;
                numberOfDataPoints++;
                if (cbComResponse.Checked)
                {
                    this.BeginInvoke(new EventHandler(delegate
                    {
                        if (cbAlphNumOutput.Checked)
                            txtComOutput.AppendText(((char)currentByte).ToString());
                        else
                            txtComOutput.AppendText(currentByte.ToString() + ", ");
                    }));
                }

                if (currentByte == 255)
                    parsingState = loadCellByte.startByte;

                // figuring out which state to read data from
                if (parsingState == loadCellByte.startByte)                 // start byte
                {
                    parsingState = loadCellByte.currentStepByte0;
                    timeSinceDataReceived += 0.016;
                }
                else if (parsingState == loadCellByte.currentStepByte0)
                {
                    parsingState = loadCellByte.currentStepByte1;
                    currentStepByte0 = currentByte;
                }
                else if (parsingState == loadCellByte.currentStepByte1)
                {
                    parsingState = loadCellByte.loadCellByte0;
                    currentStepByte1 = currentByte;
                }
                else if (parsingState == loadCellByte.loadCellByte0)        // loadCellByte0
                {
                    parsingState = loadCellByte.loadCellByte1;
                    loadCellByte0 = currentByte;
                }
                else if (parsingState == loadCellByte.loadCellByte1)        // loadCellByte1
                {
                    parsingState = loadCellByte.loadCellByte2;
                    loadCellByte1 = currentByte;
                }
                else if (parsingState == loadCellByte.loadCellByte2)        // loadCellByte2
                {
                    parsingState = loadCellByte.loadCellByte3;
                    loadCellByte2 = currentByte;
                }
                else if (parsingState == loadCellByte.loadCellByte3)        // loadCellByte3
                {
                    parsingState = loadCellByte.loadCellESCByte;
                    loadCellByte3 = currentByte;
                }
                else if (parsingState == loadCellByte.loadCellESCByte)      // loadCellESCByte
                {
                    parsingState = loadCellByte.startByte;

                    if (IsBitSet(loadCellESCByte, 5))
                        currentStepByte0 = 255;
                    if (IsBitSet(loadCellESCByte, 4))
                        currentStepByte1 = 255;
                    if (IsBitSet(loadCellESCByte, 3))
                        loadCellByte0 = 255;
                    if (IsBitSet(loadCellESCByte, 2))
                        loadCellByte1 = 255;
                    if (IsBitSet(loadCellESCByte, 1))
                        loadCellByte2 = 255;
                    if (IsBitSet(loadCellESCByte, 0))
                        loadCellByte3 = 255;

                    currentPosSteps = (currentStepByte0 << 8) + currentStepByte1;

                    // combining bytes into float
                    byte[] array = { loadCellByte0, loadCellByte1, loadCellByte2, loadCellByte3 };

                    loadCellFloat = BitConverter.ToSingle(array, 0);

                    //// add float to txtComOutput
                    //Invoke((MethodInvoker)delegate
                    //{
                    //    txtComOutput.AppendText(loadCellFloat.ToString());
                    //});



                    // plot the load cell force graph
                    chartLoadCellForce.Invoke((MethodInvoker)delegate
                    {
                        // plot new data
                        seriesLoadCellForce.Points.AddXY(timeSinceDataReceived, loadCellFloat);

                        // scroll the data window
                        int minXValue = ((int)timeSinceDataReceived - 50 > 0) ? (int)timeSinceDataReceived - 50 - (int)timeSinceDataReceived % 5 : 0;
                        maxXValueLoadCellForce = ((int)timeSinceDataReceived + 5) % 5 != 0 ? maxXValueLoadCellForce : (int)timeSinceDataReceived + 5;
                        chartLoadCellForce.ChartAreas[0].AxisX.Minimum = minXValue;
                        chartLoadCellForce.ChartAreas[0].AxisX.Maximum = maxXValueLoadCellForce;

                        // redraw graph
                        chartLoadCellForce.Invalidate();
                    });

                    // plot the current step position graph
                    chartCurrentStepPosition.Invoke((MethodInvoker)delegate
                    {
                        // plot new data
                        seriesCurrentStepPosition.Points.AddXY(timeSinceDataReceived, currentPosSteps);

                        // scroll the data window
                        int minXValue = ((int)timeSinceDataReceived - 50 > 0) ? (int)timeSinceDataReceived - 50 - (int)timeSinceDataReceived % 5 : 0;
                        maxXValueCurrentStepPosition = ((int)timeSinceDataReceived + 5) % 5 != 0 ? maxXValueCurrentStepPosition : (int)timeSinceDataReceived + 5;
                        chartCurrentStepPosition.ChartAreas[0].AxisX.Minimum = minXValue;
                        chartCurrentStepPosition.ChartAreas[0].AxisX.Maximum = maxXValueCurrentStepPosition;

                        // redraw graph
                        chartCurrentStepPosition.Invalidate();
                    });



                }

            }
            }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lblDataRate.Text = "Incoming data rate = " + numberOfDataPoints.ToString() + " bytes per second";

            // sending UART data to MCU
            if (sendData)
            {
                try
                {
                    if (serialPort1.IsOpen)
                    {
                        // split the duty cycle into 2 bytes
                        dataByte0 = dataInt >> 8;
                        dataByte0 &= 255;
                        dataByte1 = dataInt;
                        dataByte1 &= 255;

                        // check if data bytes are greater or equal to 255, then set escape byte
                        if (dataByte0 >= 255)
                        {
                            dataByte0 = 0;
                            ESCByte |= BIT1;
                        }
                        if (dataByte1 >= 255)
                        {
                            dataByte1 = 0;
                            ESCByte |= BIT0;
                        }

                        // sending data packet over UART
                        byte[] TxBytes = new Byte[5];
                        TxBytes[0] = Convert.ToByte(startByte);             // start byte
                        serialPort1.Write(TxBytes, 0, 1);
                        TxBytes[1] = Convert.ToByte(cmdByte0);              // command byte
                        serialPort1.Write(TxBytes, 1, 1);
                        TxBytes[2] = Convert.ToByte(dataByte0);             // data byte 0
                        serialPort1.Write(TxBytes, 2, 1);
                        TxBytes[3] = Convert.ToByte(dataByte1);             // data byte 1
                        serialPort1.Write(TxBytes, 3, 1);
                        TxBytes[4] = Convert.ToByte(ESCByte);               // escape byte
                        serialPort1.Write(TxBytes, 4, 1);

                        dataInt = 0;                                        // reset dataInt
                        ESCByte = 0;                                        // reset escape byte
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                sendData = false;                                           // after sending message, disable sending messages
            }

            // Auto reconnect functionality
            if (cbAutoReconnect.Checked == true && numberOfDataPoints == 0 && serialPort1.IsOpen == true)
            {
                try
                {
                    serialPort1.Close();
                    serialPort1.BaudRate = Convert.ToInt16(txtBaudRate.Text);
                    serialPort1.PortName = cmbComPorts.SelectedItem.ToString();
                    serialPort1.Open();
                }
                catch (Exception ex)
                {
                    //MessageBox.Show(ex.Message);
                }
            }

            numberOfDataPoints = 0;
        }

        private void txtKeyboardInput_TextChanged(object sender, EventArgs e)
        {
            if (txtKeyboardInput.Text != "") {
                byte[] typedCharacter = new byte[1];
                typedCharacter[0] = Convert.ToByte(txtKeyboardInput.Text.First());
                txtKeyboardInput.Text = "";

                if (serialPort1.IsOpen)
                    serialPort1.Write(typedCharacter, 0, 1);
            }
        }

        private void chkByte1_CheckedChanged(object sender, EventArgs e)
        {
            if (chkByte1.Checked == true)
                txtByte1.Enabled = true;
            else
            {
                txtByte1.Clear();
                txtByte1.Enabled = false;
            }
        }

        private void chkByte2_CheckedChanged(object sender, EventArgs e)
        {
            if (chkByte2.Checked == true)
                txtByte2.Enabled = true;
            else
            {
                txtByte2.Clear();
                txtByte2.Enabled = false;
            }
        }

        private void chkByte3_CheckedChanged(object sender, EventArgs e)
        {
            if (chkByte3.Checked == true)
                txtByte3.Enabled = true;
            else
            {
                txtByte3.Clear();
                txtByte3.Enabled = false;
            }
        }

        private void chkByte4_CheckedChanged(object sender, EventArgs e)
        {
            if (chkByte4.Checked == true)
                txtByte4.Enabled = true;
            else
            {
                txtByte4.Clear();
                txtByte4.Enabled = false;
            }
        }

        private void chkByte5_CheckedChanged(object sender, EventArgs e)
        {
            if (chkByte5.Checked == true)
                txtByte5.Enabled = true;
            else
            {
                txtByte5.Clear();
                txtByte5.Enabled = false;
            }
        }

        private void genericTextBoxEventHandler(object sender, EventArgs e)
        {
            TextBox currentTextBox = sender as TextBox;
            short parseResult;
            if (Int16.TryParse((currentTextBox.Text), out parseResult))
            {
                if (parseResult > 255)
                    parseResult = 255;
                if (parseResult <= 0)
                    parseResult = 0;
                currentTextBox.Text = parseResult.ToString();
            }
            else
                currentTextBox.Text = "";
        }

        private void btnTransmitToComPort_Click(object sender, EventArgs e)
        {
            byte[] TxBytes = new Byte[5];

            try
            {
                if (serialPort1.IsOpen)
                {
                    if (chkByte1.Checked && (txtByte1.Text != ""))
                    {
                        TxBytes[0] = Convert.ToByte(txtByte1.Text);
                        serialPort1.Write(TxBytes, 0, 1);
                    }
                    if (chkByte2.Checked && (txtByte2.Text != ""))
                    {
                        TxBytes[1] = Convert.ToByte(txtByte2.Text);
                        serialPort1.Write(TxBytes, 1, 1);
                    }
                    if (chkByte3.Checked && (txtByte3.Text != ""))
                    {
                        TxBytes[2] = Convert.ToByte(txtByte3.Text);
                        serialPort1.Write(TxBytes, 2, 1);
                    }
                    if (chkByte4.Checked && (txtByte4.Text != ""))
                    {
                        TxBytes[3] = Convert.ToByte(txtByte4.Text);
                        serialPort1.Write(TxBytes, 3, 1);
                    }
                    if (chkByte5.Checked && (txtByte5.Text != ""))
                    {
                        TxBytes[4] = Convert.ToByte(txtByte5.Text);
                        serialPort1.Write(TxBytes, 4, 1);
                    }
                }
            }
            catch (Exception Ex)
            {
                MessageBox.Show(Ex.Message);
            }
        }

        private void btnZeroPosition_Click(object sender, EventArgs e)
        {
            cmdByte0 = 0;
            sendData = true;
        }

        private void tbManualMove_ValueChanged(object sender, EventArgs e)
        {
            if (tbManualMove.Value > tbManualMove.Maximum / 2)      // positive direction
            {
                cmdByte0 = 1;
                manualSpeed  = (maxManualStepperSpeed / (tbManualMove.Maximum - tbManualMove.Maximum/2)) * tbManualMove.Value - maxManualStepperSpeed;
                dataInt = Convert.ToInt16(1 / (manualSpeed / leadPerRev * stepPerRev * pulsePerStep) * pulsePerSec);
            }
            else if (tbManualMove.Value < tbManualMove.Maximum / 2) // negative direction
            {
                cmdByte0 = 2;
                manualSpeed = (maxManualStepperSpeed / (tbManualMove.Minimum - tbManualMove.Maximum/2)) * tbManualMove.Value + maxManualStepperSpeed;
                dataInt = Convert.ToInt16(1 / (manualSpeed / leadPerRev * stepPerRev * pulsePerStep) * pulsePerSec);
            }
            else if (tbManualMove.Value == tbManualMove.Maximum / 2)
                cmdByte0 = 11;

            sendData = true;
        }

        private void btnSetMembraneSize_Click(object sender, EventArgs e)
        {
            if(txtMembraneSize.Text != "")
            {
                cmdByte0 = 3;
                membraneSize_mm = Int16.Parse(txtMembraneSize.Text);
                membraneSize_steps = Convert.ToInt16(membraneSize_mm * stepPerRev / leadPerRev);
                dataInt = membraneSize_steps;
                sendData = true;
            }
        }

        private void btnSetStrainTarget_Click(object sender, EventArgs e)
        {
            if(txtStrainTarget.Text != "")
            {
                cmdByte0 = 4;
                strainTarget_percent = Int16.Parse(txtStrainTarget.Text);
                strainTarget_steps = membraneSize_steps * strainTarget_percent / 100;
                dataInt = strainTarget_steps;
                sendData = true;
            }
        }

        private void btnSetStrainRate_Click(object sender, EventArgs e)
        {
            if(txtStrainRate.Text != "")
            {
                cmdByte0 = 5;
                strainRate_percentPerSec = Int16.Parse(txtStrainRate.Text);
                strainRate_mmPerSec = membraneSize_mm * strainRate_percentPerSec / 100;
                dataInt = Convert.ToInt16(1 / (strainRate_mmPerSec / leadPerRev * stepPerRev * pulsePerStep) * pulsePerSec);
                sendData = true;
            }
        }

        private void btnSetStrainCycles_Click(object sender, EventArgs e)
        {
            if(txtStrainCycles.Text != "")
            {
                cmdByte0 = 6;
                strainCycles = Int16.Parse(txtStrainCycles.Text);
                dataInt = strainCycles;
                sendData = true;
            }
        }

        private void btnSetStrainIncrement_Click(object sender, EventArgs e)
        {
            if(txtStrainIncrement.Text != "")
            {
                cmdByte0 = 7;
                strainIncrement_percent = Int16.Parse(txtStrainIncrement.Text);
                sendData = true;
            }
        }

        private void btnStretchtoMaxStrain_Click(object sender, EventArgs e)
        {
            cmdByte0 = 8;
            sendData = true;
        }

        private void btnReturntoZero_Click(object sender, EventArgs e)
        {
            cmdByte0 = 9;
            sendData = true;
        }

        private void btnCyclicStretching_Click(object sender, EventArgs e)
        {
            cmdByte0 = 10;
            sendData = true;
        }

        private void btnSTOP_Click(object sender, EventArgs e)
        {
            cmdByte0 = 11;
            tbManualMove.Value = tbManualMove.Maximum / 2;
            sendData = true;
        }

        private void btnLockParams_Click(object sender, EventArgs e)
        {
            if (parameterStatus)
            {
                parameterStatus = false;
                btnLockParams.Text = "Unlock Params";
                txtMembraneSize.Enabled = false;
                btnSetMembraneSize.Enabled = false;
                txtStrainTarget.Enabled = false;
                btnSetStrainTarget.Enabled = false;
                txtStrainRate.Enabled = false;
                btnSetStrainRate.Enabled = false;
                txtStrainIncrement.Enabled = false;
                btnSetStrainIncrement.Enabled = false;
            }
            else
            {
                parameterStatus = true;
                btnLockParams.Text = "Lock Params";
                txtMembraneSize.Enabled = true;
                btnSetMembraneSize.Enabled = true;
                txtStrainTarget.Enabled = true;
                btnSetStrainTarget.Enabled = true;
                txtStrainRate.Enabled = true;
                btnSetStrainRate.Enabled = true;
                txtStrainIncrement.Enabled = true;
                btnSetStrainIncrement.Enabled = true;
            }
        }

        private void txtStrainCycles_TextChanged(object sender, EventArgs e)
        {
            TextBox currentTextBox = sender as TextBox;
            short parseResult;
            if (Int16.TryParse((currentTextBox.Text), out parseResult))
            {
                if (parseResult <= 0)
                    parseResult = 0;
                currentTextBox.Text = parseResult.ToString();
            }
            else
                currentTextBox.Text = "";
        }

        bool IsBitSet(byte b, int pos)
        {
            return ((b >> pos) & 1) != 0;
        }
    }
}
