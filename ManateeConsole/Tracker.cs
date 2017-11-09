using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//Include OpenCV Libraries
using Emgu.CV;
using Emgu.CV.UI;
using Emgu.Util;
using Emgu.CV.Structure;
using Emgu.CV.CvEnum;
using Emgu.CV.Util;
using System.Windows.Forms;
using System.Drawing;

namespace BVTSDK
{
    public class Tracker
    {
        //Members
        public int number;
        public int x;
        public int y;
        public int w;
        public System.Drawing.Rectangle track_window;
        public MCvTermCriteria term_crit;
        public double score;
        public string state;
        public double min_score;
        public int count;
        public double filter;
        public int recogWidth;
        public int recogHeight;

        public Tracker(int n)
        {
            number = n;
            x = 0;
            y = 0;
            w = 150;
            track_window = new System.Drawing.Rectangle();
            term_crit = new MCvTermCriteria(10, 1);
            score = 0;
            state = "DEACTIVATED";
            min_score = .05;
            count = 0;
            filter = .035;
        }

        public void LockOn(int i, int j, int Width, int Height)
        {
            x = i;
            y = j;
            track_window = new System.Drawing.Rectangle(Convert.ToInt32(x - (w/2)), Convert.ToInt32(y - (w/2)), Convert.ToInt32(w), Convert.ToInt32(w));
            state = "ACTIVATED";
            count = 1;
            recogWidth = Width;
            recogHeight = Height;
        }

        public void Update(Mat raw_img)
        {
            VectorOfMat channels = new VectorOfMat();
            Mat r;
            System.Drawing.Rectangle old_window, new_window;

            if (state == "DEACTIVATED")
            {
                count = 0;
            }

            //height, width, channels = raw_img.shape

            if (state == "ACTIVATED")
            {
                int d = Convert.ToInt32(w / 2);

                CvInvoke.Split(raw_img, channels);
                r = channels[2];
                Mat pdf = r;

                //int twin_delta_width = Convert.ToInt32(raw_img.Width / 2);
                //int twin_delta_height = Convert.ToInt32(raw_img.Height / 2);

                old_window = track_window;
                CvInvoke.MeanShift(pdf, ref track_window, term_crit);
                new_window = track_window;

                //x0,y0,_,_ = old_window;
                int x0 = old_window.X;
                int y0 = old_window.Y;
                ////x,y,w,w = new_window;
                int xNew = new_window.X;
                int yNew = new_window.Y;
                //w = new_window.Width;
                //if (new_window.X <= 0 || new_window.Y <= 0)
                //{
                //    MessageBox.Show("old_window.X: " + old_window.X.ToString() + " | old_window.Y" + old_window.Y.ToString() + "\nnew_window.X: " + new_window.X.ToString() + " | new_window.Y: " + new_window.Y.ToString());
                //}

                xNew = Convert.ToInt32(Convert.ToDouble((1 - filter) * xNew + filter * x0));
                yNew = Convert.ToInt32(Convert.ToDouble((1 - filter) * yNew + filter * y0));

                //if (xNew<=0 || yNew<=0)
                //{
                //    MessageBox.Show("xNew: " + xNew.ToString() + "yNew: " + yNew.ToString());
                //}
                //track_window = new System.Drawing.Rectangle(x, y, Convert.ToInt32(w), Convert.ToInt32(w));
                //x = x + d;
                //y = y + d;

                //pdf_roi=pdf[self.y-d:self.y+d, self.x-d:self.x+d]
                System.Drawing.Rectangle roiRect = new System.Drawing.Rectangle(xNew, yNew, w, w);
                //System.Windows.Forms.MessageBox.Show(roiRect.X + " | " + roiRect.Y + " | " + roiRect.Height + " | " + roiRect.Width);
                try
                {
                    if (roiRect.X <= 0 || roiRect.Y <= 0 || roiRect.Width <= 0 || roiRect.Height <= 0 || roiRect.X + roiRect.Width <= pdf.Cols || roiRect.Y + roiRect.Height <= pdf.Rows)
                    {
                        //Truncate the roi to fit the dimensions of image
                        roiRect = TruncateROI(roiRect, pdf.Width, pdf.Height);
                    }
                }
                catch(Exception e)
                {
                    MessageBox.Show(e.ToString());
                }
                try
                { 
                    Mat pdf_roi = new Mat(pdf, roiRect);

                    //pdf_roi.Save(@"C:\Pictures\pdfroi\T.bmp");
                    //ret, pdf_roi=cv2.threshold(pdf_roi,1,255,cv2.THRESH_BINARY)
                    CvInvoke.Threshold(pdf_roi, pdf_roi, 1, 255, ThresholdType.Binary);
                    //score=pdf_roi.sum()/(raw_img.Height*raw_img.Width);
                    MCvScalar scalar = CvInvoke.Sum(pdf_roi);  //The first vector (v0) is the only one populated because pdf is the red channel
                    double sum = scalar.V0;
                    //System.Windows.Forms.MessageBox.Show("Scalars: " + scalar.V0.ToString());

                    score = (sum / (Convert.ToDouble(raw_img.Height) * Convert.ToDouble(raw_img.Width)));
                }
                catch (Exception e)
                {
                    //System.Windows.Forms.MessageBox.Show(e.ToString() + "\n\n\n" + "roiRect X: " + roiRect.X.ToString() + "\nroiRect Y: " + roiRect.Y.ToString() + "\nroiRect.Width: " + roiRect.Width.ToString() + "\nroiRect.Height: " + roiRect.Height.ToString());
                    //System.Windows.Forms.MessageBox.Show(xNew.ToString() + " | " + yNew.ToString());

                    MessageBox.Show(e.ToString());
                }
                //System.Windows.Forms.MessageBox.Show(roiRect.Height + "*" + roiRect.Width+ "/" + raw_img.Height + "*" + raw_img.Width + "\nScore: " + score.ToString());

                if (score != 0 && score < min_score)
                {
                    state = "DEACTIVATED";
                }

                count += 1;
            }
        }

        public bool CheckOverlap(int i, int j)
        {
            double overlap_factor = 0.3;
            int dx = i - x;
            int dy = j - y;
            if (dx * dx + dy * dy < w * w * overlap_factor)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public System.Drawing.Rectangle TruncateROI(System.Drawing.Rectangle rect, int ImageWidth, int ImageHeight)
        {
            int newX = rect.X;
            int newY = rect.Y;
            int newWidth = rect.Width;
            int newHeight = rect.Height;

            if (rect.Width<=0)
            {
                newWidth = 1;
            }
            if (rect.Height<=0)
            {
                newHeight = 1;
            }

            if(rect.X + rect.Width >= ImageWidth)
            {
                newWidth = ImageWidth - rect.X - 1;
            }

            if(rect.Y + rect.Height >= ImageHeight)
            {
                newHeight = ImageHeight - rect.Y - 1;
            }

            if (rect.X <= 0)
            {
                newX = 1;
            }
            if (rect.Y <= 0)
            {
                newY = 1;
            }

            System.Drawing.Rectangle newRectangle = new System.Drawing.Rectangle(newX, newY, newWidth, newHeight);
            //string original = "Original - X:" + rect.X + " | Y:" + rect.Y + " | Width: " + rect.Width + " | Height: " + rect.Height + "ImageHeight:" + ImageHeight.ToString() + "ImageWidth: " + ImageWidth.ToString() + "\n";
            //string Truncated = "Truncated to: X:" + newX.ToString() + " | Y:" + newY.ToString() + " | Width:" + newWidth.ToString() + " | Height:" + newHeight.ToString();
            //MessageBox.Show(original + Truncated);

            return newRectangle;
        }
    }

    public class TrackerManager
    {
        //Members
        public List<Tracker> trackers;
        public List<Point> confirmed_list;
        public int num_trackers;
        public double decay_rate;
        public int skip_decay;
        public int left_wall;
        public int right_wall;
        public int wall_thickness;
        public int alert_level;
        public int count;
        public Mat GlobalPaths, wall;
        public MCvScalar color;

        public TrackerManager(int n)
        {
            num_trackers = n;
            decay_rate = 0.9;
            skip_decay = 1;
            left_wall = 725;
            right_wall = 825;
            wall_thickness = 2;
            alert_level = 100;
            // ^ 0 to 255 - the pixel on GlobalPaths that defines presence of manatee
            count = 0;
            confirmed_list = new List<Point>();
            trackers = new List<Tracker>();
            InitializeTrackers();
        }

        public void InitializeTrackers()
        {
            //for index in range(self.num_trackers):
            for (int i = 0; i < num_trackers; i++)
            {
                //  self.trackers.append(Tracker(index))
                trackers.Add(new Tracker(i));
            }
        }

        public void UpdateTrackers(Mat raw_img)
        {
            for (int i = 0; i < num_trackers; i++)
            {
                //  self.trackers[index].Update(raw_img, display_img)
                trackers[i].Update(raw_img);
            }

        }

        public Mat DrawActiveTrackersOnImage(Mat image)
        {
            for(int i = 0; i < num_trackers; i++)
            {
                if(trackers[i].state=="ACTIVATED")
                {
                    CvInvoke.Rectangle(image, new System.Drawing.Rectangle(trackers[i].x - (trackers[i].w/2), trackers[i].y - (trackers[i].w/2), trackers[i].w, trackers[i].w), new MCvScalar(255, 0, 0, 255), 2);
                    CvInvoke.Circle(image, new System.Drawing.Point(Convert.ToInt32(trackers[i].x), Convert.ToInt32(trackers[i].y)), 5, new MCvScalar(255, 0, 0, 255), -1);
                }
            }

            return image;
        }

        public void CreateTracker(int x, int y, int Width, int Height)
        {
            for (int i = 0; i < num_trackers; i++)
            {
                if (trackers[i].state == "DEACTIVATED")
                {
                    trackers[i].LockOn(x, y, Width, Height);
                    break;
                }
            }
        }

        public bool OnTheList(int x, int y)
        {
            foreach (Point p in confirmed_list)
            {
                if (p.X == x && p.Y == y)
                {
                    return true;
                }
            }
            return false;
        }

        public void RemoveRedundantTrackers()
        {
            for (int i = 0; i < num_trackers; i++)
            {
                if (trackers[i].state == "ACTIVATED")
                {
                    for (int j = i + 1; j < num_trackers; j++)
                    {
                        if (trackers[j].state == "DEACTIVATED")
                        {
                            continue;
                        }
                        else
                        {
                            int x = trackers[j].x;
                            int y = trackers[j].y;

                            if (trackers[i].CheckOverlap(x, y))
                            {
                                if (OnTheList(x, y))
                                {
                                    trackers[i].state = "DEACTIVATED";
                                    break;
                                }
                                else
                                {
                                    trackers[j].state = "DEACTIVATED";
                                }
                            }
                        }
                    }
                }
            }
        }

        public void RemoveTrackers(int x, int y)
        {
            for (int i = 0; i < num_trackers; i++)
            {
                if (trackers[i].state == "ACTIVATED")
                {
                    if (trackers[i].CheckOverlap(x, y))
                    {
                        trackers[i].state = "DEACTIVATED";
                    }
                }
            }
        }

        public void UpdateGlobalPaths()
        {
            if (count % skip_decay == 0)
            {
                CvInvoke.AddWeighted(GlobalPaths, decay_rate * .5, GlobalPaths, decay_rate * .5, -1, GlobalPaths);
            }
            wall = GlobalPaths;

            //if(wall>alert_level)
            //{
            //    color = new MCvScalar(0, 0, 255, 255);
            //}
            //else
            //{
            //    color = new MCvScalar(0, 255, 0, 255);
            //}

            //Draw Wall
            //cv2.line(GlobalPaths,(self.left_wall,0),(self.left_wall,720),color,self.wall_thickness)
            //cv2.line(GlobalPaths,(self.right_wall,0),(self.right_wall,720),color,self.wall_thickness)

            count++;
            count = count % 100;
        }

        public void AddToList(int x, int y)
        {
            confirmed_list.Add(new Point(x, y));
        }

        public void ClearList()
        {
            confirmed_list.Clear();
        }

        public void PrintTrackers(string boxString)
        {

            string messageString = null;

            messageString += boxString + "\n------------------------\n";
            for (int i = 0; i < num_trackers; i++)
            {
                messageString += "Tracker " + i + ": X = " + trackers[i].x + "| Y= " + trackers[i].y + "| STATE=" + trackers[i].state + "| SCORE=" + trackers[i].score + "\n";
            }

            System.Windows.MessageBox.Show(messageString);
        }

        public int countActiveTrackers()
        {
            int count = 0;

            for (int i = 0; i < num_trackers; i++)
            {
                if (trackers[i].state == "ACTIVATED")
                {
                    count++;
                }
            }

            return count;
        }
    }
}
