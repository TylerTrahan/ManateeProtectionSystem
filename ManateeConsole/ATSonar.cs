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
using System.Collections;
using System.Diagnostics;

//Include BlueView SDK Libraries
using BVTSDK;

//Include OpenCV Libraries
using Emgu.CV;
using Emgu.CV.UI;
using Emgu.Util;
using Emgu.CV.Structure;
using Emgu.CV.CvEnum;
using Emgu.CV.Util;

using VinetV1;

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
        string sonarType;
        string sonarPath;
        public int ping_num = 1;                                                //Current ping number
        private ImageGenerator imager = null;
        private bool _captureInProgress = false;
        public Mat _frame;
        public ImageBox imgBx;
        public ImageBox TestImgBx;

        //---VISION DETECTION OBJECTS---\\
        public VinetV1.VinetDetector detector;
        public string netPath = @"C:\Users\VI-System2\Desktop\ManateeConsole_4.9.17\ManateeConsole_4.7.17\ManateeConsole\VinetCV\examples\";
        public float detectAccuracy;
        public float trackerAccuracy;
        public int numManatees;
        public TrackerManager trackerManager;
        public bool init;

        //---PERFORMANCE TESTING VARIABLES---\\
        public Stopwatch stopwatch;
        public string ping_time;
        public string magimage_time;
        public string colormap_time;
        public string matrix_time;
        public string recognition_time;
        public string render_time;

        public bool MotionDetection { get; set; }
        public bool Identification { get; set; }
        public bool Tracking { get; set; }

        //----ATSONAR CONSTRUCTOR----\\
        public ATSonar(string addr, int headNum, float startRange, float stopRange)
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

            //DEEP LEARNING CONFIGURATIONS AND WEIGHT FILES
            //detector = new VinetV1.VinetDetector(netPath + "yolov1.cfg", netPath + "yolov1.weights");
            detector = new VinetV1.VinetDetector(netPath + "miamiRed.cfg", netPath + "miamiRed.weights");
            //detector = new VinetV1.VinetDetector(netPath + "vinet.cfg", netPath + "vinet_148000.weights");
            //detector = new VinetV1.VinetDetector(netPath + "tiny-yolo-v1.cfg", netPath + "tiny-yolo-v1.weights");
            //detector = new VinetV1.VinetDetector(netPath + "yolo-bkh.cfg", netPath + "yolo-bkh.weights");
            stopwatch = new Stopwatch();
            detectAccuracy = 0.20f;
            trackerAccuracy = 0.50f;
            numManatees = 0;
            trackerManager = new TrackerManager(10);
            init = true;
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
            stopwatch.Start();
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
            stopwatch.Stop();
            ping_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
            return ping;
        }
        public MagImage getMagImage(int ping_num)
        {
            stopwatch.Restart();
            Ping ping = null;

            MagImage mImg;

            if (imager.IsValidForHead(head))
            {
                ping = getPingAt(ping_num);
                mImg = imager.GetImageXY(ping);
                ping.Dispose();
                stopwatch.Stop();
                magimage_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
                return mImg;
            }
            else
            {
                ping = getPingAt(ping_num);
                imager.SetHead(head);
                mImg = imager.GetImageXY(ping);
                ping.Dispose();
                stopwatch.Stop();
                magimage_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
                return mImg;
            }

        }
        public ColorImage getColorImage(MagImage mImg)
        {
            stopwatch.Restart();
            ColorMapper cmap = new ColorMapper();
            ColorImage cimg = null;

            if (mImg != null)
            {
                cmap.Load(@"C:\ColorMaps\copper.cmap");
                cimg = cmap.MapImage(mImg);
            }
            cmap.Dispose();
            mImg.Dispose();
            stopwatch.Stop();
            colormap_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
            return cimg == null ? new ColorImage() : cimg;
        }
        public Mat getMatImage(ColorImage cimg)
        {
            //stopwatch.Restart();
            Mat mat = null;
            Mat copy = null;
            IntPtr imageBits;
            //IntPtr dataPtr;
            System.Drawing.Size size1;
            int width = cimg.Width;
            int height = cimg.Height;
            //uint[] dataArray = null;

            if (width <= 0 || height <= 0)
            { throw new Exception(); }

            size1 = new System.Drawing.Size(width, height);

            UInt32 length = new UInt32();
            length = Convert.ToUInt32(width * height);

            UInt32[] data = new UInt32[length];
            cimg.CopyBits(data, length);

            //Copy bits for cimg and create the mat image with it
            //imageBits = new IntPtr(cimg.Bits);
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            imageBits = pinnedArray.AddrOfPinnedObject();

            if (imageBits != null /*& !_captureInProgress*/)
            {
                //mat = new Mat(size1, DepthType.Cv8U, 4, imageBits, width * 4);
                try
                {
                    mat = new Mat(size1, DepthType.Cv8U, 4, imageBits, width * 4);
                    //mat.Save(@"C:\Pictures\Mat_test.bmp");
                    copy = mat.Clone();
                    //mat.CopyTo(copy);
                }
                catch (Exception ex)
                {
                    //System.Windows.Forms.MessageBox.Show(ex.ToString());
                }
            }
            if (copy == null) copy = new Mat();
            //Clean up garbage
            cimg.Dispose();
            if (mat != null) mat.Dispose();
            pinnedArray.Free();

            //stopwatch.Stop();
            //matrix_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
            return copy;

        }
        private Mat getFrame(int pingNum)
        {

            Mat m;

            if (_captureInProgress == false)
            {
                _captureInProgress = true;
                MagImage mg = getMagImage(pingNum);
                if (ping_num < pingCount && sonarType != "NET")
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

        ManateeConsole.PythonMotionDetector motionDetector;
        public IImage ProcessFrame()
        {
            Mat _detected_frame = null;
            int frameCount = ping_num;

            try
            {
                // Getting frame
                if (sonarType == "FILE")
                {
                    if (ping_num == pingCount) return _frame;
                    _frame = getFrame(ping_num);
                }
                else if (sonarType == "NET")
                {
                    _frame = getFrame(-1);
                }

                if (_detected_frame != null) _detected_frame.Dispose();
                stopwatch.Restart();

                _detected_frame = _frame.Clone();
                bool memory_owner = true;

                // Motion detection (Python tracking)
                if (MotionDetection)
                {
                    var motionDetector = this.motionDetector;
                    //Trace.WriteLine("Processing Image in Python : New Frame");
                    if (motionDetector == null)
                    {
                        //Trace.WriteLine("Processing Image in Python : Getting MotionDetector");
                        motionDetector = this.motionDetector = new ManateeConsole.PythonMotionDetector();
                    }
                    UMat u_motionDetected;
                    using (CsPython.PythonRunTime.Py.GIL())
                        u_motionDetected = motionDetector.ProcessImage(_detected_frame);
                    using (u_motionDetected)
                    {
                        if (memory_owner) _detected_frame.Dispose();
                        using (Mat cur = u_motionDetected.GetMat(AccessType.ReadWrite))
                            _detected_frame = cur.Clone();
                        memory_owner = true;
                    }
                }
                else
                {
                    Console.Write(2);
                }

                // Vinet detection
                if (Identification)
                {
                    Mat cur = _detected_frame;
                    _detected_frame = DetectFrame(cur, detector).Clone();
                    if (memory_owner) cur.Dispose();
                    memory_owner = true;
                }

                stopwatch.Stop();
                recognition_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";

                if (Tracking)
                {
                    if (init)
                    {
                        trackerManager.ClearList();
                        init = false;
                    }

                    if (!init)
                    {
                        trackerManager.UpdateTrackers(_detected_frame);
                        trackerManager.RemoveRedundantTrackers();
                        //if(trackerManager.count % trackerManager.skip_decay == 0)
                        //{
                        //    CvInvoke.AddWeighted(_detected_frame, trackerManager.decay_rate * .5, _detected_frame, trackerManager.decay_rate * .5, -1, _detected_frame);
                        //}
                        trackerManager.count++;
                        trackerManager.count = trackerManager.count % 100;
                        trackerManager.DrawActiveTrackersOnImage(_detected_frame);
                    }
                }

                CvInvoke.PutText(_detected_frame, "Recognition Time:" + recognition_time, new System.Drawing.Point(_detected_frame.Width - 250, 25), Emgu.CV.CvEnum.FontFace.HersheyComplexSmall, 0.75, new MCvScalar(0, 255, 0, 255));
                //imgBx.Image = _detected_frame;
                var old = imgBx.Image;

                if (!memory_owner) _detected_frame = _detected_frame.Clone();

                imgBx.Image = _detected_frame;
                //imgBx.Image = _frame;
                if (old != null)
                {
                    try { old.Dispose(); } catch { }
                }
                _captureInProgress = false;

            }
            catch (System.Exception ex)
            {
                if (ex != null) System.Windows.Forms.MessageBox.Show(ex.ToString());
            }

            return _frame;
        }
        public IImage ProcessFrame_OLD_TODO_REMOVED()
        {
            Mat _detected_frame = null;
            int frameCount = ping_num;

            if (sonarType == "FILE")
            {
                try
                {
                    //----This is for playing a sonar file on a loop
                    if (ping_num != pingCount)
                    {
                        //frame_copy = _frame;
                        _frame = getFrame(ping_num);
                        if (_detected_frame != null) _detected_frame.Dispose();
                        stopwatch.Restart();

                        //Vinet Detection of Object
                        var motionDetector = this.motionDetector;
                        //Trace.WriteLine("Processing Image in Python : New Frame");
                        if (motionDetector == null)
                        {
                            //Trace.WriteLine("Processing Image in Python : Getting MotionDetector");
                            motionDetector = this.motionDetector = new ManateeConsole.PythonMotionDetector();
                        }

                        UMat u_motionDetected;
                        using (CsPython.PythonRunTime.Py.GIL())
                            u_motionDetected = motionDetector.ProcessImage(_frame);
                        using (u_motionDetected)
                        {
                            //Trace.WriteLine("Processing Image in Python : UMat to Mat");

                            using (var motionDetected = u_motionDetected.GetMat(AccessType.ReadWrite))
                            {
                                //motionDetected.Save(@"C:\Users\VI-System2\Desktop\manatee_training\preprocessed_images\frame_" + frameCount + ".png");
                                //I must dispose the processed frame so if it must be reused I will clone it
                                _detected_frame = DetectFrame(motionDetected, detector).Clone();
                                //Trace.WriteLine("Processing Image in Python : Dispase Mat");
                            }
                            //Trace.WriteLine("Processing Image in Python : Dispase UMat");
                        }
                        //Trace.WriteLine("Processing Image in Python : Frame Done");

                        stopwatch.Stop();
                        recognition_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
                        if (init)
                        {
                            trackerManager.ClearList();
                            init = false;
                        }

                        if (!init)
                        {
                            trackerManager.UpdateTrackers(_detected_frame);
                            trackerManager.RemoveRedundantTrackers();
                            //if(trackerManager.count % trackerManager.skip_decay == 0)
                            //{
                            //    CvInvoke.AddWeighted(_detected_frame, trackerManager.decay_rate * .5, _detected_frame, trackerManager.decay_rate * .5, -1, _detected_frame);
                            //}
                            trackerManager.count++;
                            trackerManager.count = trackerManager.count % 100;
                            trackerManager.DrawActiveTrackersOnImage(_detected_frame);
                        }
                        CvInvoke.PutText(_detected_frame, "Recognition Time:" + recognition_time, new System.Drawing.Point(_detected_frame.Width - 250, 25), Emgu.CV.CvEnum.FontFace.HersheyComplexSmall, 0.75, new MCvScalar(0, 255, 0, 255));
                        //imgBx.Image = _detected_frame;
                        var old = imgBx.Image;

                        imgBx.Image = _detected_frame;
                        imgBx.Image = _frame;
                        if (old != null)
                        {
                            try { old.Dispose(); } catch { }
                        }
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
                    //frame_copy = _frame;
                    _frame = getFrame(-1);
                    imgBx.Image = _frame;
                    if (_detected_frame != null) _detected_frame.Dispose();
                    stopwatch.Restart();

                    //Vinet Detection of Object
                    var motionDetector = this.motionDetector;
                    //Trace.WriteLine("Processing Image in Python : New Frame");
                    if (motionDetector == null)
                    {
                        //Trace.WriteLine("Processing Image in Python : Getting MotionDetector");
                        motionDetector = this.motionDetector = new ManateeConsole.PythonMotionDetector();
                    }

                    UMat u_motionDetected;
                    using (CsPython.PythonRunTime.Py.GIL())
                        u_motionDetected = motionDetector.ProcessImage(_frame);
                    using (u_motionDetected)
                    {
                        //Trace.WriteLine("Processing Image in Python : UMat to Mat");
                        using (var motionDetected = u_motionDetected.GetMat(AccessType.ReadWrite))
                        {
                            //motionDetected.Save(@"C:\Users\VI-System2\Desktop\manatee_training\preprocessed_images\frame_" + frameCount + ".png");
                            //I must dispose the processed frame so if it must be reused I will clone it
                            _detected_frame = DetectFrame(motionDetected, detector).Clone();
                        //Trace.WriteLine("Processing Image in Python : Dispase Mat");
}
                        //Trace.WriteLine("Processing Image in Python : Dispase UMat");
                    }
                    Trace.WriteLine("Processing Image in Python : Frame Done");
                    //if (frame_copy != null) frame_copy.Dispose();
                    recognition_time = stopwatch.ElapsedMilliseconds.ToString() + " ms";
                    if (init)
                    {
                        trackerManager.ClearList();
                        init = false;
                    }

                    if (!init)
                    {
                        trackerManager.UpdateTrackers(_detected_frame);
                        trackerManager.RemoveRedundantTrackers();
                        //if(trackerManager.count % trackerManager.skip_decay == 0)
                        //{
                        //    CvInvoke.AddWeighted(_detected_frame, trackerManager.decay_rate * .5, _detected_frame, trackerManager.decay_rate * .5, -1, _detected_frame);
                        //}
                        trackerManager.count++;
                        trackerManager.count = trackerManager.count % 100;
                        trackerManager.DrawActiveTrackersOnImage(_detected_frame);
                    }
                    //CvInvoke.PutText(_detected_frame, "Recognition Time:" + recognition_time, new System.Drawing.Point(_detected_frame.Width - 250, 25), Emgu.CV.CvEnum.FontFace.HersheyComplexSmall, 0.75, new MCvScalar(0, 255, 0, 255));
                    //imgBx.Image = _detected_frame;
                    var old = imgBx.Image;
                    imgBx.Image = _detected_frame;
                    if (old != null)
                    {
                        try { old.Dispose(); } catch { }
                    }
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

            return _frame;
        }

        public Mat DetectFrame(Mat frame, VinetV1.VinetDetector detector)
        {
            var boxes = detector.detect(frame, detectAccuracy);

            string boxString=null;

            foreach (var box in boxes)
            {
                CvInvoke.Rectangle(frame, new System.Drawing.Rectangle(box.x, box.y, box.w, box.h), new MCvScalar(0, 255, 0, 255), 2);
                CvInvoke.PutText(frame, box.prob.ToString() + "%", new System.Drawing.Point(box.x + 10, box.y + 20), Emgu.CV.CvEnum.FontFace.HersheyComplexSmall, 0.75, new MCvScalar(0, 255, 0, 255));
                CvInvoke.PutText(frame, "CLASS: " + box.obj_id.ToString(), new System.Drawing.Point(box.x + 10, box.y + 40), Emgu.CV.CvEnum.FontFace.HersheyComplexSmall, 0.75, new MCvScalar(0, 255, 0, 255));
                //System.Windows.MessageBox.Show("X: " + box.x + " | Y:" + box.y + " | Width:" + box.w + " | Height:" + box.h);

                //If manatee is detected, create a tracker
                if (box.prob>=trackerAccuracy)
                {
                    numManatees = numManatees + 1;
                    //Tracker tracker = new Tracker(numManatees);
                    //Mat newFrame = new Mat();
                    //newFrame = tracker.Update(frame);
                    //if (newFrame != null)
                    //{
                    //    TestImgBx.Image = newFrame;
                    //}
                    int centerX = box.x + (box.w / 2);
                    int centerY = box.y + (box.h / 2);

                    trackerManager.AddToList(centerX, centerY);
                    trackerManager.CreateTracker(centerX, centerY, box.w, box.h);

                    boxString += "X : " + centerX + "| Y: " + centerY + "| Acc: " + box.prob + "\n";
                }
            }
            CvInvoke.PutText(frame, "# MANATEES:" + trackerManager.countActiveTrackers(), new System.Drawing.Point(10, 25), Emgu.CV.CvEnum.FontFace.HersheyComplexSmall, 0.75, new MCvScalar(0, 255, 0, 255));
            //trackerManager.PrintTrackers(boxString);

            //frame.Save(@"C:\Users\VI-System2\Pictures\Test.png");

            return frame;
        }

        public TrackerManager getSonarTracker(TrackerManager tracker_manager, Mat raw_img, bool init, int x, int y, double accuracy)
        {
            int num_manatees = 0;
            string f = null;
            if (init)
            {
                tracker_manager.ClearList();
                init = false;
            }

            if (f == "m")
            {
                //num_manatees = num_manatees + 1; //Is this relevant?
                tracker_manager.AddToList(x, y);
                //tracker_manager.CreateTracker(x, y, );
            }
            if (f == "N") //using N for "hard no" and n for "soft no"
            {
                tracker_manager.RemoveTrackers(x, y);
            }

            if (!init)
            {
                tracker_manager.RemoveRedundantTrackers();
                tracker_manager.UpdateTrackers(raw_img);
                //tracker_manager.UpdateGlobalPaths();
            }
            return tracker_manager;
        }

        public String getFileName(string baseName)
        {
            DateTime t = DateTime.Now;
            string i = t.Hour.ToString() + t.Minute.ToString() + t.Second.ToString();
            string filepath = "M:/data/capture/" + baseName + i + ".bmp";

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


            //testString = String.Join(@" | ", sum.ToString(), avg.ToString());
        }
        public bool isPaired { get; set; }
        public Mat MotionDetect(Mat frame)
        {
            //Initialization in python
            //self.detector = CvInvoke.createBackgroundSubtractorMOG(history=2000, varThreshold=50, detectShadows=False)
            Emgu.CV.Cuda.CudaBackgroundSubtractorMOG2 detector = new Emgu.CV.Cuda.CudaBackgroundSubtractorMOG2(2000, 50, false);

            //self.global_mask=cv2.imread("global_mask.png",0) #0 for grayscale
            Mat global_mask = CvInvoke.Imread("global_mask.png", 0);
            bool received_first_image = false;
            float alpha = 0.35f;
            double learningRate = -1.0f;
            Mat gray = new Mat();
            Mat grayF = new Mat();
            Mat motion = new Mat();
            Mat motionF = new Mat();

            //UPDATE
            //Convert to grayscale gray=cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
            CvInvoke.CvtColor(frame, gray, ColorConversion.Bgr2Gray);
            //gray[self.global_mask==0]=0   TODO


            if (received_first_image == false)
            {
                //grayF image = copy of gray image
                gray.CopyTo(grayF);
                //motion = self.detector.apply(self.grayF)
                detector.Update(grayF, global_mask, learningRate);
                //self.motionF=motion.copy()
                motion.CopyTo(motionF);
                //self.received_first_image = true
                received_first_image = true;
            }
            else
            {
                //self.grayF = cv2.addWeighted(gray, 1 - self.alpha, self.grayF, self.alpha, 0)
                CvInvoke.AddWeighted(gray, 1 - alpha, grayF, alpha, 0, grayF);
                //motion = self.detector.apply(self.grayF)
                //detector.Update(grayF, global_mask, learningRate);
                //self.motionF = cv2.addWeighted(motion, 1 - self.alpha, self.motionF, self.alpha, 0)
                CvInvoke.AddWeighted(motion, 1 - alpha, motionF, alpha, 0, motionF);
            }

            //composite = cv2.merge([gray, gray, self.motionF])
            Mat composite = new Mat();
            VectorOfMat vm = new VectorOfMat(gray, gray, motionF);
            CvInvoke.Merge(vm, composite);

            //vis = np.concantenate((image,composite),axis=1)  --I do not need this;

            //Clean up any resources being used
            gray.Dispose();
            grayF.Dispose();
            motion.Dispose();
            motionF.Dispose();
            vm.Dispose();
            detector.Dispose();
            global_mask.Dispose();

            return composite;
        }

        #endregion

    }




}
//TMFT




