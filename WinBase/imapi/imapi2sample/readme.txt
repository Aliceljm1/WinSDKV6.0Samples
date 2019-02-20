IMAPI2SAMPLE

Usage:
 -list
 -write <dir> [-multi] [-close] [-drive <#>] [-boot <file>]
 -audio <dir> [-close] [-drive <#>]
 -image <file>[-close] [-drive <#>] [-bufe | -bufd]
 -erase [-drive <#>]
 
 list      -- list the available writers and their index.
 erase     -- quick erases the chosen recorder.
 fullerase -- full erases the chosen recorder.
 write     -- Writes a directory to the disc.
    <dir>  -- Directory to be written.
    [-close] -- Close disc (not appendable).
    [-drive <#>] -- choose the given recorder index.
    [-iso, -udf, -joliet] -- specify the file system to write.
    [-multi] -- Add addition session to disc.
    [-boot <file>] -- Create a boot disk.  File is a boot image.
 eject     -- Eject the CD tray
 inject    -- Close the CD tray