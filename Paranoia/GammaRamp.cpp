#include "stdafx.h"
#include "GammaRamp.h"

/////////////////////////////////////////////////////////////////////////
CGammaRamp::CGammaRamp()
{
  //Initialize all variables.
  hGDI32 = nullptr;
  hScreenDC = nullptr;
  pGetDeviceGammaRamp = nullptr;
  pSetDeviceGammaRamp = nullptr;
  CurrentBrightness = CGammaRamp::NORMAL; // should be at 128
}

/////////////////////////////////////////////////////////////////////////
CGammaRamp::~CGammaRamp()
{
  FreeLibrary();
}

/////////////////////////////////////////////////////////////////////////
BOOL CGammaRamp::LoadLibrary()
{
  BOOL bReturn = FALSE;

  FreeLibrary();
  //Load the GDI library.
  hGDI32 = ::LoadLibrary(_T("gdi32.dll"));
  if(hGDI32 != nullptr)
  {
    //Get the addresses of GetDeviceGammaRamp and SetDeviceGammaRamp API functions.
    pGetDeviceGammaRamp =
      (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "GetDeviceGammaRamp");

    pSetDeviceGammaRamp =
      (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "SetDeviceGammaRamp");

    //Return TRUE only if these functions exist.
    if(pGetDeviceGammaRamp == nullptr || pSetDeviceGammaRamp == nullptr)
      FreeLibrary();
    else
      bReturn = TRUE;
  }

  return bReturn;
}

/////////////////////////////////////////////////////////////////////////
void CGammaRamp::FreeLibrary()
{
  //Free the GDI library.
  if(hGDI32 != nullptr)
  {
    ::FreeLibrary(hGDI32);
    hGDI32 = nullptr;
  }
}

/////////////////////////////////////////////////////////////////////////
BOOL CGammaRamp::LoadLibraryIfNeeded()
{
  BOOL bReturn = FALSE;

  if(hGDI32 == nullptr)
    LoadLibrary();

  if(pGetDeviceGammaRamp != nullptr && pSetDeviceGammaRamp != nullptr)
    bReturn = TRUE;

  return bReturn;
}

/////////////////////////////////////////////////////////////////////////
BOOL CGammaRamp::SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
  //Call to SetDeviceGammaRamp only if this function is successfully loaded.
  if(LoadLibraryIfNeeded())
  {
    return pSetDeviceGammaRamp(hDC, lpRamp);
  }
  else
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////
BOOL CGammaRamp::GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
  //Call to GetDeviceGammaRamp only if this function is successfully loaded.
  if(LoadLibraryIfNeeded())
  {
    return pGetDeviceGammaRamp(hDC, lpRamp);
  }
  return FALSE;

}

/////////////////////////////////////////////////////////////////////////
BOOL CGammaRamp::SetBrightness(HDC hDC, WORD wBrightness)
{
  /*
  Changes the brightness of the entire screen.
  This function may not work properly in some video cards.

  The wBrightness value should be a number between 0 and 255.
  128 = Regular brightness
  above 128 = brighter
  below 128 = darker

  If hDC is nullptr, SetBrightness automatically load and release
  the display device context for you.

  */
  BOOL bReturn = FALSE;
  HDC hGammaDC = hDC;

  //Load the display device context of the entire screen if hDC is nullptr.
  if(hDC == nullptr)
  {
    hGammaDC = GetDC(nullptr);
  }

  if(hGammaDC != nullptr)
  {
    //Generate the 256-colors array for the specified wBrightness value.
    WORD GammaArray[3][256];

    for(int i = 0; i < 256; i++)
    {
      int iArrayValue = i * (wBrightness + 128);

      if(iArrayValue > 65535)
      {
        iArrayValue = 65535;
      }

      GammaArray[0][i] =
        GammaArray[1][i] =
        GammaArray[2][i] = (WORD)iArrayValue;

    }

    //Set the GammaArray values into the display device context.
    bReturn = SetDeviceGammaRamp(hGammaDC, GammaArray);
    CurrentBrightness = wBrightness;
  }

  if(hDC == nullptr)
  {
    ReleaseDC(nullptr, hGammaDC);
  }

  return bReturn;
}

/////////////////////////////////////////////////////////////////////////
WORD CGammaRamp::GetCurrentBrightness()
{
  return CurrentBrightness;
}

/////////////////////////////////////////////////////////////////////////
void CGammaRamp::ResetBrightness()
{
  SetBrightness(nullptr, 128);
}

/////////////////////////////////////////////////////////////////////////
void CGammaRamp::SetBrightness(Brightness brightness)
{
  SetBrightness(nullptr, brightness);
}
