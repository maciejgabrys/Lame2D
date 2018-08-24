#include "Lame2D.h"

struct lmNode_Struct;

typedef struct lmNode_Struct lmNode;

struct lmNode_Struct
{
	int lmNode_Value;
	lmNode* lmNode_Previous;
};
struct lmSound_Struct
{
	Mix_Chunk* lmSDL_Chunk;
	int lmSDL_Channel;
};
struct lmMap_Struct
{
	lmSize lmMap_Size;
	unsigned int **lmTile_Value;
};

static SDL_Window* lmgSDL_Window;
static SDL_Renderer* lmgSDL_Renderer;
static SDL_Texture* lmgSDL_Texture;
static lmColor lmgWindow_Background;
static double lmgSDL_FrameDuration;
static Uint64 lmgSDL_LastTime;
static lmBool lmgWindow_CloseRequest = lmFalse;
static lmNode* lmgNode_Top = NULL;
static lmState lmgKey[1024];
static lmMusic* lmgMusic_Pointer = NULL;

static void lmStack_PushStatus(lmStatus lmStack_Value);

void lmInit(const char* lmWindow_Title, lmSize lmWindow_Size, lmColor lmWindow_Background, lmSize lmScreen_Size, unsigned int lmScreen_FrameRate)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	lmgSDL_Window = SDL_CreateWindow(lmWindow_Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, lmWindow_Size.lmWidth, lmWindow_Size.lmHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_SetWindowMinimumSize(lmgSDL_Window, lmScreen_Size.lmWidth, lmScreen_Size.lmHeight);
	lmgWindow_Background = lmWindow_Background;
	lmgSDL_Renderer = SDL_CreateRenderer(lmgSDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawBlendMode(lmgSDL_Renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetIntegerScale(lmgSDL_Renderer, SDL_TRUE);
	SDL_RenderSetLogicalSize(lmgSDL_Renderer, lmScreen_Size.lmWidth, lmScreen_Size.lmHeight);
	lmgSDL_Texture = SDL_CreateTexture(lmgSDL_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, lmScreen_Size.lmWidth, lmScreen_Size.lmHeight);
	SDL_SetTextureBlendMode(lmgSDL_Texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(lmgSDL_Renderer, lmgSDL_Texture);
	for (int lmIterator = 0; lmIterator < 1024; lmIterator++)
	{
		lmgKey[lmIterator] = lmState_Released;
	}
	lmgSDL_FrameDuration = 1000.0 / lmScreen_FrameRate;
	lmgSDL_LastTime = SDL_GetPerformanceCounter();
}

void lmQuit(void)
{
	while (lmgNode_Top != NULL)
	{
		lmNode* lmNode_Previous = lmgNode_Top->lmNode_Previous;
		free(lmgNode_Top);
		lmgNode_Top = lmNode_Previous;
	}
	SDL_DestroyTexture(lmgSDL_Texture);
	SDL_DestroyRenderer(lmgSDL_Renderer);
	SDL_DestroyWindow(lmgSDL_Window);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

lmBool lmWindow_IsOpen(void)
{
	if (lmgWindow_CloseRequest)
	{
		return lmFalse;
	}
	SDL_Event lmSDL_Event;
	while (SDL_PollEvent(&lmSDL_Event))
	{
		switch (lmSDL_Event.type)
		{
		case SDL_WINDOWEVENT:
			switch (lmSDL_Event.window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:
				lmStack_PushStatus(lmStatus_Internal);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				lmStack_PushStatus(lmStatus_LostFocus);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				if (lmStack_Peek() == lmStatus_Internal)
				{
					lmStack_Pop();
				}
				else
				{
					lmStack_PushStatus(lmStatus_GainedFocus);
				}
				break;
			case SDL_WINDOWEVENT_CLOSE:
				return lmFalse;
				break;
			}
			break;
		case SDL_KEYDOWN:
			if (lmgKey[lmSDL_Event.key.keysym.sym % 0x3FFFFE00] == lmState_Released)
			{
				lmgKey[lmSDL_Event.key.keysym.sym % 0x3FFFFE00] = lmState_Pressed;
			}
			break;
		case SDL_KEYUP:
			lmgKey[lmSDL_Event.key.keysym.sym % 0x3FFFFE00] = lmState_Released;
			break;
		}
	}
	return lmTrue;
}

void lmWindow_SetMode(lmBool lmWindow_Fullscreen)
{
	SDL_SetWindowFullscreen(lmgSDL_Window, lmWindow_Fullscreen * SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_ShowCursor(!lmWindow_Fullscreen);
}

void lmWindow_Close(void)
{
	lmgWindow_CloseRequest = lmTrue;
}

void lmStack_Push(unsigned int lmStack_Value)
{
	lmNode* lmNode_Next = (lmNode*)malloc(sizeof(lmNode));
	lmNode_Next->lmNode_Value = lmStack_Value;
	lmNode_Next->lmNode_Previous = lmgNode_Top;
	lmgNode_Top = lmNode_Next;
}

static void lmStack_PushStatus(lmStatus lmStack_Value)
{
	lmNode* lmNode_Next = (lmNode*)malloc(sizeof(lmNode));
	lmNode_Next->lmNode_Value = lmStack_Value;
	lmNode_Next->lmNode_Previous = lmgNode_Top;
	lmgNode_Top = lmNode_Next;
}

void lmStack_Pop(void)
{
	if (lmgNode_Top != NULL)
	{
		lmNode* lmNode_Previous = lmgNode_Top->lmNode_Previous;
		free(lmgNode_Top);
		lmgNode_Top = lmNode_Previous;
	}
}

int lmStack_Peek(void)
{
	if (lmgNode_Top == NULL)
	{
		return lmStatus_StackEmpty;
	}
	return lmgNode_Top->lmNode_Value;
}

lmState lmKey_Read(lmKey lmKey_Key)
{
	if (lmgKey[lmKey_Key] == lmState_Pressed)
	{
		lmgKey[lmKey_Key] = lmState_Held;
		return lmState_Pressed;
	}
	return lmgKey[lmKey_Key];
}

void lmScreen_Clear(lmColor lmScreen_Background)
{
	SDL_SetRenderDrawColor(lmgSDL_Renderer, (Uint8)(lmScreen_Background >> 16), (Uint8)(lmScreen_Background >> 8 & 0xFF), (Uint8)(lmScreen_Background & 0xFF), (Uint8)(lmScreen_Background >> 24 & 0xFF));
	SDL_RenderClear(lmgSDL_Renderer);
}

double lmScreen_Refresh()
{
	SDL_SetRenderTarget(lmgSDL_Renderer, NULL);
	SDL_SetRenderDrawColor(lmgSDL_Renderer, (Uint8)(lmgWindow_Background >> 16), (Uint8)(lmgWindow_Background >> 8 & 0xFF), (Uint8)(lmgWindow_Background & 0xFF), (Uint8)(lmgWindow_Background >> 24 & 0xFF));
	SDL_RenderClear(lmgSDL_Renderer);
	SDL_RenderCopy(lmgSDL_Renderer, lmgSDL_Texture, NULL, NULL);
	double lmSDL_DeltaTime = (SDL_GetPerformanceCounter() - lmgSDL_LastTime) * 1000.0 / SDL_GetPerformanceFrequency();
	if (lmSDL_DeltaTime < lmgSDL_FrameDuration)
	{
		SDL_Delay((Uint32)(lmgSDL_FrameDuration - lmSDL_DeltaTime));
	}
	double SDL_Framerate = (double)SDL_GetPerformanceFrequency() / (SDL_GetPerformanceCounter() - lmgSDL_LastTime);
	lmgSDL_LastTime = SDL_GetPerformanceCounter();
	SDL_RenderPresent(lmgSDL_Renderer);
	SDL_SetRenderTarget(lmgSDL_Renderer, lmgSDL_Texture);
	return SDL_Framerate;
}

lmColor lmScreen_GetColor(lmPoint lmScreen_Point)
{
	lmColor lmSDL_Pixels;
	SDL_Rect lmSDL_Rect = { lmScreen_Point.lmLeft, lmScreen_Point.lmTop, 1, 1 };
	SDL_RenderReadPixels(lmgSDL_Renderer, &lmSDL_Rect, SDL_PIXELFORMAT_ARGB8888, &lmSDL_Pixels, 1);
	return lmSDL_Pixels;
}

lmGraphic* lmScreen_Shot(void)
{
	int lmWidth;
	int lmHeight;
	SDL_GetRendererOutputSize(lmgSDL_Renderer, &lmWidth, &lmHeight);
	lmGraphic* lmSDL_Texture = SDL_CreateTexture(lmgSDL_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, lmWidth, lmHeight);
	SDL_SetTextureBlendMode(lmSDL_Texture, SDL_BLENDMODE_BLEND);
	void* lmSDL_Pixels;
	int lmSDL_Pitch;
	SDL_LockTexture(lmSDL_Texture, NULL, &lmSDL_Pixels, &lmSDL_Pitch);
	SDL_RenderReadPixels(lmgSDL_Renderer, NULL, SDL_PIXELFORMAT_ARGB8888, lmSDL_Pixels, lmSDL_Pitch);
	SDL_UnlockTexture(lmSDL_Texture);
	return lmSDL_Texture;
}

void lmDraw_Point(lmPoint lmScreen_Point, lmColor lmScreen_Color)
{
	SDL_SetRenderDrawColor(lmgSDL_Renderer, (Uint8)(lmScreen_Color >> 16), (Uint8)(lmScreen_Color >> 8 & 0xFF), (Uint8)(lmScreen_Color & 0xFF), (Uint8)(lmScreen_Color >> 24 & 0xFF));
	SDL_RenderDrawPoint(lmgSDL_Renderer, lmScreen_Point.lmLeft, lmScreen_Point.lmTop);
}

void lmDraw_Line(lmPoint lmScreen_Point1, lmPoint lmScreen_Point2, lmColor lmScreen_Color)
{
	SDL_SetRenderDrawColor(lmgSDL_Renderer, (Uint8)(lmScreen_Color >> 16), (Uint8)(lmScreen_Color >> 8 & 0xFF), (Uint8)(lmScreen_Color & 0xFF), (Uint8)(lmScreen_Color >> 24 & 0xFF));
	SDL_RenderDrawLine(lmgSDL_Renderer, lmScreen_Point1.lmLeft, lmScreen_Point1.lmTop, lmScreen_Point2.lmLeft, lmScreen_Point2.lmTop);
}

void lmDraw_Box(lmRectangle lmScreen_Rectangle, lmColor lmScreen_Color)
{
	SDL_Rect lmSDL_ScreenRect = { lmScreen_Rectangle.lmLeft, lmScreen_Rectangle.lmTop, lmScreen_Rectangle.lmWidth, lmScreen_Rectangle.lmHeight };
	SDL_SetRenderDrawColor(lmgSDL_Renderer, (Uint8)(lmScreen_Color >> 16), (Uint8)(lmScreen_Color >> 8 & 0xFF), (Uint8)(lmScreen_Color & 0xFF), (Uint8)(lmScreen_Color >> 24 & 0xFF));
	SDL_RenderDrawRect(lmgSDL_Renderer, &lmSDL_ScreenRect);
}

void lmDraw_Rectangle(lmRectangle lmScreen_Rectangle, lmColor lmScreen_Color)
{
	SDL_Rect lmSDL_ScreenRect = { lmScreen_Rectangle.lmLeft, lmScreen_Rectangle.lmTop, lmScreen_Rectangle.lmWidth, lmScreen_Rectangle.lmHeight };
	SDL_SetRenderDrawColor(lmgSDL_Renderer, (Uint8)(lmScreen_Color >> 16), (Uint8)(lmScreen_Color >> 8 & 0xFF), (Uint8)(lmScreen_Color & 0xFF), (Uint8)(lmScreen_Color >> 24 & 0xFF));
	SDL_RenderFillRect(lmgSDL_Renderer, &lmSDL_ScreenRect);
}

void lmDraw_Text(lmRectangle lmScreen_Rectangle, lmFont* lmFont_Pointer, const char* lmFont_Text, double lmFont_Angle, lmColor lmFont_Color)
{
	SDL_Surface* lmSDL_Surface = TTF_RenderUTF8_Blended_Wrapped(lmFont_Pointer, lmFont_Text, (SDL_Color) { (Uint8)(lmFont_Color >> 16), (Uint8)(lmFont_Color >> 8 & 0xFF), (Uint8)(lmFont_Color & 0xFF), 0xFF }, lmScreen_Rectangle.lmWidth);
	lmGraphic* lmSDL_Texture = SDL_CreateTextureFromSurface(lmgSDL_Renderer, lmSDL_Surface);
	SDL_FreeSurface(lmSDL_Surface);
	SDL_SetTextureBlendMode(lmSDL_Texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(lmSDL_Texture, (Uint8)(lmFont_Color >> 24 & 0xFF));
	SDL_Rect lmSDL_FontRect = { 0, 0, 0, 0 };
	SDL_QueryTexture(lmSDL_Texture, NULL, NULL, &lmSDL_FontRect.w, &lmSDL_FontRect.h);
	SDL_Point lmSDL_FontPoint = { lmSDL_FontRect.w / 2, lmSDL_FontRect.h / 2 };
	SDL_Rect lmSDL_ScreenRect = { lmScreen_Rectangle.lmLeft, lmScreen_Rectangle.lmTop, lmSDL_FontRect.w, lmSDL_FontRect.h };
	SDL_RenderCopyEx(lmgSDL_Renderer, lmSDL_Texture, &lmSDL_FontRect, &lmSDL_ScreenRect, lmFont_Angle, &lmSDL_FontPoint, SDL_FLIP_NONE);
	SDL_DestroyTexture(lmSDL_Texture);
}

void lmDraw_Graphic(lmGraphic* lmGraphic_Pointer, lmPoint lmScreen_Point)
{
	SDL_Rect lmSDL_ScreenRect = { lmScreen_Point.lmLeft, lmScreen_Point.lmTop, 0, 0 };
	SDL_QueryTexture(lmGraphic_Pointer, NULL, NULL, &lmSDL_ScreenRect.w, &lmSDL_ScreenRect.h);
	SDL_RenderCopy(lmgSDL_Renderer, lmGraphic_Pointer, NULL, &lmSDL_ScreenRect);
}

void lmDraw_GraphicEx(lmGraphic* lmGraphic_Pointer, lmRectangle lmScreen_Rectangle, lmPoint lmGraphic_Point, double lmGraphic_Angle, lmFlip lmDraw_Flip)
{
	SDL_Rect lmSDL_ScreenRect = { lmScreen_Rectangle.lmLeft, lmScreen_Rectangle.lmTop, lmScreen_Rectangle.lmWidth, lmScreen_Rectangle.lmHeight };
	SDL_Point lmSDL_TexturePoint = { lmGraphic_Point.lmLeft, lmGraphic_Point.lmTop };
	SDL_RenderCopyEx(lmgSDL_Renderer, lmGraphic_Pointer, NULL, &lmSDL_ScreenRect, lmGraphic_Angle, &lmSDL_TexturePoint, lmDraw_Flip);
}

lmFont* lmFont_Load(const char* lmFont_File, lmStyle lmFont_Style, int lmFont_Height, int lmFont_Outline, lmHinting lmFont_Hinting, lmBool lmFont_Kerning)
{
	lmFont* lmFont_Pointer = TTF_OpenFont(lmFont_File, lmFont_Height);
	if (TTF_GetFontStyle(lmFont_Pointer) != lmFont_Style)
	{
		TTF_SetFontStyle(lmFont_Pointer, lmFont_Style);
	}
	if (TTF_GetFontOutline(lmFont_Pointer) != lmFont_Outline)
	{
		TTF_SetFontOutline(lmFont_Pointer, lmFont_Outline);
	}
	if (TTF_GetFontHinting(lmFont_Pointer) != lmFont_Hinting)
	{
		TTF_SetFontHinting(lmFont_Pointer, lmFont_Hinting);
	}
	if (TTF_GetFontKerning(lmFont_Pointer) != lmFont_Kerning)
	{
		TTF_SetFontKerning(lmFont_Pointer, lmFont_Kerning);
	}
	return lmFont_Pointer;
}

lmGraphic* lmGraphic_Load(const char* lmGraphic_File)
{
	SDL_Surface* lmSDL_Surface = IMG_Load(lmGraphic_File);
	lmGraphic* lmSDL_Texture = SDL_CreateTextureFromSurface(lmgSDL_Renderer, lmSDL_Surface);
	SDL_FreeSurface(lmSDL_Surface);
	SDL_SetTextureBlendMode(lmSDL_Texture, SDL_BLENDMODE_BLEND);
	return lmSDL_Texture;
}

lmGraphic* lmGraphic_Copy(lmGraphic* lmGraphic_Pointer, lmRectangle lmGraphic_Rectangle)
{
	lmGraphic* lmSDL_TextureTemporary = SDL_CreateTexture(lmgSDL_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, lmGraphic_Rectangle.lmWidth, lmGraphic_Rectangle.lmHeight);
	SDL_SetTextureBlendMode(lmSDL_TextureTemporary, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(lmgSDL_Renderer, lmSDL_TextureTemporary);
	SDL_Rect lmSDL_TextureRect = { lmGraphic_Rectangle.lmLeft, lmGraphic_Rectangle.lmTop, lmGraphic_Rectangle.lmWidth, lmGraphic_Rectangle.lmHeight };
	SDL_RenderCopy(lmgSDL_Renderer, lmGraphic_Pointer, &lmSDL_TextureRect, NULL);
	lmGraphic* lmSDL_Texture = lmScreen_Shot();
	SDL_SetRenderTarget(lmgSDL_Renderer, lmgSDL_Texture);
	SDL_DestroyTexture(lmSDL_TextureTemporary);
	return lmSDL_Texture;
}

void lmGraphic_SetBlending(lmGraphic* lmGraphic_Pointer, lmBlending lmGraphic_Blending, lmColor lmGraphic_Color)
{
	SDL_SetTextureBlendMode(lmGraphic_Pointer, lmGraphic_Blending);
	SDL_SetTextureColorMod(lmGraphic_Pointer, (Uint8)(lmGraphic_Color >> 16), (Uint8)(lmGraphic_Color >> 8 & 0xFF), (Uint8)(lmGraphic_Color & 0xFF));
	SDL_SetTextureAlphaMod(lmGraphic_Pointer, (Uint8)(lmGraphic_Color >> 24 & 0xFF));
}

lmMusic* lmMusic_Load(const char* lmMusic_File)
{
	return Mix_LoadMUS(lmMusic_File);
}

void lmMusic_Control(lmMusic* lmMusic_Pointer, lmAction lmMusic_Action, int lmMusic_Fade, lmBool lmMusic_Loop)
{
	switch (lmMusic_Action)
	{
	case lmAction_Play:
		if (Mix_PlayingMusic())
		{
			Mix_HaltMusic();
		}
		Mix_FadeInMusic(lmMusic_Pointer, 1 - 2 * lmMusic_Loop, lmMusic_Fade);
		lmgMusic_Pointer = lmMusic_Pointer;
		break;
	case lmAction_Pause:
		Mix_PauseMusic();
		break;
	case lmAction_Resume:
		Mix_ResumeMusic();
		break;
	case lmAction_Stop:
		Mix_FadeOutMusic(lmMusic_Fade);
		break;
	}
}

lmSound* lmSound_Load(const char* lmSound_File)
{
	lmSound* lmSound_Pointer = (lmSound*)malloc(sizeof(lmSound));
	lmSound_Pointer->lmSDL_Chunk = Mix_LoadWAV(lmSound_File);
	lmSound_Pointer->lmSDL_Channel = -1;
	return lmSound_Pointer;
}

void lmSound_Control(lmSound* lmSound_Pointer, lmAction lmSound_Action, int lmSound_Fade, lmBool lmSound_Loop)
{
	switch (lmSound_Action)
	{
	case lmAction_Play:
		if (lmSound_Pointer->lmSDL_Channel == -1 || Mix_GetChunk(lmSound_Pointer->lmSDL_Channel) != lmSound_Pointer->lmSDL_Chunk)
		{
			lmSound_Pointer->lmSDL_Channel = Mix_FadeInChannel(-1, lmSound_Pointer->lmSDL_Chunk, -lmSound_Loop, lmSound_Fade);
		}
		else
		{
			if (Mix_Playing(lmSound_Pointer->lmSDL_Channel))
			{
				Mix_HaltChannel(lmSound_Pointer->lmSDL_Channel);
			}
			Mix_FadeInChannel(lmSound_Pointer->lmSDL_Channel, lmSound_Pointer->lmSDL_Chunk, -lmSound_Loop, lmSound_Fade);
		}
		break;
	case lmAction_Pause:
		if (Mix_GetChunk(lmSound_Pointer->lmSDL_Channel) == lmSound_Pointer->lmSDL_Chunk)
		{
			Mix_Pause(lmSound_Pointer->lmSDL_Channel);
		}
		break;
	case lmAction_Resume:
		if (Mix_GetChunk(lmSound_Pointer->lmSDL_Channel) == lmSound_Pointer->lmSDL_Chunk)
		{
			Mix_Resume(lmSound_Pointer->lmSDL_Channel);
		}
		break;
	case lmAction_Stop:
		if (Mix_GetChunk(lmSound_Pointer->lmSDL_Channel) == lmSound_Pointer->lmSDL_Chunk)
		{
			Mix_FadeOutChannel(lmSound_Pointer->lmSDL_Channel, lmSound_Fade);
		}
		break;
	}
}

lmMap* lmMap_Load(const char* lmMap_File, lmSize lmMap_Size)
{
	lmMap* lmMap_Pointer = (lmMap*)malloc(sizeof(lmMap));
	lmMap_Pointer->lmMap_Size = lmMap_Size;
	lmMap_Pointer->lmTile_Value = (unsigned int**)malloc(lmMap_Size.lmWidth * sizeof(unsigned int*));
	for (int lmIterator = 0; lmIterator < lmMap_Size.lmWidth; lmIterator++)
	{
		lmMap_Pointer->lmTile_Value[lmIterator] = (unsigned int*)malloc(lmMap_Size.lmHeight * sizeof(unsigned int));
	}
	FILE *lmFile_Pointer = fopen(lmMap_File, "rb");
	int lmFile_Char;
	int lmPosition = 0;
	lmBool lmFlag = lmFalse;
	while ((lmFile_Char = fgetc(lmFile_Pointer)) != EOF)
	{
		switch (lmFile_Char)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (!lmFlag)
			{
				lmMap_Pointer->lmTile_Value[lmPosition % lmMap_Size.lmWidth][lmPosition / lmMap_Size.lmWidth] = lmFile_Char - '0';
				lmFlag = lmTrue;
			}
			else
			{
				lmMap_Pointer->lmTile_Value[lmPosition % lmMap_Size.lmWidth][lmPosition / lmMap_Size.lmWidth] = lmMap_Pointer->lmTile_Value[lmPosition % lmMap_Size.lmWidth][lmPosition / lmMap_Size.lmWidth] * 10 + lmFile_Char - '0';
			}
			break;
		case ',':
		case '\r':
		case '\n':
			if (lmFlag)
			{
				lmPosition++;
				lmFlag = lmFalse;
			}
			break;
		}
	}
	fclose(lmFile_Pointer);
	return lmMap_Pointer;
}

int lmMap_GetTile(lmMap* lmMap_Pointer, lmPoint lmTile_Point)
{
	if (lmTile_Point.lmLeft < 0 || lmTile_Point.lmLeft >= lmMap_Pointer->lmMap_Size.lmWidth || lmTile_Point.lmTop < 0 || lmTile_Point.lmTop >= lmMap_Pointer->lmMap_Size.lmHeight)
	{
		return -1;
	}
	return lmMap_Pointer->lmTile_Value[lmTile_Point.lmLeft][lmTile_Point.lmTop];
}

void lmMap_SetTile(lmMap* lmMap_Pointer, lmPoint lmTile_Point, unsigned int lmTile_Value)
{
	if (lmTile_Point.lmLeft >= 0 && lmTile_Point.lmLeft < lmMap_Pointer->lmMap_Size.lmWidth && lmTile_Point.lmTop >= 0 && lmTile_Point.lmTop < lmMap_Pointer->lmMap_Size.lmHeight)
	{
		lmMap_Pointer->lmTile_Value[lmTile_Point.lmLeft][lmTile_Point.lmTop] = lmTile_Value;
	}
}

lmGraphic* lmMap_Render(lmMap* lmMap_Pointer, lmGraphic* lmGraphic_Pointer, lmSize lmTile_Size)
{
	int lmGraphic_Pitch;
	SDL_QueryTexture(lmGraphic_Pointer, NULL, NULL, &lmGraphic_Pitch, NULL);
	lmGraphic_Pitch /= lmTile_Size.lmWidth;
	lmGraphic* lmSDL_TextureTemporary = SDL_CreateTexture(lmgSDL_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, lmMap_Pointer->lmMap_Size.lmWidth * lmTile_Size.lmWidth, lmMap_Pointer->lmMap_Size.lmHeight * lmTile_Size.lmHeight);
	SDL_SetTextureBlendMode(lmSDL_TextureTemporary, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(lmgSDL_Renderer, lmSDL_TextureTemporary);
	SDL_Rect lmSDL_TileRect = { 0, 0, lmTile_Size.lmWidth, lmTile_Size.lmHeight };
	SDL_Rect lmSDL_TextureRect = lmSDL_TileRect;
	for (int lmTop = 0; lmTop < lmMap_Pointer->lmMap_Size.lmHeight; lmTop++)
	{
		for (int lmLeft = 0; lmLeft < lmMap_Pointer->lmMap_Size.lmWidth; lmLeft++)
		{
			lmSDL_TextureRect.x = (lmMap_Pointer->lmTile_Value[lmLeft][lmTop] % lmGraphic_Pitch) * lmTile_Size.lmWidth;
			lmSDL_TextureRect.y = (lmMap_Pointer->lmTile_Value[lmLeft][lmTop] / lmGraphic_Pitch) * lmTile_Size.lmHeight;
			SDL_RenderCopy(lmgSDL_Renderer, lmGraphic_Pointer, &lmSDL_TextureRect, &lmSDL_TileRect);
			lmSDL_TileRect.x += lmTile_Size.lmWidth;
		}
		lmSDL_TileRect.x = 0;
		lmSDL_TileRect.y += lmTile_Size.lmHeight;
	}
	lmGraphic* lmSDL_Texture = lmScreen_Shot();
	SDL_SetRenderTarget(lmgSDL_Renderer, lmgSDL_Texture);
	SDL_DestroyTexture(lmSDL_TextureTemporary);
	return lmSDL_Texture;
}

void lmDestroy_Font(lmFont* lmFont_Pointer)
{
	if (lmFont_Pointer != NULL)
	{
		TTF_CloseFont(lmFont_Pointer);
		lmFont_Pointer = NULL;
	}
}

void lmDestroy_Graphic(lmGraphic* lmGraphic_Pointer)
{
	if (lmGraphic_Pointer != NULL)
	{
		SDL_DestroyTexture(lmGraphic_Pointer);
		lmGraphic_Pointer = NULL;
	}
}

void lmDestroy_Music(lmMusic* lmMusic_Pointer)
{
	if (lmMusic_Pointer != NULL)
	{
		if (lmMusic_Pointer == lmgMusic_Pointer)
		{
			Mix_HaltMusic();
		}
		Mix_FreeMusic(lmMusic_Pointer);
		lmMusic_Pointer = NULL;
	}
}

void lmDestroy_Sound(lmSound* lmSound_Pointer)
{
	if (lmSound_Pointer != NULL)
	{
		if (Mix_GetChunk(lmSound_Pointer->lmSDL_Channel) == lmSound_Pointer->lmSDL_Chunk)
		{
			Mix_HaltChannel(lmSound_Pointer->lmSDL_Channel);
		}
		Mix_FreeChunk(lmSound_Pointer->lmSDL_Chunk);
		free(lmSound_Pointer);
		lmSound_Pointer = NULL;
	}
}

void lmDestroy_Map(lmMap* lmMap_Pointer)
{
	if (lmMap_Pointer != NULL)
	{
		for (int lmIterator = 0; lmIterator < lmMap_Pointer->lmMap_Size.lmHeight; lmIterator++)
		{
			free(lmMap_Pointer->lmTile_Value[lmIterator]);
		}
		free(lmMap_Pointer->lmTile_Value);
		free(lmMap_Pointer);
		lmMap_Pointer = NULL;
	}
}
