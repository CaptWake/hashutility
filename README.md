# hashutils
A Windows hash utility module written in c++ 
## Example usage
The hashclient application uses the module to hash files from the file system.
```sh
>.\hashclient.exe hashclient.exe
File : hashclient.exe
---------------------
 MD5: aaccb78b6bbe523201e28a5816bae54
 SHA1: e035e3ec7c100501e2ec7c100f7ff29be
 SHA256: c1dc3945dbaacab86ff436278437b168c84f26f1b1b3731611d2b8e9b5cd227
 SHA512: 322e59e372a3669b143bb51f5655c480b1f3166bd3ce7058781afd7035d6db102b558cbe9e50acf1b32954e36b334795ac877489d12063202b395159c8a63ee
 ```
 ## Compile from source
 ### Problem with dependencies
 The project uses the nuget package manager to install the WIL dependency. Visual Studio should help you automatically adjust it, but, in case he doesn't, you should adjust the path from the dependency inside the vcxproj file.
 
 ## Contribute
 You are welcome contributing to this project opening issues or improving the code base!
