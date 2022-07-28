#pragma once
#include <stdio.h>
#include <vector>
#include <Windows.h>
#include <wil/resource.h>
#include <bcrypt.h>

// link bcrypt library to resolve external symbol reference error
#pragma comment(lib, "bcrypt.lib")

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)
#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)