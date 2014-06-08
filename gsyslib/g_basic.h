#pragma once
#ifndef GSYS_BASIC
#define GSYS_BASIC

#define GSYS_LOWORD(l)           ((gsys_word)(((gsys_dword)(l)) & 0xffff))
#define GSYS_HIWORD(l)           ((gsys_word)((((gsys_dword)(l)) >> 16) & 0xffff))
#define GSYS_LOBYTE(w)           ((gsys_byte)(((gsys_dword)(w)) & 0xff))
#define GSYS_HIBYTE(w)           ((gsys_byte)((((gsys_dword)(w)) >> 8) & 0xff))

#endif