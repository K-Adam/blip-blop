#pragma once

void Bug(const char* txt);
void Warning(const char* txt);

// Opens the window, audio and input and loads what the title screen needs. safe_mode keeps the window
bool InitApp(bool safe_mode);
