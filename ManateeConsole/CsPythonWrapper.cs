using System;
using CsPython.PythonRunTime;
using System.Reflection;
using System.Diagnostics;
using System.Collections.Generic;

namespace ManateeConsole
{


    class PythonMotionDetector : IDisposable
    {
        private PyObject UMatType { get; set; }
        private PyObject Cv2Module { get; set; }
        private PyObject PythonCodeModule { get; set; }


        static object initCallBacksLock = new object();
        static PyObject CallBackModule;
        static IntPtr CallBack(IntPtr self, IntPtr args)
        {
            var obj = new PyObject(args);
            return 0.ToPython().Handle;
        }

        static void InitCallBacks()
        {

            lock (initCallBacksLock)
            {
                if (CallBackModule != null) return;
                var dict = new Dictionary<string, Runtime.PyCFunction>();
                dict["nCallBack"] = CallBack;
                CallBackModule = new PyObject(PythonEngine.CreateModule("_NativeCallBack", dict));
                CallBackModule.KeepAlive();
            }
        }

        public PythonMotionDetector()
        {
            using (Py.GIL())
            {
                InitCallBacks();
                PythonCodeModule = PythonEngine.ModuleFromString("code" + DateTime.Now.Ticks, PythonCode);
                Cv2Module = ((dynamic)PythonCodeModule).cv2;
                UMatType = ((dynamic)Cv2Module).UMat;
                //to ensure the correct full path of the mask file set it through c# to python
                ((dynamic)PythonCodeModule).globalMaskPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "global_mask.png");
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        bool disposed;
        protected virtual void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (PythonEngine.IsPythonIsInitialized && !PythonEngine.IsFinalizing)
                {
                    IntPtr gs = PythonEngine.AcquireLock();
                    if (UMatType != null) UMatType.Dispose();
                    if (Cv2Module != null) Cv2Module.Dispose();
                    if (PythonCodeModule != null) PythonCodeModule.Dispose();

                    UMatType = null;
                    Cv2Module = null;
                    PythonCodeModule = null;
                    PythonEngine.ReleaseLock(gs);
                }
                disposed = true;
            }
        }
        //you may need to reset the motion detector
        //static PythonMotionDetector _inst;
        //public   static PythonMotionDetector Instance
        //{
        //    get
        //    {
        //        return (_inst ?? (_inst = new PythonMotionDetector()));
        //    }
        //}
        protected string SharedCode
        {
            get
            {
                return
                @"
from __future__ import print_function
import sys
PY3 = sys.version_info[0] == 3
from _NativeCallBack import nCallBack
if PY3:
    xrange = range

import numpy as np
import cv2
##

def asNumpy(x):
    if isinstance(x,np.ndarray):
        return x
    if isinstance(x,cv2.UMat):
        return x.get()
    raise Exception('not valid type: ' + str(type(x)))

def asUMat(x):
    if isinstance(x,cv2.UMat):
        return x
    elif isinstance(x,np.ndarray):
        return cv2.UMat(x)
    raise Exception('not valid type: ' + str(type(x)))

def toGray(image):
    return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
";
            }
        }
        protected string PythonCode
        {
            get
            {

                return SharedCode + @"
globalMaskPath = ''
class MotionDetector(object):

    def __init__(self):

        global globalMaskPath
        self.detector = cv2.createBackgroundSubtractorMOG2(history=2000, varThreshold=50, detectShadows=False)
        self.global_mask=cv2.imread(globalMaskPath,0)#0 for grayscale
        self.received_first_image = False
        self.alpha = .35
        self.grayF = None
        self.motionF = None

    def Update(self, image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)


        #for now mask does not fit the target image size:
        #gray[self.global_mask == 0] = 0

        if (self.received_first_image == False):
            self.grayF = gray.copy()
            motion = self.detector.apply(self.grayF)
            self.motionF = motion.copy()
            self.received_first_image = True
        else:
            self.grayF = cv2.addWeighted(gray, 1 - self.alpha, self.grayF, self.alpha, 0)
            motion = self.detector.apply(self.grayF)
            self.motionF = cv2.addWeighted(motion, 1 - self.alpha, self.motionF, self.alpha, 0)
        
        
        composite = cv2.merge([gray, gray, self.motionF])

        #for debugging perpose
        #nCallBack(('image shape:',image.shape,'composed shape:',composite.shape))

        #vis = np.concatenate((image, composite), axis = 1)
        #return vis
        return composite


motion_detector=None
def InPythonProcessImage(image):
    global motion_detector
    image = asNumpy(image)
    if not motion_detector :
        motion_detector=MotionDetector()
    res = motion_detector.Update(image)
    return asUMat(res)

";
            }
        }

        int PyUMatOffset
        {
            get
            {
                return TypeOffset.ob_type + IntPtr.Size;
            }
        }

        /// <summary>
        /// create cv2.UMat from managed c# Emgu::CV::UMat native OpenCV UMat object
        /// </summary>
        /// <param name="openCVUMat"></param>
        /// <returns></returns>
        private unsafe PyObject NewPyUMat(IntPtr openCVUMat)
        {
            IntPtr py = PythonEngine.GenericPyObjectAlloc(UMatType.Handle, 0);
            *(IntPtr*)((byte*)py + PyUMatOffset) = openCVUMat;

            return new PyObject(py);
        }
        public unsafe void RemoveUMatData(IntPtr pyMatObject)
        {
            *(IntPtr*)((byte*)pyMatObject + PyUMatOffset) = IntPtr.Zero;
        }
        private unsafe IntPtr GetNativeUMatFromPython(PyObject py)
        {
            return *(IntPtr*)((byte*)py.Handle + PyUMatOffset);
        }

        private PyObject PythonProcessImage(PyObject numpArray)
        {
            PyObject proc = ((dynamic)PythonCodeModule).InPythonProcessImage(numpArray);
            Runtime.CheckExceptionOccurred();
            return proc;
        }

        private global::Emgu.CV.UMat NativeUMatToManaged(IntPtr umat)
        {
            var flags = BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Public;
            var ctor = typeof(global::Emgu.CV.UMat).GetConstructor(flags, null, new[] { typeof(IntPtr), typeof(bool) }, null);
            var umatMngd = (global::Emgu.CV.UMat)ctor.Invoke(new object[] { umat, false });
            // GC.SuppressFinalize(umatMngd);
            return umatMngd;
        }
        public global::Emgu.CV.UMat ProcessImage(global::Emgu.CV.UMat mat)
        {
            global::Emgu.CV.UMat r = null;
            using (var uMat = NewPyUMat(mat.Ptr))
            {
                using (PyObject pyMat = PythonProcessImage(uMat))
                {

                    var uMatPtr = GetNativeUMatFromPython(pyMat);

                    r = NativeUMatToManaged(uMatPtr).Clone();//.GetMat(Emgu.CV.CvEnum.AccessType.ReadWrite).Clone();
                }
                RemoveUMatData(uMat.Handle);
            }
            return r;
        }


        public global::Emgu.CV.UMat ProcessImage(global::Emgu.CV.Mat mat)
        {

            using (var umat = mat.GetUMat(Emgu.CV.CvEnum.AccessType.ReadWrite))
            {
                {
                    var result = ProcessImage(umat);
                    return result;
                }
            }
        }
    }

    class CsPythonWrapper
    {


#if Test
    class ToPython : IDisposable
        {
            private PyObject UMatType { get; set; }
            private PyObject Cv2Module { get; set; }
            private PyObject PythonCodeModule { get; set; }

            protected ToPython()
            {
                using (Py.GIL())
                {
                    PythonCodeModule = PythonEngine.ModuleFromString("code" + DateTime.Now.Ticks, PythonCode);
                    Cv2Module = ((dynamic)PythonCodeModule).cv2;
                    UMatType = ((dynamic)Cv2Module).UMat;
                }
            }

            static ToPython _inst;
            public static ToPython Instance
            {
                get
                {
                    return (_inst ?? (_inst = new ToPython()));
                }
            }
            protected string SharedCode
            {
                get
                {
                    return
                    @"
from __future__ import print_function
import sys
PY3 = sys.version_info[0] == 3

if PY3:
    xrange = range

import numpy as np
import cv2


def asNumpy(x):
    if isinstance(x,np.ndarray):
        return x
    if isinstance(x,cv2.UMat):
        return x.get()
    raise Exception('not valid type: ' + str(type(x)))

def asUMat(x):
    if isinstance(x,cv2.UMat):
        return x
    elif isinstance(x,np.ndarray):
        return cv2.UMat(x)
    raise Exception('not valid type: ' + str(type(x)))

def toGray(image):
    return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
";
                }
            }

            public virtual string PythonCode
            {
                get
                {
                    return SharedCode
+ @"

def make_image():
    img = np.zeros((500, 500), np.uint8)
    black, white = 0, 255
    for i in xrange(6):
        dx = int((i%2)*250 - 30)
        dy = int((i/2.)*150)

        if i == 0:
            for j in xrange(11):
                angle = (j+5)*np.pi/21
                c, s = np.cos(angle), np.sin(angle)
                x1, y1 = np.int32([dx+100+j*10-80*c, dy+100-90*s])
                x2, y2 = np.int32([dx+100+j*10-30*c, dy+100-30*s])
                cv2.line(img, (x1, y1), (x2, y2), white)

        cv2.ellipse( img, (dx+150, dy+100), (100,70), 0, 0, 360, white, -1 )
        cv2.ellipse( img, (dx+115, dy+70), (30,20), 0, 0, 360, black, -1 )
        cv2.ellipse( img, (dx+185, dy+70), (30,20), 0, 0, 360, black, -1 )
        cv2.ellipse( img, (dx+115, dy+70), (15,15), 0, 0, 360, white, -1 )
        cv2.ellipse( img, (dx+185, dy+70), (15,15), 0, 0, 360, white, -1 )
        cv2.ellipse( img, (dx+115, dy+70), (5,5), 0, 0, 360, black, -1 )
        cv2.ellipse( img, (dx+185, dy+70), (5,5), 0, 0, 360, black, -1 )
        cv2.ellipse( img, (dx+150, dy+100), (10,5), 0, 0, 360, black, -1 )
        cv2.ellipse( img, (dx+150, dy+150), (40,10), 0, 0, 360, black, -1 )
        cv2.ellipse( img, (dx+27, dy+100), (20,35), 0, 0, 360, white, -1 )
        cv2.ellipse( img, (dx+273, dy+100), (20,35), 0, 0, 360, white, -1 )
    return img



def ProcessImage(img):

    h, w = img.shape[:2]

    _, contours0, hierarchy = cv2.findContours( img.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours = [cv2.approxPolyDP(cnt, 3, True) for cnt in contours0]
    result = dict()
    result['r'] = img
    def update(levels):
        vis = np.zeros((h, w, 3), np.uint8)
        levels = levels - 3
        cv2.drawContours( vis, contours, (-1, 2)[levels <= 0], (128,255,255),
            3, cv2.LINE_AA, hierarchy, abs(levels) )
        cv2.imshow('contours', vis)
        result['r'] = vis

    update(3)
    cv2.createTrackbar( 'levels + 3', 'contours', 3, 7, update )
    cv2.imshow('image', img)
    cv2.waitKey()
    cv2.destroyAllWindows()
    r = result['r']
    del result['r']
    return r

";
                }
            }

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
            bool disposed;
            protected virtual void Dispose(bool disposing)
            {
                if (!disposed)
                {
                    if (PythonEngine.IsPythonIsInitialized && !PythonEngine.IsFinalizing)
                    {
                        IntPtr gs = PythonEngine.AcquireLock();
                        if (UMatType != null) UMatType.Dispose();
                        if (Cv2Module != null) Cv2Module.Dispose();
                        if (PythonCodeModule != null) PythonCodeModule.Dispose();

                        UMatType = null;
                        Cv2Module = null;
                        PythonCodeModule = null;
                        PythonEngine.ReleaseLock(gs);
                    }
                    disposed = true;
                }
            }

            int PyUMatOffset
            {
                get
                {
                    return TypeOffset.ob_type + IntPtr.Size;
                }
            }


            internal unsafe PyObject NewPyUMat(IntPtr emguUMat)
            {
                IntPtr py = PythonEngine.GenericPyObjectAlloc(UMatType.Handle, 0);
                *(IntPtr*)((byte*)py + PyUMatOffset) = emguUMat;
                return new PyObject(py);
            }
            internal unsafe IntPtr GetNativeUMatFromPython(PyObject py)
            {
                return *(IntPtr*)((byte*)py.Handle + PyUMatOffset);
            }




            internal global::Emgu.CV.UMat NativeUMatToManaged(IntPtr umat)
            {
                var flags = BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Public;
                var ctor = typeof(global::Emgu.CV.UMat).GetConstructor(flags, null, new[] { typeof(IntPtr), typeof(bool) }, null);
                var umatMngd = (global::Emgu.CV.UMat)ctor.Invoke(new object[] { umat, false });
                return umatMngd;
            }


            public PyObject MakeImage()
            {
                PyObject proc = ((dynamic)PythonCodeModule).make_image();
                Runtime.CheckExceptionOccurred();
                return proc;
            }

            public PyObject ProcessImage(PyObject numpArray)
            {
                PyObject proc = ((dynamic)PythonCodeModule).ProcessImage(numpArray);
                Runtime.CheckExceptionOccurred();
                return proc;
            }
            public PyObject AsUMat(PyObject mat)
            {
                PyObject proc = ((dynamic)PythonCodeModule).asUMat(mat);
                Runtime.CheckExceptionOccurred();
                return proc;
            }
            public PyObject ToGray(PyObject numPyArray)
            {
                PyObject proc = ((dynamic)PythonCodeModule).toGray(numPyArray);
                Runtime.CheckExceptionOccurred();
                return proc;
            }

        }
#endif


        [Conditional("DEBUG")]
        public static void TestPython()
        {
            var detector = new PythonMotionDetector();
            var step1 = @"C:\Users\VI-System2\Desktop\step1.PNG";
            var step2 = @"C:\Users\VI-System2\Desktop\step2.PNG";
            using (var mat1 = LoadImage(step1))
            using (var mat2 = LoadImage(step2))
            {
                try
                {
                    var m1 = detector.ProcessImage(mat1);
                    var m2 = detector.ProcessImage(mat2);
                    var imgs = new[] { m1, m2 };
                    foreach (var mi in imgs)
                    {
                        var f = new System.Windows.Forms.Form();
                        var viewVBox = new Emgu.CV.UI.ImageBox() { Dock = System.Windows.Forms.DockStyle.Fill };
                        viewVBox.Image = mi;
                        f.Controls.Add(viewVBox);
                        f.ShowDialog();
                    }
                }
                catch (PythonException pex)
                { }
                catch (Exception ex)
                { }

            }
        }
        static
                Emgu.CV.Mat LoadImage(string path)
        {

            using (var bitmap = (System.Drawing.Bitmap)System.Drawing.Image.FromFile(path))
            using (var image = new Emgu.CV.Image<Emgu.CV.Structure.Bgr, byte>(bitmap))
                return image.Mat.Clone();//.get(Emgu.CV.CvEnum.AccessType.ReadWrite);

        }
#if TEST
        [Conditional("DEBUG")]
        static void Test2()
        {
            var path = @"C:\Users\VI-System2\Desktop\Capture.PNG";
            using (var bitmap = (System.Drawing.Bitmap)System.Drawing.Image.FromFile(path))
            using (var image = new Emgu.CV.Image<Emgu.CV.Structure.Bgr, byte>(bitmap))
            using (var mat = image.Mat.GetUMat(Emgu.CV.CvEnum.AccessType.ReadWrite))
                for (int i = 0; i < 10; i++)
                    using (var uMat = ToPython.Instance.NewPyUMat(mat.Ptr))
                    {
                        using (var pyNumpArray = ((dynamic)uMat).get())
                        {
                            var shape1 = pyNumpArray.shape;
                            using (var gray = ToPython.Instance.ToGray(pyNumpArray))
                            {
                                var shape2 = gray.shape;
                                using (PyObject proc = ToPython.Instance.ProcessImage(gray))
                                using (PyObject pyMat = ToPython.Instance.AsUMat(proc))
                                {
                                    var uMatPtr = ToPython.Instance.GetNativeUMatFromPython(pyMat);
                                    using (var umat = ToPython.Instance.NativeUMatToManaged(uMatPtr))
                                    {
                                        //  this.imageBox.Image = umat.Clone();
                                    }
                                }
                            }
                        }
                    }
        }
#endif
        [Conditional("DEBUG")]
        static void Test1()
        {

            // I would prefere vs 2017 since it has c#7 features 
            //CsPython.PythonRunTime is my customized version of pythonet where python net has so many translation between native and managed code causing many performance issues
            using (Py.GIL())
            {
                try
                {
                    dynamic embededPythonModule = PythonEngine.ModuleFromString("testcod", "import cv2");
                    var cv2 = embededPythonModule.cv2;

                    //testing if cv 2 
                    var cvVersion = cv2.__version__;

                }
                catch (PythonException pexp)
                {


                }
                catch (Exception exp)
                {


                }
            }
        }
    }
}
