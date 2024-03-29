/*
 This file is part of DMGBoy.
 
 DMGBoy is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 DMGBoy is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with DMGBoy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Settings.h"
#include "MainFrame.h"
#include "RendererBase.h"
#include "../Def.h"

static BYTE palettes[][4][3] =
{
	{
		{ 16,  57,  16},
		{ 49,  99,  49},
		{140, 173,  16},
		{156, 189,  16}
	},
	{
		{  0,   0,   0},
		{ 85,  85,  85},
		{170, 170, 170},
		{255, 255, 255}
	}
};

RendererBase::RendererBase()
{
    imgBuf1 = NULL;
    imgBuf2 = NULL;
    frontBuffer = NULL;
    backBuffer = NULL;
	winRenderer = NULL;
	CreateScreen();
}

RendererBase::~RendererBase()
{
    if (imgBuf1)
        delete[] imgBuf1;
    
    if (imgBuf2)
        delete[] imgBuf2;
}

wxWindow * RendererBase::GetWinRenderer()
{
    return winRenderer;
}

void RendererBase::SetWinRenderer(wxWindow * parent, wxWindow *renderer)
{
    renderer->SetMinSize(wxSize(GB_SCREEN_W, GB_SCREEN_H));
	this->winRenderer = renderer;
	renderer->SetDropTarget(new DnDFile(parent));
    //renderer->SetCursor( wxCursor( wxCURSOR_BLANK ) ); 
}

void RendererBase::CreateScreen() {
	imgBuf1 = new BYTE[GB_SCREEN_W*GB_SCREEN_H*3];
    imgBuf2 = new BYTE[GB_SCREEN_W*GB_SCREEN_H*3];
    backBuffer = imgBuf1;
    frontBuffer = imgBuf2;
	OnClear();
	ChangePalette(SettingsGetGreenScale());
}

void RendererBase::ChangePalette(bool original)
{
	if (original)
		selPalette = 0;
	else
		selPalette = 1;
}

void RendererBase::OnClear()
{
	int size = GB_SCREEN_W*GB_SCREEN_H*3;
    memset(backBuffer, 0, size);
    memset(frontBuffer, 0, size);
	PageFlip();
}

void RendererBase::PageFlip()
{
    BYTE * aux = frontBuffer;
    frontBuffer = backBuffer;
    backBuffer = aux;
}

//Cuando se actualiza la pantalla de la gameboy
void RendererBase::OnRefreshGBScreen()
{
	PageFlip();
}

//Cuando se actualiza la pantalla del PC
void RendererBase::OnRefreshRealScreen() {
    if (winRenderer) {
        winRenderer->Refresh(false);
        winRenderer->Update();
    }
}

void RendererBase::OnPreDraw()
{

}

void RendererBase::OnPostDraw()
{

}

void RendererBase::OnDrawPixel(int idColor, int x, int y)
{
	BYTE r = palettes[selPalette][idColor][0];
	BYTE g = palettes[selPalette][idColor][1];
	BYTE b = palettes[selPalette][idColor][2];
	
	OnDrawPixel(r, g, b, x, y);
}

void RendererBase::OnDrawPixel(BYTE r, BYTE g, BYTE b, int x, int y)
{
    int sizeLine = GB_SCREEN_W * 3;
	int offsetX = x * 3;
	int offsetY = y * sizeLine;
	int offsetBuf = offsetY + offsetX;
	
	backBuffer[offsetBuf + 0] = r;
	backBuffer[offsetBuf + 1] = g;
	backBuffer[offsetBuf + 2] = b;
}

DnDFile::DnDFile(wxWindow * parent)
{
	this->parent = parent;
}

bool DnDFile::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	MainFrame * frame = (MainFrame *)parent;
	frame->ChangeFile(filenames[0]);
	
	return true;
}
