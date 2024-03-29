///
/// This file has been generated by bvtidl.pl. DO NOT MODIFY!
///

using System;
using System.Runtime.InteropServices;


namespace BVTSDK
{
/// <summary>
/// The SDK is capable of producing a significant amount of debugging 
/// output.  The Logger object exists to allow the user to control 
/// (or disable) the output.  Users can also use Logger to add their
/// own custom log messages.
/// </summary>
    public class Logger : IDisposable
    {

        internal Logger(IntPtr p)
        {
            _handle = new HandleRef(this, p);
        }

        /// Non-deterministic destruction of this object on the Finalizer thread
        ~Logger()
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
            NativeMethods.BVTLogger_Destroy(_handle);
        }

        /// <summary>
        /// 
        /// </summary></returns>        
        public static Logger Create
        {
            get
            {
                IntPtr logger_ptr = new IntPtr();
                int error_code = NativeMethods.BVTLogger_Create(ref logger_ptr);
                if (0 != error_code)
                    throw new SdkException(error_code);
                Logger logger = new Logger(logger_ptr);
                return logger;
            }
        }

        /// <summary>
        /// Set the log threshold level.  Events above level
        /// will be logged to the target.
        /// </summary>
        /// <param name="level">Log level </param>      
        public static void SetLevel(LogLevel level)
        {
            NativeMethods.BVTLogger_SetLevel(((int) level));
        }

        /// <summary>
        /// The log target can be a filename, "stdout", "stderr", or "null".
        /// If null is specified, log output is disabled.
        /// </summary>
        /// <param name="target">File/device to log output to </param>      
        public static void SetTarget(string target)
        {
            int error_code = NativeMethods.BVTLogger_SetTarget(BVTSDK.InteropHelpers.StringToUtf8Ptr(target));
            if (0 != error_code)
                throw new SdkException(error_code);
        }

        /// <summary>
        /// Write a message to the log.
        /// </summary>
        /// <param name="level">The log level of the message </param>
        /// <param name="message">A string to log </param>      
        public static void Write(LogLevel level, string message)
        {
            NativeMethods.BVTLogger_Write(((int) level), BVTSDK.InteropHelpers.StringToUtf8Ptr(message));
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
            public static extern IntPtr BVTLogger_Destroy(HandleRef obj);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTLogger_Create(ref IntPtr logger);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern void BVTLogger_SetLevel(int level);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern int BVTLogger_SetTarget(byte[] target);
            [DllImport("bvtsdk4", CallingConvention=CallingConvention.Cdecl)]
            public static extern void BVTLogger_Write(int level, byte[] message);

        }
    }

}
