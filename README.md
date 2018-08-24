# Lame2D

Lame2D is a C game framework based on [SDL2](https://www.libsdl.org/) with the use of [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/), [SDL_image](https://www.libsdl.org/projects/SDL_image/) and [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/).


## Functionality

Lame2D is directed towards pixel art game developers. It handles drawing on a small canvas which then gets upscaled to fit the window, keeping pixels sharp and square.

At the first glance, it may look like an another fantasy console—but it's not the case. Although I don't discourage anybody from utilizing it in such a way!

To be more precise, size of the canvas is not imposed in advance, so it is up to a programmer to specify it. The same goes to setting a frame rate and whether game should start in fullscreen. Not to mention there's no limitation on a color palette size or a level of sophistication of music and sounds.

As for resources, Lame2D can handle:

* _TTF_ fonts,
* _PNG_ graphics,
* _MP3_ music,
* _WAV_ sounds,
* _CSV_ maps.

The last being compatible with maps exported directly from [Tiled](https://www.mapeditor.org/).


## Game structure

To start writing a game with Lame2D, you only need a few lines of code:

```C
#include "Lame2D.h"

int main(int argc, char* argv[])
{
	lmInit("Empty window", (lmSize) { 320, 240 }, 0xFF000000,
		(lmSize) { 64, 56 }, 20); //initialization
	//load your resources
	//play music
	while (lmWindow_IsOpen()) //game loop
	{
		lmScreen_Clear(0xFFFFFFFF);
		//draw scene
		//handle controls
		lmScreen_Refresh();
	}
	//destroy resources
	lmQuit(); //termination
	return EXIT_SUCCESS;
}
```

![An empty window.](https://i.imgur.com/abeII9j.png)

Rest of the API isn't much harder than that, you can find whole documentation on the [wiki](https://github.com/Kotziauke/Lame2D/wiki).


## Made with Lame2D

### Slime

A short game which makes use of almost every aspect of Lame2D. You can check it out [here](https://github.com/Kotziauke/Slime).

![Title screen](https://i.imgur.com/G3vXkHH.png) ![Third level](https://i.imgur.com/77fvnXi.png)


## Installing

If you want to manually setup everything by yourself, the installation process is described on the [installing page](https://github.com/Kotziauke/Lame2D/wiki/Installing). However, you may just want to download a premade [Visual Studio 2017 project](https://github.com/Kotziauke/Lame2D/releases) with everything configured and ready to use!


## Authors

**Kotziauke**—initial work.


## License

This project is licensed under the MIT License—see [LICENSE.md](LICENSE.md) for details.

