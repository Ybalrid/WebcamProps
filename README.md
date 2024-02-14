# WebCamProps

This program opens the old DirectShow video properties dialog of a video input device, based on a partial match of the name of the device.

## Usage 

```cmd
.\webcamprops.exe c920
```

Note
- This program only look at one string
- The string is case insensitive
- You only need this string to be a substring of the name of a device
- If there are multiple matches, only "the first one" will work. Whichever one is "the first one" ~~is left as an exercise for the reader~~ is not well defined by me. It depends on libdshowcapture enumeration code.


## Build

Just generate a project with CMake and build it. You will need to bring together the libdshowcapture.dll and the webcamprops.exe file in the same work directory by hand.

Thanks [@steffandonal](https://github.com/steffandonal) for the hint on to open that dialog easilly using libdshowcapture
