// Giao tiếp qua Serial
using System;
using System.IO;
using System.IO.Ports;


using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Text;

// Sử dụng thư viện ZedGraph đễ vẽ đò thị
using ZedGraph;

namespace WindowsFormsApplication1 // khai báo biến 
{
    public partial class Form1 : Form     
    {
        int Time = 0;
        private bool isClicked = false;

        public string datafromcom;
        public float data;
        public string mode;
        public double Scale_x_max = 50;
        public double Scale_x_min = 0;
        public double Scale_y_max = 6;
        public double Scale_y_min = 0;
       
        int set = 0;
      
        public bool chanel1 = false;
        public bool chanel2 = false;
        public bool chanel3 = false;
        public int chanel = 0;
        public bool bd = false;



        public Form1()
        {
            InitializeComponent();

        }

        private void txtkq_TextChanged(object sender, EventArgs e)
        {


        }


        private void label1_Click(object sender, EventArgs e)
        {

        }



        private void Button3_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e) // Conect tới Serial port
        {
            if (serialPort1.IsOpen)
            { }
            else
            {
                try
                {
                    serialPort1.PortName = comboBox1.Text; // Danh sách port
                    serialPort1.Open(); // Mở cổng port
                    Status.Text = "Conected";
                    button1.Enabled = false;
                    button2.Enabled = true;

                }
                catch (Exception)
                { MessageBox.Show("Moi ban chon cong COM"); }
            }
        }

        Boolean clicked = false;
        private void button2_Click(object sender, EventArgs e) // Disconect Port
        {
            if (serialPort1.IsOpen)
            {
                {
                    serialPort1.Close();
                    Status.Text = "Disconected";
                    //clicked = false;
                    button2.Enabled = false;
                    button1.Enabled = true;
                    isClicked = false;
                }
            }
            else { } ;
        }

        private void Status_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }
        private async Task asyncTask()
        {
            Task delay = Task.Delay(50);
            await delay;
        }

        private async void button3_Click(object sender, EventArgs e) // Nút Start
        {

            if (Status.Text == "Conected")
            {
                
                isClicked = true; 
            }
            else isClicked = false;


        }


        private void label1_Click_1(object sender, EventArgs e)
        {

        }


        // Khởi tạo giao diện lúc load chưởng trình
        private void Form1_Load(object sender, EventArgs e)  
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            // Title
            myPane.Title.Text = "ADC";  
            myPane.XAxis.Title.Text = "TIME(S)"; 
            myPane.YAxis.Title.Text = "VOLT(V)";

            // Tạo số điểm của các list dữ liệu
            RollingPointPairList list1 = new RollingPointPairList(120000);
            RollingPointPairList list2 = new RollingPointPairList(120000);
            RollingPointPairList list3 = new RollingPointPairList(120000);

            // Tạo các đường vẽ dữ liệu
            LineItem curve1 = myPane.AddCurve("Chanel_1", list1, System.Drawing.Color.Red, SymbolType.None);
            LineItem curve2 = myPane.AddCurve("Chanel_2", list2, System.Drawing.Color.Blue, SymbolType.None);
            LineItem curve3 = myPane.AddCurve("Chanel_3", list3, System.Drawing.Color.Yellow, SymbolType.None);

            // Cho phép vẽ grid 
            myPane.XAxis.MinorGrid.IsVisible = true;
            myPane.XAxis.MajorGrid.IsVisible = true;
            myPane.YAxis.MinorGrid.IsVisible = true;
            myPane.YAxis.MajorGrid.IsVisible = true;

            // Scale_X,Scale_Y
            myPane.XAxis.Scale.Min = Scale_x_min;
            myPane.XAxis.Scale.Max = Scale_x_max;
            myPane.XAxis.Scale.MinorStep = 5;
            myPane.XAxis.Scale.MajorStep = 10;

            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max;
            myPane.YAxis.Scale.MinorStep = 1;
            myPane.YAxis.Scale.MajorStep = 2;

            // Tắt các đường vẽ
            ClearZedGraph(0, false);
            ClearZedGraph(1, false);
            ClearZedGraph(2, false);

            zedGraphControl1.AxisChange();

            // Lấy thời gian của hệ thống
            Time = Environment.TickCount;


        }

        // Hàm vẽ đồ thị
        public void draw(double setpoint1, double setpoint2, double setpoint3, bool chanel1, bool chanel2, bool chanel3, bool bd)

        {

            if (zedGraphControl1.GraphPane.CurveList.Count <= 0)
            { return; }

            LineItem curve1 = zedGraphControl1.GraphPane.CurveList[0] as LineItem;
            LineItem curve2 = zedGraphControl1.GraphPane.CurveList[1] as LineItem;
            LineItem curve3 = zedGraphControl1.GraphPane.CurveList[2] as LineItem;


            if (curve1 == null) return; if (curve2 == null) return; if (curve3 == null) return;
            IPointListEdit list1 = curve1.Points as IPointListEdit;
            IPointListEdit list2 = curve2.Points as IPointListEdit;
            IPointListEdit list3 = curve3.Points as IPointListEdit;
            if (list1 == null) return;
            if (list2 == null) return;
            if (list3 == null) return;

            double time = (Environment.TickCount-Time) / 1000.0;
            Scale xScale = zedGraphControl1.GraphPane.XAxis.Scale;
            Scale yScale = zedGraphControl1.GraphPane.YAxis.Scale;

            { list1.Add(time, setpoint1); }

            { list2.Add(time, setpoint2); }

            { list3.Add(time, setpoint3); }

            // Tự động Scale theo trục x
            if (time > xScale.Max - xScale.MajorStep && bd==true)
             {
                 xScale.Max = time + xScale.MinorStep;
                 xScale.Min = xScale.Max -10;
                 // xScale.Max = time + xScale.MajorStep;
                 //xScale.Min = 0;

             }

            zedGraphControl1.AxisChange();

            zedGraphControl1.Invalidate();
        }

        // Hàm tắt chanel vẽ
        private void ClearZedGraph(int chanel, bool confirm)
        {
            //zedGraphControl1.GraphPane.CurveList[chanel].Clear(); // Xóa đường
            //zedGraphControl1.GraphPane.GraphObjList.Clear(); // Xóa đối tượng
            if (confirm == false)
                zedGraphControl1.GraphPane.CurveList[chanel].IsVisible = false;
            else
                zedGraphControl1.GraphPane.CurveList[chanel].IsVisible = true;

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }




        int i;

        // Timer đọc dữ liệu từ serial port
        private void timer1_Tick(object sender, EventArgs e)
        {

           {
                if(isClicked == true  && Status.Text == "Conected")
                
                draw(Volt1, Volt2, Volt3, chanel1, chanel2, chanel3,bd);
            }


        }


        float g = 0;


        float Volt1;
        float Volt2;
        float Volt3;
        float stime;


        // hàm ngắt nhận và xử lý dữ liệu
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                String[] data_Recive = serialPort1.ReadLine().Split('|');
                string temp = serialPort1.ReadLine();
                showdata.Text = temp;

                {
                    float.TryParse(data_Recive[3], out Volt1);
                    Volt1 = Volt1 / 100;
                    float.TryParse(data_Recive[2], out Volt2);
                    Volt2 = Volt2 / 100;
                    float.TryParse(data_Recive[1], out Volt3);
                    Volt3 = Volt3 / 100;
                    float.TryParse(data_Recive[4], out stime);


                    // Hiển thị dữ liệu
                    Display(Volt1.ToString());
                    Display1(Volt2.ToString());
                    Display2(Volt3.ToString());
                    Display3(stime.ToString());
                }
            }
            catch (Exception)
            {

                return;
            }


        }




        // Nút Stop
        private void button4_Click(object sender, EventArgs e)
        {

            if(Status.Text == "Conected" && isClicked == true)

            isClicked = false;

            else { };

            // lưu gia tri
            /*StreamWriter sw = new StreamWriter("C:\\Users\\Minh Phuc\\Desktop\\save.txt");
           
            sw.WriteLine(Speed.ToString());
            sw.Close();*/
        }




        // Time/div :x50
        private void Save_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 5000 ;
            myPane.XAxis.Scale.MinorStep = 50;
            myPane.XAxis.Scale.MajorStep = 100;

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();

        }
        int a = 0;
        int n;
        int k = 0;

        // Timer cập nhật port Serial
        private void timer2_Tick(object sender, EventArgs e)
        {


            String[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            if (a != ports.Length)
            {
                a = ports.Length;
                comboBox1.Items.Clear();
                for (int j = 0; j < a; j++)
                {
                    comboBox1.Items.Add(ports[j]);
                }

            }

        }

        // Hàm hiển thị Chanel 1
        private void button4_Click_1(object sender, EventArgs e)
        {

        }
        private delegate void DlDisplay(string chl_1);
        private void Display(string chl_1)
        {
            if (C1.InvokeRequired)
            {
                DlDisplay sd = new DlDisplay(Display);
                C1.Invoke(sd, new object[] { chl_1 });

            }
            else
            {
                C1.Text = chl_1;

                //System.Diagnostics.Debug.WriteLine("Recive");
            }
        }

        // Hàm hiển thị Chanel 2
        private delegate void DlDisplay1(string chl_2);
        private void Display1(string chl_2)
        {
            if (C2.InvokeRequired)
            {
                DlDisplay sd = new DlDisplay(Display1);
                C2.Invoke(sd, new object[] { chl_2 });

            }
            else
            {
                C2.Text = chl_2;

                //System.Diagnostics.Debug.WriteLine("Recive");
            }
        }

        // Hàm hiển thị chanel 3
        private delegate void DlDisplay2(string chl_3);
        private void Display2(string chl_3)
        {
            if (C3.InvokeRequired)
            {
                DlDisplay sd = new DlDisplay(Display2);
                C3.Invoke(sd, new object[] { chl_3 });

            }
            else
            {
                C3.Text = chl_3;

                //System.Diagnostics.Debug.WriteLine("Recive");
            }
        }

        // Hàm hiển thị Tms
        private delegate void DlDisplay3(string chl_4);
        private void Display3(string chl_4)
        {
            if (time123.InvokeRequired)
            {
                DlDisplay sd = new DlDisplay(Display3);
                time123.Invoke(sd, new object[] { chl_4 });

            }
            else
            {
                time123.Text = chl_4;

                //System.Diagnostics.Debug.WriteLine("Recive");
            }
        }

        private void C1_TextChanged(object sender, EventArgs e)
        {

            C1.SelectionStart = C1.TextLength;
            C1.ScrollToCaret();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click_2(object sender, EventArgs e)
        {

        }

        private void label3_Click_1(object sender, EventArgs e)
        {

        }


        // Time/div :x10
        private void button4_Click_2(object sender, EventArgs e)
        {
           
            GraphPane myPane = zedGraphControl1.GraphPane;

            myPane.XAxis.Scale.Min =0;
            myPane.XAxis.Scale.Max=1000 ;
            myPane.XAxis.Scale.MinorStep = 10;
            myPane.XAxis.Scale.MajorStep = 20;


            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();





        }

        private void txt_Kd_TextChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void zedGraphControl1_Load(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void txt_T_mau_TextChanged(object sender, EventArgs e)
        { }



        // Time/div :x100
        private void button5_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max =10000;
            myPane.XAxis.Scale.MinorStep = 100;
            myPane.XAxis.Scale.MajorStep = 200;

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();


        }


        // Time/div :x0.01
        private void button6_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
           
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 10;
            myPane.XAxis.Scale.MinorStep = 0.1;
            myPane.XAxis.Scale.MajorStep = 0.2;

                zedGraphControl1.AxisChange();
                zedGraphControl1.Invalidate();
            
        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        // Chanel 1
        private void button7_Click(object sender, EventArgs e)
        {
            if (isClicked == true && Status.Text == "Conected")
            {
                chanel = 0;

                if (chanel1 == false)
                {
                    chanel1 = true;

                }
                else
                {
                    chanel1 = false;
                    chanel = 0;
                }

                ClearZedGraph(chanel, chanel1);


            }
            
        }

        // Chanel 2
        private void button8_Click(object sender, EventArgs e)
        {
            if (isClicked == true && Status.Text == "Conected")
            {
                chanel = 1;
                if (chanel2 == false)
                {
                    chanel2 = true;
                    

                }
                else
                {
                    chanel2 = false;
                    chanel = 1;
                }
                ClearZedGraph(chanel, chanel2);


            }
        }

        // Chanel 3
        private void button9_Click(object sender, EventArgs e)
        {
            if (isClicked == true && Status.Text == "Conected")
            {
                chanel = 2;
                if (chanel3 == false)
                {
                    chanel3 = true;
                    
                }
                else
                {
                    chanel3 = false;
                  
                }
                ClearZedGraph(chanel, chanel3);

            }
        }
      
       
        private void label1_Click_3(object sender, EventArgs e)
        {

        }

        private void kpp_TextChanged(object sender, EventArgs e)
        {

        }

        // Volt/div :x0.1
        private void button11_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            Scale_y_max = 3;
            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max ;
            myPane.YAxis.Scale.MinorStep = 1 ;
            myPane.YAxis.Scale.MajorStep =  2;

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }


        // Volt/div :x3
        private void button13_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            Scale_y_max = 3 * 3;
            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max;
            myPane.YAxis.Scale.MinorStep = 3;
            myPane.YAxis.Scale.MajorStep = 6;

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }


        // Volt/div :x6
        private void button12_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            Scale_y_max =  18;
            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max;
            myPane.YAxis.Scale.MinorStep = 6;
            myPane.YAxis.Scale.MajorStep = 12;


            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }



        int Tms;
        int temp=0;

        // Nút nhấn gửi Tms (4 byte) xuống Serial port
        private void button14_Click(object sender, EventArgs e)
        {
            if( Status.Text == "Conected")
            
            int.TryParse(txt_T_mau.Text, out Tms);
            while(Tms/1000 < 1) { Tms = Tms * 10;temp = temp + 1; }
            if (temp == 0) {
                string X = txt_T_mau.Text;
                serialPort1.Write(X);

            }
            if (temp == 1)
            {
                string X = "0"+txt_T_mau.Text;
                serialPort1.Write(X);

            }
            if (temp == 2)
            {
                string X = "0" +"0" + txt_T_mau.Text;
                serialPort1.Write(X);

            }
            if (temp == 3)
            {
                string X = "0" + "0" +"0"+ txt_T_mau.Text;
                serialPort1.Write(X);

            }
            temp = 0;



        }


        // Dịch trái Oy
        private void button15_Click(object sender, EventArgs e)
        {
            Scale_x_min = Scale_x_min + 20;
            Scale_x_max = Scale_x_max + 20;
            GraphPane myPane = zedGraphControl1.GraphPane;
            myPane.XAxis.Scale.Min = Scale_x_min;
            myPane.XAxis.Scale.Max = Scale_x_max;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();

        }
        
        // Nâng trục Ox
        private void button18_Click(object sender, EventArgs e)
        {
            Scale_y_min = Scale_y_min - 1;
            Scale_y_max = Scale_y_max - 1;
            GraphPane myPane = zedGraphControl1.GraphPane;
          
            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();

        }


        // Hạ trục Ox
        private void button17_Click(object sender, EventArgs e)
        {
            Scale_y_min = Scale_y_min + 1;
            Scale_y_max = Scale_y_max + 1;
            GraphPane myPane = zedGraphControl1.GraphPane;
      
            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();


        }
        

        // Cho phép tự động Scale trục Ox
        private void button19_Click(object sender, EventArgs e)
        {
            if (bd == false)
            {
                GraphPane myPane = zedGraphControl1.GraphPane;

                myPane.XAxis.Scale.Min = 0;
                myPane.XAxis.Scale.Max = 100;
                myPane.XAxis.Scale.MinorStep = 5;
                myPane.XAxis.Scale.MajorStep = 10;

                myPane.YAxis.Scale.Min = 0;
                myPane.YAxis.Scale.Max = 6;
                myPane.YAxis.Scale.MinorStep = 1;
                myPane.YAxis.Scale.MajorStep = 2;

                zedGraphControl1.AxisChange();
                zedGraphControl1.Invalidate();
                bd = true;
            }
            else bd = false;

        }


        // Dịch phải Oy
        private void button16_Click(object sender, EventArgs e)
        {
            Scale_x_min = Scale_x_min - 20;
            Scale_x_max = Scale_x_max - 20;
           
            GraphPane myPane = zedGraphControl1.GraphPane;
            
            myPane.XAxis.Scale.Min = Scale_x_min;
            myPane.XAxis.Scale.Max = Scale_x_max;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();

        }


        // Volt/div :x0.5
        private void button10_Click(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            Scale_y_max = 2;
            myPane.YAxis.Scale.Min = Scale_y_min;
            myPane.YAxis.Scale.Max = Scale_y_max;
            myPane.YAxis.Scale.MinorStep = 0.5;
            myPane.YAxis.Scale.MajorStep = 1;

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }

        private void time123_TextChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click_2(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
         // Nút thoát
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            
            
                if (MessageBox.Show(" BẠN MUỐN ĐÓNG CHƯƠNG TRÌNH ?", " THÔNG BÁO ", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    serialPort1.Close();
                    Status.Text = "Disconcted";
                    isClicked = false;
                    Application.Exit();
                }
                else
                {
                    
                }
            
        }
    }
    
}