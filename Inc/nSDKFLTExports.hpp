#pragma once

#define FLT_EXPORT_CREATEFILTER comment(linker, "/EXPORT:CreateFilter=" __FUNCDNAME__ ",@1")
#define FLT_EXPORT_GETFILTERID comment(linker, "/EXPORT:GetFilterID=" __FUNCDNAME__ ",@3")
#define FLT_EXPORT_CANREADFILE comment(linker, "/EXPORT:CanReadFile=" __FUNCDNAME__ ",@5")
#define FLT_EXPORT_GETPRIORITY comment(linker, "/EXPORT:GetPriority=" __FUNCDNAME__ ",@6")

#ifdef _UNICODE
#define FLT_EXPORT_GETFILTERNAME comment(linker, "/EXPORT:GetFilterNameW=" __FUNCDNAME__ ",@2")
#define FLT_EXPORT_GETFILTEREXTS comment(linker, "/EXPORT:GetFilterExtsW=" __FUNCDNAME__ ",@4")
#define FLT_EXPORT_GETDEPENDENCIES comment(linker, "/EXPORT:GetDependenciesW=" __FUNCDNAME__ ",@7")
#else
#define FLT_EXPORT_GETFILTERNAME comment(linker, "/EXPORT:GetFilterName=" __FUNCDNAME__ ",@2")
#define FLT_EXPORT_GETFILTEREXTS comment(linker, "/EXPORT:GetFilterExts=" __FUNCDNAME__ ",@4")
#define FLT_EXPORT_GETDEPENDENCIES comment(linker, "/EXPORT:GetDependencies=" __FUNCDNAME__ ",@7")
#endif