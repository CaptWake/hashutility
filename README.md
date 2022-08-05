# hashutils
A Windows hash utility module written in c++ 
## Example usage
The HashCalc application uses the module to hash files from the file system.
```sh
>.\HashCalc.exe HashCalc.exe
File : HashCalc.exe
-------------------
 MD5: cdcbcabb92ccbc2cee23c81d2f8576df
 SHA1: 0a89c69bb1005019c69bb100ec2ad5a7
 SHA256: 6af48bb7688c3f6348b97794f8b92c5489ca84a23f7e6579f2127a20d85f40
 SHA512: a143d1b3549a5c97e3310f153f79fe680da5e3f59682b32ba2b8696dd36ecfa6a9fffa72a019871743bc12f59330f5c382d0ece0f43c5baa0b26622bc9c
 ```
 ## Compile from source
 ### Problem with dependencies
 The project uses the nuget package manager to install the WIL dependency. Visual Studio should help you automatically adjust it, but, in case he doesn't, you should adjust the path from the dependency inside the vcxproj file.
 
 ## Contribute
 You are welcome contributing to this project opening issues or improving the code base!
