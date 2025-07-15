/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   sliderknob_png;
    const int            sliderknob_pngSize = 86992;

    extern const char*   kit_jpeg;
    const int            kit_jpegSize = 8260;

    extern const char*   PluginProcessor_cpp;
    const int            PluginProcessor_cppSize = 9664;

    extern const char*   PluginProcessor_h;
    const int            PluginProcessor_hSize = 2993;

    extern const char*   PluginEditor_cpp;
    const int            PluginEditor_cppSize = 2506;

    extern const char*   PluginEditor_h;
    const int            PluginEditor_hSize = 1385;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

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
