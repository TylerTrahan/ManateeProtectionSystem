using System;
using System.Drawing;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
// This example only compiles if you install NI-DAQmx support from the Device Drivers CD
using NationalInstruments.DAQmx;
using NationalInstruments;
using NationalInstruments.Analysis;
using NationalInstruments.Analysis.Conversion;
using NationalInstruments.Analysis.Dsp;
using NationalInstruments.Analysis.Dsp.Filters;
using NationalInstruments.Analysis.Math;
using NationalInstruments.Analysis.Monitoring;
using NationalInstruments.Analysis.SignalGeneration;
// This example only compiles with Enterprise Analysis
using NationalInstruments.Analysis.SpectralMeasurements;
using System.Windows;

namespace ManateeConsole
{
    public class ATHydrophone
    {
        // This example only compiles if you install NI-DAQmx support from the Device Drivers CD.
        private Task myTask;
        private AnalogSingleChannelReader reader;
        private Task runningTask;
        private AnalogWaveform<double> data = new AnalogWaveform<double>(1000);
        //public double[] data = new double[1000];
        public ViewModel viewmodel;

        double samplingRate;
        int samplesPerChannel;

        public ATHydrophone()
        {
            viewmodel = MainWindow.viewmodel;
        }

        private void myCallback(IAsyncResult ar)
        {
            try
            {
                if (runningTask != null && runningTask == ar.AsyncState)
                {
                    data = reader.EndReadWaveform(ar);
                    
                    //Acquire Data
                    for (int i = 0; i < data.SampleCount; i++)
                    {
                         double d = data.Samples[i].Value;
                    }

                    // Continue to acquire if task still running
                    reader.BeginMemoryOptimizedReadWaveform(samplesPerChannel, new AsyncCallback(myCallback), myTask, data);
                    
                }
            }
            catch (DaqException ex)
            {
                MessageBox.Show(ex.Message);
                runningTask = null;
                myTask.Dispose();
            }
        }

        public void acquisitionStart()
        {
            try 
            {   // Acquisition ON
                if (runningTask == null) 
                {
                    samplingRate = 1000;
                    samplesPerChannel = 1000;

                    myTask = new Task();

                    myTask.AIChannels.CreateVoltageChannel(@"Dev1/ai0", "aiChannel", AITerminalConfiguration.Differential, -10.0,
                        10.0, AIVoltageUnits.Volts);  
            
                    myTask.Timing.ConfigureSampleClock("",samplingRate,SampleClockActiveEdge.Rising,
                        SampleQuantityMode.ContinuousSamples, samplesPerChannel);
                    runningTask = myTask;
                    reader = new AnalogSingleChannelReader(myTask.Stream);
                    
                    reader.SynchronizeCallbacks = true;
                    reader.BeginMemoryOptimizedReadWaveform(samplesPerChannel, new AsyncCallback(myCallback), myTask, data);
                }
            }
            catch (DaqException ex)
            {
                MessageBox.Show(ex.Message);
                runningTask = null;
                myTask.Dispose();
            }

        }

        public void acquisitionStop()
        {
            try
            {
                if (runningTask != null)
                {
                    runningTask = null;
                    myTask.Dispose();
                }
            }
            catch (DaqException ex)
            {
                MessageBox.Show(ex.Message);
                runningTask = null;
                myTask.Dispose();
            }
        }
    }

}
