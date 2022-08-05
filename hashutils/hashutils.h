#pragma once
// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#pragma comment (lib, "bcrypt.lib")
#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)
#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)

namespace hashutils {
	class Hash {
	public:
		explicit Hash(PBYTE& digest) : _digest(digest) {};
		Hash(const PBYTE& digest) = delete;
		Hash(PBYTE&& digest) : _digest(digest) { digest = nullptr; }
		~Hash() { ::HeapFree(::GetProcessHeap(), 0, _digest); }
		PBYTE getDigest() { return _digest; }

	private:
		PBYTE _digest;
	};

	typedef enum {
		md5,
		sha1,
		sha256,
		sha512
	} hashAlgorithm;

	extern "C" PBYTE CngHash(_In_ hashAlgorithm hashAlg, _In_ PBYTE data, _In_ ULONG datasz, _Out_ PULONG digestsz);
	extern "C" PBYTE LegacyHash(_In_ hashAlgorithm hashAlg, _In_ LPBYTE data, _In_ ULONG datasz, _Out_ PULONG digestsz);
};