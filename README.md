# yok.scr

![lkthink](https://user-images.githubusercontent.com/50504215/211129980-9516e63c-790a-484f-8fb8-3f9fa38f69cb.png)

**The doughbyte-themed windows screensaver** - built with ancient Win32 + OpenGL, just like you would in the early 00's. Half doughbyte shitpost, half love letter to classic screensavers.

[Home page](https://doughbyte.com/aut/yokscr)

## Building

_You can get a pre-compiled binary from the Releases page or the Home page above._

There are two steps to building:
* In the repo root, run `python bitmaps_to_bmp.py` to generate the `bitmaps` folder. This script depends on [Pillow](https://pillow.readthedocs.io/en/stable/installation.html), so you'll have to install that first.
* Load the `.sln` in Visual Studio and build the project in Release mode.

## Contributing

I certainly don't expect anyone to, but I encourage you to! :)
I won't be adding anything more to this project, but that doesn't mean you can't. You don't need to be familiar with Win32 or OpenGL, since they make up relatively little of the project and they're well isolated from the core logic, which is all good old fashioned C++ (14).

Just don't commit to master - be nice and make a PR.
