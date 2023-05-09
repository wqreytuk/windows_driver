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

the `DesiredAccess` parameter can be set to `GENERIC_READ/WRITE/ALL`, it's all your choice

for the `CreateOptions` parameter, if you set it to `FILE_SYNCHRONOUS_IO_NONALERT` or `FILE_SYNCHRONOUS_IO_ALERT`, the system will keep tracking current file-position offset (I don't know what the fuck does this mean, I mean why the system would keep tracking file offset, what is it for? from the original article, the author said with this, you can sequentially read or write to the file later, and access file at random location)



here is an example code:

```c
HANDLE			handle;
NTSTATUS		ntstatus;
IO_STATUS_BLOCK	ioStatusBlock;

// Do not try to perform any file operations at high IRQL levels
// Instead, you may use a work item or a system worker thread to perform file operation
// yes I've seen work item in another book, but I never understand it

// PASSIVE_LEVEL is the lowest IRQL
if(KeGetCurrentIrql() != PASSIVE_LEVEL)
	return STATUS_INVALID_DEVICE_STATE;

ntstatus = ZwCreateFile(&handle,
                       GENERATE_WRITE,
                       &objAttr, &ioStatusBlock, NULL,
                       FILE_ATTRIBUTE_NORMAL,
                       0,
                       FILE_OVERWRITE_IF,
                       FILE_SYNCHRONOUS_IO_NONALERT,
                       NULL, 0);
```



# Read form or write to a file



now we have a handle of the target file, then we can do reading or writing

`ZwReadFile`/`ZwWriteFile`, after finishing the operation, we need use `ZwClose` to close file handle



here is an example code for file writing:

```c
#define BUFFER_SIZE 30
CHAR	buffer[BUFFER_SIZE];
size_t	cb;

if(NT_SUCCESS(ntstatus)) {
	ntstatus = RtlStringCbPrintfA(buffer, sizeof(buffer), "This is %d test\r\n", 0x0);
    if(NT_SUCCESS(ntstatus) {	
        ntstatus = RtlStringCbLengthA(buffer, sizeof(buffer), &cb);
        if(NT_SUCCESS(ntstatus)) {
            ntstatus = ZwWriteFile(handle, NULL, NULL, NULL, &ioStatusBlock, buffer, c, NULL, NULL);
		}
	}
     ZwClose(handle);
}
```



the first function `RtlStringCbPrintfA` is something like `sprintf`, the second function `RtlStringCbLengthA` get the length of buffer, and the third function write buffer to file



here is an example for file reading:

```c
LARGE_INTEGER	byteOffset;

// if I write this code, I'd like set `DesiredAccess` to `GENERIC_ALL`, so I don't have to call ZwCreateFile twice (for writing and reading)
ntstatus = ZwCreateFile(&handle,
                       	GENERIC_READ,
                        &objAttr, &ioStatusBlock,
                        NULL,
                        FILE_ARRTIBUTE_NORMAL,
                        0,
                        FILE_OPEN,
                        FILE_SYNCHRONOUS_IO_NONALERT,
                        NULL, 0);

if(NT_SUCCESS(ntstatus)) {
    byteOffset.LowPart = byteOffset.HighPart = 0;
    ntstatus = ZwReadFile(handle, NULL, NULL, NULL, &ioStatusBlock, buffer, BUFFER_SIZE, &byteOffset, NULL);
    if(NT_SUCESS(ntstatus)) {
        buffer[BUFFER_SIZE-1] = '\0';
        DbgPrint("%s\n", buffer);
    }
    ZwCLose(handle);
}
```













