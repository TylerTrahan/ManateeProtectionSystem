//-------------------------------------------------
//  Manatee Protection System Sonar Object
//  Copyright Tyler Trahan - Aqua-tech Services 2017
//-------------------------------------------------

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Forms;
using System.Windows.Threading;  
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Drawing.Imaging;
using System.Text.RegularExpressions;

//Include BlueView SDK Libraries
using BVTSDK;

//Include OpenCV Libraries
using Emgu.CV;
using Emgu.CV.UI;
using Emgu.Util;
using Emgu.CV.Structure;
using Emgu.CV.CvEnum;

namespace BVTSDK
{
        public class ATSonar : IDisposable
        {
            //-----ATSONAR FIELDS----\\
            Sonar son = new Sonar();
            Head head;
            bool IsConnected = false;
            bool _disposed;
            int ret = -1;                                                    //Used in error checking
            public int headCount = -1;
            public int pingCount = -1;
            float start_range;
            float stop_range;
            public string testString = "0";                                  //Used for my testing
            string sonarType;
            string sonarPath;
            public int ping_num = 1;                                                //Current ping number
            private ImageGenerator imager = null;
            private bool _captureInProgress = false; 
            public Mat _frame;
            public ImageBox imgBx;
            
            //These will be deprecated when I write File Manager
            public string magImagePath = @"C:\MagImg.PGM";
            public string colorImagePath = @"C:\ColorImg.PPM";

            //----ATSONAR CONSTRUCTOR----\\
            public ATSonar(string addr, int headNum,float startRange, float stopRange)
            {
                //This is an ImageBox to display the sonars on the main console  - It works similar to ATCamera
                imgBx = new ImageBox();
                imgBx.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;
                imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
                imgBx.BackColor = System.Drawing.Color.Black;

                if (IsValidIP(addr))
                {
                    sonarPath = addr;
                    sonarType = "NET"; 
                }
                else 
                {
                    sonarPath = addr;
                    sonarType = "FILE";
                }

                //Initialize this sonar
                createSonar(addr);
                connectHead(headNum);
                start_range = startRange;
                stop_range = stopRange;
                ping_num = 1;
                setRange(start_range, stop_range);
                imager = new ImageGenerator();
                if (head != null)
                {
                    imager.SetHead(head);
                }
                if (son != null && head != null)
                {
                    IsConnected = true;
                }
                _disposed = false;
            }
                    /// Non-deterministic destruction of this object on the Finalizer thread
            ~ATSonar()
            {
                Dispose(false);
            }

            /// Dispose deterministically
            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }

            protected virtual void Dispose(bool disposing)
            {
                if (_disposed)
                    return;
                _disposed = true;
                if (imager != null) imager.Dispose();
                if (_frame != null) _frame.Dispose();
                if (imgBx != null) imgBx.Dispose();
                if (head != null) head.Dispose();
                if (son != null) son.Dispose();
            }


            //----ATSONAR MEMBERS----\\
            private bool IsValidIP(string addrText)
            {
                if (!Regex.IsMatch(addrText, @"\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b"))
                    return false;

                return true;
            }
            private string returnLiveOrFile(string addrText)
            {
                if (IsValidIP(addrText))
                {
                    return "NET";
                }
                else
                {
                    return "FILE";
                }
            }

            public bool isConnected
            {
                get { return IsConnected; }
                set
                {
                    if (value != this.IsConnected)
                    {
                    this.IsConnected = value;
                    }
                }
            }
            public void createSonar(string sonarPath)
            {
                //Open the sonar
                son.Open(sonarType, sonarPath);
            }                            
            public int getHeadCount()                                       
            {
                //Count the number of heads 
                headCount = son.HeadCount;

                return headCount;
            }
            public void connectHead(int headNum)   //TODO: Add multi-head selection in the else bracket 
            {
                if (son != null)
                {
                    head = son.GetHead(headNum);
                    
                    if (head != null)
                    {
                        //Set pingCount to the number of pings
                        pingCount = head.PingCount;
                    }
                    else { System.Windows.MessageBox.Show("No head to get at 0"); }
                }
                else { System.Windows.MessageBox.Show("Sonar is null. Cannot set head"); }
            }
            public void setRange(float start, float stop)
            {
                //Set the range from ATSonar Variables
                head.SetRange(start, stop);
            }

            public Ping getPingAt(int ping_num)
            {
                Ping ping = null;

                //if (ping_num <= pingCount)
                //{
                    if (head != null)
                    {
                        ping = head.GetPing(ping_num);
                    }
                    else { System.Windows.MessageBox.Show("Cannot get ping. Head is null"); }
                //}
                //else { System.Windows.MessageBox.Show("Ping out of range"); }

                return ping;    
            }                         
            public MagImage getMagImage(int ping_num)
            {
                    Ping ping = null;

                    MagImage mImg;

                    if(imager.IsValidForHead(head))
                    {
                        ping = getPingAt(ping_num);
                        mImg = imager.GetImageXY(ping);
                        ping.Dispose();
                        return mImg;
                    }
                    else
                    {
                        ping = getPingAt(ping_num);
                        imager.SetHead(head);
                        mImg = imager.GetImageXY(ping);
                        ping.Dispose();
                        return mImg;
                    }
            }                    
            public ColorImage getColorImage(MagImage mImg)
            {
                ColorMapper cmap = new ColorMapper();
                ColorImage cimg = null;

                if(mImg != null)
                {
                    cmap.Load(@"C:\jet.cmap");
                    cimg = cmap.MapImage(mImg);
                }
                cmap.Dispose();
                mImg.Dispose();

                return cimg == null ? new ColorImage() : cimg;
            }               //TODO: Examine different color maps, choose
            public Mat getMatImage(ColorImage cimg)
            {
                Mat mat = null;
                Mat copy = null;
                IntPtr imageBits;
                //IntPtr dataPtr;
                System.Drawing.Size size1;
                int width = cimg.Width;
                int height = cimg.Height;
                //uint[] dataArray = null;

                if(width<=0||height<=0)
                    {throw new Exception();}

                size1 = new System.Drawing.Size(width, height);
                
                //Copy bits for cimg and create the mat image with it
                imageBits = new IntPtr(cimg.Bits);

                //mat = new Mat(size1, DepthType.Cv8U, 4, imageBits, width * 4);
                try
                {
                    mat = new Mat(size1, DepthType.Cv8U, 4, imageBits, width * 4);
                    copy = mat.Clone();
                }
                catch (Exception ex)
                {
                    System.Windows.Forms.MessageBox.Show(ex.ToString());
                }
                if (copy == null) copy = new Mat();
                //Clean up garbage
                cimg.Dispose();
                if (mat != null) mat.Dispose();

                return copy;
            }
            private Mat getFrame(int pingNum)
            {
                Mat m;

                if (_captureInProgress==false)
                {
                    _captureInProgress = true;
                    MagImage mg = getMagImage(pingNum);
                    if (ping_num < pingCount && sonarType!="NET")
                    {
                        ping_num++;
                    }
                    
                    ColorImage c = getColorImage(mg);
                    m = getMatImage(c);

                    //Colorimage needs to be disposed after being sent to screen
                    //c.Dispose();
                }
                else
                {
                    //Do nothing if it is already processing?
                    m = new Mat();
                }

                return m;
            }
            public void ProcessFrame()
            {
                Mat frame_copy = null;
                if (sonarType == "FILE")
                {
                    try
                    {
                        //----This is for playing a sonar file on a loop
                        if (ping_num != pingCount)
                        {
                            frame_copy = _frame;
                            _frame = getFrame(ping_num);
                            imgBx.Image = _frame;
                            if (frame_copy != null) frame_copy.Dispose();
                            _captureInProgress = false;
                        }
                    }
                    catch (Exception ex)
                    {
                        System.Windows.Forms.MessageBox.Show(ex.ToString());
                    }
                }   //----This is for playing a live sonar
                else if (sonarType == "NET")
                {
                    try
                    {
                        frame_copy = _frame;
                        _frame = getFrame(-1);
                        imgBx.Image = _frame;
                        if (frame_copy != null) frame_copy.Dispose();
                        _captureInProgress = false;
                    }
                    catch (Exception ex)
                    {
                        System.Windows.Forms.MessageBox.Show(ex.ToString());
                    }
                }
                else
                {
                    System.Windows.MessageBox.Show("Error in choosing the sonar type");
                }
               
            }

            public String getFileName(string baseName)
            {
                DateTime t = DateTime.Now;
                string i = t.Hour.ToString() + t.Minute.ToString() + t.Second.ToString();
                string filepath = "C:/" + baseName + i + ".bmp";

                return filepath;
            }

            public void DisconnectSonar()
            {
                head.Dispose();
                son.Dispose();
                _frame.Dispose();
                imgBx.Dispose();
                isConnected = false;
            }

            public float startRange { get; set; }
            public float stopRange { get; set; }
            #region Functions Not Being Used
            [DllImport("gdi32")]
            private static extern int DeleteObject(IntPtr o);
            public static BitmapSource ToBitmapSource(IImage image)
            {
                using (System.Drawing.Bitmap source = image.Bitmap)
                {
                    IntPtr ptr = source.GetHbitmap(); //obtain the Hbitmap

                    BitmapSource bs = System.Windows.Interop
                      .Imaging.CreateBitmapSourceFromHBitmap(
                      ptr,
                      IntPtr.Zero,
                      Int32Rect.Empty,
                      System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());

                    DeleteObject(ptr); //release the HBitmap
                    return bs;
                }
            }
            public BitmapSource getBitmap(ColorImage cimg)
            {
                int width = cimg.Width;
                int height = cimg.Height;

                System.Drawing.Size size1 = new System.Drawing.Size(width, height);

                IntPtr imageBits = new IntPtr(cimg.Bits);

                Mat MatImage = new Mat(size1, DepthType.Cv8U, 4, imageBits, width * 4);

                Image<Rgb, Byte> img = MatImage.ToImage<Rgb, Byte>();

                BitmapSource bitmap = ToBitmapSource(img);

                return bitmap;
            }
            public Image<Rgb, Byte> getImage(Mat m)
            {
                IntPtr ptr = m.DataPointer;

                Image<Rgb, Byte> img = m.ToImage<Rgb, Byte>();
                //CvInvoke.cvReleaseImageHeader(ref ptr);

                return img;
            }
            public void saveMagImage(MagImage mImg, string filePath)
            {
                mImg.SavePGM(filePath);
                mImg.Dispose();
            }
            public void saveColorImage(ColorImage cImg, string filePath)
            {
                cImg.SavePPM(filePath);
                cImg.Dispose();
            }
            public void getAllPings()                                       //TODO: Revise this function, it was using it for testing
            {
                Double sum = 0;
                Double avg;
                Double start = 0;
                Double prevTime = 0;
                //Get the pings
                for (int ping_num = 1; ping_num < head.PingCount; ping_num++)
                {
                    Ping ping = head.GetPing(ping_num);

                    if (ping_num == 1)
                    {
                        start = ping.Timestamp;
                        prevTime = ping.Timestamp;
                    }

                    sum = sum + (ping.Timestamp - prevTime);

                    ping.Dispose();
                }

                avg = sum / head.PingCount;


                testString = String.Join(@" | ", sum.ToString(), avg.ToString());
            }
            public bool isPaired { get; set; }
            #endregion

        }
}
//TMFT


