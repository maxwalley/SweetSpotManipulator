/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   libfakenect_0_5_5_dylib;
    const int            libfakenect_0_5_5_dylibSize = 16520;

    extern const char*   libfakenect_0_5_dylib;
    const int            libfakenect_0_5_dylibSize = 16520;

    extern const char*   libfakenect_dylib;
    const int            libfakenect_dylibSize = 16520;

    extern const char*   libFreenectDriver_0_5_5_dylib;
    const int            libFreenectDriver_0_5_5_dylibSize = 162080;

    extern const char*   libFreenectDriver_0_5_dylib;
    const int            libFreenectDriver_0_5_dylibSize = 162080;

    extern const char*   libFreenectDriver_dylib;
    const int            libFreenectDriver_dylibSize = 162080;

    extern const char*   libfreenect_pc;
    const int            libfreenect_pcSize = 309;

    extern const char*   libfreenect_0_5_5_dylib;
    const int            libfreenect_0_5_5_dylibSize = 59672;

    extern const char*   libfreenect_0_5_dylib;
    const int            libfreenect_0_5_dylibSize = 59672;

    extern const char*   libfreenect_a;
    const int            libfreenect_aSize = 71848;

    extern const char*   libfreenect_dylib;
    const int            libfreenect_dylibSize = 59672;

    extern const char*   libfreenect_sync_0_5_5_dylib;
    const int            libfreenect_sync_0_5_5_dylibSize = 20344;

    extern const char*   libfreenect_sync_0_5_dylib;
    const int            libfreenect_sync_0_5_dylibSize = 20344;

    extern const char*   libfreenect_sync_a;
    const int            libfreenect_sync_aSize = 10216;

    extern const char*   libfreenect_sync_dylib;
    const int            libfreenect_sync_dylibSize = 20344;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 15;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
