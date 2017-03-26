using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Configuration;
using System.Data.Common;
using System.Data.Entity;
using System.Data.SQLite;
using System.Data.SQLite.Linq;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using Emgu.CV;

namespace ManateeConsole
{
    internal class CaptureStorage : IDisposable
    {
        private static readonly Lazy<CaptureStorage> LazyInstance = new Lazy<CaptureStorage>(() => new CaptureStorage());
        public static CaptureStorage Instance { get { return LazyInstance.Value; } }

        private readonly DbConnection _db;
        private readonly object _dbLock = new object();

        public string ImageFolder
        {
            get { return _imageFolderInfo.FullName; }
            set { _imageFolderInfo = string.IsNullOrEmpty(value) ? null : new DirectoryInfo(value); }
        }

        private DirectoryInfo _imageFolderInfo;

        private CaptureStorage()
        {
            ImageFolder = ConfigurationManager.AppSettings["CaptureStorageFolder"];
            try
            {
                _db = SQLiteProviderFactory.Instance.CreateConnection();
                Debug.Assert(_db != null);
                _db.ConnectionString = ConfigurationManager.ConnectionStrings["CaptureStorageDB"].ConnectionString;
                _db.Open();
                using (var command = _db.CreateCommand())
                {
                    command.CommandText =
                        "CREATE TABLE IF NOT EXISTS CaptureEntries ([Id] integer PRIMARY KEY AUTOINCREMENT, [Timestamp] datetime NOT NULL, [GateState] int NOT NULL, [Source] int NOT NULL, [FilePath] nvarchar);" +
                        "CREATE INDEX IF NOT EXISTS IX_Captures_Timestamp ON CaptureEntries (Timestamp);";
                    command.ExecuteNonQuery();
                }
            }
            catch (Exception e)
            {
                throw new CaptureStorageException("Failed to initialize DB", e);
            }
        }

        public void AddCapture(IImage image, CaptureSource source, GateState state = GateState.Open, DateTime? timestamp = null)
        {
            var capture = new CaptureEntry()
            {
                Timestamp = timestamp ?? DateTime.UtcNow,
                GateState = state,
                Source = source,
                FilePath = SaveImage(image, source.ToString()),
            };

            var command = _db.CreateCommand();
            command.CommandText =
                string.Format(
                    "INSERT INTO CaptureEntries (Timestamp, GateState, Source, FilePath) VALUES ('{0}Z', {1}, {2}, '{3}')",
                    capture.Timestamp.ToString("yyyy-MM-dd HH:mm:ss.fffffff"), (int)capture.GateState, (int)capture.Source, capture.FilePath);
            try
            {
                lock (_dbLock)
                {
                    command.ExecuteNonQuery();
                }
            }
            catch (Exception e)
            {
                TryDeleteImage(capture.FilePath);
                throw new CaptureStorageException("Failed to add a capture to DB", e);
            }
            finally
            {
                command.Dispose();
            }
        }

        public CaptureEntry[] GetCaptures(DateTime time, int msMargin)
        {
            var from = time.ToUniversalTime().AddMilliseconds(-msMargin);
            var to = time.ToUniversalTime().AddMilliseconds(msMargin);
            List<CaptureEntry> captures = new List<CaptureEntry>();
            var command = _db.CreateCommand();
            command.CommandText =
                string.Format(
                    "SELECT Id, Timestamp, GateState, Source, FilePath FROM CaptureEntries WHERE Timestamp >= '{0}Z' AND Timestamp < '{1}Z'",
                    from.ToString("yyyy-MM-dd HH:mm:ss.fffffff"), to.ToString("yyyy-MM-dd HH:mm:ss.fffffff"));
            try
            {
                lock (_dbLock)
                {
                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            var capture = new CaptureEntry()
                            {
                                Id = reader.GetInt64(0),
                                Timestamp = reader.GetDateTime(1),
                                GateState = (GateState)reader.GetInt32(2),
                                Source = (CaptureSource)reader.GetInt32(3),
                                FilePath = reader.GetString(4)
                            };
                            captures.Add(capture);
                        }    
                    }
                }
            }
            catch (Exception e)
            {
                throw new CaptureStorageException("Failed to retrive captures from DB", e);
            }
            finally
            {
                command.Dispose();
            }
            return captures.GroupBy(x => x.Source, (source, entries) => entries.FirstOrDefault()).ToArray();
        }

        #region IO

        private string SaveImage(IImage image, string suffix)
        {
            if (image == null) return null;

            if (_imageFolderInfo == null) throw new CaptureStorageException("Capture folder isn't set up");

            if (!_imageFolderInfo.Exists)
            {
                try
                {
                    _imageFolderInfo.Create();
                }
                catch (Exception e)
                {
                    throw new CaptureStorageException(string.Format("Failed to create Capture folder '{0}'", _imageFolderInfo.FullName), e);
                }
            }
            var now = DateTimeOffset.UtcNow;
            string fileName = String.Format("{0}.{1}.bmp", now.ToString("yyyyMMdd_HHmmssfffffff"), suffix);
            var filePath = Path.Combine(_imageFolderInfo.FullName, fileName);
            try
            {
                image.Save(filePath);
            }
            catch (Exception e)
            {
                throw new CaptureStorageException(String.Format("Failed to save image to file '{0}'", filePath), e);
            }
            return filePath;
        }

        private bool TryDeleteImage(string path)
        {
            try
            {
                DeleteImage(path);
                return true;
            }
            catch
            {
                return false;
            }
        }

        private void DeleteImage(string path)
        {
            if (string.IsNullOrEmpty(path)) return;
            var file = new FileInfo(path);
            if (!file.Exists) return;
            file.Delete();
        }

        #endregion

        #region Disposable     

        private bool _disposed = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!this._disposed)
            {
                if (disposing)
                {
                    _db.Dispose();
                }
            }
            this._disposed = true;
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        
        #endregion
    }

    class CaptureStorageException : ApplicationException
    {
        public CaptureStorageException()
        {
        }

        public CaptureStorageException(string message) : base(message)
        {
        }

        public CaptureStorageException(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected CaptureStorageException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        }
    }

    internal class CaptureEntry
    {
        [Key]
        public long Id { get; set; }
        public DateTime Timestamp { get; set; }
        public GateState GateState { get; set; }
        public CaptureSource Source { get; set; }
        public string FilePath { get; set; }
    }

    internal enum CaptureSource
    {
        Sonar1,
        Sonar2,
        Camera1,
        Camera2,
        IpCamera1,
        IpCamera2
    }

    internal enum GateState
    {
        Open,
        Opening,
        Closed,
        Closing
    }
}
