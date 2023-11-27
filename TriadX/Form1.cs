


using System.IO.Ports;
using System.Windows.Forms;

namespace TriadX
{
    public partial class Form1 : Form
    {

        //////////////////////////////////////////////////////////////// VARIABLES //////////////////////////////////////////////////////////////////

        ClSerial SerialHnd;
        FormComSettings FormComSettingsHnd;
        private SerialPort ComPort;
        private string RxBuffer;


        //////////////////////////////////////////////////////////////// METHODS //////////////////////////////////////////////////////////////////

        public Form1()
        {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            FormInit();      
        }


        private void FormInit()
        {
            SerialHnd = new ClSerial();
            ComPort = new SerialPort();
            ComPort.DataReceived += new SerialDataReceivedEventHandler(SerialDataReceivedHandler);
            SerialHnd.InitPort(ComPort);
        }


        private void FormDeInit()
        {
            if (ComPort.IsOpen)
            {
                SerialHnd.ClosePort();
            }
        }


        

        private void SerialDataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            if (ConsoleBox.InvokeRequired)
            {
                Invoke(new myDelegate(UpdateGUI));
            }
        }

        public delegate void myDelegate();

        private void UpdateGUI()
        {
            int index, numbytes;
            string text = ComPort.ReadExisting();
            numbytes = text.Length;

            // Preprocess data to enqueue
            if (text.Contains("\r"))
            {
                // Carriage return received
                index = text.IndexOf('\r');
                RxBuffer += text.Substring(0, index);

                if (text.Length == index + 1)
                {    
                    // UPDATE CHART HERE!!!!!!

                    ConsoleBox.AppendText(RxBuffer + '\r');
                    // Carriage return was last character. Reset buffer
                    RxBuffer = "";
                }
                else if (text.Length > index + 1)
                {
                    // UPDATE CHART HERE!!!!!!

                    ConsoleBox.AppendText(RxBuffer + '\r');
                    // More data after carriage return. Append buffer with leftovers
                    RxBuffer = text.Substring(index + 1, text.Length - index - 1);
                }
            }
            else
            {
                // No carriage return received. Append buffer
                RxBuffer += text;
            }
        } 



        ///////////////////////////////////////////////////// MAIN MENU CLICK EVENTS ///////////////////////////////////////////////////////////////


        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormDeInit();
            Application.Exit();
        }


        private void cOMPortToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormComSettingsHnd = new FormComSettings();
            FormComSettingsHnd.Show();
            FormComSettingsHnd.PopulateSettings();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RxBuffer = "";
            SerialHnd.OpenPort();
            if (ComPort.IsOpen)
            {
                openToolStripMenuItem.Enabled = false;
                closeToolStripMenuItem.Enabled = true;
                statusStrip1.Items[0].Text = "Connected: " + ComPort.PortName;             
            }  
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ComPort.IsOpen)
            {
                SerialHnd.ClosePort();
                if (!ComPort.IsOpen)
                {
                    openToolStripMenuItem.Enabled = true;
                    closeToolStripMenuItem.Enabled = false;
                    statusStrip1.Items[0].Text = "Disconnected";
                }         
            }     
        }
    }
}