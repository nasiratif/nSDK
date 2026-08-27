#pragma once

#define EXT_EXPORT_CREATEFILTER comment(linker, "/EXPORT:CreateFilter=" __FUNCDNAME__ ",@1")
#define EXT_EXPORT_GETFILTERNAME comment(linker, "/EXPORT:GetFilterName=" __FUNCDNAME__ ",@2")
#define EXT_EXPORT_GETFILTERID comment(linker, "/EXPORT:GetFilterID=" __FUNCDNAME__ ",@3")
#define EXT_EXPORT_GETFILTEREXTS comment(linker, "/EXPORT:GetFilterExts=" __FUNCDNAME__ ",@4")
#define EXT_EXPORT_CANREADFILE comment(linker, "/EXPORT:CanReadFile=" __FUNCDNAME__ ",@5")
#define EXT_EXPORT_GETPRIORITY comment(linker, "/EXPORT:GetPriority=" __FUNCDNAME__ ",@6")