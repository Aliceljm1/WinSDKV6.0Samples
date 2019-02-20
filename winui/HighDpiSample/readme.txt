This sample demonstrates a process calling SetProcessDPIAware which tells
the system that it will do its own scaling when the DPI is high (above the
default of 96) and that the window manager should not apply scaling.  This
application actually does no scaling, so the window drawn before
SetProcessDPIAware is called will look different from the window drawn
afterwards if the DPI is high.

To build this application use the VSS build environment.

To run this application type HighDpiSample.exe from the command line.