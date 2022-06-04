using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace IOT_LAB11
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
                    strData = strData.Substring(1, strData.Length - 1).Trim();
                    dData = Convert.ToDecimal(Decimal.Parse(strData.ToString(), System.Globalization.NumberStyles.Float));
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
            String stringIP = "192.168.10.200";
            int port = 10005;
            IPAddress ip = IPAddress.Parse(stringIP);
            IPEndPoint ipEnd = new IPEndPoint(ip, port);
            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.Connect(ipEnd);
            byte[] send = new byte[40960];
            string strSend = "FData,0\n";
            string strs = "时间 通道1;通道2;通道3;通道4;通道5;通道6;通道7;通道8;\n";
            for (int j=0;j<20;j++)
            {
                send = Encoding.UTF8.GetBytes(strSend);
                socket.Send(send, send.Length, SocketFlags.None);
                Thread.Sleep(500);
                byte[] receive = new byte[4096];
                string strReceive;
                int length = socket.Receive(receive);
                strReceive = Encoding.UTF8.GetString(receive, 0, length);
                // Console.WriteLine(strReceive);
                string[] striparr = strReceive.Split(new string[] { "\r\n" }, StringSplitOptions.None);
                string[] date = striparr[1].Split(new string[] { " " }, StringSplitOptions.None);
                string[] time = striparr[2].Split(new string[] { " " }, StringSplitOptions.None);
                string realtime = date[1] + "," + time[1];
                // Console.WriteLine(realtime);
                strs = strs + realtime + " ";
                for (int i = 3; i < 11; i++)
                {
                    string[] substriparr = striparr[i].Split(new string[] { "         " }, StringSplitOptions.None);
                    string temp = substriparr[1].Substring(0, 1);
                    substriparr[1] = ChangeDataToD(substriparr[1]).ToString();
                    substriparr[1] = temp + substriparr[1];
                    strs += substriparr[1] + ";";
                   
                }
                strs = strs + "\n";
            }
            
            System.IO.File.WriteAllText(@"E:\19020033017.txt", strs);
            socket.Shutdown(SocketShutdown.Both);
            socket.Close();
    
        }
    }
}
