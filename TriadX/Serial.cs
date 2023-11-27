using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;
using System.ComponentModel;

namespace TriadX
{

    internal class ClSerial
    {
        private SerialPort ComPort;
        
        

        public ClSerial()
        {
            
        }


        public void InitPort(SerialPort PortHnd)
        {
            ComPort = PortHnd;
            ComPort.PortName = "COM5";
            ComPort.BaudRate = 9600;
            ComPort.DataBits = 8;
            ComPort.Parity = Parity.None;
            ComPort.StopBits = StopBits.One;     
            ComPort.Handshake = Handshake.None;
        }


       


        public void OpenPort()
        {
            try
            {
                ComPort.Open();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }


        public void ClosePort()
        {
            try
            {
                ComPort.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }    
        }

    }


}
