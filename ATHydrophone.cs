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

        double samplingRate;
        int samplesPerChannel;

        double[] autoPowerSpectrum;
        double searchFrequency;
        double equivalentNoiseBandwidth;
        double coherentGain;
        double df;

        public NationalInstruments.Controls.ChartCollection<double> chartCollection;
        List<double> pointsForAnalysis;


        private void myCallback(IAsyncResult ar)
        {
            try
            {
                if (runningTask != null && runningTask == ar.AsyncState)
                {
                    data = reader.EndReadWaveform(ar);
                    
                    //Acquire Data
                    //acquiredDataWaveformGraph.PlotWaveform(data);
                    //chartCollection.Append(data.GetRawData());
                    double[] dataPoint = data.GetRawData(0,1);
                    MessageBox.Show(dataPoint.ToString());

                    //getUnitConvertedAutoPowerSpectrum(data); // Get power spectrum of signal waveform. 
                    // Call the following function to calculate current powerPeak and frequencyPeak.
                    //currentPeakData();

                    // continue to acquire if task still running
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

        public void acquisitionChanged(object sender, NationalInstruments.UI.ActionEventArgs e)
        {
            try 
            {   // Acquisition ON
                if (runningTask == null) 
                {
                    samplingRate = 1000;
                    samplesPerChannel = 1000;

                    myTask = new Task();

                    myTask.AIChannels.CreateVoltageChannel(@"Dev1 / ai0", "aiChannel", AITerminalConfiguration.Differential, -10.0,
                        10.0, AIVoltageUnits.Volts);  
            
                    myTask.Timing.ConfigureSampleClock("",samplingRate,SampleClockActiveEdge.Rising,
                        SampleQuantityMode.ContinuousSamples, samplesPerChannel);
                    runningTask = myTask;
                    reader = new AnalogSingleChannelReader(myTask.Stream);
                    
                    reader.SynchronizeCallbacks = true;
                    reader.BeginMemoryOptimizedReadWaveform(samplesPerChannel, new AsyncCallback(myCallback), myTask,data);

                    //rateNumericEdit.Enabled = false;
                    //samplesNumericEdit.Enabled = false;
                    //channelTextBox.Enabled = false;
                }
                            
                else // Acquisition OFF
                {
                    if (runningTask != null)    
                    {                       
                        runningTask = null;
                        myTask.Dispose();           
                    }
                    //rateNumericEdit.Enabled = true;
                    //samplesNumericEdit.Enabled = true;
                    //channelTextBox.Enabled = true;
                }
            }
            catch (DaqException ex)
            {
                MessageBox.Show(ex.Message);
                runningTask = null;
                myTask.Dispose();
            }

        }
                
        
        //void getUnitConvertedAutoPowerSpectrum(AnalogWaveform<double> waveform)
        //{
        //    double []unitConvertedSpectrum;
        //    double []subsetOfUnitConvertedSpectrum = new double[samplesPerChannel/2];
        //    System.Text.StringBuilder unit;
        //    int i;           
            
        //    ScalingMode scalingMode = ScalingMode.Linear;
        //    DisplayUnits displayUnit = DisplayUnits.VoltsRms;
        //    ScaledWindow scaledWindow;

        //    double[] data = waveform.GetScaledData();
        //    scaledWindow.Apply(data, out equivalentNoiseBandwidth, out coherentGain); 
            
        //    // Calculate the auto power spectrum of signal waveform.
        //    autoPowerSpectrum = new double[samplesPerChannel];
        //    autoPowerSpectrum = Measurements.AutoPowerSpectrum(data, waveform.Timing.SampleInterval.TotalSeconds, out df);
            
        //    // Unit conversion of auto power spectrum as specified by the user.
        //    unit = new System.Text.StringBuilder("V", 256);         
        //    unitConvertedSpectrum = new double[samplesPerChannel];
        //    unitConvertedSpectrum = Measurements.SpectrumUnitConversion(autoPowerSpectrum,
        //        SpectrumType.Power, scalingMode, displayUnit, df, 
        //        equivalentNoiseBandwidth, coherentGain, unit);

        //    //Set the caption of yAxis according to the chosen unit of display.
        //    powerSpectrumYAxis.Caption = unit.ToString();

        //    for(i=0; i<(samplesPerChannel/2); i++)
        //    {
        //        subsetOfUnitConvertedSpectrum[i] = unitConvertedSpectrum[i];
        //    }

        //    // Plot unitConvertedSpectrum.
        //    powerSpectrumWaveformGraph.PlotY(subsetOfUnitConvertedSpectrum, 0, df);
        //}
        

        //void currentPeakData()
        //{
        //    double frequencyPeak;
        //    double powerPeak;       
            
        //    // Get the current XPosition of cursor
        //    searchFrequency = xyCursor.XPosition; 

        //    // Apply PowerFrequencyEstimate function 
        //    Measurements.PowerFrequencyEstimate(autoPowerSpectrum, searchFrequency,
        //        equivalentNoiseBandwidth, coherentGain, df, 7, out frequencyPeak, out powerPeak);

        //    // Update Frequency Peak and Power Frequency 
        //    peakFrequencyNumericEdit.Value = frequencyPeak;
        //    peakPowerNumericEdit.Value = powerPeak;
        //}
    }

}
