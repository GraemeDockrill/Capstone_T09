﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CFP_control_program
{
    public partial class Form1 : Form
    {

        public int numberOfDataPoints = 0;
        public bool sendData = false;
        public int startByte = 255;
        public int cmdByte0 = 0;                    // 0 = WS CW, 1 = WS CCW, 2 = HS CW, 3 = HS CCW, 4 = WS CW Cont., 5 = WS CCW Cont., 6 = HS CW Cont., 7 = HW CCW Cont. for STEPPER MOTOR
        public int dataByte0 = 0;
        public int dataByte1 = 0;
        public int ESCByte = 0;

        public int dataInt = 0;

        // define bit constants
        public int BIT0 = 0x0001;
        public int BIT1 = 0x0002;
        public int BIT2 = 0x0004;
        public int BIT3 = 0x0008;

        public Form1()
        {
            InitializeComponent();
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
                int currentByte = serialPort1.ReadByte();
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
    }
}
