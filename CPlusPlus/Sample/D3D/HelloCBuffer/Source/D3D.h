#pragma once

#include "Engine.h"



using namespace Engine;
using namespace Engine::Windows;


Void D3DInit(Void* handle, SizeType width, SizeType height);
Void D3DUpdate();
Void D3DRender();
Void D3DDestroy();

Void D3DExecute();
Void D3DWait();