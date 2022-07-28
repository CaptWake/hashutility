#pragma once
#include "stdafx.h"

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

	PBYTE CngHash(_In_ hashAlgorithm hashAlg, _In_ PBYTE data, _In_ ULONG datasz, _Out_ PULONG digestsz);
	PBYTE LegacyHash(_In_ hashAlgorithm hashAlg, _In_ LPBYTE data, _In_ ULONG datasz, _Out_ PULONG digestsz);
};