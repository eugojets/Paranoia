#pragma once
#include "stdafx.h"
#include <Windows.h>

/*
  Adapted from http://www.nirsoft.net/vc/change_screen_brightness.html
*/

class CGammaRamp
{
public:
  enum Brightness
  {
    BRIGHTEST = 255,
    BRIGHT = 192,
    NORMAL = 128,
    DIM = 64,
    DIMMEST = 0
  };

protected:
  HMODULE hGDI32;
  HDC hScreenDC;
  typedef BOOL(WINAPI *Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);

  Type_SetDeviceGammaRamp pGetDeviceGammaRamp;
  Type_SetDeviceGammaRamp pSetDeviceGammaRamp;
  WORD CurrentBrightness;

public:

  CGammaRamp();
  ~CGammaRamp();
  BOOL LoadLibrary();
  void FreeLibrary();
  BOOL LoadLibraryIfNeeded();
  BOOL SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
  BOOL GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
  BOOL SetBrightness(HDC hDC, WORD wBrightness);
  WORD GetCurrentBrightness();
  void ResetBrightness();
  void SetBrightness(Brightness brightness);
};