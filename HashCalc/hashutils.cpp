#include "stdafx.h"
#include "hashutils.h"

PBYTE hashutils::CngHash(_In_ hashAlgorithm hashAlg, _In_ PBYTE data, _In_ ULONG datasz, _Out_ PULONG digestsz) {
	wil::unique_bcrypt_algorithm hAlgorithm;
	wil::unique_bcrypt_hash hHash;
	std::vector<BYTE> hashObj;
	BCRYPT_HANDLE hObject = nullptr;
	ULONG len = 0;
	DWORD hashObjSize = 0;
	DWORD hashCodeSize = 0;
	LPCWSTR pszAlgId;
	NTSTATUS status = STATUS_UNSUCCESSFUL;

	if (!data) {
		fprintf(stderr, "Invalid data pointer\n");
		return nullptr;
	}

	if (!datasz) {
		fprintf(stderr, "Invalid data size\n");
		return nullptr;
	}

	if (!digestsz) {
		fprintf(stderr, "Invalid digest size pointer\n");
		return nullptr;
	}

	switch (hashAlg) {
	case md5:
		pszAlgId = BCRYPT_MD5_ALGORITHM;
		break;
	case sha1:
		pszAlgId = BCRYPT_SHA1_ALGORITHM;
		break;
	case sha256:
		pszAlgId = BCRYPT_SHA256_ALGORITHM;
		break;
	case sha512:
		pszAlgId = BCRYPT_SHA512_ALGORITHM;
		break;
	default:
		fprintf(stderr, "Error hash algorithm not found\n");
		return nullptr;
	}

	if (!NT_SUCCESS(status = ::BCryptOpenAlgorithmProvider(hAlgorithm.addressof(), pszAlgId, nullptr, 0))) {
		fprintf(stderr, "Error acquiring crypto provider file 0x%X\n", status);
		return nullptr;
	}
	if (!NT_SUCCESS(status = ::BCryptGetProperty(hAlgorithm.get(), BCRYPT_OBJECT_LENGTH, (BYTE*)&hashObjSize, sizeof(DWORD), &len, 0))) {
		fprintf(stderr, "Error creating hash object 0x%X\n", status);
		return nullptr;
	}
	hashObj.reserve(hashObjSize);
	if (!NT_SUCCESS(status = ::BCryptCreateHash(hAlgorithm.get(), hHash.addressof(), hashObj.data(), hashObjSize, nullptr, 0, 0))) {
		fprintf(stderr, "Error creating hash object 0x%X\n", status);
		return nullptr;
	}
	if (!NT_SUCCESS(status = ::BCryptHashData(hHash.get(), data, datasz, 0))) {
		fprintf(stderr, "Error calculating hash 0x%X\n", status);
		return nullptr;
	}
	if (!NT_SUCCESS(status = ::BCryptGetProperty(hHash.get(), BCRYPT_HASH_LENGTH, (BYTE*)&hashCodeSize, sizeof(DWORD), &len, 0))) {
		fprintf(stderr, "Error finishing hash 0x%X\n", status);
		return nullptr;
	}
	auto digest = reinterpret_cast<PBYTE>(::HeapAlloc(::GetProcessHeap(), 0, hashCodeSize));
	if (!digest) {
		fprintf(stderr, "Error allocating digest buffer 0x%X\n", ::GetLastError());
		return nullptr;
	}
	if (!NT_SUCCESS(status = ::BCryptFinishHash(hHash.get(), digest, hashCodeSize, 0))) {
		fprintf(stderr, "Error finishing hash 0x%X\n", status);
		return nullptr;
	}

	*digestsz = hashCodeSize;
	return digest;
}

PBYTE hashutils::LegacyHash(_In_ hashAlgorithm hashAlg, _In_ LPBYTE data, _In_ ULONG datasz, _Out_ PULONG digestsz) {
	wil::unique_hcrypthash hHash;
	wil::unique_hcryptprov hProv;
	DWORD hashSize = 0;
	DWORD len = sizeof(DWORD);
	ALG_ID algId;
	LPCWSTR szProvider;
	DWORD dwProvType;

	if (!data) {
		fprintf(stderr, "Invalid data pointer\n");
		return nullptr;
	}

	if (!datasz) {
		fprintf(stderr, "Invalid data size\n");
		return nullptr;
	}

	if (!digestsz) {
		fprintf(stderr, "Invalid digest size pointer\n");
		return nullptr;
	}

	switch (hashAlg) {
	case md5:
		szProvider = MS_DEF_PROV;
		dwProvType = PROV_RSA_FULL;
		algId = CALG_MD5;
		break;
	case sha1:
		szProvider = MS_DEF_PROV;
		dwProvType = PROV_RSA_FULL;
		algId = CALG_SHA1;
		break;
	case sha256:
		szProvider = MS_ENH_RSA_AES_PROV;
		dwProvType = PROV_RSA_AES;
		algId = CALG_SHA_256;
		break;
	case sha512:
		szProvider = MS_ENH_RSA_AES_PROV;
		dwProvType = PROV_RSA_AES;
		algId = CALG_SHA_512;
		break;
	default:
		fprintf(stderr, "Error hash algorithm not found\n");
		return nullptr;
	}

	if (!::CryptAcquireContextW(hProv.addressof(), nullptr, szProvider, dwProvType, CRYPT_VERIFYCONTEXT)) {
		fprintf(stderr, "Error acquiring crypto provider file 0x%X\n", ::GetLastError());
		return nullptr;
	}
	if (!::CryptCreateHash(hProv.get(), algId, 0, 0, hHash.addressof())) {
		fprintf(stderr, "Error creating hash object 0x%X\n", ::GetLastError());
		return nullptr;
	}
	if (!::CryptHashData(hHash.get(), data, datasz, 0)) {
		fprintf(stderr, "Error acquiring hash 0x%X\n", ::GetLastError());
		return nullptr;
	}
	if (!::CryptGetHashParam(hHash.get(), HP_HASHSIZE, (BYTE*)&hashSize, &len, 0)) {
		fprintf(stderr, "Error retrieving hash info 0x%X\n", ::GetLastError());
		return nullptr;
	}
	auto digest = reinterpret_cast<PBYTE>(::HeapAlloc(::GetProcessHeap(), 0, hashSize));
	if (!digest) {
		fprintf(stderr, "Error allocating digest buffer 0x%X\n", ::GetLastError());
		return nullptr;
	}
	::CryptGetHashParam(hHash.get(), HP_HASHVAL, digest, &hashSize, 0);

	*digestsz = hashSize;
	return digest;
}
