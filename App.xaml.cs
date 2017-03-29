using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

using SciChart.Charting.Visuals;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {

        public App()
        {
            // Ensure SetLicenseKey is called once, before any SciChartSurface instance is created 
            // Check this code into your version-control and it will enable SciChart 
            // for end-users of your application who are not activated
            SciChartSurface.SetRuntimeLicenseKey(@"<LicenseContract>
        <Customer>Aqua-Tech Services</Customer>
        <OrderId>ABT170324-5714-19111</OrderId>
        <LicenseCount>1</LicenseCount>
        <IsTrialLicense>false</IsTrialLicense>
        <SupportExpires>06/22/2017 00:00:00</SupportExpires>
        <ProductCode>SC-WPF-2D-PRO</ProductCode>
        <KeyCode>lwAAAAEAAABaO0bgPaTSAXIAQ3VzdG9tZXI9QXF1YS1UZWNoIFNlcnZpY2VzO09yZGVySWQ9QUJUMTcwMzI0LTU3MTQtMTkxMTE7U3Vic2NyaXB0aW9uVmFsaWRUbz0yMi1KdW4tMjAxNztQcm9kdWN0Q29kZT1TQy1XUEYtMkQtUFJPEuj4oHbI0Yf9/do2jRti4XEr61r2r8rlCdLgd2IUWMDcFNrdqQhh9gaqkVVNkpEC</KeyCode>
        </LicenseContract>");
        }
        private void App_OnStartup(object sender, StartupEventArgs e)
        {
            MainWindow = new MainWindow();
            MainWindow.Show();
        }
    }
}
