#include "hashutils.h"
#include <wil/resource.h>
#include <string>

#pragma comment(lib, "hashutils.lib")

int WINAPI wmain(int argc, wchar_t** argv)
{

	if (argc < 2) {
		printf("Usage: %ws <filename>\n", argv[0]);
		::ExitProcess(EXIT_FAILURE);
	}

	wil::unique_handle hFile(::CreateFileW(argv[1], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr));
	if (!hFile) {
		fprintf(stderr, "Error opening file %d\n", ::GetLastError());
		::ExitProcess(EXIT_FAILURE);
	}
	DWORD size = ::GetFileSize(hFile.get(), nullptr);
	wil::unique_handle hMap(::CreateFileMappingW(hFile.get(), nullptr, PAGE_READONLY, 0, 0, nullptr));
	if (!hMap) {
		fprintf(stderr, "Error creating file mapping %d\n", ::GetLastError());
		::ExitProcess(EXIT_FAILURE);
	}
	wil::unique_mapview_ptr<BYTE> buf(static_cast<BYTE*>(::MapViewOfFile(hMap.get(), FILE_MAP_READ, 0, 0, 0)));
	ULONG hashCodeSize = 0;

	printf("File : %ws\n", argv[1]);
	printf("%s\n", std::string(wcslen(L"File : ") + wcslen(argv[1]), '-').c_str());
	printf("%2 MD5: ");
	auto hObj = hashutils::Hash(hashutils::CngHash(hashutils::md5, buf.get(), size, &hashCodeSize));
	for (ULONG i = 0; i < hashCodeSize; ++i) {
		printf("%x", hObj.getDigest()[i]);
	}
	printf("\n%2 SHA1: ");
	hObj = hashutils::Hash(hashutils::CngHash(hashutils::sha1, buf.get(), size, &hashCodeSize));
	for (ULONG i = 0; i < hashCodeSize; ++i) {
		printf("%x", hObj.getDigest()[i]);
	}
	printf("\n%2 SHA256: ");
	hObj = hashutils::Hash(hashutils::CngHash(hashutils::sha256, buf.get(), size, &hashCodeSize));
	for (ULONG i = 0; i < hashCodeSize; ++i) {
		printf("%x", hObj.getDigest()[i]);
	}
	printf("\n%2 SHA512: ");
	hObj = hashutils::Hash(hashutils::CngHash(hashutils::sha512, buf.get(), size, &hashCodeSize));
	for (ULONG i = 0; i < hashCodeSize; ++i) {
		printf("%x", hObj.getDigest()[i]);
	}
	ExitProcess(EXIT_SUCCESS);
}
