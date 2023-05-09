references:

 - https://support.microsoft.com/en-us/topic/how-to-open-a-file-from-a-kernel-mode-device-driver-and-how-to-read-from-or-write-to-the-file-45f7de35-cac4-8d83-9520-a7e480ca404d

write file in kernel mode code

different from user mode, you need to refer a file with its object name rather than file name

for example, the object name of file `C:\Windows\Example.txt` is `\DosDevices\C:\Windows\Example.txt`

the object name is then encapsulated into an structure `OBJECT_ATTRIBUTES` by calling function `InitializeObjectAttributes`



but if your driver is loaded earlier, the `\DosDevices` namespace may not yet exist

therefore `\DosDevices` namespace can't be access by out driver because no driver letter is exposed

the only part of the file system that is guaranteed to be available is the `\SystemRoot` namespace

this namespace is mapped to the folder where the operation system is installed, such as: `C:\Windows` or `D:\Winnt`



the following code illustrates how to refer to a file by its object name:

```c
UNICODE_STRING		uniName;
OBJECT_ATTRIBUTEs	objAttr;

// you can also use L"\\SystemRoot\\example.txt"
RtlInitUnicodeString(&uniName, L"\\DosDevices\\C:\\Windows\\Example.txt");
InitializeObjectAttributes(&objAttr, &uniName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                          NULL, NULL);
```

# obtain a file handle

OK, you already know how to refer a file, then we shall study how to get a handle of it

`ZwCraeteFile` accept an `OBJECT_ARRTIBUTES` structure as a parameter

the `DesriedAccess` parameter can be set to `GENERIC_READ/WRITE/ALL`, it's all your choice

for the `CreateOptions` parameter, if you set it to `FILE_SYNCHRONOUS_IO_NONALERT` or `FILE_SYNCHRONOUS_IO_ALERT`, the system will keep tracking current file-position offset









