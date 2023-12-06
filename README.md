# SetDPI

[![image](https://user-images.githubusercontent.com/79026235/152910441-59ba653c-5607-4f59-90c0-bc2851bf2688.png)Download the zip file](https://github.com/LesFerch/SetDPI/releases/download/1.1.0/SetDPI.zip)

This is a console app to change the windows display scaling setting via the command line. It supports multiple monitors.

Only the preset display scaling values are supported, such as 100, 125, 150, 175, and so on. Custom display scaling values cannot be used.

This app is a fork of <https://github.com/imniko/SetDPI>. It provides all of the functionality of the original, but also supports an additional optional argument to restart Explorer and runs on both 32 bit and 64 bit Windows.

## How to Download and Run

1. Download the zip file using the link above.
2. Extract **SetDPI.exe**.
3. Open a Cmd prompt and run **SetDPI.exe** with the appropriate arguments.
5. Optionally create one or more **SetDPI.exe** shortcuts for the scaling values you need.

**Note**: Some antivirus software may falsely detect the download as a virus. This can happen any time you download a new executable and may require extra steps to whitelist the file.

## How to use

It takes up to 3 arguments, a scale percent value, an optional monitor number (which you can get from windows display settings), and an option to restart Explorer.

`SetDPI.exe [DPI scale value] [monitor number] [/e]`

If you have only one monitor, or want only the main monitor to change, you can omit the monitor number.

If `/e` is provided as the second or third argument, Explorer will be restarted after the scale is set.

To get the current scale setting, replace the scale argument with `get` or `value` to display the current scaling.

## Examples

Set main monitor to a scale value of 125:	`SetDPI 125`

Set monitor 2 to a scale value of 125:	`SetDPI 125 2`

Set main monitor to a scale value of 125 and restart Explorer:	`SetDPI 125 /e`

Display the scale of monitor 2: `SetDPI get 2` displays `Current Resolution: 125`

Display the scale of monitor 2: `SetDPI value 2` displays `125`

## Credits

<https://github.com/lihas/windows-DPI-scaling-sample>
