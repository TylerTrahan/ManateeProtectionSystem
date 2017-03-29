///
/// This file has been generated by bvtidl.pl. DO NOT MODIFY!
///

using System;
using System.Runtime.InteropServices;


namespace BVTSDK
{
[StructLayout(LayoutKind.Sequential)]
public class BVTProfilePoint
{
    float range;
    float bearing;
    float x;
    float y;
    float z;
    int delay;
    UInt16 intensity;
    UInt16 valid;
};

/// <summary>
/// @warning RangeProfile functions will fail on a sonar with old firmware, or a file recorded from a sonar with old firmware.
/// The RangeProfile interface provides a 1-D view of a single ping data. It consists of a vector of ranges from the sonar head to the sonic reflection point, indexed along the the bearing (theta) dimension.
/// For each angle, the range and raw intensity of the return beam at that range is stored. There are a number of approaches to choosing the reflection point. This interface provides a settable minimum intensity threshold that must be crossed and a choosable algorithm, returning either the first (nearest) point that exceeds the threshold (THRESHOLD_POLICY_NEAREST) or the distance to sample of greatest intensity (THRESHOLD_POLICY_LARGEST).
/// </summary>
    public class RangeProfile : IDisposable
    {

        internal RangeProfile(IntPtr p)
        {
            _handle = new HandleRef(this, p);
        }

        /// Non-deterministic destruction of this object on the Finalizer thread
        ~RangeProfile()
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
            NativeMethods.BVTRangeProfile_Destroy(_handle);
        }

        /// <summary>
        /// Values greater than this indicate no range could be measured. 
        /// </summary>
        public const int MAX_RANGE = 999;

        /// <summary>
        /// Return the largest intensity greater than the threshold found along a beam. 
        /// </summary>
        public const int THRESHOLD_POLICY_LARGEST = 1;

        /// <summary>
        /// Return the nearest intensity greater than the threshold along a beam. 
        /// </summary>
        public const int THRESHOLD_POLICY_NEAREST = 2;

        /// <summary>
        /// Returns the number of range values stored for this ping.
        /// </summary>
        /// <returns>range data entry count</returns>       
        public int Count
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                int count;
                int error_code = NativeMethods.BVTRangeProfile_GetCount(_handle, out count);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return count;
            }
        }

        /// <summary>
        /// Returns the number of valid range values stored for this ping.
        /// </summary>
        /// <returns>range data entry count</returns>       
        public int ValidCount
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                int count;
                int error_code = NativeMethods.BVTRangeProfile_GetValidCount(_handle, out count);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return count;
            }
        }

        /// <summary>
        /// Returns the intensity threshold used to populate this RangeProfile structure. The intensity threshold serves as a noise floor, below which no sample will be considered a candidate for the beam's RangeProfile point. 
        /// </summary>
        /// <returns>raw intensity threshold</returns>      
        public UInt16 IntensityThreshold
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                UInt16 threshold;
                int error_code = NativeMethods.BVTRangeProfile_GetIntensityThreshold(_handle, out threshold);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return threshold;
            }
        }

        /// <summary>
        /// Returns the resolution of the range values, in meters. The RangeProfile range value at a given bearing should be considered approximate to within +- resolution.
        /// </summary>
        /// <returns>range resolution</returns>     
        public double RangeResolution
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                double resolution;
                int error_code = NativeMethods.BVTRangeProfile_GetRangeResolution(_handle, out resolution);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return resolution;
            }
        }

        /// <summary>
        /// Returns the resolution of the bearing (in degrees) of each RangeProfile range value. This is the difference in bearing between adjacent range values in the array.
        /// <br>
        /// </summary>
        /// <returns>bearing (angular) resolution</returns>     
        public double BearingResolution
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                double resolution;
                int error_code = NativeMethods.BVTRangeProfile_GetBearingResolution(_handle, out resolution);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return resolution;
            }
        }

        /// <summary>
        /// Return the minimum angle for the sonar's imaging field of view.
        /// This is the angle of the first range value, as all
        /// angles are "left referenced." The angle is returned in degrees.
        /// Note that this may not represent the actual physical field of view
        /// of a particular sonar, but does represent the field of view of the
        /// data being returned. Some outer angles may have range values
        /// indicating they are out of range.
        /// </summary>
        /// <returns>minimum angle in field-of-view</returns>       
        public float FOVMinAngle
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                float minAngle;
                int error_code = NativeMethods.BVTRangeProfile_GetFOVMinAngle(_handle, out minAngle);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return minAngle;
            }
        }

        /// <summary>
        /// Return the maximum angle for the sonar's imaging field of view.
        /// This is the angle of the last range value, as all
        /// angles are "left referenced." The angle is returned in degrees.
        /// Note that this may not represent the actual physical field of view
        /// of a particular sonar, but does represent the field of view of the
        /// data being returned. Some outer angles may have range values
        /// indicating they are out of range.
        /// </summary>
        /// <returns>maximum angle in field-of-view</returns>       
        public float FOVMaxAngle
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
                float maxAngle;
                int error_code = NativeMethods.BVTRangeProfile_GetFOVMaxAngle(_handle, out maxAngle);
                GC.KeepAlive(this);
                if (0 != error_code)
                    throw new SdkException(error_code);
                return maxAngle;
            }
        }

        /// <summary>
        /// Copies the range values into the user specified buffer. The
        /// buffer must hold the entire number of ranges (See Count() above),
        /// or an error is returned.
        /// </summary>
        /// <param name="ranges">Pointer to a valid buffer of type float.</param>
        /// <param name="number_of_ranges">Number of values the buffer can hold.</param>        
        public void CopyRangeValues(float[] ranges, int number_of_ranges)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int error_code = NativeMethods.BVTRangeProfile_CopyRangeValues(_handle, ranges, number_of_ranges);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
        }

        /// <summary>
        /// Copies the bearing values into the user specified buffer. The
        /// buffer must hold the entire number of bearings (See Count() above),
        /// or an error is returned.
        /// </summary>
        /// <param name="bearings">Pointer to a valid buffer of type float.</param>
        /// <param name="number_of_bearings">Number of values the buffer can hold.</param>      
        public void CopyBearingValues(float[] bearings, int number_of_bearings)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int error_code = NativeMethods.BVTRangeProfile_CopyBearingValues(_handle, bearings, number_of_bearings);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
        }

        /// <summary>
        /// Copies the intensity values into the user specified buffer. The
        /// buffer must hold the entire number of intensities (See Count() above),
        /// or an error is returned.
        /// </summary>
        /// <param name="intensities">Pointer to a valid buffer of type float.</param>
        /// <param name="number_of_intensities">Number of values the buffer can hold.</param>       
        public void CopyIntensityValues(UInt16[] intensities, int number_of_intensities)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int error_code = NativeMethods.BVTRangeProfile_CopyIntensityValues(_handle, intensities, number_of_intensities);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
        }

        /// <summary>
        /// Returns the range from the sonar head, in meters, at a particular
        /// index into the array. <br>
        /// NOTE: Check all returned values for validity. If range > BVTRANGEPROFILE_MAX_RANGE
        /// then the range at the given bearing (index) is not valid.
        /// This is the result of either the nearest object at the given bearing was out of view of the sonar, or no return along that beam crossed the specified threshold.
        /// </summary>
        /// <param name="index">index into the array of RangeProfile values  </param>
        /// <returns>range at given index </returns>        
        public float GetRangeValue(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            float range;
            int error_code = NativeMethods.BVTRangeProfile_GetRangeValue(_handle, index, out range);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return range;
        }

        /// <summary>
        /// Returns the intensity value at the specified index into the RangeProfile array. <br>
        /// </summary>
        /// <param name="index">index into the array of RangeProfile values  </param>
        /// <returns>raw intensity value </returns>     
        public UInt16 GetIntensityValue(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            UInt16 intensity;
            int error_code = NativeMethods.BVTRangeProfile_GetIntensityValue(_handle, index, out intensity);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return intensity;
        }

        /// <summary>
        /// Returns the bearing from the center of the sonar head, in degrees (positive is clockwise as viewed from above) at the given index into the RangeProfile array.
        /// </summary>
        /// <param name="index">index into the array of RangeProfile values  </param>
        /// <returns>bearing (angle) at given index </returns>      
        public float GetBearingValue(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            float bearing;
            int error_code = NativeMethods.BVTRangeProfile_GetBearingValue(_handle, index, out bearing);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return bearing;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="points">an array to be filled with points on the profile</param>
        /// <param name="number_of_points">the number of points to return (shall not be greater then result of GetCount) </param>       
        public void CopyProfileValues(BVTProfilePoint[]  points, int number_of_points)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int error_code = NativeMethods.BVTRangeProfile_CopyProfileValues(_handle, points, number_of_points);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="points">an array to be filled with valid points on the profile</param>
        /// <param name="number_of_points">the number of points to return (shall not be greater then result of GetValidCount) </param>      
        public void CopyValidProfileValues(BVTProfilePoint[]  points, int number_of_points)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int error_code = NativeMethods.BVTRangeProfile_CopyValidProfileValues(_handle, points, number_of_points);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
        }

        /// <summary>
        /// Returns the x position seen from the center of the sonar head.
        /// </summary>
        /// <param name="index">index into the array of values  </param>
        /// <returns>x at given index </returns>        
        public float Get_X_Value(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            float x;
            int error_code = NativeMethods.BVTRangeProfile_Get_X_Value(_handle, index, out x);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return x;
        }

        /// <summary>
        /// Returns the y position seen from the center of the sonar head.
        /// </summary>
        /// <param name="index">index into the array of values  </param>
        /// <returns>y at given index </returns>        
        public float Get_Y_Value(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            float y;
            int error_code = NativeMethods.BVTRangeProfile_Get_Y_Value(_handle, index, out y);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return y;
        }

        /// <summary>
        /// Returns the z position seen from the center of the sonar head.
        /// </summary>
        /// <param name="index">index into the array of values  </param>
        /// <returns>z at given index </returns>        
        public float Get_Z_Value(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            float z;
            int error_code = NativeMethods.BVTRangeProfile_Get_Z_Value(_handle, index, out z);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return z;
        }

        /// <summary>
        /// Returns the time (in ms) relative to the first sub ping of the sonar head.
        /// </summary>
        /// <param name="index">index into the array of values  </param>
        /// <returns>time at given index </returns>     
        public int GetTimeValue(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int time;
            int error_code = NativeMethods.BVTRangeProfile_GetTimeValue(_handle, index, out time);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return time;
        }

        /// <summary>
        /// Returns a bool telling if the given index in the value array is valid
        /// </summary>
        /// <param name="index">index into the array of values  </param>
        /// <returns>valid at given index </returns>        
        public bool GetValidValue(int index)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int valid;
            int error_code = NativeMethods.BVTRangeProfile_GetValidValue(_handle, index, out valid);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return valid > 0;
        }

        /// <summary>
        /// Returns the X coordinate for the pixel in the passed ColorImage, which
        /// maps to the range and bearing at the index passed. This allows placing
        /// of the range data on a colorimage, easing analysis of the algorithm
        /// used for thresholding.
        /// </summary>
        /// <param name="index">index into the array of RangeProfile values  </param>
        /// <param name="image">ColorImage object where the pixel coordinate is needed </param>
        /// <returns>pixel index in the X axis </returns>       
        public int GetColorImagePixelX(int index, ColorImage image)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int x;
            int error_code = NativeMethods.BVTRangeProfile_GetColorImagePixelX(_handle, index, image.Handle, out x);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return x;
        }

        /// <summary>
        /// Returns the Y coordinate for the pixel in the passed ColorImage which
        /// maps to the range and bearing at the index passed. (see similar function,
        /// above, for more details)
        /// </summary>
        /// <param name="index">index into the array of RangeProfile values  </param>
        /// <param name="image">ColorImage object where the pixel coordinate is needed </param>
        /// <returns>pixel index in the Y axis</returns>        
        public int GetColorImagePixelY(int index, ColorImage image)
        {
            if (_disposed) throw new ObjectDisposedException("BVTRangeProfile");
            int y;
            int error_code = NativeMethods.BVTRangeProfile_GetColorImagePixelY(_handle, index, image.Handle, out y);
            GC.KeepAlive(this);
            if (0 != error_code)
                throw new SdkException(error_code);
            return y;
        }


        /// SDK object pointer
        internal HandleRef Handle
        {
            get { return _handle; }
        }
        private readonly HandleRef _handle;

        private bool _disposed;

        /// P/Invoke 'wrapper' class
        private static class NativeMethods
        {
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern IntPtr BVTRangeProfile_Destroy(HandleRef obj);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetCount(HandleRef obj, out int count);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetValidCount(HandleRef obj, out int count);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetIntensityThreshold(HandleRef obj, out UInt16 threshold);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetRangeResolution(HandleRef obj, out double resolution);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetBearingResolution(HandleRef obj, out double resolution);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetFOVMinAngle(HandleRef obj, out float minAngle);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetFOVMaxAngle(HandleRef obj, out float maxAngle);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_CopyRangeValues(HandleRef obj, float[] ranges, int number_of_ranges);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_CopyBearingValues(HandleRef obj, float[] bearings, int number_of_bearings);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_CopyIntensityValues(HandleRef obj, UInt16[] intensities, int number_of_intensities);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetRangeValue(HandleRef obj, int index, out float range);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetIntensityValue(HandleRef obj, int index, out UInt16 intensity);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetBearingValue(HandleRef obj, int index, out float bearing);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_CopyProfileValues(HandleRef obj, BVTProfilePoint[]  points, int number_of_points);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_CopyValidProfileValues(HandleRef obj, BVTProfilePoint[]  points, int number_of_points);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_Get_X_Value(HandleRef obj, int index, out float x);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_Get_Y_Value(HandleRef obj, int index, out float y);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_Get_Z_Value(HandleRef obj, int index, out float z);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetTimeValue(HandleRef obj, int index, out int time);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetValidValue(HandleRef obj, int index, out int valid);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetColorImagePixelX(HandleRef obj, int index, HandleRef image, out int x);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTRangeProfile_GetColorImagePixelY(HandleRef obj, int index, HandleRef image, out int y);

        }
    }

}
