# SetDPI

[![image](https://user-images.githubusercontent.com/79026235/152910441-59ba653c-5607-4f59-90c0-bc2851bf2688.png)Download the zip file](https://github.com/LesFerch/SetDPI/releases/download/1.1.0/SetDPI.zip)

This is a sample and simple C++ console project to change windows Scale and Layout aka DPI scaling from command line parameters. It can work on single and multi-monitor setup.

This is a fork of https://github.com/imniko/SetDPI that supports an additional optional argument to restart Explorer. Also, the provided Exe has been compiled to work on both 32 bit and 64 bit Windows builds.

# How to Download and Run

1. Download the zip file using the link above.
2. Extract **SetDPI.exe**.
3. Open a Cmd prompt and run **SetDPI.exe** with the appropriate arguments to set .
5. Use **SetDPI.exe** from the Cmd prompt, or from your own script, to set SetDPIaper and/or background color.

**Note**: Some antivirus software may falsely detect the download as a virus. This can happen anytime you download a new executable and may require extra steps to whitelist the file. In testing, no issues were encountered using Windows Defender on Windows 10 and 11, but a false positive "virus detected" occurred on Windows Server 2022.

## How to use

It takes up to 2 parameters, first being the scale in percent. The second argument is the monitor index, which you get from windows display settings, if you click identify it shows you the index.

`SetDPI.exe [DPI scale value] [monitor index] [/e]`

If you have only one monitor or want only main monitor to change you can omit the monitor index.

If `/e` is provided as the second or third argument, Explorer will be restarted after the scale is set.

To get the current scale of a monitor, replace the scale argument with `get` to print the current scaling. For automation with `value` only the scale is printed.

## Examples

- To set 2nd monitor to a scale value of 250
	- `SetDPI.exe 250 2`
- To set main monitor to a scale value of 125
	- `SetDPI.exe 125`
- To set main monitor to a scale value of 125 and restart Explorer
	- `SetDPI.exe 125 /e`
- To get the scale of the 2nd monitor
	- `SetDPI.exe get 2` prints `Current Resolution: 250`
	- `SetDPI.exe value 2` prints `250`

## Credits

https://github.com/lihas/windows-DPI-scaling-sample
