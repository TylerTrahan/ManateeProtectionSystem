using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Data.Entity;
using System.Data.SQLite;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using Emgu.CV;
using SQLite.CodeFirst;

namespace ManateeConsole
{
    internal class CaptureStorage : IDisposable
    {
        private static readonly Lazy<CaptureStorage> LazyInstance = new Lazy<CaptureStorage>(() => new CaptureStorage());
        public static CaptureStorage Instance => LazyInstance.Value;

        private readonly CaptureStorageContext _db;

        public string ImageFolder
        {
            get { return _imageFolderInfo.FullName; }
            set { _imageFolderInfo = string.IsNullOrEmpty(value) ? null : new DirectoryInfo(value); }
        }

        private DirectoryInfo _imageFolderInfo = new DirectoryInfo(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "capture"));

        private CaptureStorage()
        {
            _db = new CaptureStorageContext();
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
            _db.CaptureEntries.Add(capture);

            try
            {
                _db.SaveChanges();
            }
            catch (Exception e)
            {
                TryDeleteImage(capture.FilePath);
                throw new CaptureStorageException("Failed to add a capture to DB", e);
            }
        }

        public CaptureEntry[] GetCaptures(DateTime time, int msMargin)
        {
            var from = time.ToUniversalTime().AddMilliseconds(-msMargin);
            var to = time.ToUniversalTime().AddMilliseconds(msMargin);
            var captures = _db.CaptureEntries.Where(x => x.Timestamp >= from && x.Timestamp < to).ToArray();
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
                    throw new CaptureStorageException($"Failed to create Capture folder '{_imageFolderInfo.FullName}'", e);
                }
            }
            var now = DateTimeOffset.UtcNow;
            string fileName = $"{now.ToString("yyyyMMdd_HHmmssfffffff")}.{suffix}.bmp";
            var filePath = Path.Combine(_imageFolderInfo.FullName, fileName);
            try
            {
                image.Save(filePath);
            }
            catch (Exception e)
            {
                throw new CaptureStorageException($"Failed to save image to file '{filePath}'", e);
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

    class CaptureStorageContext : DbContext
    {
        public DbSet<CaptureEntry> CaptureEntries { get; set; }

        public CaptureStorageContext()
            : base("CaptureStorageContext")
        {
            Database.Log = s => Debug.WriteLine(s);
        }
        
        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            Database.SetInitializer(new SqliteCreateDatabaseIfNotExists<CaptureStorageContext>(modelBuilder));
        }
    }

    internal class CaptureEntry
    {
        [Key, Autoincrement]
        public long Id { get; set; }
        [Index]
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
