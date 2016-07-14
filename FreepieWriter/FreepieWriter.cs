using System;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace FreepieWriter
{
    public class FreepieWriter
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct FreepieData
        {
            public float yaw;
            public float pitch;
            public float roll;

            public float x;
            public float y;
            public float z;
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool SetDllDirectory(string lpPathName);

        [DllImport("freepie_io.dll")]
        private static extern int freepie_io_6dof_slots();

        [DllImport("freepie_io.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int freepie_io_6dof_write(int index, int length, FreepieData[] data);

        private static bool bInit = false;

        private static void init()
        {
            //Set freepie DLL path so we don't have to include it
            var path = Registry.GetValue(string.Format("{0}\\Software\\{1}", Registry.CurrentUser, "FreePIE"), "path", null) as string;
            SetDllDirectory(path);
            bInit = true;
        }

        public static void WriteData(FreepieData poseData, FreepieData buttonData1, FreepieData buttonData2, Int32 freepiePoseIndex = 0, Int32 freepieButton1Index = 1, Int32 freepieButton2Index = 2)
        {
            if (!bInit)
            {
                init();
            }

            FreepieData[] toArray = new FreepieData[1];
            toArray[0] = poseData;
            int result = freepie_io_6dof_write(freepiePoseIndex, 1, toArray);

            if (result != 0)
            {
                throw new Exception("FreepieWriter - Could not write pose to freepie index " + freepiePoseIndex + ", Freepie result " + result);
            }
            
            toArray[0] = buttonData1;
            result = freepie_io_6dof_write(freepieButton1Index, 1, toArray);

            if (result != 0)
            {
                throw new Exception("FreepieWriter - Could not write buttons 1 to freepie index " + freepieButton1Index + ", Freepie result " + result);
            }
            
            toArray[0] = buttonData2;
            result = freepie_io_6dof_write(freepieButton2Index, 1, toArray);

            if (result != 0)
            {
                throw new Exception("FreepieWriter - Could not write buttons 2 to freepie index " + freepieButton2Index + ", Freepie result " + result);
            }
        }
    }
}
