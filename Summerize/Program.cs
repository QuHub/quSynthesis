using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace Summerize
{
  class Program
  {
    static void Main(string[] args)
    {
      if (args.Length < 1)
      {
        Console.WriteLine("Usage: Summerize {folder name}");
        return;
      }

      string[] fi = EnumertateFiles(args[0]);

      StreamWriter sw = new StreamWriter("Results.csv");
      sw.WriteLine("Sequence, NBits, Cost, Time");

      foreach (string f in fi)
      {
        Console.WriteLine(f);
        ExtractData(sw, f);
      }

      sw.Close();
    }

    static void ExtractData(StreamWriter sw, string file)
    {
      StreamReader sr = new StreamReader(file);

      string line = sr.ReadLine();

      Match m = Regex.Match(line, @"Bit Count: (\d+)");
      if (m.Success)
        sw.Write("Hey,{0},", m.Groups[1].Value);


      line = sr.ReadLine();

      m = Regex.Match(line, @"Qua.+Cost: (\d+)");
      if (m.Success)
        sw.Write("{0},", m.Groups[1].Value);

      m = Regex.Match(line, @"Seconds: (\d+).+");
      if (m.Success)
        sw.Write("{0},\n", m.Groups[1].Value);

      sr.Close();
    }

    static string[] EnumertateFiles(string folder)
    {
      string[] folders = Directory.GetDirectories(folder);
      string[] files ={};

      foreach (string f in folders)
        files = files.Concat(EnumertateFiles(f)).ToArray();

      string[] fs = Directory.GetFiles(folder, "*.qsy");
      files = files.Concat(fs).ToArray();
      
      return files;
    }
  }
}
