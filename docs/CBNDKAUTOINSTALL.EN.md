### Installing the NDK installer C :: B template

<<<<<<< HEAD
----------

- Download the installer `NDK C::B template` [CodeBlocksNdkTemplate v.0.0.14.79 / win32 (07/03/2019)](https://clnviewer.github.io/Code-Blocks-Android-NDK/CodeBlocksNdkTemplate.exe)  
- Run the installer and install the `NDK C::B template`  
- Edit the file `config.script` and add the template to the list used.  

### Editing the C::B file with config.script

 The file must be in the following path:

```
<CodeBlocks base directory>\share\CodeBlocks\templates\wizard\config.script

```

You must add the following code to the `RegisterWizards()` function:

```
       function RegisterWizards()
       {
            //...
            RegisterWizard(
                          wizProject,
                          _T("ndk_android"),
                          _T("NDK Android"),
                          _T("Native")
            );
            //...

```

Before using the template, after making these changes, you must restart `C::B`
 
 
### NDK C::B template Wizard
=======
 ----------

 - Download the installer `NDK C :: B template` [CodeBlocksNdkTemplate v.0.0.14.79 / win32 (07/03/2019)] (https://clnviewer.github.io/Code-Blocks-Android-NDK/CodeBlocksNdkTemplate.exe)
 - Run the installer and install the `NDK C :: B template`
 - Edit the file `config.script` and add the template to the list used.

 ### Editing the C :: B file with config.script

 The file must be in the following path:
 
 
 
 You must add the following code to the `RegisterWizards ()` function:
 
 
 Before using the template, after making these changes, you must restart `C :: B`
 
 
 ### NDK C::B template Wizard
>>>>>>> 28c9fc9e65a2a00f161685d9f968855799076423

![Image1](img/Wizard1.png)

![Image1](img/Wizard2.png)

![Image1](img/Wizard3.png)

![Image1](img/Wizard4.png)

![Image1](img/Wizard5.png)

![Image1](img/Wizard6.png)

![Image1](img/Wizard7.png)


### Features

<<<<<<< HEAD
> __Bag? __: unfortunately I did not find a direct way to add debugger parameters directly to the `C::B` project using its API.  
> Currently, the debugger parameters are entered into the `Extensions/debugger1` section, but for operation it is necessary that these parameters be in the `Extensions/debugger` section.  
> After creating the project, use the editor to remove the `Extensions/debugger` section and rename the `Extensions/debugger1` section to `Extensions/debugger`.  
> An example of the contents of this section is given below.  

```xml
<debugger>
	/// One actually platform path
	// <search_path add="obj/local/arm64-v8a" />
	// <search_path add="obj/local/x86" />
	// <search_path add="obj/local/x86_64" />
	<search_path add="obj/local/armeabi-v7a" />
	<remote_debugging target="Debug">
		<options
			conn_type="0"
			serial_baud="115200"
			ip_address="127.0.0.1"
			ip_port="59999"
			additional_cmds_before="set solib-search-path obj/local/armeabi-v7a&#x0A;file obj/local/armeabi-v7a/$(TARGET_OUTPUT_BASENAME)&#x0A;"
		/>
	</remote_debugging>
</debugger>
```

> __Attention! __: the `cbp2ndk` and` android-elf-cleaner` utilities used in `NDK C::B template` are built for the 32bit version of Windows.  
> If you are using a 64bit platform and want to have executable files of the corresponding bitness, you need to rebuild the project yourself.  
> The `cbp2ndk` and` android-elf-cleaner` utilities in `C::B` are located along the following path:  

```
<CodeBlocks base directory>\share\CodeBlocks\templates\wizard\ndk_android\exec\
```

## License
 
  _MIT_
 
  
=======
 > __Bag? __: unfortunately I did not find a direct way to add debugger parameters directly to the `C :: B` project using its API.
 > Currently, the debugger parameters are entered into the `Extensions / debugger1` section, but for operation it is necessary that these parameters be in the` Extensions / debugger` section.
 > After creating the project, use the editor to remove the `Extensions / debugger` section and rename the` Extensions / debugger1` section to `Extensions / debugger`.
 > An example of the contents of this section is given below.
 
 
 > __Attention! __: the `cbp2ndk` and` android-elf-cleaner` utilities used in `NDK C :: B template` are built for the 32bit version of Windows.
 > If you are using a 64bit platform and want to have executable files of the corresponding bitness, you need to rebuild the project yourself.
 > The `cbp2ndk` and` android-elf-cleaner` utilities in `C :: B` are located along the following path:
 
 
>>>>>>> 28c9fc9e65a2a00f161685d9f968855799076423
 
