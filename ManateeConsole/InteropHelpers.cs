using System;

namespace BVTSDK
{
    internal static class InteropHelpers
    {
        /// <summary>
        /// Convert a source array of UTF-8 bytes to the runtime's string encoding
        /// </summary>
        /// <param name="src"></param>
        /// <returns></returns>
        internal static string Utf8ToString(byte[] src)
        {
            const char null_terminator = '\0';
            int i;
            for (i = 0; i < src.Length; ++i )
            {
                if (src[i] == null_terminator)
                    break;
            }

            return System.Text.Encoding.UTF8.GetString(src, 0, i);
        }

        /// <summary>
        /// Convert a (possibly native) pointer to UTF-8 bytes to the runtime's string encoding
        /// </summary>
        /// <param name="utf8"></param>
        /// <returns></returns>
        internal static string Utf8ToString(IntPtr utf8)
        {
            if (utf8 == IntPtr.Zero)
                return string.Empty;

            int utf8_len = 0;
            const char null_terminator = '\0';
            while (System.Runtime.InteropServices.Marshal.ReadByte(utf8, utf8_len) != null_terminator)
                utf8_len++;

            if (utf8_len == 0)
                return string.Empty;

            byte[] dst = new byte[utf8_len];
            System.Runtime.InteropServices.Marshal.Copy(utf8, dst, 0, utf8_len);
            return System.Text.Encoding.UTF8.GetString(dst, 0, utf8_len);
        }
        /// <summary>
        /// Convert a string (in whatever the runtime's encoding is) to an array of UTF-8 bytes
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        internal static byte[] StringToUtf8Ptr(string str)
        {
            var runtimeEncoding = System.Text.Encoding.Unicode;
            byte[] unicodeBytes = runtimeEncoding.GetBytes(str);

            byte[] utf8 = System.Text.Encoding.Convert(runtimeEncoding,
                                           System.Text.Encoding.UTF8,
                                           unicodeBytes);
            return utf8;
        }
    }
}
