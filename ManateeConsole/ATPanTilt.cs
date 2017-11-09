//--------------------------------------------------------
//  Manatee Protection System Sidus Pan & Tilt Controller
//  Copyright Tyler Trahan - Aqua-tech Services 2017
//--------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms; 
using WindowsControlLibrary1; 

namespace ManateeConsole
{
    class ATPanTilt
    {
        /// <summary>
        /// Do we even need this function or should I just use Scott's code?
        /// 
        /// //-----Test Pan & Tilt-----\\
        ////**** These are the controls to connect to a Pan & Tilt
        //// Create the interop host control.
        //       System.Windows.Forms.Integration.WindowsFormsHost host = new System.Windows.Forms.Integration.WindowsFormsHost();
        //// Create the MaskedTextBox control.
        //       WindowsControlLibrary1.smPanTilt pt1 = new WindowsControlLibrary1.smPanTilt();
        //// Assign the MaskedTextBox control as the host control's child.
        //       host.Child = pt1;
        ///
        ///---Controls for Pan & Tilt---\\
        ///pt1.cBaudRate = 9600;
        ///pt1.cDataBits = 8;
        ///pt1.cParity = 0;
        ///pt1.cStopBits = 1;
        ///pt1.cPort = "COM1";
        ///pt1.cPortOpen();
        ///var PanAngle = pt1.pAngle;
        ///var tiltAngle = pt1.tAngle;
        /////---Testing of Pan & Tilt---\\
        ///    txtBx_SonarTest.Text = pt1.pAngle.ToString() + ":" + pt1.tAngle.ToString();    
        ///  -Add the interop host control to the Grid control's collection of child controls.
        /// </summary>
        /// 
    }
}
