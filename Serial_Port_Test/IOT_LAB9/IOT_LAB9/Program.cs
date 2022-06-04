using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.IO;

namespace IOT_LAB9
{
    class Program
    {
        static double ChangeDataToD(string strData)
        {
            decimal dData = 0.0M;
            try
            {
                if (strData.Contains("E") || strData.Contains("e"))
                {
                    strData = strData.Substring(0, strData.Length - 1).Trim();
                    dData = Convert.ToDecimal(Decimal.Parse(strData.ToString(),System.Globalization.NumberStyles.Float));
                }
                else
                {
                    dData = decimal.Parse(strData);
                }
            }
            catch (Exception)
            {
                dData = 0;
            }
            return double.Parse(dData.ToString());
        }

        static void Main(string[] args)
        {
            
            SerialPortTest port = new SerialPortTest();

            // 定义字符串数组接受数据
            string[] wt310 = new string[20];
            string[] data = new string[6];
            string realdata = "";
            string time;

            for (int i=0;i<20;i++)
            {
                port.Send();
                time = DateTime.Now.ToString("yyyy-MM-dd,hh:mm:ss") + " ";
                wt310[i] = time + port.Receive();

            }

            using (StreamWriter sw = new StreamWriter("E:\\data_analysis.txt"))
            {
                sw.WriteLine("YYYY-MM-DD,hh:mm:ss 数据1;数据2;数据3;数据4;数据5;数据6");
                foreach (string s in wt310)
                {
                    data = s.Split(',');
                    //realdata =  DateTime.Now.ToString("yyyy-MM-dd,hh:mm:ss") + " "; 
                    foreach(string data_s in data)
                    {
                        realdata += ChangeDataToD(data_s).ToString() + ";";
                    }

                    sw.WriteLine(realdata);

                }
            }
            port.Close();
        }


    }

    class SerialPortTest
    {

        SerialPort port;
        public SerialPortTest()
        {
            port = new SerialPort("COM8");
            port.BaudRate = 9600;
            try
            {
                port.Open();
            }
            catch (Exception)
            {
                Console.WriteLine("打开COM串口失败");
        
            }
        }

        public string Receive()
        {
            System.Threading.Thread.Sleep(1000);
            string strReceive = "0";
            if (port!=null)
            {
                int n = port.BytesToRead;
                byte[] byteReceive = new byte[n];
                port.Read(byteReceive, 0, n);
                strReceive = Encoding.UTF8.GetString(byteReceive, 0, n);
                Console.WriteLine(strReceive);
            }
            return strReceive;
        }

        public void Send()
        {
            byte[] command = Encoding.UTF8.GetBytes(":NUMeric:NORMal:VALue?\r\n");
            port.Write(command, 0, command.Length);
        }

        public void Close()
        {
            if(port!=null&port.IsOpen)
            {
                port.Close();
                port.Dispose();
            }
        }

        
    }
}
