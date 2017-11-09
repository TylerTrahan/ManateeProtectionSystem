using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using BVTSDK;


class Program
{
    static void Main()
    {

        string sonarPath = @"C:\Swimmer.son";
        string type = "FILE";
        Console.WriteLine("Begin Test");

        ATSonar son = new ATSonar();

        Console.WriteLine(son.getString());
        son.createSonar(type, sonarPath);
        Console.WriteLine(son.getString());

        Console.ReadLine();

    }

}