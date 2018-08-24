#ifndef LAME2D_H_
#define LAME2D_H_

#define LMVERSION_MAJOR 1
#define LMVERSION_MINOR 0
#define LMVERSION_PATCH 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

enum lmBool_Enum { lmFalse = 0, lmTrue = 1 };
enum lmStatus_Enum { lmStatus_StackEmpty = -1, lmStatus_LostFocus = -2, lmStatus_GainedFocus = -3, lmStatus_Internal = -4 };
enum lmState_Enum { lmState_Released = 0, lmState_Pressed = 1, lmState_Held = 2 };
enum lmKey_Enum { lmKey_Backspace = 8, lmKey_Tab = 9, lmKey_Return = 13, lmKey_Escape = 27, lmKey_Space = 32, lmKey_ExclaimationMark = 33, lmKey_QuoteDbl = 34, lmKey_Hash = 35, lmKey_Dollar = 36, lmKey_Percent = 37, lmKey_Ampersand = 38, lmKey_Quote = 39, lmKey_LeftParenthesis = 40, lmKey_RightParenthesis = 41, lmKey_Asterisk = 42, lmKey_Plus = 43, lmKey_Comma = 44, lmKey_Minus = 45, lmKey_Period = 46, lmKey_Slash = 47, lmKey_0 = 48, lmKey_1 = 49, lmKey_2 = 50, lmKey_3 = 51, lmKey_4 = 52, lmKey_5 = 53, lmKey_6 = 54, lmKey_7 = 55, lmKey_8 = 56, lmKey_9 = 57, lmKey_Colon = 58, lmKey_Semicolon = 59, lmKey_Less = 60, lmKey_Equals = 61, lmKey_Greater = 62, lmKey_Question = 63, lmKey_At = 64, lmKey_LeftBracket = 91, lmKey_Backslash = 92, lmKey_RightBracket = 93, lmKey_Caret = 94, lmKey_Underscore = 95, lmKey_Backquote = 96, lmKey_A = 97, lmKey_B = 98, lmKey_C = 99, lmKey_D = 100, lmKey_E = 101, lmKey_F = 102, lmKey_G = 103, lmKey_H = 104, lmKey_I = 105, lmKey_J = 106, lmKey_K = 107, lmKey_L = 108, lmKey_M = 109, lmKey_N = 110, lmKey_O = 111, lmKey_P = 112, lmKey_Q = 113, lmKey_R = 114, lmKey_S = 115, lmKey_T = 116, lmKey_U = 117, lmKey_V = 118, lmKey_W = 119, lmKey_X = 120, lmKey_Y = 121, lmKey_Z = 122, lmKey_Delete = 127, lmKey_CapsLock = 569, lmKey_F1 = 570, lmKey_F2 = 571, lmKey_F3 = 572, lmKey_F4 = 573, lmKey_F5 = 574, lmKey_F6 = 575, lmKey_F7 = 576, lmKey_F8 = 577, lmKey_F9 = 578, lmKey_F10 = 579, lmKey_F11 = 580, lmKey_F12 = 581, lmKey_PrintScreen = 582, lmKey_ScrollLock = 583, lmKey_Pause = 584, lmKey_Insert = 585, lmKey_Home = 586, lmKey_PageUp = 587, lmKey_End = 589, lmKey_PageDown = 590, lmKey_Right = 591, lmKey_Left = 592, lmKey_Down = 593, lmKey_Up = 594, lmKey_KPLockClear = 595, lmKey_KPDivide = 596, lmKey_KPMultiply = 597, lmKey_KPMinus = 598, lmKey_KPPlus = 599, lmKey_KPEnter = 600, lmKey_KP1 = 601, lmKey_KP2 = 602, lmKey_KP3 = 603, lmKey_KP4 = 604, lmKey_KP5 = 605, lmKey_KP6 = 606, lmKey_KP7 = 607, lmKey_KP8 = 608, lmKey_KP9 = 609, lmKey_KP0 = 610, lmKey_KPPeriod = 611, lmKey_Application = 613, lmKey_Power = 614, lmKey_KPEquals = 615, lmKey_F13 = 616, lmKey_F14 = 617, lmKey_F15 = 618, lmKey_F16 = 619, lmKey_F17 = 620, lmKey_F18 = 621, lmKey_F19 = 622, lmKey_F20 = 623, lmKey_F21 = 624, lmKey_F22 = 625, lmKey_F23 = 626, lmKey_F24 = 627, lmKey_Execute = 628, lmKey_Help = 629, lmKey_Menu = 630, lmKey_Select = 631, lmKey_Stop = 632, lmKey_Again = 633, lmKey_Undo = 634, lmKey_Cut = 635, lmKey_Copy = 636, lmKey_Paste = 637, lmKey_Find = 638, lmKey_Mute = 639, lmKey_VolumeUp = 640, lmKey_VolumeDown = 641, lmKey_KPComma = 645, lmKey_KPEqualsAS400 = 646, lmKey_AltErase = 665, lmKey_SysReq = 666, lmKey_Cancel = 667, lmKey_Clear = 668, lmKey_Prior = 669, lmKey_Return2 = 670, lmKey_Separator = 671, lmKey_Out = 672, lmKey_Oper = 673, lmKey_ClearAgain = 674, lmKey_CrSel = 675, lmKey_ExSel = 676, lmKey_KP00 = 688, lmKey_KP000 = 689, lmKey_ThousandsSeparator = 690, lmKey_DecimalSeparator = 691, lmKey_CurrencyUnit = 692, lmKey_CurrencySubUnit = 693, lmKey_KPLeftParenthesis = 694, lmKey_KPRightParenthesis = 695, lmKey_KPLeftBracket = 696, lmKey_KPRightBracket = 697, lmKey_KPTab = 698, lmKey_KPBackspace = 699, lmKey_KPA = 700, lmKey_KPB = 701, lmKey_KPC = 702, lmKey_KPD = 703, lmKey_KPE = 704, lmKey_KPF = 705, lmKey_KPXor = 706, lmKey_KPPower = 707, lmKey_KPPercent = 708, lmKey_KPLess = 709, lmKey_KPGreater = 710, lmKey_KPAmpersand = 711, lmKey_KPDblAmpersand = 712, lmKey_KPVerticalBar = 713, lmKey_KPDblVerticalBar = 714, lmKey_KPColon = 715, lmKey_KPHash = 716, lmKey_KPSpace = 717, lmKey_KPAt = 718, lmKey_KPExclamationMark = 719, lmKey_KPMemStore = 720, lmKey_KPMemRecall = 721, lmKey_KPMemClear = 722, lmKey_KPMemAdd = 723, lmKey_KPMemSubtract = 724, lmKey_KPMemMultiply = 725, lmKey_KPMemDivide = 726, lmKey_KPPlusMinus = 727, lmKey_KPClear = 728, lmKey_KPClearEntry = 729, lmKey_KPBinary = 730, lmKey_KPOctal = 731, lmKey_KPDecimal = 732, lmKey_KPHexadecimal = 733, lmKey_LeftCtrl = 736, lmKey_LeftShift = 737, lmKey_LeftAlt = 738, lmKey_LeftGui = 739, lmKey_RightCtrl = 740, lmKey_RightShift = 741, lmKey_RightAlt = 742, lmKey_RightGui = 743, lmKey_Mode = 769, lmKey_AudioNext = 770, lmKey_AudioPrev = 771, lmKey_AudioStop = 772, lmKey_AudioPlay = 773, lmKey_AudioMute = 774, lmKey_MediaSelect = 775, lmKey_WWW = 776, lmKey_Mail = 777, lmKey_Calculator = 778, lmKey_Computer = 779, lmKey_ACSearch = 780, lmKey_ACHome = 781, lmKey_ACBack = 782, lmKey_ACForward = 783, lmKey_ACStop = 784, lmKey_ACRefresh = 785, lmKey_ACBookmarks = 786, lmKey_BrightnessDown = 787, lmKey_BrightnessUp = 788, lmKey_DisplaySwitch = 789, lmKey_KbdIlluminationToggle = 790, lmKey_KbdIlluminationDown = 791, lmKey_KbdIlluminationUp = 792, lmKey_Eject = 793, lmKey_Sleep = 794 };
enum lmStyle_Enum { lmStyle_Normal = 0, lmStyle_Bold = 1, lmStyle_Italic = 2, lmStyle_Underline = 4, lmStyle_Strikethrough = 8 };
enum lmHinting_Enum { lmHinting_Normal = 0, lmHinting_Light = 1, lmHinting_Mono = 2, lmHinting_None = 3 };
enum lmFlip_Enum { lmFlip_None = 0, lmFlip_Horizontal = 1, lmFlip_Vertical = 2 };
enum lmBlending_Enum { lmBlending_None = 0, lmBlending_Alpha = 1, lmBlending_Additive = 2, lmBlending_Modulate = 3 };
enum lmAction_Enum { lmAction_Play = 0, lmAction_Pause = 1, lmAction_Resume = 2, lmAction_Stop = 3 };

struct lmPoint_Struct
{
	int lmLeft;
	int lmTop;
};
struct lmSize_Struct
{
	int lmWidth;
	int lmHeight;
};
struct lmRectangle_Struct
{
	int lmLeft;
	int lmTop;
	int lmWidth;
	int lmHeight;
};
struct lmSound_Struct;
struct lmMap_Struct;

typedef enum lmBool_Enum lmBool;
typedef enum lmStatus_Enum lmStatus;
typedef enum lmState_Enum lmState;
typedef enum lmKey_Enum lmKey;
typedef enum lmStyle_Enum lmStyle;
typedef enum lmHinting_Enum lmHinting;
typedef enum lmFlip_Enum lmFlip;
typedef enum lmBlending_Enum lmBlending;
typedef enum lmAction_Enum lmAction;

typedef struct lmPoint_Struct lmPoint;
typedef struct lmSize_Struct lmSize;
typedef struct lmRectangle_Struct lmRectangle;
typedef Uint32 lmColor;
typedef TTF_Font lmFont;
typedef SDL_Texture lmGraphic;
typedef Mix_Music lmMusic;
typedef struct lmSound_Struct lmSound;
typedef struct lmMap_Struct lmMap;

void lmInit(const char* lmWindow_Title, lmSize lmWindow_Size, lmColor lmWindow_Background, lmSize lmScreen_Size, unsigned int lmScreen_FrameRate);
void lmQuit(void);
lmBool lmWindow_IsOpen(void);
void lmWindow_SetMode(lmBool lmWindow_Fullscreen);
void lmWindow_Close(void);
void lmStack_Push(unsigned int lmStack_Value);
void lmStack_Pop(void);
int lmStack_Peek(void);
lmState lmKey_Read(lmKey lmKey_Key);
void lmScreen_Clear(lmColor lmScreen_Color);
double lmScreen_Refresh();
lmColor lmScreen_GetColor(lmPoint lmScreen_Point);
lmGraphic* lmScreen_Shot(void);
void lmDraw_Point(lmPoint lmScreen_Point, lmColor lmScreen_Color);
void lmDraw_Line(lmPoint lmScreen_Point1, lmPoint lmScreen_Point2, lmColor lmScreen_Color);
void lmDraw_Box(lmRectangle lmScreen_Rectangle, lmColor lmScreen_Color);
void lmDraw_Rectangle(lmRectangle lmScreen_Rectangle, lmColor lmScreen_Color);
void lmDraw_Text(lmRectangle lmScreen_Rectangle, lmFont* lmFont_Pointer, const char* lmFont_Text, double lmFont_Angle, lmColor lmFont_Color);
void lmDraw_Graphic(lmGraphic* lmGraphic_Pointer, lmPoint lmScreen_Point);
void lmDraw_GraphicEx(lmGraphic* lmGraphic_Pointer, lmRectangle lmScreen_Rectangle, lmPoint lmGraphic_Point, double lmGraphic_Angle, lmFlip lmDraw_Flip);
lmFont* lmFont_Load(const char* lmFont_File, lmStyle lmFont_Style, int lmFont_Height, int lmFont_Outline, lmHinting lmFont_Hinting, lmBool lmFont_Kerning);
lmGraphic* lmGraphic_Load(const char* lmGraphic_File);
lmGraphic* lmGraphic_Copy(lmGraphic* lmGraphic_Pointer, lmRectangle lmGraphic_Recntagle);
void lmGraphic_SetBlending(lmGraphic* lmGraphic_Pointer, lmBlending lmGraphic_Blending, lmColor lmGraphic_Color);
lmMusic* lmMusic_Load(const char* lmMusic_File);
void lmMusic_Control(lmMusic* lmMusic_Pointer, lmAction lmMusic_Action, int lmMusic_Fade, lmBool lmMusic_Loop);
lmSound* lmSound_Load(const char* lmSound_File);
void lmSound_Control(lmSound* lmSound_Pointer, lmAction lmSound_Action, int lmSound_Fade, lmBool lmSound_Loop);
lmMap* lmMap_Load(const char* lmMap_File, lmSize lmMap_Size);
int lmMap_GetTile(lmMap* lmMap_Pointer, lmPoint lmTile_Point);
void lmMap_SetTile(lmMap* lmMap_Pointer, lmPoint lmTile_Point, unsigned int lmTile_Value);
lmGraphic* lmMap_Render(lmMap* lmMap_Pointer, lmGraphic* lmGraphic_Pointer, lmSize lmTile_Size);
void lmDestroy_Font(lmFont* lmFont_Pointer);
void lmDestroy_Graphic(lmGraphic* lmGraphic_Pointer);
void lmDestroy_Music(lmMusic* lmMusic_Pointer);
void lmDestroy_Sound(lmSound* lmSound_Pointer);
void lmDestroy_Map(lmMap* lmMap_Pointer);

#endif
