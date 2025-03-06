/*******************************************************************************************
*
*   raygui v4.5-dev - A simple and easy-to-use immediate-mode gui library
*
*   DESCRIPTION:
*       raygui is a tools-dev-focused immediate-mode-gui library based on raylib but also
*       available as a standalone library, as long as input and drawing functions are provided.
*
*   FEATURES:
*       - Immediate-mode gui, minimal retained data
*       - +25 controls provided (basic and advanced)
*       - Styling system for colors, font and metrics
*       - Icons supported, embedded as a 1-bit icons pack
*       - Standalone mode option (custom input/graphics backend)
*       - Multiple support tools provided for raygui development
*
*   POSSIBLE IMPROVEMENTS:
*       - Better standalone mode API for easy plug of custom backends
*       - Externalize required inputs, allow user easier customization
*
*   LIMITATIONS:
*       - No editable multi-line word-wraped text box supported
*       - No auto-layout mechanism, up to the user to define controls position and size
*       - Standalone mode requires library modification and some user work to plug another backend
*
*   NOTES:
*       - WARNING: GuiLoadStyle() and GuiLoadStyle{Custom}() functions, allocate memory for
*         font atlas recs and glyphs, freeing that memory is (usually) up to the user,
*         no unload function is explicitly provided... but note that GuiLoadStyleDefault() unloads
*         by default any previously loaded font (texture, recs, glyphs).
*       - Global UI alpha (guiAlpha) is applied inside GuiDrawRectangle() and GuiDrawText() functions
*
*   CONTROLS PROVIDED:
*     # Container/separators Controls
*       - WindowBox     --> StatusBar, Panel
*       - GroupBox      --> Line
*       - Line
*       - Panel         --> StatusBar
*       - ScrollPanel   --> StatusBar
*       - TabBar        --> Button
*
*     # Basic Controls
*       - Label
*       - LabelButton   --> Label
*       - Button
*       - Toggle
*       - ToggleGroup   --> Toggle
*       - ToggleSlider
*       - CheckBox
*       - ComboBox
*       - DropdownBox
*       - TextBox
*       - ValueBox      --> TextBox
*       - Spinner       --> Button, ValueBox
*       - Slider
*       - SliderBar     --> Slider
*       - ProgressBar
*       - StatusBar
*       - DummyRec
*       - Grid
*
*     # Advance Controls
*       - ListView
*       - ColorPicker   --> ColorPanel, ColorBarHue
*       - MessageBox    --> Window, Label, Button
*       - TextInputBox  --> Window, Label, TextBox, Button
*
*     It also provides a set of functions for styling the controls based on its properties (size, color).
*
*
*   RAYGUI STYLE (guiStyle):
*       raygui uses a global data array for all gui style properties (allocated on data segment by default),
*       when a new style is loaded, it is loaded over the global style... but a default gui style could always be
*       recovered with GuiLoadStyleDefault() function, that overwrites the current style to the default one
*
*       The global style array size is fixed and depends on the number of controls and properties:
*
*           static unsigned int guiStyle[RAYGUI_MAX_CONTROLS*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)];
*
*       guiStyle size is by default: 16*(16 + 8) = 384*4 = 1536 bytes = 1.5 KB
*
*       Note that the first set of BASE properties (by default guiStyle[0..15]) belong to the generic style
*       used for all controls, when any of those base values is set, it is automatically populated to all
*       controls, so, specific control values overwriting generic style should be set after base values.
*
*       After the first BASE set we have the EXTENDED properties (by default guiStyle[16..23]), those
*       properties are actually common to all controls and can not be overwritten individually (like BASE ones)
*       Some of those properties are: TEXT_SIZE, TEXT_SPACING, LINE_COLOR, BACKGROUND_COLOR
*
*       Custom control properties can be defined using the EXTENDED properties for each independent control.
*
*       TOOL: rGuiStyler is a visual tool to customize raygui style: github.com/raysan5/rguistyler
*
*
*   RAYGUI ICONS (guiIcons):
*       raygui could use a global array containing icons data (allocated on data segment by default),
*       a custom icons set could be loaded over this array using GuiLoadIcons(), but loaded icons set
*       must be same RAYGUI_ICON_SIZE and no more than RAYGUI_ICON_MAX_ICONS will be loaded
*
*       Every icon is codified in binary form, using 1 bit per pixel, so, every 16x16 icon
*       requires 8 integers (16*16/32) to be stored in memory.
*
*       When the icon is draw, actually one quad per pixel is drawn if the bit for that pixel is set.
*
*       The global icons array size is fixed and depends on the number of icons and size:
*
*           static unsigned int guiIcons[RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS];
*
*       guiIcons size is by default: 256*(16*16/32) = 2048*4 = 8192 bytes = 8 KB
*
*       TOOL: rGuiIcons is a visual tool to customize/create raygui icons: github.com/raysan5/rguiicons
*
*   RAYGUI LAYOUT:
*       raygui currently does not provide an auto-layout mechanism like other libraries,
*       layouts must be defined manually on controls drawing, providing the right bounds Rectangle for it.
*
*       TOOL: rGuiLayout is a visual tool to create raygui layouts: github.com/raysan5/rguilayout
*
*   CONFIGURATION:
*       #define RAYGUI_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RAYGUI_STANDALONE
*           Avoid raylib.h header inclusion in this file. Data types defined on raylib are defined
*           internally in the library and input management and drawing functions must be provided by
*           the user (check library implementation for further details).
*
*       #define RAYGUI_NO_ICONS
*           Avoid including embedded ricons data (256 icons, 16x16 pixels, 1-bit per pixel, 2KB)
*
*       #define RAYGUI_CUSTOM_ICONS
*           Includes custom ricons.h header defining a set of custom icons,
*           this file can be generated using rGuiIcons tool
*
*       #define RAYGUI_DEBUG_RECS_BOUNDS
*           Draw control bounds rectangles for debug
*
*       #define RAYGUI_DEBUG_TEXT_BOUNDS
*           Draw text bounds rectangles for debug
*
*   VERSIONS HISTORY:
*       4.5-dev (Sep-2024)    Current dev version...
*                         ADDED: guiControlExclusiveMode and guiControlExclusiveRec for exclusive modes
*                         ADDED: GuiValueBoxFloat()
*                         ADDED: GuiDropdonwBox() properties: DROPDOWN_ARROW_HIDDEN, DROPDOWN_ROLL_UP
*                         ADDED: GuiListView() property: LIST_ITEMS_BORDER_WIDTH
*                         ADDED: Multiple new icons
*                         REVIEWED: GuiTabBar(), close tab with mouse middle button
*                         REVIEWED: GuiScrollPanel(), scroll speed proportional to content
*                         REVIEWED: GuiDropdownBox(), support roll up and hidden arrow
*                         REVIEWED: GuiTextBox(), cursor position initialization
*                         REVIEWED: GuiSliderPro(), control value change check
*                         REVIEWED: GuiGrid(), simplified implementation
*                         REVIEWED: GuiIconText(), increase buffer size and reviewed padding
*                         REVIEWED: GuiDrawText(), improved wrap mode drawing
*                         REVIEWED: GuiScrollBar(), minor tweaks
*                         REVIEWED: Functions descriptions, removed wrong return value reference
*                         REDESIGNED: GuiColorPanel(), improved HSV <-> RGBA convertion
*
*       4.0 (12-Sep-2023) ADDED: GuiToggleSlider()
*                         ADDED: GuiColorPickerHSV() and GuiColorPanelHSV()
*                         ADDED: Multiple new icons, mostly compiler related
*                         ADDED: New DEFAULT properties: TEXT_LINE_SPACING, TEXT_ALIGNMENT_VERTICAL, TEXT_WRAP_MODE
*                         ADDED: New enum values: GuiTextAlignment, GuiTextAlignmentVertical, GuiTextWrapMode
*                         ADDED: Support loading styles with custom font charset from external file
*                         REDESIGNED: GuiTextBox(), support mouse cursor positioning
*                         REDESIGNED: GuiDrawText(), support multiline and word-wrap modes (read only)
*                         REDESIGNED: GuiProgressBar() to be more visual, progress affects border color
*                         REDESIGNED: Global alpha consideration moved to GuiDrawRectangle() and GuiDrawText()
*                         REDESIGNED: GuiScrollPanel(), get parameters by reference and return result value
*                         REDESIGNED: GuiToggleGroup(), get parameters by reference and return result value
*                         REDESIGNED: GuiComboBox(), get parameters by reference and return result value
*                         REDESIGNED: GuiCheckBox(), get parameters by reference and return result value
*                         REDESIGNED: GuiSlider(), get parameters by reference and return result value
*                         REDESIGNED: GuiSliderBar(), get parameters by reference and return result value
*                         REDESIGNED: GuiProgressBar(), get parameters by reference and return result value
*                         REDESIGNED: GuiListView(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorPicker(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorPanel(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorBarAlpha(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorBarHue(), get parameters by reference and return result value
*                         REDESIGNED: GuiGrid(), get parameters by reference and return result value
*                         REDESIGNED: GuiGrid(), added extra parameter
*                         REDESIGNED: GuiListViewEx(), change parameters order
*                         REDESIGNED: All controls return result as int value
*                         REVIEWED: GuiScrollPanel() to avoid smallish scroll-bars
*                         REVIEWED: All examples and specially controls_test_suite
*                         RENAMED: gui_file_dialog module to gui_window_file_dialog
*                         UPDATED: All styles to include ISO-8859-15 charset (as much as possible)
*
*       3.6 (10-May-2023) ADDED: New icon: SAND_TIMER
*                         ADDED: GuiLoadStyleFromMemory() (binary only)
*                         REVIEWED: GuiScrollBar() horizontal movement key
*                         REVIEWED: GuiTextBox() crash on cursor movement
*                         REVIEWED: GuiTextBox(), additional inputs support
*                         REVIEWED: GuiLabelButton(), avoid text cut
*                         REVIEWED: GuiTextInputBox(), password input
*                         REVIEWED: Local GetCodepointNext(), aligned with raylib
*                         REDESIGNED: GuiSlider*()/GuiScrollBar() to support out-of-bounds
*
*       3.5 (20-Apr-2023) ADDED: GuiTabBar(), based on GuiToggle()
*                         ADDED: Helper functions to split text in separate lines
*                         ADDED: Multiple new icons, useful for code editing tools
*                         REMOVED: Unneeded icon editing functions
*                         REMOVED: GuiTextBoxMulti(), very limited and broken
*                         REMOVED: MeasureTextEx() dependency, logic directly implemented
*                         REMOVED: DrawTextEx() dependency, logic directly implemented
*                         REVIEWED: GuiScrollBar(), improve mouse-click behaviour
*                         REVIEWED: Library header info, more info, better organized
*                         REDESIGNED: GuiTextBox() to support cursor movement
*                         REDESIGNED: GuiDrawText() to divide drawing by lines
*
*       3.2 (22-May-2022) RENAMED: Some enum values, for unification, avoiding prefixes
*                         REMOVED: GuiScrollBar(), only internal
*                         REDESIGNED: GuiPanel() to support text parameter
*                         REDESIGNED: GuiScrollPanel() to support text parameter
*                         REDESIGNED: GuiColorPicker() to support text parameter
*                         REDESIGNED: GuiColorPanel() to support text parameter
*                         REDESIGNED: GuiColorBarAlpha() to support text parameter
*                         REDESIGNED: GuiColorBarHue() to support text parameter
*                         REDESIGNED: GuiTextInputBox() to support password
*
*       3.1 (12-Jan-2022) REVIEWED: Default style for consistency (aligned with rGuiLayout v2.5 tool)
*                         REVIEWED: GuiLoadStyle() to support compressed font atlas image data and unload previous textures
*                         REVIEWED: External icons usage logic
*                         REVIEWED: GuiLine() for centered alignment when including text
*                         RENAMED: Multiple controls properties definitions to prepend RAYGUI_
*                         RENAMED: RICON_ references to RAYGUI_ICON_ for library consistency
*                         Projects updated and multiple tweaks
*
*       3.0 (04-Nov-2021) Integrated ricons data to avoid external file
*                         REDESIGNED: GuiTextBoxMulti()
*                         REMOVED: GuiImageButton*()
*                         Multiple minor tweaks and bugs corrected
*
*       2.9 (17-Mar-2021) REMOVED: Tooltip API
*       2.8 (03-May-2020) Centralized rectangles drawing to GuiDrawRectangle()
*       2.7 (20-Feb-2020) ADDED: Possible tooltips API
*       2.6 (09-Sep-2019) ADDED: GuiTextInputBox()
*                         REDESIGNED: GuiListView*(), GuiDropdownBox(), GuiSlider*(), GuiProgressBar(), GuiMessageBox()
*                         REVIEWED: GuiTextBox(), GuiSpinner(), GuiValueBox(), GuiLoadStyle()
*                         Replaced property INNER_PADDING by TEXT_PADDING, renamed some properties
*                         ADDED: 8 new custom styles ready to use
*                         Multiple minor tweaks and bugs corrected
*
*       2.5 (28-May-2019) Implemented extended GuiTextBox(), GuiValueBox(), GuiSpinner()
*       2.3 (29-Apr-2019) ADDED: rIcons auxiliar library and support for it, multiple controls reviewed
*                         Refactor all controls drawing mechanism to use control state
*       2.2 (05-Feb-2019) ADDED: GuiScrollBar(), GuiScrollPanel(), reviewed GuiListView(), removed Gui*Ex() controls
*       2.1 (26-Dec-2018) REDESIGNED: GuiCheckBox(), GuiComboBox(), GuiDropdownBox(), GuiToggleGroup() > Use combined text string
*                         REDESIGNED: Style system (breaking change)
*       2.0 (08-Nov-2018) ADDED: Support controls guiLock and custom fonts
*                         REVIEWED: GuiComboBox(), GuiListView()...
*       1.9 (09-Oct-2018) REVIEWED: GuiGrid(), GuiTextBox(), GuiTextBoxMulti(), GuiValueBox()...
*       1.8 (01-May-2018) Lot of rework and redesign to align with rGuiStyler and rGuiLayout
*       1.5 (21-Jun-2017) Working in an improved styles system
*       1.4 (15-Jun-2017) Rewritten all GUI functions (removed useless ones)
*       1.3 (12-Jun-2017) Complete redesign of style system
*       1.1 (01-Jun-2017) Complete review of the library
*       1.0 (07-Jun-2016) Converted to header-only by Ramon Santamaria.
*       0.9 (07-Mar-2016) Reviewed and tested by Albert Martos, Ian Eito, Sergio Martinez and Ramon Santamaria.
*       0.8 (27-Aug-2015) Initial release. Implemented by Kevin Gato, Daniel Nicolás and Ramon Santamaria.
*
*   DEPENDENCIES:
*       raylib 5.0  - Inputs reading (keyboard/mouse), shapes drawing, font loading and text drawing
*
*   STANDALONE MODE:
*       By default raygui depends on raylib mostly for the inputs and the drawing functionality but that dependency can be disabled
*       with the config flag RAYGUI_STANDALONE. In that case is up to the user to provide another backend to cover library needs.
*
*       The following functions should be redefined for a custom backend:
*
*           - Vector2 GetMousePosition(void);
*           - float GetMouseWheelMove(void);
*           - bool IsMouseButtonDown(int button);
*           - bool IsMouseButtonPressed(int button);
*           - bool IsMouseButtonReleased(int button);
*           - bool IsKeyDown(int key);
*           - bool IsKeyPressed(int key);
*           - int GetCharPressed(void);         // -- GuiTextBox(), GuiValueBox()
*
*           - void DrawRectangle(int x, int y, int width, int height, Color color); // -- GuiDrawRectangle()
*           - void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4); // -- GuiColorPicker()
*
*           - Font GetFontDefault(void);                            // -- GuiLoadStyleDefault()
*           - Font LoadFontEx(const char *fileName, int fontSize, int *codepoints, int codepointCount); // -- GuiLoadStyle()
*           - Texture2D LoadTextureFromImage(Image image);          // -- GuiLoadStyle(), required to load texture from embedded font atlas image
*           - void SetShapesTexture(Texture2D tex, Rectangle rec);  // -- GuiLoadStyle(), required to set shapes rec to font white rec (optimization)
*           - char *LoadFileText(const char *fileName);             // -- GuiLoadStyle(), required to load charset data
*           - void UnloadFileText(char *text);                      // -- GuiLoadStyle(), required to unload charset data
*           - const char *GetDirectoryPath(const char *filePath);   // -- GuiLoadStyle(), required to find charset/font file from text .rgs
*           - int *LoadCodepoints(const char *text, int *count);    // -- GuiLoadStyle(), required to load required font codepoints list
*           - void UnloadCodepoints(int *codepoints);               // -- GuiLoadStyle(), required to unload codepoints list
*           - unsigned char *DecompressData(const unsigned char *compData, int compDataSize, int *dataSize); // -- GuiLoadStyle()
*
*   CONTRIBUTORS:
*       Ramon Santamaria:   Supervision, review, redesign, update and maintenance
*       Vlad Adrian:        Complete rewrite of GuiTextBox() to support extended features (2019)
*       Sergio Martinez:    Review, testing (2015) and redesign of multiple controls (2018)
*       Adria Arranz:       Testing and implementation of additional controls (2018)
*       Jordi Jorba:        Testing and implementation of additional controls (2018)
*       Albert Martos:      Review and testing of the library (2015)
*       Ian Eito:           Review and testing of the library (2015)
*       Kevin Gato:         Initial implementation of basic components (2014)
*       Daniel Nicolas:     Initial implementation of basic components (2014)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RAYGUI_H
#define RAYGUI_H

#define RAYGUI_VERSION_MAJOR 4
#define RAYGUI_VERSION_MINOR 5
#define RAYGUI_VERSION_PATCH 0
#define RAYGUI_VERSION  "4.5-dev"

#if !defined(RAYGUI_STANDALONE)
    #include "raylib.h"
#endif

#if defined(RAYGUI_IMPLEMENTATION)
#if defined(_WIN32) && defined(BUILD_LIBTYPE_SHARED)
#define RAYGUIAPI extern __declspec(dllexport) // We are building raylib as a Win32 shared library (.dll)
#elif defined(BUILD_LIBTYPE_SHARED)
#define RAYGUIAPI __attribute__((visibility("default"))) // We are building raylib as a Unix shared library (.so/.dylib)
#elif defined(_WIN32) && defined(USE_LIBTYPE_SHARED)
#define RAYGUIAPI __declspec(dllimport)         // We are using raylib as a Win32 shared library (.dll)
#else
#define RAYGUIAPI extern inline // Provide external definition
#endif
#elif defined(RAYMATH_STATIC_INLINE)
#define RAYGUIAPI static inline // Functions may be inlined, no external out-of-line definition
#else
#if defined(__TINYC__)
#define RAYGUIAPI static inline // plain inline not supported by tinycc (See issue #435)
#else
#define RAYGUIAPI inline        // Functions may be inlined or external definition used
#endif
#endif

// Function specifiers definition
#ifndef RAYGUIAPI
    #define RAYGUIAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Allow custom memory allocators
#ifndef RAYGUI_MALLOC
    #define RAYGUI_MALLOC(sz)       malloc(sz)
#endif
#ifndef RAYGUI_CALLOC
    #define RAYGUI_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RAYGUI_FREE
    #define RAYGUI_FREE(p)          free(p)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define RAYGUI_SUPPORT_LOG_INFO
#if defined(RAYGUI_SUPPORT_LOG_INFO)
  #define RAYGUI_LOG(...)           printf(__VA_ARGS__)
#else
  #define RAYGUI_LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Some types are required for RAYGUI_STANDALONE usage
//----------------------------------------------------------------------------------
#if defined(RAYGUI_STANDALONE)
    #ifndef __cplusplus
    // Boolean type
        #ifndef true
            typedef enum { false, true } bool;
        #endif
    #endif

    // Vector2 type
    typedef struct Vector2 {
        float x;
        float y;
    } Vector2;

    // Vector3 type                 // -- ConvertHSVtoRGB(), ConvertRGBtoHSV()
    typedef struct Vector3 {
        float x;
        float y;
        float z;
    } Vector3;

    // Color type, RGBA (32bit)
    typedef struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } Color;

    // Rectangle type
    typedef struct Rectangle {
        float x;
        float y;
        float width;
        float height;
    } Rectangle;

    // TODO: Texture2D type is very coupled to raylib, required by Font type
    // It should be redesigned to be provided by user
    typedef struct Texture2D {
        unsigned int id;        // OpenGL texture id
        int width;              // Texture base width
        int height;             // Texture base height
        int mipmaps;            // Mipmap levels, 1 by default
        int format;             // Data format (PixelFormat type)
    } Texture2D;

    // Image, pixel data stored in CPU memory (RAM)
    typedef struct Image {
        void *data;             // Image raw data
        int width;              // Image base width
        int height;             // Image base height
        int mipmaps;            // Mipmap levels, 1 by default
        int format;             // Data format (PixelFormat type)
    } Image;

    // GlyphInfo, font characters glyphs info
    typedef struct GlyphInfo {
        int value;              // Character value (Unicode)
        int offsetX;            // Character offset X when drawing
        int offsetY;            // Character offset Y when drawing
        int advanceX;           // Character advance position X
        Image image;            // Character image data
    } GlyphInfo;

    // TODO: Font type is very coupled to raylib, mostly required by GuiLoadStyle()
    // It should be redesigned to be provided by user
    typedef struct Font {
        int baseSize;           // Base size (default chars height)
        int glyphCount;         // Number of glyph characters
        int glyphPadding;       // Padding around the glyph characters
        Texture2D texture;      // Texture atlas containing the glyphs
        Rectangle *recs;        // Rectangles in texture for the glyphs
        GlyphInfo *glyphs;      // Glyphs info data
    } Font;
#endif

// Style property
// NOTE: Used when exporting style as code for convenience
typedef struct GuiStyleProp {
    unsigned short controlId;   // Control identifier
    unsigned short propertyId;  // Property identifier
    int propertyValue;          // Property value
} GuiStyleProp;

/*
// Controls text style -NOT USED-
// NOTE: Text style is defined by control
typedef struct GuiTextStyle {
    unsigned int size;
    int charSpacing;
    int lineSpacing;
    int alignmentH;
    int alignmentV;
    int padding;
} GuiTextStyle;
*/

// Gui control state
typedef enum {
    STATE_NORMAL = 0,
    STATE_FOCUSED,
    STATE_PRESSED,
    STATE_DISABLED
} GuiState;

// Gui control text alignment
typedef enum {
    TEXT_ALIGN_LEFT = 0,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
} GuiTextAlignment;

// Gui control text alignment vertical
// NOTE: Text vertical position inside the text bounds
typedef enum {
    TEXT_ALIGN_TOP = 0,
    TEXT_ALIGN_MIDDLE,
    TEXT_ALIGN_BOTTOM
} GuiTextAlignmentVertical;

// Gui control text wrap mode
// NOTE: Useful for multiline text
typedef enum {
    TEXT_WRAP_NONE = 0,
    TEXT_WRAP_CHAR,
    TEXT_WRAP_WORD
} GuiTextWrapMode;

// Gui controls
typedef enum {
    // Default -> populates to all controls when set
    DEFAULT = 0,

    // Basic controls
    LABEL,          // Used also for: LABELBUTTON
    BUTTON,
    TOGGLE,         // Used also for: TOGGLEGROUP
    SLIDER,         // Used also for: SLIDERBAR, TOGGLESLIDER
    PROGRESSBAR,
    CHECKBOX,
    COMBOBOX,
    DROPDOWNBOX,
    TEXTBOX,        // Used also for: TEXTBOXMULTI
    VALUEBOX,
    SPINNER,        // Uses: BUTTON, VALUEBOX
    LISTVIEW,
    COLORPICKER,
    SCROLLBAR,
    STATUSBAR
} GuiControl;

// Gui base properties for every control
// NOTE: RAYGUI_MAX_PROPS_BASE properties (by default 16 properties)
typedef enum {
    BORDER_COLOR_NORMAL = 0,    // Control border color in STATE_NORMAL
    BASE_COLOR_NORMAL,          // Control base color in STATE_NORMAL
    TEXT_COLOR_NORMAL,          // Control text color in STATE_NORMAL
    BORDER_COLOR_FOCUSED,       // Control border color in STATE_FOCUSED
    BASE_COLOR_FOCUSED,         // Control base color in STATE_FOCUSED
    TEXT_COLOR_FOCUSED,         // Control text color in STATE_FOCUSED
    BORDER_COLOR_PRESSED,       // Control border color in STATE_PRESSED
    BASE_COLOR_PRESSED,         // Control base color in STATE_PRESSED
    TEXT_COLOR_PRESSED,         // Control text color in STATE_PRESSED
    BORDER_COLOR_DISABLED,      // Control border color in STATE_DISABLED
    BASE_COLOR_DISABLED,        // Control base color in STATE_DISABLED
    TEXT_COLOR_DISABLED,        // Control text color in STATE_DISABLED
    BORDER_WIDTH,               // Control border size, 0 for no border
    //TEXT_SIZE,                  // Control text size (glyphs max height) -> GLOBAL for all controls
    //TEXT_SPACING,               // Control text spacing between glyphs -> GLOBAL for all controls
    //TEXT_LINE_SPACING           // Control text spacing between lines -> GLOBAL for all controls
    TEXT_PADDING,               // Control text padding, not considering border
    TEXT_ALIGNMENT,             // Control text horizontal alignment inside control text bound (after border and padding)
    //TEXT_WRAP_MODE              // Control text wrap-mode inside text bounds -> GLOBAL for all controls
} GuiControlProperty;

// TODO: Which text styling properties should be global or per-control?
// At this moment TEXT_PADDING and TEXT_ALIGNMENT is configured and saved per control while
// TEXT_SIZE, TEXT_SPACING, TEXT_LINE_SPACING, TEXT_ALIGNMENT_VERTICAL, TEXT_WRAP_MODE are global and
// should be configured by user as needed while defining the UI layout

// Gui extended properties depend on control
// NOTE: RAYGUI_MAX_PROPS_EXTENDED properties (by default, max 8 properties)
//----------------------------------------------------------------------------------
// DEFAULT extended properties
// NOTE: Those properties are common to all controls or global
// WARNING: We only have 8 slots for those properties by default!!! -> New global control: TEXT?
typedef enum {
    TEXT_SIZE = 16,             // Text size (glyphs max height)
    TEXT_SPACING,               // Text spacing between glyphs
    LINE_COLOR,                 // Line control color
    BACKGROUND_COLOR,           // Background color
    TEXT_LINE_SPACING,          // Text spacing between lines
    TEXT_ALIGNMENT_VERTICAL,    // Text vertical alignment inside text bounds (after border and padding)
    TEXT_WRAP_MODE              // Text wrap-mode inside text bounds
    //TEXT_DECORATION             // Text decoration: 0-None, 1-Underline, 2-Line-through, 3-Overline
    //TEXT_DECORATION_THICK       // Text decoration line thickness
} GuiDefaultProperty;

// Other possible text properties:
// TEXT_WEIGHT                  // Normal, Italic, Bold -> Requires specific font change
// TEXT_INDENT                  // Text indentation -> Now using TEXT_PADDING...

// Label
//typedef enum { } GuiLabelProperty;

// Button/Spinner
//typedef enum { } GuiButtonProperty;

// Toggle/ToggleGroup
typedef enum {
    GROUP_PADDING = 16,         // ToggleGroup separation between toggles
} GuiToggleProperty;

// Slider/SliderBar
typedef enum {
    SLIDER_WIDTH = 16,          // Slider size of internal bar
    SLIDER_PADDING              // Slider/SliderBar internal bar padding
} GuiSliderProperty;

// ProgressBar
typedef enum {
    PROGRESS_PADDING = 16,      // ProgressBar internal padding
} GuiProgressBarProperty;

// ScrollBar
typedef enum {
    ARROWS_SIZE = 16,           // ScrollBar arrows size
    ARROWS_VISIBLE,             // ScrollBar arrows visible
    SCROLL_SLIDER_PADDING,      // ScrollBar slider internal padding
    SCROLL_SLIDER_SIZE,         // ScrollBar slider size
    SCROLL_PADDING,             // ScrollBar scroll padding from arrows
    SCROLL_SPEED,               // ScrollBar scrolling speed
} GuiScrollBarProperty;

// CheckBox
typedef enum {
    CHECK_PADDING = 16          // CheckBox internal check padding
} GuiCheckBoxProperty;

// ComboBox
typedef enum {
    COMBO_BUTTON_WIDTH = 16,    // ComboBox right button width
    COMBO_BUTTON_SPACING        // ComboBox button separation
} GuiComboBoxProperty;

// DropdownBox
typedef enum {
    ARROW_PADDING = 16,         // DropdownBox arrow separation from border and items
    DROPDOWN_ITEMS_SPACING,     // DropdownBox items separation
    DROPDOWN_ARROW_HIDDEN,      // DropdownBox arrow hidden
    DROPDOWN_ROLL_UP            // DropdownBox roll up flag (default rolls down)
} GuiDropdownBoxProperty;

// TextBox/TextBoxMulti/ValueBox/Spinner
typedef enum {
    TEXT_READONLY = 16,         // TextBox in read-only mode: 0-text editable, 1-text no-editable
} GuiTextBoxProperty;

// Spinner
typedef enum {
    SPIN_BUTTON_WIDTH = 16,     // Spinner left/right buttons width
    SPIN_BUTTON_SPACING,        // Spinner buttons separation
} GuiSpinnerProperty;

// ListView
typedef enum {
    LIST_ITEMS_HEIGHT = 16,     // ListView items height
    LIST_ITEMS_SPACING,         // ListView items separation
    SCROLLBAR_WIDTH,            // ListView scrollbar size (usually width)
    SCROLLBAR_SIDE,             // ListView scrollbar side (0-SCROLLBAR_LEFT_SIDE, 1-SCROLLBAR_RIGHT_SIDE)
    LIST_ITEMS_BORDER_WIDTH     // ListView items border width
} GuiListViewProperty;

// ColorPicker
typedef enum {
    COLOR_SELECTOR_SIZE = 16,
    HUEBAR_WIDTH,               // ColorPicker right hue bar width
    HUEBAR_PADDING,             // ColorPicker right hue bar separation from panel
    HUEBAR_SELECTOR_HEIGHT,     // ColorPicker right hue bar selector height
    HUEBAR_SELECTOR_OVERFLOW    // ColorPicker right hue bar selector overflow
} GuiColorPickerProperty;

#define SCROLLBAR_LEFT_SIDE     0
#define SCROLLBAR_RIGHT_SIDE    1

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// Global gui state control functions
RAYGUIAPI void GuiEnable(void);                                 // Enable gui controls (global state)
RAYGUIAPI void GuiDisable(void);                                // Disable gui controls (global state)
RAYGUIAPI void GuiLock(void);                                   // Lock gui controls (global state)
RAYGUIAPI void GuiUnlock(void);                                 // Unlock gui controls (global state)
RAYGUIAPI bool GuiIsLocked(void);                               // Check if gui is locked (global state)
RAYGUIAPI void GuiSetAlpha(float alpha);                        // Set gui controls alpha (global state), alpha goes from 0.0f to 1.0f
RAYGUIAPI void GuiSetState(int state);                          // Set gui state (global state)
RAYGUIAPI int GuiGetState(void);                                // Get gui state (global state)

// Font set/get functions
RAYGUIAPI void GuiSetFont(Font font);                           // Set gui custom font (global state)
RAYGUIAPI Font GuiGetFont(void);                                // Get gui custom font (global state)

// Style set/get functions
RAYGUIAPI void GuiSetStyle(int control, int property, int value); // Set one style property
RAYGUIAPI int GuiGetStyle(int control, int property);           // Get one style property

// Styles loading functions
RAYGUIAPI void GuiLoadStyle(const char *fileName);              // Load style file over global style variable (.rgs)
RAYGUIAPI void GuiLoadStyleDefault(void);                       // Load style default over global style

// Tooltips management functions
RAYGUIAPI void GuiEnableTooltip(void);                          // Enable gui tooltips (global state)
RAYGUIAPI void GuiDisableTooltip(void);                         // Disable gui tooltips (global state)
RAYGUIAPI void GuiSetTooltip(const char *tooltip);              // Set tooltip string

// Icons functionality
RAYGUIAPI const char *GuiIconText(int iconId, const char *text); // Get text with icon id prepended (if supported)
#if !defined(RAYGUI_NO_ICONS)
RAYGUIAPI void GuiSetIconScale(int scale);                      // Set default icon drawing size
RAYGUIAPI unsigned int *GuiGetIcons(void);                      // Get raygui icons data pointer
RAYGUIAPI char **GuiLoadIcons(const char *fileName, bool loadIconsName); // Load raygui icons file (.rgi) into internal icons data
RAYGUIAPI void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color); // Draw icon using pixel size at specified position
#endif

// Controls
//----------------------------------------------------------------------------------------------------------
// Container/separator controls, useful for controls organization
RAYGUIAPI int GuiWindowBox(Rectangle bounds, const char *title);                                       // Window Box control, shows a window that can be closed
// RAYGUIAPI int GuiGroupBox(Rectangle bounds, const char *text);                                         // Group Box control with text name
RAYGUIAPI int GuiLine(Rectangle bounds, const char *text);                                             // Line separator control, could contain text
RAYGUIAPI int GuiPanel(Rectangle bounds, const char *text);                                            // Panel control, useful to group controls
RAYGUIAPI int GuiTabBar(Rectangle bounds, const char **text, int count, int *active);                  // Tab Bar control, returns TAB to be closed or -1
RAYGUIAPI int GuiScrollPanel(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view); // Scroll Panel control

// Basic controls set
RAYGUIAPI int GuiLabel(Rectangle bounds, const char *text);                                            // Label control
RAYGUIAPI int GuiButton(Rectangle bounds, const char *text);                                           // Button control, returns true when clicked
RAYGUIAPI int GuiLabelButton(Rectangle bounds, const char *text);                                      // Label button control, returns true when clicked
RAYGUIAPI int GuiToggle(Rectangle bounds, const char *text, bool *active);                             // Toggle Button control
RAYGUIAPI int GuiToggleGroup(Rectangle bounds, const char *text, int *active);                         // Toggle Group control
RAYGUIAPI int GuiToggleSlider(Rectangle bounds, const char *text, int *active);                        // Toggle Slider control
RAYGUIAPI int GuiCheckBox(Rectangle bounds, const char *text, bool *checked);                          // Check Box control, returns true when active
RAYGUIAPI int GuiComboBox(Rectangle bounds, const char *text, int *active);                            // Combo Box control

RAYGUIAPI int GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode);          // Dropdown Box control
RAYGUIAPI int GuiSpinner(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode); // Spinner control
RAYGUIAPI int GuiValueBox(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode); // Value Box control, updates input text with numbers
RAYGUIAPI int GuiValueBoxFloat(Rectangle bounds, const char *text, char *textValue, float *value, bool editMode); // Value box control for float values
RAYGUIAPI int GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode);                   // Text Box control, updates input text

RAYGUIAPI int GuiSlider(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Slider control
RAYGUIAPI int GuiSliderBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Slider Bar control
RAYGUIAPI int GuiProgressBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Progress Bar control
RAYGUIAPI int GuiStatusBar(Rectangle bounds, const char *text);                                        // Status Bar control, shows info text
RAYGUIAPI int GuiDummyRec(Rectangle bounds, const char *text);                                         // Dummy control for placeholders
RAYGUIAPI int GuiGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 *mouseCell); // Grid control

// Advance controls set
RAYGUIAPI int GuiListView(Rectangle bounds, const char *text, int *scrollIndex, int *active);          // List View control
RAYGUIAPI int GuiListViewEx(Rectangle bounds, const char **text, int count, int *scrollIndex, int *active, int *focus); // List View with extended parameters
RAYGUIAPI int GuiMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons); // Message Box control, displays a message
RAYGUIAPI int GuiTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool *secretViewActive); // Text Input Box control, ask for text, supports secret
RAYGUIAPI int GuiColorPicker(Rectangle bounds, const char *text, Color *color);                        // Color Picker control (multiple color controls)
RAYGUIAPI int GuiColorPanel(Rectangle bounds, const char *text, Color *color);                         // Color Panel control
RAYGUIAPI int GuiColorBarAlpha(Rectangle bounds, const char *text, float *alpha);                      // Color Bar Alpha control
RAYGUIAPI int GuiColorBarHue(Rectangle bounds, const char *text, float *value);                        // Color Bar Hue control
RAYGUIAPI int GuiColorPickerHSV(Rectangle bounds, const char *text, Vector3 *colorHsv);                // Color Picker control that avoids conversion to RGB on each call (multiple color controls)
RAYGUIAPI int GuiColorPanelHSV(Rectangle bounds, const char *text, Vector3 *colorHsv);                 // Color Panel control that updates Hue-Saturation-Value color value, used by GuiColorPickerHSV()
RAYGUIAPI void GuiLoadStyleCherry(void);
RAYGUIAPI void GuiLoadStyleTerminal(void);
RAYGUIAPI void GuiLoadStyleSunny(void);
RAYGUIAPI void GuiLoadStyleCandy(void);
RAYGUIAPI void GuiLoadStyleAshes(void);
RAYGUIAPI void GuiLoadStyleEnefete(void);
//----------------------------------------------------------------------------------------------------------

#if !defined(RAYGUI_NO_ICONS)

#if !defined(RAYGUI_CUSTOM_ICONS)
//----------------------------------------------------------------------------------
// Icons enumeration
//----------------------------------------------------------------------------------
typedef enum {
    ICON_NONE                     = 0,
    ICON_FOLDER_FILE_OPEN         = 1,
    ICON_FILE_SAVE_CLASSIC        = 2,
    ICON_FOLDER_OPEN              = 3,
    ICON_FOLDER_SAVE              = 4,
    ICON_FILE_OPEN                = 5,
    ICON_FILE_SAVE                = 6,
    ICON_FILE_EXPORT              = 7,
    ICON_FILE_ADD                 = 8,
    ICON_FILE_DELETE              = 9,
    ICON_FILETYPE_TEXT            = 10,
    ICON_FILETYPE_AUDIO           = 11,
    ICON_FILETYPE_IMAGE           = 12,
    ICON_FILETYPE_PLAY            = 13,
    ICON_FILETYPE_VIDEO           = 14,
    ICON_FILETYPE_INFO            = 15,
    ICON_FILE_COPY                = 16,
    ICON_FILE_CUT                 = 17,
    ICON_FILE_PASTE               = 18,
    ICON_CURSOR_HAND              = 19,
    ICON_CURSOR_POINTER           = 20,
    ICON_CURSOR_CLASSIC           = 21,
    ICON_PENCIL                   = 22,
    ICON_PENCIL_BIG               = 23,
    ICON_BRUSH_CLASSIC            = 24,
    ICON_BRUSH_PAINTER            = 25,
    ICON_WATER_DROP               = 26,
    ICON_COLOR_PICKER             = 27,
    ICON_RUBBER                   = 28,
    ICON_COLOR_BUCKET             = 29,
    ICON_TEXT_T                   = 30,
    ICON_TEXT_A                   = 31,
    ICON_SCALE                    = 32,
    ICON_RESIZE                   = 33,
    ICON_FILTER_POINT             = 34,
    ICON_FILTER_BILINEAR          = 35,
    ICON_CROP                     = 36,
    ICON_CROP_ALPHA               = 37,
    ICON_SQUARE_TOGGLE            = 38,
    ICON_SYMMETRY                 = 39,
    ICON_SYMMETRY_HORIZONTAL      = 40,
    ICON_SYMMETRY_VERTICAL        = 41,
    ICON_LENS                     = 42,
    ICON_LENS_BIG                 = 43,
    ICON_EYE_ON                   = 44,
    ICON_EYE_OFF                  = 45,
    ICON_FILTER_TOP               = 46,
    ICON_FILTER                   = 47,
    ICON_TARGET_POINT             = 48,
    ICON_TARGET_SMALL             = 49,
    ICON_TARGET_BIG               = 50,
    ICON_TARGET_MOVE              = 51,
    ICON_CURSOR_MOVE              = 52,
    ICON_CURSOR_SCALE             = 53,
    ICON_CURSOR_SCALE_RIGHT       = 54,
    ICON_CURSOR_SCALE_LEFT        = 55,
    ICON_UNDO                     = 56,
    ICON_REDO                     = 57,
    ICON_REREDO                   = 58,
    ICON_MUTATE                   = 59,
    ICON_ROTATE                   = 60,
    ICON_REPEAT                   = 61,
    ICON_SHUFFLE                  = 62,
    ICON_EMPTYBOX                 = 63,
    ICON_TARGET                   = 64,
    ICON_TARGET_SMALL_FILL        = 65,
    ICON_TARGET_BIG_FILL          = 66,
    ICON_TARGET_MOVE_FILL         = 67,
    ICON_CURSOR_MOVE_FILL         = 68,
    ICON_CURSOR_SCALE_FILL        = 69,
    ICON_CURSOR_SCALE_RIGHT_FILL  = 70,
    ICON_CURSOR_SCALE_LEFT_FILL   = 71,
    ICON_UNDO_FILL                = 72,
    ICON_REDO_FILL                = 73,
    ICON_REREDO_FILL              = 74,
    ICON_MUTATE_FILL              = 75,
    ICON_ROTATE_FILL              = 76,
    ICON_REPEAT_FILL              = 77,
    ICON_SHUFFLE_FILL             = 78,
    ICON_EMPTYBOX_SMALL           = 79,
    ICON_BOX                      = 80,
    ICON_BOX_TOP                  = 81,
    ICON_BOX_TOP_RIGHT            = 82,
    ICON_BOX_RIGHT                = 83,
    ICON_BOX_BOTTOM_RIGHT         = 84,
    ICON_BOX_BOTTOM               = 85,
    ICON_BOX_BOTTOM_LEFT          = 86,
    ICON_BOX_LEFT                 = 87,
    ICON_BOX_TOP_LEFT             = 88,
    ICON_BOX_CENTER               = 89,
    ICON_BOX_CIRCLE_MASK          = 90,
    ICON_POT                      = 91,
    ICON_ALPHA_MULTIPLY           = 92,
    ICON_ALPHA_CLEAR              = 93,
    ICON_DITHERING                = 94,
    ICON_MIPMAPS                  = 95,
    ICON_BOX_GRID                 = 96,
    ICON_GRID                     = 97,
    ICON_BOX_CORNERS_SMALL        = 98,
    ICON_BOX_CORNERS_BIG          = 99,
    ICON_FOUR_BOXES               = 100,
    ICON_GRID_FILL                = 101,
    ICON_BOX_MULTISIZE            = 102,
    ICON_ZOOM_SMALL               = 103,
    ICON_ZOOM_MEDIUM              = 104,
    ICON_ZOOM_BIG                 = 105,
    ICON_ZOOM_ALL                 = 106,
    ICON_ZOOM_CENTER              = 107,
    ICON_BOX_DOTS_SMALL           = 108,
    ICON_BOX_DOTS_BIG             = 109,
    ICON_BOX_CONCENTRIC           = 110,
    ICON_BOX_GRID_BIG             = 111,
    ICON_OK_TICK                  = 112,
    ICON_CROSS                    = 113,
    ICON_ARROW_LEFT               = 114,
    ICON_ARROW_RIGHT              = 115,
    ICON_ARROW_DOWN               = 116,
    ICON_ARROW_UP                 = 117,
    ICON_ARROW_LEFT_FILL          = 118,
    ICON_ARROW_RIGHT_FILL         = 119,
    ICON_ARROW_DOWN_FILL          = 120,
    ICON_ARROW_UP_FILL            = 121,
    ICON_AUDIO                    = 122,
    ICON_FX                       = 123,
    ICON_WAVE                     = 124,
    ICON_WAVE_SINUS               = 125,
    ICON_WAVE_SQUARE              = 126,
    ICON_WAVE_TRIANGULAR          = 127,
    ICON_CROSS_SMALL              = 128,
    ICON_PLAYER_PREVIOUS          = 129,
    ICON_PLAYER_PLAY_BACK         = 130,
    ICON_PLAYER_PLAY              = 131,
    ICON_PLAYER_PAUSE             = 132,
    ICON_PLAYER_STOP              = 133,
    ICON_PLAYER_NEXT              = 134,
    ICON_PLAYER_RECORD            = 135,
    ICON_MAGNET                   = 136,
    ICON_LOCK_CLOSE               = 137,
    ICON_LOCK_OPEN                = 138,
    ICON_CLOCK                    = 139,
    ICON_TOOLS                    = 140,
    ICON_GEAR                     = 141,
    ICON_GEAR_BIG                 = 142,
    ICON_BIN                      = 143,
    ICON_HAND_POINTER             = 144,
    ICON_LASER                    = 145,
    ICON_COIN                     = 146,
    ICON_EXPLOSION                = 147,
    ICON_1UP                      = 148,
    ICON_PLAYER                   = 149,
    ICON_PLAYER_JUMP              = 150,
    ICON_KEY                      = 151,
    ICON_DEMON                    = 152,
    ICON_TEXT_POPUP               = 153,
    ICON_GEAR_EX                  = 154,
    ICON_CRACK                    = 155,
    ICON_CRACK_POINTS             = 156,
    ICON_STAR                     = 157,
    ICON_DOOR                     = 158,
    ICON_EXIT                     = 159,
    ICON_MODE_2D                  = 160,
    ICON_MODE_3D                  = 161,
    ICON_CUBE                     = 162,
    ICON_CUBE_FACE_TOP            = 163,
    ICON_CUBE_FACE_LEFT           = 164,
    ICON_CUBE_FACE_FRONT          = 165,
    ICON_CUBE_FACE_BOTTOM         = 166,
    ICON_CUBE_FACE_RIGHT          = 167,
    ICON_CUBE_FACE_BACK           = 168,
    ICON_CAMERA                   = 169,
    ICON_SPECIAL                  = 170,
    ICON_LINK_NET                 = 171,
    ICON_LINK_BOXES               = 172,
    ICON_LINK_MULTI               = 173,
    ICON_LINK                     = 174,
    ICON_LINK_BROKE               = 175,
    ICON_TEXT_NOTES               = 176,
    ICON_NOTEBOOK                 = 177,
    ICON_SUITCASE                 = 178,
    ICON_SUITCASE_ZIP             = 179,
    ICON_MAILBOX                  = 180,
    ICON_MONITOR                  = 181,
    ICON_PRINTER                  = 182,
    ICON_PHOTO_CAMERA             = 183,
    ICON_PHOTO_CAMERA_FLASH       = 184,
    ICON_HOUSE                    = 185,
    ICON_HEART                    = 186,
    ICON_CORNER                   = 187,
    ICON_VERTICAL_BARS            = 188,
    ICON_VERTICAL_BARS_FILL       = 189,
    ICON_LIFE_BARS                = 190,
    ICON_INFO                     = 191,
    ICON_CROSSLINE                = 192,
    ICON_HELP                     = 193,
    ICON_FILETYPE_ALPHA           = 194,
    ICON_FILETYPE_HOME            = 195,
    ICON_LAYERS_VISIBLE           = 196,
    ICON_LAYERS                   = 197,
    ICON_WINDOW                   = 198,
    ICON_HIDPI                    = 199,
    ICON_FILETYPE_BINARY          = 200,
    ICON_HEX                      = 201,
    ICON_SHIELD                   = 202,
    ICON_FILE_NEW                 = 203,
    ICON_FOLDER_ADD               = 204,
    ICON_ALARM                    = 205,
    ICON_CPU                      = 206,
    ICON_ROM                      = 207,
    ICON_STEP_OVER                = 208,
    ICON_STEP_INTO                = 209,
    ICON_STEP_OUT                 = 210,
    ICON_RESTART                  = 211,
    ICON_BREAKPOINT_ON            = 212,
    ICON_BREAKPOINT_OFF           = 213,
    ICON_BURGER_MENU              = 214,
    ICON_CASE_SENSITIVE           = 215,
    ICON_REG_EXP                  = 216,
    ICON_FOLDER                   = 217,
    ICON_FILE                     = 218,
    ICON_SAND_TIMER               = 219,
    ICON_WARNING                  = 220,
    ICON_HELP_BOX                 = 221,
    ICON_INFO_BOX                 = 222,
    ICON_PRIORITY                 = 223,
    ICON_LAYERS_ISO               = 224,
    ICON_LAYERS2                  = 225,
    ICON_MLAYERS                  = 226,
    ICON_MAPS                     = 227,
    ICON_228                      = 228,
    ICON_229                      = 229,
    ICON_230                      = 230,
    ICON_231                      = 231,
    ICON_232                      = 232,
    ICON_233                      = 233,
    ICON_234                      = 234,
    ICON_235                      = 235,
    ICON_236                      = 236,
    ICON_237                      = 237,
    ICON_238                      = 238,
    ICON_239                      = 239,
    ICON_240                      = 240,
    ICON_241                      = 241,
    ICON_242                      = 242,
    ICON_243                      = 243,
    ICON_244                      = 244,
    ICON_245                      = 245,
    ICON_246                      = 246,
    ICON_247                      = 247,
    ICON_248                      = 248,
    ICON_249                      = 249,
    ICON_250                      = 250,
    ICON_251                      = 251,
    ICON_252                      = 252,
    ICON_253                      = 253,
    ICON_254                      = 254,
    ICON_255                      = 255,
} GuiIconName;
#endif

#endif

#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif // RAYGUI_H

/***********************************************************************************
*
*   RAYGUI IMPLEMENTATION
*
************************************************************************************/

#if defined(RAYGUI_IMPLEMENTATION)

#include <stdio.h>              // Required for: FILE, fopen(), fclose(), fprintf(), feof(), fscanf(), vsprintf() [GuiLoadStyle(), GuiLoadIcons()]
#include <stdlib.h>             // Required for: malloc(), calloc(), free() [GuiLoadStyle(), GuiLoadIcons()]
#include <string.h>             // Required for: strlen() [GuiTextBox(), GuiValueBox()], memset(), memcpy()
#include <stdarg.h>             // Required for: va_list, va_start(), vfprintf(), va_end() [TextFormat()]
#include <math.h>               // Required for: roundf() [GuiColorPicker()]

#ifdef __cplusplus
    #define RAYGUI_CLITERAL(name) name
#else
    #define RAYGUI_CLITERAL(name) (name)
#endif

// Check if two rectangles are equal, used to validate a slider bounds as an id
#ifndef CHECK_BOUNDS_ID
    #define CHECK_BOUNDS_ID(src, dst) ((src.x == dst.x) && (src.y == dst.y) && (src.width == dst.width) && (src.height == dst.height))
#endif

#if !defined(RAYGUI_NO_ICONS) && !defined(RAYGUI_CUSTOM_ICONS)

// Embedded icons, no external file provided
#define RAYGUI_ICON_SIZE               16          // Size of icons in pixels (squared)
#define RAYGUI_ICON_MAX_ICONS         256          // Maximum number of icons
#define RAYGUI_ICON_MAX_NAME_LENGTH    32          // Maximum length of icon name id

// Icons data is defined by bit array (every bit represents one pixel)
// Those arrays are stored as unsigned int data arrays, so,
// every array element defines 32 pixels (bits) of information
// One icon is defined by 8 int, (8 int * 32 bit = 256 bit = 16*16 pixels)
// NOTE: Number of elemens depend on RAYGUI_ICON_SIZE (by default 16x16 pixels)
#define RAYGUI_ICON_DATA_ELEMENTS   (RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32)

//----------------------------------------------------------------------------------
// Icons data for all gui possible icons (allocated on data segment by default)
//
// NOTE 1: Every icon is codified in binary form, using 1 bit per pixel, so,
// every 16x16 icon requires 8 integers (16*16/32) to be stored
//
// NOTE 2: A different icon set could be loaded over this array using GuiLoadIcons(),
// but loaded icons set must be same RAYGUI_ICON_SIZE and no more than RAYGUI_ICON_MAX_ICONS
//
// guiIcons size is by default: 256*(16*16/32) = 2048*4 = 8192 bytes = 8 KB
//----------------------------------------------------------------------------------
static unsigned int guiIcons[RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_NONE
    0x3ff80000, 0x2f082008, 0x2042207e, 0x40027fc2, 0x40024002, 0x40024002, 0x40024002, 0x00007ffe,      // ICON_FOLDER_FILE_OPEN
    0x3ffe0000, 0x44226422, 0x400247e2, 0x5ffa4002, 0x57ea500a, 0x500a500a, 0x40025ffa, 0x00007ffe,      // ICON_FILE_SAVE_CLASSIC
    0x00000000, 0x0042007e, 0x40027fc2, 0x40024002, 0x41024002, 0x44424282, 0x793e4102, 0x00000100,      // ICON_FOLDER_OPEN
    0x00000000, 0x0042007e, 0x40027fc2, 0x40024002, 0x41024102, 0x44424102, 0x793e4282, 0x00000000,      // ICON_FOLDER_SAVE
    0x3ff00000, 0x201c2010, 0x20042004, 0x21042004, 0x24442284, 0x21042104, 0x20042104, 0x00003ffc,      // ICON_FILE_OPEN
    0x3ff00000, 0x201c2010, 0x20042004, 0x21042004, 0x21042104, 0x22842444, 0x20042104, 0x00003ffc,      // ICON_FILE_SAVE
    0x3ff00000, 0x201c2010, 0x00042004, 0x20041004, 0x20844784, 0x00841384, 0x20042784, 0x00003ffc,      // ICON_FILE_EXPORT
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042004, 0x22042204, 0x22042f84, 0x20042204, 0x00003ffc,      // ICON_FILE_ADD
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042004, 0x25042884, 0x25042204, 0x20042884, 0x00003ffc,      // ICON_FILE_DELETE
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042ff4, 0x20042ff4, 0x20042ff4, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_TEXT
    0x3ff00000, 0x201c2010, 0x27042004, 0x244424c4, 0x26442444, 0x20642664, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_AUDIO
    0x3ff00000, 0x201c2010, 0x26042604, 0x20042004, 0x35442884, 0x2414222c, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_IMAGE
    0x3ff00000, 0x201c2010, 0x20c42004, 0x22442144, 0x22442444, 0x20c42144, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_PLAY
    0x3ff00000, 0x3ffc2ff0, 0x3f3c2ff4, 0x3dbc2eb4, 0x3dbc2bb4, 0x3f3c2eb4, 0x3ffc2ff4, 0x00002ff4,      // ICON_FILETYPE_VIDEO
    0x3ff00000, 0x201c2010, 0x21842184, 0x21842004, 0x21842184, 0x21842184, 0x20042184, 0x00003ffc,      // ICON_FILETYPE_INFO
    0x0ff00000, 0x381c0810, 0x28042804, 0x28042804, 0x28042804, 0x28042804, 0x20102ffc, 0x00003ff0,      // ICON_FILE_COPY
    0x00000000, 0x701c0000, 0x079c1e14, 0x55a000f0, 0x079c00f0, 0x701c1e14, 0x00000000, 0x00000000,      // ICON_FILE_CUT
    0x01c00000, 0x13e41bec, 0x3f841004, 0x204420c4, 0x20442044, 0x20442044, 0x207c2044, 0x00003fc0,      // ICON_FILE_PASTE
    0x00000000, 0x3aa00fe0, 0x2abc2aa0, 0x2aa42aa4, 0x20042aa4, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_CURSOR_HAND
    0x00000000, 0x003c000c, 0x030800c8, 0x30100c10, 0x10202020, 0x04400840, 0x01800280, 0x00000000,      // ICON_CURSOR_POINTER
    0x00000000, 0x00180000, 0x01f00078, 0x03e007f0, 0x07c003e0, 0x04000e40, 0x00000000, 0x00000000,      // ICON_CURSOR_CLASSIC
    0x00000000, 0x04000000, 0x11000a00, 0x04400a80, 0x01100220, 0x00580088, 0x00000038, 0x00000000,      // ICON_PENCIL
    0x04000000, 0x15000a00, 0x50402880, 0x14102820, 0x05040a08, 0x015c028c, 0x007c00bc, 0x00000000,      // ICON_PENCIL_BIG
    0x01c00000, 0x01400140, 0x01400140, 0x0ff80140, 0x0ff80808, 0x0aa80808, 0x0aa80aa8, 0x00000ff8,      // ICON_BRUSH_CLASSIC
    0x1ffc0000, 0x5ffc7ffe, 0x40004000, 0x00807f80, 0x01c001c0, 0x01c001c0, 0x01c001c0, 0x00000080,      // ICON_BRUSH_PAINTER
    0x00000000, 0x00800000, 0x01c00080, 0x03e001c0, 0x07f003e0, 0x036006f0, 0x000001c0, 0x00000000,      // ICON_WATER_DROP
    0x00000000, 0x3e003800, 0x1f803f80, 0x0c201e40, 0x02080c10, 0x00840104, 0x00380044, 0x00000000,      // ICON_COLOR_PICKER
    0x00000000, 0x07800300, 0x1fe00fc0, 0x3f883fd0, 0x0e021f04, 0x02040402, 0x00f00108, 0x00000000,      // ICON_RUBBER
    0x00c00000, 0x02800140, 0x08200440, 0x20081010, 0x2ffe3004, 0x03f807fc, 0x00e001f0, 0x00000040,      // ICON_COLOR_BUCKET
    0x00000000, 0x21843ffc, 0x01800180, 0x01800180, 0x01800180, 0x01800180, 0x03c00180, 0x00000000,      // ICON_TEXT_T
    0x00800000, 0x01400180, 0x06200340, 0x0c100620, 0x1ff80c10, 0x380c1808, 0x70067004, 0x0000f80f,      // ICON_TEXT_A
    0x78000000, 0x50004000, 0x00004800, 0x03c003c0, 0x03c003c0, 0x00100000, 0x0002000a, 0x0000000e,      // ICON_SCALE
    0x75560000, 0x5e004002, 0x54001002, 0x41001202, 0x408200fe, 0x40820082, 0x40820082, 0x00006afe,      // ICON_RESIZE
    0x00000000, 0x3f003f00, 0x3f003f00, 0x3f003f00, 0x00400080, 0x001c0020, 0x001c001c, 0x00000000,      // ICON_FILTER_POINT
    0x6d800000, 0x00004080, 0x40804080, 0x40800000, 0x00406d80, 0x001c0020, 0x001c001c, 0x00000000,      // ICON_FILTER_BILINEAR
    0x40080000, 0x1ffe2008, 0x14081008, 0x11081208, 0x10481088, 0x10081028, 0x10047ff8, 0x00001002,      // ICON_CROP
    0x00100000, 0x3ffc0010, 0x2ab03550, 0x22b02550, 0x20b02150, 0x20302050, 0x2000fff0, 0x00002000,      // ICON_CROP_ALPHA
    0x40000000, 0x1ff82000, 0x04082808, 0x01082208, 0x00482088, 0x00182028, 0x35542008, 0x00000002,      // ICON_SQUARE_TOGGLE
    0x00000000, 0x02800280, 0x06c006c0, 0x0ea00ee0, 0x1e901eb0, 0x3e883e98, 0x7efc7e8c, 0x00000000,      // ICON_SYMMETRY
    0x01000000, 0x05600100, 0x1d480d50, 0x7d423d44, 0x3d447d42, 0x0d501d48, 0x01000560, 0x00000100,      // ICON_SYMMETRY_HORIZONTAL
    0x01800000, 0x04200240, 0x10080810, 0x00001ff8, 0x00007ffe, 0x0ff01ff8, 0x03c007e0, 0x00000180,      // ICON_SYMMETRY_VERTICAL
    0x00000000, 0x010800f0, 0x02040204, 0x02040204, 0x07f00308, 0x1c000e00, 0x30003800, 0x00000000,      // ICON_LENS
    0x00000000, 0x061803f0, 0x08240c0c, 0x08040814, 0x0c0c0804, 0x23f01618, 0x18002400, 0x00000000,      // ICON_LENS_BIG
    0x00000000, 0x00000000, 0x1c7007c0, 0x638e3398, 0x1c703398, 0x000007c0, 0x00000000, 0x00000000,      // ICON_EYE_ON
    0x00000000, 0x10002000, 0x04700fc0, 0x610e3218, 0x1c703098, 0x001007a0, 0x00000008, 0x00000000,      // ICON_EYE_OFF
    0x00000000, 0x00007ffc, 0x40047ffc, 0x10102008, 0x04400820, 0x02800280, 0x02800280, 0x00000100,      // ICON_FILTER_TOP
    0x00000000, 0x40027ffe, 0x10082004, 0x04200810, 0x02400240, 0x02400240, 0x01400240, 0x000000c0,      // ICON_FILTER
    0x00800000, 0x00800080, 0x00000080, 0x3c9e0000, 0x00000000, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_POINT
    0x00800000, 0x00800080, 0x00800080, 0x3f7e01c0, 0x008001c0, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_SMALL
    0x00800000, 0x00800080, 0x03e00080, 0x3e3e0220, 0x03e00220, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_BIG
    0x01000000, 0x04400280, 0x01000100, 0x43842008, 0x43849ab2, 0x01002008, 0x04400100, 0x01000280,      // ICON_TARGET_MOVE
    0x01000000, 0x04400280, 0x01000100, 0x41042108, 0x41049ff2, 0x01002108, 0x04400100, 0x01000280,      // ICON_CURSOR_MOVE
    0x781e0000, 0x500a4002, 0x04204812, 0x00000240, 0x02400000, 0x48120420, 0x4002500a, 0x0000781e,      // ICON_CURSOR_SCALE
    0x00000000, 0x20003c00, 0x24002800, 0x01000200, 0x00400080, 0x00140024, 0x003c0004, 0x00000000,      // ICON_CURSOR_SCALE_RIGHT
    0x00000000, 0x0004003c, 0x00240014, 0x00800040, 0x02000100, 0x28002400, 0x3c002000, 0x00000000,      // ICON_CURSOR_SCALE_LEFT
    0x00000000, 0x00100020, 0x10101fc8, 0x10001020, 0x10001000, 0x10001000, 0x00001fc0, 0x00000000,      // ICON_UNDO
    0x00000000, 0x08000400, 0x080813f8, 0x00080408, 0x00080008, 0x00080008, 0x000003f8, 0x00000000,      // ICON_REDO
    0x00000000, 0x3ffc0000, 0x20042004, 0x20002000, 0x20402000, 0x3f902020, 0x00400020, 0x00000000,      // ICON_REREDO
    0x00000000, 0x3ffc0000, 0x20042004, 0x27fc2004, 0x20202000, 0x3fc82010, 0x00200010, 0x00000000,      // ICON_MUTATE
    0x00000000, 0x0ff00000, 0x10081818, 0x11801008, 0x10001180, 0x18101020, 0x00100fc8, 0x00000020,      // ICON_ROTATE
    0x00000000, 0x04000200, 0x240429fc, 0x20042204, 0x20442004, 0x3f942024, 0x00400020, 0x00000000,      // ICON_REPEAT
    0x00000000, 0x20001000, 0x22104c0e, 0x00801120, 0x11200040, 0x4c0e2210, 0x10002000, 0x00000000,      // ICON_SHUFFLE
    0x7ffe0000, 0x50024002, 0x44024802, 0x41024202, 0x40424082, 0x40124022, 0x4002400a, 0x00007ffe,      // ICON_EMPTYBOX
    0x00800000, 0x03e00080, 0x08080490, 0x3c9e0808, 0x08080808, 0x03e00490, 0x00800080, 0x00000000,      // ICON_TARGET
    0x00800000, 0x00800080, 0x00800080, 0x3ffe01c0, 0x008001c0, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_SMALL_FILL
    0x00800000, 0x00800080, 0x03e00080, 0x3ffe03e0, 0x03e003e0, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_BIG_FILL
    0x01000000, 0x07c00380, 0x01000100, 0x638c2008, 0x638cfbbe, 0x01002008, 0x07c00100, 0x01000380,      // ICON_TARGET_MOVE_FILL
    0x01000000, 0x07c00380, 0x01000100, 0x610c2108, 0x610cfffe, 0x01002108, 0x07c00100, 0x01000380,      // ICON_CURSOR_MOVE_FILL
    0x781e0000, 0x6006700e, 0x04204812, 0x00000240, 0x02400000, 0x48120420, 0x700e6006, 0x0000781e,      // ICON_CURSOR_SCALE_FILL
    0x00000000, 0x38003c00, 0x24003000, 0x01000200, 0x00400080, 0x000c0024, 0x003c001c, 0x00000000,      // ICON_CURSOR_SCALE_RIGHT_FILL
    0x00000000, 0x001c003c, 0x0024000c, 0x00800040, 0x02000100, 0x30002400, 0x3c003800, 0x00000000,      // ICON_CURSOR_SCALE_LEFT_FILL
    0x00000000, 0x00300020, 0x10301ff8, 0x10001020, 0x10001000, 0x10001000, 0x00001fc0, 0x00000000,      // ICON_UNDO_FILL
    0x00000000, 0x0c000400, 0x0c081ff8, 0x00080408, 0x00080008, 0x00080008, 0x000003f8, 0x00000000,      // ICON_REDO_FILL
    0x00000000, 0x3ffc0000, 0x20042004, 0x20002000, 0x20402000, 0x3ff02060, 0x00400060, 0x00000000,      // ICON_REREDO_FILL
    0x00000000, 0x3ffc0000, 0x20042004, 0x27fc2004, 0x20202000, 0x3ff82030, 0x00200030, 0x00000000,      // ICON_MUTATE_FILL
    0x00000000, 0x0ff00000, 0x10081818, 0x11801008, 0x10001180, 0x18301020, 0x00300ff8, 0x00000020,      // ICON_ROTATE_FILL
    0x00000000, 0x06000200, 0x26042ffc, 0x20042204, 0x20442004, 0x3ff42064, 0x00400060, 0x00000000,      // ICON_REPEAT_FILL
    0x00000000, 0x30001000, 0x32107c0e, 0x00801120, 0x11200040, 0x7c0e3210, 0x10003000, 0x00000000,      // ICON_SHUFFLE_FILL
    0x00000000, 0x30043ffc, 0x24042804, 0x21042204, 0x20442084, 0x20142024, 0x3ffc200c, 0x00000000,      // ICON_EMPTYBOX_SMALL
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX
    0x00000000, 0x23c43ffc, 0x23c423c4, 0x200423c4, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_TOP
    0x00000000, 0x3e043ffc, 0x3e043e04, 0x20043e04, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_TOP_RIGHT
    0x00000000, 0x20043ffc, 0x20042004, 0x3e043e04, 0x3e043e04, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_RIGHT
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x3e042004, 0x3e043e04, 0x3ffc3e04, 0x00000000,      // ICON_BOX_BOTTOM_RIGHT
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x23c42004, 0x23c423c4, 0x3ffc23c4, 0x00000000,      // ICON_BOX_BOTTOM
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x207c2004, 0x207c207c, 0x3ffc207c, 0x00000000,      // ICON_BOX_BOTTOM_LEFT
    0x00000000, 0x20043ffc, 0x20042004, 0x207c207c, 0x207c207c, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_LEFT
    0x00000000, 0x207c3ffc, 0x207c207c, 0x2004207c, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_TOP_LEFT
    0x00000000, 0x20043ffc, 0x20042004, 0x23c423c4, 0x23c423c4, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_CENTER
    0x7ffe0000, 0x40024002, 0x47e24182, 0x4ff247e2, 0x47e24ff2, 0x418247e2, 0x40024002, 0x00007ffe,      // ICON_BOX_CIRCLE_MASK
    0x7fff0000, 0x40014001, 0x40014001, 0x49555ddd, 0x4945495d, 0x400149c5, 0x40014001, 0x00007fff,      // ICON_POT
    0x7ffe0000, 0x53327332, 0x44ce4cce, 0x41324332, 0x404e40ce, 0x48125432, 0x4006540e, 0x00007ffe,      // ICON_ALPHA_MULTIPLY
    0x7ffe0000, 0x53327332, 0x44ce4cce, 0x41324332, 0x5c4e40ce, 0x44124432, 0x40065c0e, 0x00007ffe,      // ICON_ALPHA_CLEAR
    0x7ffe0000, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x00007ffe,      // ICON_DITHERING
    0x07fe0000, 0x1ffa0002, 0x7fea000a, 0x402a402a, 0x5b2a512a, 0x5128552a, 0x40205128, 0x00007fe0,      // ICON_MIPMAPS
    0x00000000, 0x1ff80000, 0x12481248, 0x12481ff8, 0x1ff81248, 0x12481248, 0x00001ff8, 0x00000000,      // ICON_BOX_GRID
    0x12480000, 0x7ffe1248, 0x12481248, 0x12487ffe, 0x7ffe1248, 0x12481248, 0x12487ffe, 0x00001248,      // ICON_GRID
    0x00000000, 0x1c380000, 0x1c3817e8, 0x08100810, 0x08100810, 0x17e81c38, 0x00001c38, 0x00000000,      // ICON_BOX_CORNERS_SMALL
    0x700e0000, 0x700e5ffa, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x5ffa700e, 0x0000700e,      // ICON_BOX_CORNERS_BIG
    0x3f7e0000, 0x21422142, 0x21422142, 0x00003f7e, 0x21423f7e, 0x21422142, 0x3f7e2142, 0x00000000,      // ICON_FOUR_BOXES
    0x00000000, 0x3bb80000, 0x3bb83bb8, 0x3bb80000, 0x3bb83bb8, 0x3bb80000, 0x3bb83bb8, 0x00000000,      // ICON_GRID_FILL
    0x7ffe0000, 0x7ffe7ffe, 0x77fe7000, 0x77fe77fe, 0x777e7700, 0x777e777e, 0x777e777e, 0x0000777e,      // ICON_BOX_MULTISIZE
    0x781e0000, 0x40024002, 0x00004002, 0x01800000, 0x00000180, 0x40020000, 0x40024002, 0x0000781e,      // ICON_ZOOM_SMALL
    0x781e0000, 0x40024002, 0x00004002, 0x03c003c0, 0x03c003c0, 0x40020000, 0x40024002, 0x0000781e,      // ICON_ZOOM_MEDIUM
    0x781e0000, 0x40024002, 0x07e04002, 0x07e007e0, 0x07e007e0, 0x400207e0, 0x40024002, 0x0000781e,      // ICON_ZOOM_BIG
    0x781e0000, 0x5ffa4002, 0x1ff85ffa, 0x1ff81ff8, 0x1ff81ff8, 0x5ffa1ff8, 0x40025ffa, 0x0000781e,      // ICON_ZOOM_ALL
    0x00000000, 0x2004381c, 0x00002004, 0x00000000, 0x00000000, 0x20040000, 0x381c2004, 0x00000000,      // ICON_ZOOM_CENTER
    0x00000000, 0x1db80000, 0x10081008, 0x10080000, 0x00001008, 0x10081008, 0x00001db8, 0x00000000,      // ICON_BOX_DOTS_SMALL
    0x35560000, 0x00002002, 0x00002002, 0x00002002, 0x00002002, 0x00002002, 0x35562002, 0x00000000,      // ICON_BOX_DOTS_BIG
    0x7ffe0000, 0x40024002, 0x48124ff2, 0x49924812, 0x48124992, 0x4ff24812, 0x40024002, 0x00007ffe,      // ICON_BOX_CONCENTRIC
    0x00000000, 0x10841ffc, 0x10841084, 0x1ffc1084, 0x10841084, 0x10841084, 0x00001ffc, 0x00000000,      // ICON_BOX_GRID_BIG
    0x00000000, 0x00000000, 0x10000000, 0x04000800, 0x01040200, 0x00500088, 0x00000020, 0x00000000,      // ICON_OK_TICK
    0x00000000, 0x10080000, 0x04200810, 0x01800240, 0x02400180, 0x08100420, 0x00001008, 0x00000000,      // ICON_CROSS
    0x00000000, 0x02000000, 0x00800100, 0x00200040, 0x00200010, 0x00800040, 0x02000100, 0x00000000,      // ICON_ARROW_LEFT
    0x00000000, 0x00400000, 0x01000080, 0x04000200, 0x04000800, 0x01000200, 0x00400080, 0x00000000,      // ICON_ARROW_RIGHT
    0x00000000, 0x00000000, 0x00000000, 0x08081004, 0x02200410, 0x00800140, 0x00000000, 0x00000000,      // ICON_ARROW_DOWN
    0x00000000, 0x00000000, 0x01400080, 0x04100220, 0x10040808, 0x00000000, 0x00000000, 0x00000000,      // ICON_ARROW_UP
    0x00000000, 0x02000000, 0x03800300, 0x03e003c0, 0x03e003f0, 0x038003c0, 0x02000300, 0x00000000,      // ICON_ARROW_LEFT_FILL
    0x00000000, 0x00400000, 0x01c000c0, 0x07c003c0, 0x07c00fc0, 0x01c003c0, 0x004000c0, 0x00000000,      // ICON_ARROW_RIGHT_FILL
    0x00000000, 0x00000000, 0x00000000, 0x0ff81ffc, 0x03e007f0, 0x008001c0, 0x00000000, 0x00000000,      // ICON_ARROW_DOWN_FILL
    0x00000000, 0x00000000, 0x01c00080, 0x07f003e0, 0x1ffc0ff8, 0x00000000, 0x00000000, 0x00000000,      // ICON_ARROW_UP_FILL
    0x00000000, 0x18a008c0, 0x32881290, 0x24822686, 0x26862482, 0x12903288, 0x08c018a0, 0x00000000,      // ICON_AUDIO
    0x00000000, 0x04800780, 0x004000c0, 0x662000f0, 0x08103c30, 0x130a0e18, 0x0000318e, 0x00000000,      // ICON_FX
    0x00000000, 0x00800000, 0x08880888, 0x2aaa0a8a, 0x0a8a2aaa, 0x08880888, 0x00000080, 0x00000000,      // ICON_WAVE
    0x00000000, 0x00600000, 0x01080090, 0x02040108, 0x42044204, 0x24022402, 0x00001800, 0x00000000,      // ICON_WAVE_SINUS
    0x00000000, 0x07f80000, 0x04080408, 0x04080408, 0x04080408, 0x7c0e0408, 0x00000000, 0x00000000,      // ICON_WAVE_SQUARE
    0x00000000, 0x00000000, 0x00a00040, 0x22084110, 0x08021404, 0x00000000, 0x00000000, 0x00000000,      // ICON_WAVE_TRIANGULAR
    0x00000000, 0x00000000, 0x04200000, 0x01800240, 0x02400180, 0x00000420, 0x00000000, 0x00000000,      // ICON_CROSS_SMALL
    0x00000000, 0x18380000, 0x12281428, 0x10a81128, 0x112810a8, 0x14281228, 0x00001838, 0x00000000,      // ICON_PLAYER_PREVIOUS
    0x00000000, 0x18000000, 0x11801600, 0x10181060, 0x10601018, 0x16001180, 0x00001800, 0x00000000,      // ICON_PLAYER_PLAY_BACK
    0x00000000, 0x00180000, 0x01880068, 0x18080608, 0x06081808, 0x00680188, 0x00000018, 0x00000000,      // ICON_PLAYER_PLAY
    0x00000000, 0x1e780000, 0x12481248, 0x12481248, 0x12481248, 0x12481248, 0x00001e78, 0x00000000,      // ICON_PLAYER_PAUSE
    0x00000000, 0x1ff80000, 0x10081008, 0x10081008, 0x10081008, 0x10081008, 0x00001ff8, 0x00000000,      // ICON_PLAYER_STOP
    0x00000000, 0x1c180000, 0x14481428, 0x15081488, 0x14881508, 0x14281448, 0x00001c18, 0x00000000,      // ICON_PLAYER_NEXT
    0x00000000, 0x03c00000, 0x08100420, 0x10081008, 0x10081008, 0x04200810, 0x000003c0, 0x00000000,      // ICON_PLAYER_RECORD
    0x00000000, 0x0c3007e0, 0x13c81818, 0x14281668, 0x14281428, 0x1c381c38, 0x08102244, 0x00000000,      // ICON_MAGNET
    0x07c00000, 0x08200820, 0x3ff80820, 0x23882008, 0x21082388, 0x20082108, 0x1ff02008, 0x00000000,      // ICON_LOCK_CLOSE
    0x07c00000, 0x08000800, 0x3ff80800, 0x23882008, 0x21082388, 0x20082108, 0x1ff02008, 0x00000000,      // ICON_LOCK_OPEN
    0x01c00000, 0x0c180770, 0x3086188c, 0x60832082, 0x60034781, 0x30062002, 0x0c18180c, 0x01c00770,      // ICON_CLOCK
    0x0a200000, 0x1b201b20, 0x04200e20, 0x04200420, 0x04700420, 0x0e700e70, 0x0e700e70, 0x04200e70,      // ICON_TOOLS
    0x01800000, 0x3bdc318c, 0x0ff01ff8, 0x7c3e1e78, 0x1e787c3e, 0x1ff80ff0, 0x318c3bdc, 0x00000180,      // ICON_GEAR
    0x01800000, 0x3ffc318c, 0x1c381ff8, 0x781e1818, 0x1818781e, 0x1ff81c38, 0x318c3ffc, 0x00000180,      // ICON_GEAR_BIG
    0x00000000, 0x08080ff8, 0x08081ffc, 0x0aa80aa8, 0x0aa80aa8, 0x0aa80aa8, 0x08080aa8, 0x00000ff8,      // ICON_BIN
    0x00000000, 0x00000000, 0x20043ffc, 0x08043f84, 0x04040f84, 0x04040784, 0x000007fc, 0x00000000,      // ICON_HAND_POINTER
    0x00000000, 0x24400400, 0x00001480, 0x6efe0e00, 0x00000e00, 0x24401480, 0x00000400, 0x00000000,      // ICON_LASER
    0x00000000, 0x03c00000, 0x08300460, 0x11181118, 0x11181118, 0x04600830, 0x000003c0, 0x00000000,      // ICON_COIN
    0x00000000, 0x10880080, 0x06c00810, 0x366c07e0, 0x07e00240, 0x00001768, 0x04200240, 0x00000000,      // ICON_EXPLOSION
    0x00000000, 0x3d280000, 0x2528252c, 0x3d282528, 0x05280528, 0x05e80528, 0x00000000, 0x00000000,      // ICON_1UP
    0x01800000, 0x03c003c0, 0x018003c0, 0x0ff007e0, 0x0bd00bd0, 0x0a500bd0, 0x02400240, 0x02400240,      // ICON_PLAYER
    0x01800000, 0x03c003c0, 0x118013c0, 0x03c81ff8, 0x07c003c8, 0x04400440, 0x0c080478, 0x00000000,      // ICON_PLAYER_JUMP
    0x3ff80000, 0x30183ff8, 0x30183018, 0x3ff83ff8, 0x03000300, 0x03c003c0, 0x03e00300, 0x000003e0,      // ICON_KEY
    0x3ff80000, 0x3ff83ff8, 0x33983ff8, 0x3ff83398, 0x3ff83ff8, 0x00000540, 0x0fe00aa0, 0x00000fe0,      // ICON_DEMON
    0x00000000, 0x0ff00000, 0x20041008, 0x25442004, 0x10082004, 0x06000bf0, 0x00000300, 0x00000000,      // ICON_TEXT_POPUP
    0x00000000, 0x11440000, 0x07f00be8, 0x1c1c0e38, 0x1c1c0c18, 0x07f00e38, 0x11440be8, 0x00000000,      // ICON_GEAR_EX
    0x00000000, 0x20080000, 0x0c601010, 0x07c00fe0, 0x07c007c0, 0x0c600fe0, 0x20081010, 0x00000000,      // ICON_CRACK
    0x00000000, 0x20080000, 0x0c601010, 0x04400fe0, 0x04405554, 0x0c600fe0, 0x20081010, 0x00000000,      // ICON_CRACK_POINTS
    0x00000000, 0x00800080, 0x01c001c0, 0x1ffc3ffe, 0x03e007f0, 0x07f003e0, 0x0c180770, 0x00000808,      // ICON_STAR
    0x0ff00000, 0x08180810, 0x08100818, 0x0a100810, 0x08180810, 0x08100818, 0x08100810, 0x00001ff8,      // ICON_DOOR
    0x0ff00000, 0x08100810, 0x08100810, 0x10100010, 0x4f902010, 0x10102010, 0x08100010, 0x00000ff0,      // ICON_EXIT
    0x00040000, 0x001f000e, 0x0ef40004, 0x12f41284, 0x0ef41214, 0x10040004, 0x7ffc3004, 0x10003000,      // ICON_MODE_2D
    0x78040000, 0x501f600e, 0x0ef44004, 0x12f41284, 0x0ef41284, 0x10140004, 0x7ffc300c, 0x10003000,      // ICON_MODE_3D
    0x7fe00000, 0x50286030, 0x47fe4804, 0x44224402, 0x44224422, 0x241275e2, 0x0c06140a, 0x000007fe,      // ICON_CUBE
    0x7fe00000, 0x5ff87ff0, 0x47fe4ffc, 0x44224402, 0x44224422, 0x241275e2, 0x0c06140a, 0x000007fe,      // ICON_CUBE_FACE_TOP
    0x7fe00000, 0x50386030, 0x47c2483c, 0x443e443e, 0x443e443e, 0x241e75fe, 0x0c06140e, 0x000007fe,      // ICON_CUBE_FACE_LEFT
    0x7fe00000, 0x50286030, 0x47fe4804, 0x47fe47fe, 0x47fe47fe, 0x27fe77fe, 0x0ffe17fe, 0x000007fe,      // ICON_CUBE_FACE_FRONT
    0x7fe00000, 0x50286030, 0x47fe4804, 0x44224402, 0x44224422, 0x3bf27be2, 0x0bfe1bfa, 0x000007fe,      // ICON_CUBE_FACE_BOTTOM
    0x7fe00000, 0x70286030, 0x7ffe7804, 0x7c227c02, 0x7c227c22, 0x3c127de2, 0x0c061c0a, 0x000007fe,      // ICON_CUBE_FACE_RIGHT
    0x7fe00000, 0x6fe85ff0, 0x781e77e4, 0x7be27be2, 0x7be27be2, 0x24127be2, 0x0c06140a, 0x000007fe,      // ICON_CUBE_FACE_BACK
    0x00000000, 0x2a0233fe, 0x22022602, 0x22022202, 0x2a022602, 0x00a033fe, 0x02080110, 0x00000000,      // ICON_CAMERA
    0x00000000, 0x200c3ffc, 0x000c000c, 0x3ffc000c, 0x30003000, 0x30003000, 0x3ffc3004, 0x00000000,      // ICON_SPECIAL
    0x00000000, 0x0022003e, 0x012201e2, 0x0100013e, 0x01000100, 0x79000100, 0x4f004900, 0x00007800,      // ICON_LINK_NET
    0x00000000, 0x44007c00, 0x45004600, 0x00627cbe, 0x00620022, 0x45007cbe, 0x44004600, 0x00007c00,      // ICON_LINK_BOXES
    0x00000000, 0x0044007c, 0x0010007c, 0x3f100010, 0x3f1021f0, 0x3f100010, 0x3f0021f0, 0x00000000,      // ICON_LINK_MULTI
    0x00000000, 0x0044007c, 0x00440044, 0x0010007c, 0x00100010, 0x44107c10, 0x440047f0, 0x00007c00,      // ICON_LINK
    0x00000000, 0x0044007c, 0x00440044, 0x0000007c, 0x00000010, 0x44007c10, 0x44004550, 0x00007c00,      // ICON_LINK_BROKE
    0x02a00000, 0x22a43ffc, 0x20042004, 0x20042ff4, 0x20042ff4, 0x20042ff4, 0x20042004, 0x00003ffc,      // ICON_TEXT_NOTES
    0x3ffc0000, 0x20042004, 0x245e27c4, 0x27c42444, 0x2004201e, 0x201e2004, 0x20042004, 0x00003ffc,      // ICON_NOTEBOOK
    0x00000000, 0x07e00000, 0x04200420, 0x24243ffc, 0x24242424, 0x24242424, 0x3ffc2424, 0x00000000,      // ICON_SUITCASE
    0x00000000, 0x0fe00000, 0x08200820, 0x40047ffc, 0x7ffc5554, 0x40045554, 0x7ffc4004, 0x00000000,      // ICON_SUITCASE_ZIP
    0x00000000, 0x20043ffc, 0x3ffc2004, 0x13c81008, 0x100813c8, 0x10081008, 0x1ff81008, 0x00000000,      // ICON_MAILBOX
    0x00000000, 0x40027ffe, 0x5ffa5ffa, 0x5ffa5ffa, 0x40025ffa, 0x03c07ffe, 0x1ff81ff8, 0x00000000,      // ICON_MONITOR
    0x0ff00000, 0x6bfe7ffe, 0x7ffe7ffe, 0x68167ffe, 0x08106816, 0x08100810, 0x0ff00810, 0x00000000,      // ICON_PRINTER
    0x3ff80000, 0xfffe2008, 0x870a8002, 0x904a888a, 0x904a904a, 0x870a888a, 0xfffe8002, 0x00000000,      // ICON_PHOTO_CAMERA
    0x0fc00000, 0xfcfe0cd8, 0x8002fffe, 0x84428382, 0x84428442, 0x80028382, 0xfffe8002, 0x00000000,      // ICON_PHOTO_CAMERA_FLASH
    0x00000000, 0x02400180, 0x08100420, 0x20041008, 0x23c42004, 0x22442244, 0x3ffc2244, 0x00000000,      // ICON_HOUSE
    0x00000000, 0x1c700000, 0x3ff83ef8, 0x3ff83ff8, 0x0fe01ff0, 0x038007c0, 0x00000100, 0x00000000,      // ICON_HEART
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0xe000c000,      // ICON_CORNER
    0x00000000, 0x14001c00, 0x15c01400, 0x15401540, 0x155c1540, 0x15541554, 0x1ddc1554, 0x00000000,      // ICON_VERTICAL_BARS
    0x00000000, 0x03000300, 0x1b001b00, 0x1b601b60, 0x1b6c1b60, 0x1b6c1b6c, 0x1b6c1b6c, 0x00000000,      // ICON_VERTICAL_BARS_FILL
    0x00000000, 0x00000000, 0x403e7ffe, 0x7ffe403e, 0x7ffe0000, 0x43fe43fe, 0x00007ffe, 0x00000000,      // ICON_LIFE_BARS
    0x7ffc0000, 0x43844004, 0x43844284, 0x43844004, 0x42844284, 0x42844284, 0x40044384, 0x00007ffc,      // ICON_INFO
    0x40008000, 0x10002000, 0x04000800, 0x01000200, 0x00400080, 0x00100020, 0x00040008, 0x00010002,      // ICON_CROSSLINE
    0x00000000, 0x1ff01ff0, 0x18301830, 0x1f001830, 0x03001f00, 0x00000300, 0x03000300, 0x00000000,      // ICON_HELP
    0x3ff00000, 0x2abc3550, 0x2aac3554, 0x2aac3554, 0x2aac3554, 0x2aac3554, 0x2aac3554, 0x00003ffc,      // ICON_FILETYPE_ALPHA
    0x3ff00000, 0x201c2010, 0x22442184, 0x28142424, 0x29942814, 0x2ff42994, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_HOME
    0x07fe0000, 0x04020402, 0x7fe20402, 0x44224422, 0x44224422, 0x402047fe, 0x40204020, 0x00007fe0,      // ICON_LAYERS_VISIBLE
    0x07fe0000, 0x04020402, 0x7c020402, 0x44024402, 0x44024402, 0x402047fe, 0x40204020, 0x00007fe0,      // ICON_LAYERS
    0x00000000, 0x40027ffe, 0x7ffe4002, 0x40024002, 0x40024002, 0x40024002, 0x7ffe4002, 0x00000000,      // ICON_WINDOW
    0x09100000, 0x09f00910, 0x09100910, 0x00000910, 0x24a2779e, 0x27a224a2, 0x709e20a2, 0x00000000,      // ICON_HIDPI
    0x3ff00000, 0x201c2010, 0x2a842e84, 0x2e842a84, 0x2ba42004, 0x2aa42aa4, 0x20042ba4, 0x00003ffc,      // ICON_FILETYPE_BINARY
    0x00000000, 0x00000000, 0x00120012, 0x4a5e4bd2, 0x485233d2, 0x00004bd2, 0x00000000, 0x00000000,      // ICON_HEX
    0x01800000, 0x381c0660, 0x23c42004, 0x23c42044, 0x13c82204, 0x08101008, 0x02400420, 0x00000180,      // ICON_SHIELD
    0x007e0000, 0x20023fc2, 0x40227fe2, 0x400a403a, 0x400a400a, 0x400a400a, 0x4008400e, 0x00007ff8,      // ICON_FILE_NEW
    0x00000000, 0x0042007e, 0x40027fc2, 0x44024002, 0x5f024402, 0x44024402, 0x7ffe4002, 0x00000000,      // ICON_FOLDER_ADD
    0x44220000, 0x12482244, 0xf3cf0000, 0x14280420, 0x48122424, 0x08100810, 0x1ff81008, 0x03c00420,      // ICON_ALARM
    0x0aa00000, 0x1ff80aa0, 0x1068700e, 0x1008706e, 0x1008700e, 0x1008700e, 0x0aa01ff8, 0x00000aa0,      // ICON_CPU
    0x07e00000, 0x04201db8, 0x04a01c38, 0x04a01d38, 0x04a01d38, 0x04a01d38, 0x04201d38, 0x000007e0,      // ICON_ROM
    0x00000000, 0x03c00000, 0x3c382ff0, 0x3c04380c, 0x01800000, 0x03c003c0, 0x00000180, 0x00000000,      // ICON_STEP_OVER
    0x01800000, 0x01800180, 0x01800180, 0x03c007e0, 0x00000180, 0x01800000, 0x03c003c0, 0x00000180,      // ICON_STEP_INTO
    0x01800000, 0x07e003c0, 0x01800180, 0x01800180, 0x00000180, 0x01800000, 0x03c003c0, 0x00000180,      // ICON_STEP_OUT
    0x00000000, 0x0ff003c0, 0x181c1c34, 0x303c301c, 0x30003000, 0x1c301800, 0x03c00ff0, 0x00000000,      // ICON_RESTART
    0x00000000, 0x00000000, 0x07e003c0, 0x0ff00ff0, 0x0ff00ff0, 0x03c007e0, 0x00000000, 0x00000000,      // ICON_BREAKPOINT_ON
    0x00000000, 0x00000000, 0x042003c0, 0x08100810, 0x08100810, 0x03c00420, 0x00000000, 0x00000000,      // ICON_BREAKPOINT_OFF
    0x00000000, 0x00000000, 0x1ff81ff8, 0x1ff80000, 0x00001ff8, 0x1ff81ff8, 0x00000000, 0x00000000,      // ICON_BURGER_MENU
    0x00000000, 0x00000000, 0x00880070, 0x0c880088, 0x1e8810f8, 0x3e881288, 0x00000000, 0x00000000,      // ICON_CASE_SENSITIVE
    0x00000000, 0x02000000, 0x07000a80, 0x07001fc0, 0x02000a80, 0x00300030, 0x00000000, 0x00000000,      // ICON_REG_EXP
    0x00000000, 0x0042007e, 0x40027fc2, 0x40024002, 0x40024002, 0x40024002, 0x7ffe4002, 0x00000000,      // ICON_FOLDER
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x00003ffc,      // ICON_FILE
    0x1ff00000, 0x20082008, 0x17d02fe8, 0x05400ba0, 0x09200540, 0x23881010, 0x2fe827c8, 0x00001ff0,      // ICON_SAND_TIMER
    0x01800000, 0x02400240, 0x05a00420, 0x09900990, 0x11881188, 0x21842004, 0x40024182, 0x00003ffc,      // ICON_WARNING
    0x7ffe0000, 0x4ff24002, 0x4c324ff2, 0x4f824c02, 0x41824f82, 0x41824002, 0x40024182, 0x00007ffe,      // ICON_HELP_BOX
    0x7ffe0000, 0x41824002, 0x40024182, 0x41824182, 0x41824182, 0x41824182, 0x40024182, 0x00007ffe,      // ICON_INFO_BOX
    0x01800000, 0x04200240, 0x10080810, 0x7bde2004, 0x0a500a50, 0x08500bd0, 0x08100850, 0x00000ff0,      // ICON_PRIORITY
    0x01800000, 0x18180660, 0x80016006, 0x98196006, 0x99996666, 0x19986666, 0x01800660, 0x00000000,      // ICON_LAYERS_ISO
    0x07fe0000, 0x1c020402, 0x74021402, 0x54025402, 0x54025402, 0x500857fe, 0x40205ff8, 0x00007fe0,      // ICON_LAYERS2
    0x0ffe0000, 0x3ffa0802, 0x7fea200a, 0x402a402a, 0x422a422a, 0x422e422a, 0x40384e28, 0x00007fe0,      // ICON_MLAYERS
    0x0ffe0000, 0x3ffa0802, 0x7fea200a, 0x402a402a, 0x5b2a512a, 0x512e552a, 0x40385128, 0x00007fe0,      // ICON_MAPS
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_228
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_229
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_230
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_231
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_232
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_233
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_234
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_235
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_236
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_237
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_238
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_239
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_240
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_241
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_242
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_243
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_244
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_245
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_246
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_247
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_248
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_249
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_250
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_251
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_252
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_253
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_254
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_255
};

// NOTE: A pointer to current icons array should be defined
static unsigned int *guiIconsPtr = guiIcons;

#endif      // !RAYGUI_NO_ICONS && !RAYGUI_CUSTOM_ICONS

#ifndef RAYGUI_ICON_SIZE
    #define RAYGUI_ICON_SIZE             0
#endif

// WARNING: Those values define the total size of the style data array,
// if changed, previous saved styles could become incompatible
#define RAYGUI_MAX_CONTROLS             16      // Maximum number of controls
#define RAYGUI_MAX_PROPS_BASE           16      // Maximum number of base properties
#define RAYGUI_MAX_PROPS_EXTENDED        8      // Maximum number of extended properties

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Gui control property style color element
typedef enum { BORDER = 0, BASE, TEXT, OTHER } GuiPropertyElement;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static GuiState guiState = STATE_NORMAL;        // Gui global state, if !STATE_NORMAL, forces defined state

static Font guiFont = { 0 };                    // Gui current font (WARNING: highly coupled to raylib)
static bool guiLocked = false;                  // Gui lock state (no inputs processed)
static float guiAlpha = 1.0f;                   // Gui controls transparency

static unsigned int guiIconScale = 1;           // Gui icon default scale (if icons enabled)

static bool guiTooltip = false;                 // Tooltip enabled/disabled
static const char *guiTooltipPtr = NULL;        // Tooltip string pointer (string provided by user)

static bool guiControlExclusiveMode = false;    // Gui control exclusive mode (no inputs processed except current control)
static Rectangle guiControlExclusiveRec = { 0 }; // Gui control exclusive bounds rectangle, used as an unique identifier

static int textBoxCursorIndex = 0;              // Cursor index, shared by all GuiTextBox*()
//static int blinkCursorFrameCounter = 0;       // Frame counter for cursor blinking
static int autoCursorCooldownCounter = 0;       // Cooldown frame counter for automatic cursor movement on key-down
static int autoCursorDelayCounter = 0;          // Delay frame counter for automatic cursor movement

//----------------------------------------------------------------------------------
// Style data array for all gui style properties (allocated on data segment by default)
//
// NOTE 1: First set of BASE properties are generic to all controls but could be individually
// overwritten per control, first set of EXTENDED properties are generic to all controls and
// can not be overwritten individually but custom EXTENDED properties can be used by control
//
// NOTE 2: A new style set could be loaded over this array using GuiLoadStyle(),
// but default gui style could always be recovered with GuiLoadStyleDefault()
//
// guiStyle size is by default: 16*(16 + 8) = 384*4 = 1536 bytes = 1.5 KB
//----------------------------------------------------------------------------------
static unsigned int guiStyle[RAYGUI_MAX_CONTROLS*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)] = { 0 };

static bool guiStyleLoaded = false;         // Style loaded flag for lazy style initialization

//----------------------------------------------------------------------------------
// Standalone Mode Functions Declaration
//
// NOTE: raygui depend on some raylib input and drawing functions
// To use raygui as standalone library, below functions must be defined by the user
//----------------------------------------------------------------------------------
#if defined(RAYGUI_STANDALONE)

#define KEY_RIGHT           262
#define KEY_LEFT            263
#define KEY_DOWN            264
#define KEY_UP              265
#define KEY_BACKSPACE       259
#define KEY_ENTER           257

#define MOUSE_LEFT_BUTTON     0

// Input required functions
//-------------------------------------------------------------------------------
static Vector2 GetMousePosition(void);
static float GetMouseWheelMove(void);
static bool IsMouseButtonDown(int button);
static bool IsMouseButtonPressed(int button);
static bool IsMouseButtonReleased(int button);

static bool IsKeyDown(int key);
static bool IsKeyPressed(int key);
static int GetCharPressed(void);         // -- GuiTextBox(), GuiValueBox()
//-------------------------------------------------------------------------------

// Drawing required functions
//-------------------------------------------------------------------------------
static void DrawRectangle(int x, int y, int width, int height, Color color);        // -- GuiDrawRectangle()
static void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4); // -- GuiColorPicker()
//-------------------------------------------------------------------------------

// Text required functions
//-------------------------------------------------------------------------------
static Font GetFontDefault(void);                            // -- GuiLoadStyleDefault()
static Font LoadFontEx(const char *fileName, int fontSize, int *codepoints, int codepointCount); // -- GuiLoadStyle(), load font

static Texture2D LoadTextureFromImage(Image image);          // -- GuiLoadStyle(), required to load texture from embedded font atlas image
static void SetShapesTexture(Texture2D tex, Rectangle rec);  // -- GuiLoadStyle(), required to set shapes rec to font white rec (optimization)

static char *LoadFileText(const char *fileName);             // -- GuiLoadStyle(), required to load charset data
static void UnloadFileText(char *text);                      // -- GuiLoadStyle(), required to unload charset data

static const char *GetDirectoryPath(const char *filePath);   // -- GuiLoadStyle(), required to find charset/font file from text .rgs

static int *LoadCodepoints(const char *text, int *count);    // -- GuiLoadStyle(), required to load required font codepoints list
static void UnloadCodepoints(int *codepoints);               // -- GuiLoadStyle(), required to unload codepoints list

static unsigned char *DecompressData(const unsigned char *compData, int compDataSize, int *dataSize); // -- GuiLoadStyle()
//-------------------------------------------------------------------------------

// raylib functions already implemented in raygui
//-------------------------------------------------------------------------------
//static Color GetColor(int hexValue);                // Returns a Color struct from hexadecimal value
static int ColorToInt(Color color);                 // Returns hexadecimal value for a Color
static bool CheckCollisionPointRec(Vector2 point, Rectangle rec);   // Check if point is inside rectangle
static const char *TextFormat(const char *text, ...);               // Formatting of text with variables to 'embed'
static const char **TextSplit(const char *text, char delimiter, int *count);    // Split text into multiple strings
static int TextToInteger(const char *text);         // Get integer value from text
static float TextToFloat(const char *text);         // Get float value from text

static int GetCodepointNext(const char *text, int *codepointSize);  // Get next codepoint in a UTF-8 encoded text
static const char *CodepointToUTF8(int codepoint, int *byteSize);   // Encode codepoint into UTF-8 text (char array size returned as parameter)

static void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2);  // Draw rectangle vertical gradient
//-------------------------------------------------------------------------------

#endif      // RAYGUI_STANDALONE

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
static void GuiLoadStyleFromMemory(const unsigned char *fileData, int dataSize);    // Load style from memory (binary only)

static int GetTextWidth(const char *text);                      // Gui get text width using gui font and style
static Rectangle GetTextBounds(int control, Rectangle bounds);  // Get text bounds considering control bounds
static const char *GetTextIcon(const char *text, int *iconId);  // Get text icon if provided and move text cursor

static void GuiDrawText(const char *text, Rectangle textBounds, int alignment, Color tint);     // Gui draw text using default font
static void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color);   // Gui draw rectangle using default raygui style

static const char **GuiTextSplit(const char *text, char delimiter, int *count, int *textRow);   // Split controls text into multiple strings
static Vector3 ConvertHSVtoRGB(Vector3 hsv);                    // Convert color data from HSV to RGB
static Vector3 ConvertRGBtoHSV(Vector3 rgb);                    // Convert color data from RGB to HSV

static int GuiScrollBar(Rectangle bounds, int value, int minValue, int maxValue);   // Scroll bar control, used by GuiScrollPanel()
static void GuiTooltip(Rectangle controlRec);                   // Draw tooltip using control rec position

static Color GuiFade(Color color, float alpha);         // Fade color by an alpha factor

//----------------------------------------------------------------------------------
// Gui Setup Functions Definition
//----------------------------------------------------------------------------------
// Enable gui global state
// NOTE: We check for STATE_DISABLED to avoid messing custom global state setups
void GuiEnable(void) { if (guiState == STATE_DISABLED) guiState = STATE_NORMAL; }

// Disable gui global state
// NOTE: We check for STATE_NORMAL to avoid messing custom global state setups
void GuiDisable(void) { if (guiState == STATE_NORMAL) guiState = STATE_DISABLED; }

// Lock gui global state
void GuiLock(void) { guiLocked = true; }

// Unlock gui global state
void GuiUnlock(void) { guiLocked = false; }

// Check if gui is locked (global state)
bool GuiIsLocked(void) { return guiLocked; }

// Set gui controls alpha global state
void GuiSetAlpha(float alpha)
{
    if (alpha < 0.0f) alpha = 0.0f;
    else if (alpha > 1.0f) alpha = 1.0f;

    guiAlpha = alpha;
}

// Set gui state (global state)
void GuiSetState(int state) { guiState = (GuiState)state; }

// Get gui state (global state)
int GuiGetState(void) { return guiState; }

// Set custom gui font
// NOTE: Font loading/unloading is external to raygui
void GuiSetFont(Font font)
{
    if (font.texture.id > 0)
    {
        // NOTE: If we try to setup a font but default style has not been
        // lazily loaded before, it will be overwritten, so we need to force
        // default style loading first
        if (!guiStyleLoaded) GuiLoadStyleDefault();

        guiFont = font;
    }
}

// Get custom gui font
Font GuiGetFont(void)
{
    return guiFont;
}

// Set control style property value
void GuiSetStyle(int control, int property, int value)
{
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    guiStyle[control*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + property] = value;

    // Default properties are propagated to all controls
    if ((control == 0) && (property < RAYGUI_MAX_PROPS_BASE))
    {
        for (int i = 1; i < RAYGUI_MAX_CONTROLS; i++) guiStyle[i*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + property] = value;
    }
}

Color GetColor(unsigned int hexValue)
{
    Color color;

    color.r = (unsigned char)(hexValue >> 24) & 0xFF;
    color.g = (unsigned char)(hexValue >> 16) & 0xFF;
    color.b = (unsigned char)(hexValue >> 8) & 0xFF;
    color.a = (unsigned char)hexValue & 0xFF;

    return color;
}


// Get control style property value
int GuiGetStyle(int control, int property)
{
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    return guiStyle[control*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + property];
}

//----------------------------------------------------------------------------------
// Gui Controls Functions Definition
//----------------------------------------------------------------------------------

// Window Box control
int GuiWindowBox(Rectangle bounds, const char *title)
{
    // Window title bar height (including borders)
    // NOTE: This define is also used by GuiMessageBox() and GuiTextInputBox()
    #if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
        #define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT        24
    #endif

    int result = 0;
    //GuiState state = guiState;

    int statusBarHeight = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;

    Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)statusBarHeight };
    if (bounds.height < statusBarHeight*2.0f) bounds.height = statusBarHeight*2.0f;

    Rectangle windowPanel = { bounds.x, bounds.y + (float)statusBarHeight - 1, bounds.width, bounds.height - (float)statusBarHeight + 1 };
    Rectangle closeButtonRec = { statusBar.x + statusBar.width - GuiGetStyle(STATUSBAR, BORDER_WIDTH) - 20,
                                 statusBar.y + statusBarHeight/2.0f - 18.0f/2.0f, 18, 18 };

    // Update control
    //--------------------------------------------------------------------
    // NOTE: Logic is directly managed by button
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiStatusBar(statusBar, title); // Draw window header as status bar
    GuiPanel(windowPanel, NULL);    // Draw window base

    // Draw window close button
    int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
    int tempTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
#if defined(RAYGUI_NO_ICONS)
    result = GuiButton(closeButtonRec, "x");
#else
    result = GuiButton(closeButtonRec, GuiIconText(ICON_CROSS_SMALL, NULL));
#endif
    GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlignment);
    //--------------------------------------------------------------------

    return result;      // Window close button clicked: result = 1
}

// Group Box control with text name
RAYGUIAPI int GuiGroupBox(Rectangle bounds, const char *text)
{
    #if !defined(RAYGUI_GROUPBOX_LINE_THICK)
        #define RAYGUI_GROUPBOX_LINE_THICK     1
    #endif

    int result = 0;
    GuiState state = guiState;

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y, RAYGUI_GROUPBOX_LINE_THICK, bounds.height }, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR)));
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height - 1, bounds.width, RAYGUI_GROUPBOX_LINE_THICK }, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR)));
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - 1, bounds.y, RAYGUI_GROUPBOX_LINE_THICK, bounds.height }, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR)));

    GuiLine(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y - GuiGetStyle(DEFAULT, TEXT_SIZE)/2, bounds.width, (float)GuiGetStyle(DEFAULT, TEXT_SIZE) }, text);
    //--------------------------------------------------------------------

    return result;
}

// Line control
int GuiLine(Rectangle bounds, const char *text)
{
    #if !defined(RAYGUI_LINE_ORIGIN_SIZE)
        #define RAYGUI_LINE_MARGIN_TEXT  12
    #endif
    #if !defined(RAYGUI_LINE_TEXT_PADDING)
        #define RAYGUI_LINE_TEXT_PADDING  4
    #endif

    int result = 0;
    GuiState state = guiState;

    Color color = GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR));

    // Draw control
    //--------------------------------------------------------------------
    if (text == NULL) GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height/2, bounds.width, 1 }, 0, BLANK, color);
    else
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = bounds.height;
        textBounds.x = bounds.x + RAYGUI_LINE_MARGIN_TEXT;
        textBounds.y = bounds.y;

        // Draw line with embedded text label: "--- text --------------"
        GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height/2, RAYGUI_LINE_MARGIN_TEXT - RAYGUI_LINE_TEXT_PADDING, 1 }, 0, BLANK, color);
        GuiDrawText(text, textBounds, TEXT_ALIGN_LEFT, color);
        GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + 12 + textBounds.width + 4, bounds.y + bounds.height/2, bounds.width - textBounds.width - RAYGUI_LINE_MARGIN_TEXT - RAYGUI_LINE_TEXT_PADDING, 1 }, 0, BLANK, color);
    }
    //--------------------------------------------------------------------

    return result;
}

// Panel control
int GuiPanel(Rectangle bounds, const char *text)
{
    #if !defined(RAYGUI_PANEL_BORDER_WIDTH)
        #define RAYGUI_PANEL_BORDER_WIDTH   1
    #endif

    int result = 0;
    GuiState state = guiState;

    // Text will be drawn as a header bar (if provided)
    Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
    if ((text != NULL) && (bounds.height < RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f)) bounds.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f;

    if (text != NULL)
    {
        // Move panel bounds after the header bar
        bounds.y += (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
        bounds.height -= (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
    }

    // Draw control
    //--------------------------------------------------------------------
    if (text != NULL) GuiStatusBar(statusBar, text);  // Draw panel header as status bar

    GuiDrawRectangle(bounds, RAYGUI_PANEL_BORDER_WIDTH, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED: LINE_COLOR)),
                     GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BASE_COLOR_DISABLED : BACKGROUND_COLOR)));
    //--------------------------------------------------------------------

    return result;
}

// Tab Bar control
// NOTE: Using GuiToggle() for the TABS
int GuiTabBar(Rectangle bounds, const char **text, int count, int *active)
{
    #define RAYGUI_TABBAR_ITEM_WIDTH    160

    int result = -1;
    //GuiState state = guiState;

    Rectangle tabBounds = { bounds.x, bounds.y, RAYGUI_TABBAR_ITEM_WIDTH, bounds.height };

    if (*active < 0) *active = 0;
    else if (*active > count - 1) *active = count - 1;

    int offsetX = 0;    // Required in case tabs go out of screen
    offsetX = (*active + 2)*RAYGUI_TABBAR_ITEM_WIDTH - GetScreenWidth();
    if (offsetX < 0) offsetX = 0;

    bool toggle = false;    // Required for individual toggles

    // Draw control
    //--------------------------------------------------------------------
    for (int i = 0; i < count; i++)
    {
        tabBounds.x = bounds.x + (RAYGUI_TABBAR_ITEM_WIDTH + 4)*i - offsetX;

        if (tabBounds.x < GetScreenWidth())
        {
            // Draw tabs as toggle controls
            int textAlignment = GuiGetStyle(TOGGLE, TEXT_ALIGNMENT);
            int textPadding = GuiGetStyle(TOGGLE, TEXT_PADDING);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(TOGGLE, TEXT_PADDING, 8);

            if (i == (*active))
            {
                toggle = true;
                GuiToggle(tabBounds, GuiIconText(12, text[i]), &toggle);
            }
            else
            {
                toggle = false;
                GuiToggle(tabBounds, GuiIconText(12, text[i]), &toggle);
                if (toggle) *active = i;
            }

            // Close tab with middle mouse button pressed
            if (CheckCollisionPointRec(GetMousePosition(), tabBounds) && IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) result = i;

            GuiSetStyle(TOGGLE, TEXT_PADDING, textPadding);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, textAlignment);

            // Draw tab close button
            // NOTE: Only draw close button for current tab: if (CheckCollisionPointRec(mousePosition, tabBounds))
            int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
            int tempTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
            GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
#if defined(RAYGUI_NO_ICONS)
            if (GuiButton(RAYGUI_CLITERAL(Rectangle){ tabBounds.x + tabBounds.width - 14 - 5, tabBounds.y + 5, 14, 14 }, "x")) result = i;
#else
            if (GuiButton(RAYGUI_CLITERAL(Rectangle){ tabBounds.x + tabBounds.width - 14 - 5, tabBounds.y + 5, 14, 14 }, GuiIconText(ICON_CROSS_SMALL, NULL))) result = i;
#endif
            GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlignment);
        }
    }

    // Draw tab-bar bottom line
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height - 1, bounds.width, 1 }, 0, BLANK, GetColor(GuiGetStyle(TOGGLE, BORDER_COLOR_NORMAL)));
    //--------------------------------------------------------------------

    return result;     // Return as result the current TAB closing requested
}

// Scroll Panel control
int GuiScrollPanel(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view)
{
    #define RAYGUI_MIN_SCROLLBAR_WIDTH     40
    #define RAYGUI_MIN_SCROLLBAR_HEIGHT    40
    #define RAYGUI_MIN_MOUSE_WHEEL_SPEED   20

    int result = 0;
    GuiState state = guiState;

    Rectangle temp = { 0 };
    if (view == NULL) view = &temp;

    Vector2 scrollPos = { 0.0f, 0.0f };
    if (scroll != NULL) scrollPos = *scroll;

    // Text will be drawn as a header bar (if provided)
    Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
    if (bounds.height < RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f) bounds.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f;

    if (text != NULL)
    {
        // Move panel bounds after the header bar
        bounds.y += (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
        bounds.height -= (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 1;
    }

    bool hasHorizontalScrollBar = (content.width > bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH))? true : false;
    bool hasVerticalScrollBar = (content.height > bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH))? true : false;

    // Recheck to account for the other scrollbar being visible
    if (!hasHorizontalScrollBar) hasHorizontalScrollBar = (hasVerticalScrollBar && (content.width > (bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH))))? true : false;
    if (!hasVerticalScrollBar) hasVerticalScrollBar = (hasHorizontalScrollBar && (content.height > (bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH))))? true : false;

    int horizontalScrollBarWidth = hasHorizontalScrollBar? GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH) : 0;
    int verticalScrollBarWidth =  hasVerticalScrollBar? GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH) : 0;
    Rectangle horizontalScrollBar = {
        (float)((GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)bounds.x + verticalScrollBarWidth : (float)bounds.x) + GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)bounds.y + bounds.height - horizontalScrollBarWidth - GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)bounds.width - verticalScrollBarWidth - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)horizontalScrollBarWidth
    };
    Rectangle verticalScrollBar = {
        (float)((GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)bounds.x + GuiGetStyle(DEFAULT, BORDER_WIDTH) : (float)bounds.x + bounds.width - verticalScrollBarWidth - GuiGetStyle(DEFAULT, BORDER_WIDTH)),
        (float)bounds.y + GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)verticalScrollBarWidth,
        (float)bounds.height - horizontalScrollBarWidth - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH)
    };

    // Make sure scroll bars have a minimum width/height
    if (horizontalScrollBar.width < RAYGUI_MIN_SCROLLBAR_WIDTH) horizontalScrollBar.width = RAYGUI_MIN_SCROLLBAR_WIDTH;
    if (verticalScrollBar.height < RAYGUI_MIN_SCROLLBAR_HEIGHT) verticalScrollBar.height = RAYGUI_MIN_SCROLLBAR_HEIGHT;

    // Calculate view area (area without the scrollbars)
    *view = (GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)?
                RAYGUI_CLITERAL(Rectangle){ bounds.x + verticalScrollBarWidth + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.y + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth, bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth } :
                RAYGUI_CLITERAL(Rectangle){ bounds.x + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.y + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth, bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth };

    // Clip view area to the actual content size
    if (view->width > content.width) view->width = content.width;
    if (view->height > content.height) view->height = content.height;

    float horizontalMin = hasHorizontalScrollBar? ((GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)-verticalScrollBarWidth : 0) - (float)GuiGetStyle(DEFAULT, BORDER_WIDTH) : (((float)GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)-verticalScrollBarWidth : 0) - (float)GuiGetStyle(DEFAULT, BORDER_WIDTH);
    float horizontalMax = hasHorizontalScrollBar? content.width - bounds.width + (float)verticalScrollBarWidth + GuiGetStyle(DEFAULT, BORDER_WIDTH) - (((float)GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)verticalScrollBarWidth : 0) : (float)-GuiGetStyle(DEFAULT, BORDER_WIDTH);
    float verticalMin = hasVerticalScrollBar? 0.0f : -1.0f;
    float verticalMax = hasVerticalScrollBar? content.height - bounds.height + (float)horizontalScrollBarWidth + (float)GuiGetStyle(DEFAULT, BORDER_WIDTH) : (float)-GuiGetStyle(DEFAULT, BORDER_WIDTH);

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

#if defined(SUPPORT_SCROLLBAR_KEY_INPUT)
            if (hasHorizontalScrollBar)
            {
                if (IsKeyDown(KEY_RIGHT)) scrollPos.x -= GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                if (IsKeyDown(KEY_LEFT)) scrollPos.x += GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
            }

            if (hasVerticalScrollBar)
            {
                if (IsKeyDown(KEY_DOWN)) scrollPos.y -= GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                if (IsKeyDown(KEY_UP)) scrollPos.y += GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
            }
#endif
            float wheelMove = GetMouseWheelMove();

            // Set scrolling speed with mouse wheel based on ratio between bounds and content
            Vector2 mouseWheelSpeed = { content.width/bounds.width, content.height/bounds.height };
            if (mouseWheelSpeed.x < RAYGUI_MIN_MOUSE_WHEEL_SPEED) mouseWheelSpeed.x = RAYGUI_MIN_MOUSE_WHEEL_SPEED;
            if (mouseWheelSpeed.y < RAYGUI_MIN_MOUSE_WHEEL_SPEED) mouseWheelSpeed.y = RAYGUI_MIN_MOUSE_WHEEL_SPEED;

            // Horizontal and vertical scrolling with mouse wheel
            if (hasHorizontalScrollBar && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SHIFT))) scrollPos.x += wheelMove*mouseWheelSpeed.x;
            else scrollPos.y += wheelMove*mouseWheelSpeed.y; // Vertical scroll
        }
    }

    // Normalize scroll values
    if (scrollPos.x > -horizontalMin) scrollPos.x = -horizontalMin;
    if (scrollPos.x < -horizontalMax) scrollPos.x = -horizontalMax;
    if (scrollPos.y > -verticalMin) scrollPos.y = -verticalMin;
    if (scrollPos.y < -verticalMax) scrollPos.y = -verticalMax;
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (text != NULL) GuiStatusBar(statusBar, text);  // Draw panel header as status bar

    GuiDrawRectangle(bounds, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));        // Draw background

    // Save size of the scrollbar slider
    const int slider = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);

    // Draw horizontal scrollbar if visible
    if (hasHorizontalScrollBar)
    {
        // Change scrollbar slider size to show the diff in size between the content width and the widget width
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth)/(int)content.width)*((int)bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth)));
        scrollPos.x = (float)-GuiScrollBar(horizontalScrollBar, (int)-scrollPos.x, (int)horizontalMin, (int)horizontalMax);
    }
    else scrollPos.x = 0.0f;

    // Draw vertical scrollbar if visible
    if (hasVerticalScrollBar)
    {
        // Change scrollbar slider size to show the diff in size between the content height and the widget height
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth)/(int)content.height)*((int)bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth)));
        scrollPos.y = (float)-GuiScrollBar(verticalScrollBar, (int)-scrollPos.y, (int)verticalMin, (int)verticalMax);
    }
    else scrollPos.y = 0.0f;

    // Draw detail corner rectangle if both scroll bars are visible
    if (hasHorizontalScrollBar && hasVerticalScrollBar)
    {
        Rectangle corner = { (GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (bounds.x + GuiGetStyle(DEFAULT, BORDER_WIDTH) + 2) : (horizontalScrollBar.x + horizontalScrollBar.width + 2), verticalScrollBar.y + verticalScrollBar.height + 2, (float)horizontalScrollBarWidth - 4, (float)verticalScrollBarWidth - 4 };
        GuiDrawRectangle(corner, 0, BLANK, GetColor(GuiGetStyle(LISTVIEW, TEXT + (state*3))));
    }

    // Draw scrollbar lines depending on current state
    GuiDrawRectangle(bounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER + (state*3))), BLANK);

    // Set scrollbar slider size back to the way it was before
    GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, slider);
    //--------------------------------------------------------------------

    if (scroll != NULL) *scroll = scrollPos;

    return result;
}

// Label control
int GuiLabel(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    //...
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Button control, returns true when clicked
int GuiButton(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), GetColor(GuiGetStyle(BUTTON, BORDER + (state*3))), GetColor(GuiGetStyle(BUTTON, BASE + (state*3))));
    GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT), GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))));

    if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //------------------------------------------------------------------

    return result;      // Button pressed: result = 1
}

// Label button control
int GuiLabelButton(Rectangle bounds, const char *text)
{
    GuiState state = guiState;
    bool pressed = false;

    // NOTE: We force bounds.width to be all text
    float textWidth = (float)GetTextWidth(text);
    if ((bounds.width - 2*GuiGetStyle(LABEL, BORDER_WIDTH) - 2*GuiGetStyle(LABEL, TEXT_PADDING)) < textWidth) bounds.width = textWidth + 2*GuiGetStyle(LABEL, BORDER_WIDTH) + 2*GuiGetStyle(LABEL, TEXT_PADDING) + 2;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check checkbox state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) pressed = true;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return pressed;
}

// Toggle Button control
int GuiToggle(Rectangle bounds, const char *text, bool *active)
{
    int result = 0;
    GuiState state = guiState;

    bool temp = false;
    if (active == NULL) active = &temp;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check toggle button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state = STATE_NORMAL;
                *active = !(*active);
            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_NORMAL)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TOGGLE, BORDER_WIDTH), GetColor(GuiGetStyle(TOGGLE, ((*active)? BORDER_COLOR_PRESSED : (BORDER + state*3)))), GetColor(GuiGetStyle(TOGGLE, ((*active)? BASE_COLOR_PRESSED : (BASE + state*3)))));
        GuiDrawText(text, GetTextBounds(TOGGLE, bounds), GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TOGGLE, ((*active)? TEXT_COLOR_PRESSED : (TEXT + state*3)))));
    }
    else
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TOGGLE, BORDER_WIDTH), GetColor(GuiGetStyle(TOGGLE, BORDER + state*3)), GetColor(GuiGetStyle(TOGGLE, BASE + state*3)));
        GuiDrawText(text, GetTextBounds(TOGGLE, bounds), GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TOGGLE, TEXT + state*3)));
    }

    if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //--------------------------------------------------------------------

    return result;
}

// Toggle Group control
int GuiToggleGroup(Rectangle bounds, const char *text, int *active)
{
    #if !defined(RAYGUI_TOGGLEGROUP_MAX_ITEMS)
        #define RAYGUI_TOGGLEGROUP_MAX_ITEMS    32
    #endif

    int result = 0;
    float initBoundsX = bounds.x;

    int temp = 0;
    if (active == NULL) active = &temp;

    bool toggle = false;    // Required for individual toggles

    // Get substrings items from text (items pointers)
    int rows[RAYGUI_TOGGLEGROUP_MAX_ITEMS] = { 0 };
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, rows);

    int prevRow = rows[0];

    for (int i = 0; i < itemCount; i++)
    {
        if (prevRow != rows[i])
        {
            bounds.x = initBoundsX;
            bounds.y += (bounds.height + GuiGetStyle(TOGGLE, GROUP_PADDING));
            prevRow = rows[i];
        }

        if (i == (*active))
        {
            toggle = true;
            GuiToggle(bounds, items[i], &toggle);
        }
        else
        {
            toggle = false;
            GuiToggle(bounds, items[i], &toggle);
            if (toggle) *active = i;
        }

        bounds.x += (bounds.width + GuiGetStyle(TOGGLE, GROUP_PADDING));
    }

    return result;
}

// Toggle Slider control extended
int GuiToggleSlider(Rectangle bounds, const char *text, int *active)
{
    int result = 0;
    GuiState state = guiState;

    int temp = 0;
    if (active == NULL) active = &temp;

    //bool toggle = false;    // Required for individual toggles

    // Get substrings items from text (items pointers)
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, NULL);

    Rectangle slider = {
        0,      // Calculated later depending on the active toggle
        bounds.y + GuiGetStyle(SLIDER, BORDER_WIDTH) + GuiGetStyle(SLIDER, SLIDER_PADDING),
        (bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - (itemCount + 1)*GuiGetStyle(SLIDER, SLIDER_PADDING))/itemCount,
        bounds.height - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - 2*GuiGetStyle(SLIDER, SLIDER_PADDING) };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                (*active)++;
                result = 1;
            }
            else state = STATE_FOCUSED;
        }

        if ((*active) && (state != STATE_FOCUSED)) state = STATE_PRESSED;
    }

    if (*active >= itemCount) *active = 0;
    slider.x = bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH) + (*active + 1)*GuiGetStyle(SLIDER, SLIDER_PADDING) + (*active)*slider.width;
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(SLIDER, BORDER_WIDTH), GetColor(GuiGetStyle(TOGGLE, BORDER + (state*3))),
        GetColor(GuiGetStyle(TOGGLE, BASE_COLOR_NORMAL)));

    // Draw internal slider
    if (state == STATE_NORMAL) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)));
    else if (state == STATE_FOCUSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_FOCUSED)));
    else if (state == STATE_PRESSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)));

    // Draw text in slider
    if (text != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(text);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = slider.x + slider.width/2 - textBounds.width/2;
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(items[*active], textBounds, GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(TOGGLE, TEXT + (state*3))), guiAlpha));
    }
    //--------------------------------------------------------------------

    return result;
}

// Check Box control, returns 1 when state changed
int GuiCheckBox(Rectangle bounds, const char *text, bool *checked)
{
    int result = 0;
    GuiState state = guiState;

    bool temp = false;
    if (checked == NULL) checked = &temp;

    Rectangle textBounds = { 0 };

    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(CHECKBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        Rectangle totalBounds = {
            (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT)? textBounds.x : bounds.x,
            bounds.y,
            bounds.width + textBounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING),
            bounds.height,
        };

        // Check checkbox state
        if (CheckCollisionPointRec(mousePoint, totalBounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                *checked = !(*checked);
                result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(CHECKBOX, BORDER_WIDTH), GetColor(GuiGetStyle(CHECKBOX, BORDER + (state*3))), BLANK);

    if (*checked)
    {
        Rectangle check = { bounds.x + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.y + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.width - 2*(GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)),
                            bounds.height - 2*(GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)) };
        GuiDrawRectangle(check, 0, BLANK, GetColor(GuiGetStyle(CHECKBOX, TEXT + state*3)));
    }

    GuiDrawText(text, textBounds, (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Combo Box control
int GuiComboBox(Rectangle bounds, const char *text, int *active)
{
    int result = 0;
    GuiState state = guiState;

    int temp = 0;
    if (active == NULL) active = &temp;

    bounds.width -= (GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH) + GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING));

    Rectangle selector = { (float)bounds.x + bounds.width + GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING),
                           (float)bounds.y, (float)GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH), (float)bounds.height };

    // Get substrings items from text (items pointers, lengths and count)
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, NULL);

    if (*active < 0) *active = 0;
    else if (*active > (itemCount - 1)) *active = itemCount - 1;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && (itemCount > 1) && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds) ||
            CheckCollisionPointRec(mousePoint, selector))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                *active += 1;
                if (*active >= itemCount) *active = 0;      // Cyclic combobox
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    // Draw combo box main
    GuiDrawRectangle(bounds, GuiGetStyle(COMBOBOX, BORDER_WIDTH), GetColor(GuiGetStyle(COMBOBOX, BORDER + (state*3))), GetColor(GuiGetStyle(COMBOBOX, BASE + (state*3))));
    GuiDrawText(items[*active], GetTextBounds(COMBOBOX, bounds), GuiGetStyle(COMBOBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(COMBOBOX, TEXT + (state*3))));

    // Draw selector using a custom button
    // NOTE: BORDER_WIDTH and TEXT_ALIGNMENT forced values
    int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
    int tempTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiButton(selector, TextFormat("%i/%i", *active + 1, itemCount));

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlign);
    GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);
    //--------------------------------------------------------------------

    return result;
}

// Dropdown Box control
// NOTE: Returns mouse click
int GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode)
{
    int result = 0;
    GuiState state = guiState;

    int temp = 0;
    if (active == NULL) active = &temp;

    int itemSelected = *active;
    int itemFocused = -1;

    int direction = 0; // Dropdown box open direction: down (default)
    if (GuiGetStyle(DROPDOWNBOX, DROPDOWN_ROLL_UP) == 1) direction = 1; // Up

    // Get substrings items from text (items pointers, lengths and count)
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, NULL);

    Rectangle boundsOpen = bounds;
    boundsOpen.height = (itemCount + 1)*(bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));
    if (direction == 1) boundsOpen.y -= itemCount*(bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING)) + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING);

    Rectangle itemBounds = bounds;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && (editMode || !guiLocked) && (itemCount > 1) && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        if (editMode)
        {
            state = STATE_PRESSED;

            // Check if mouse has been pressed or released outside limits
            if (!CheckCollisionPointRec(mousePoint, boundsOpen))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
            }

            // Check if already selected item has been pressed again
            if (CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;

            // Check focused and selected item
            for (int i = 0; i < itemCount; i++)
            {
                // Update item rectangle y position for next item
                if (direction == 0) itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));
                else itemBounds.y -= (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

                if (CheckCollisionPointRec(mousePoint, itemBounds))
                {
                    itemFocused = i;
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                    {
                        itemSelected = i;
                        result = 1;         // Item selected
                    }
                    break;
                }
            }

            itemBounds = bounds;
        }
        else
        {
            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    result = 1;
                    state = STATE_PRESSED;
                }
                else state = STATE_FOCUSED;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (editMode) GuiPanel(boundsOpen, NULL);

    GuiDrawRectangle(bounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER + state*3)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE + state*3)));
    GuiDrawText(items[itemSelected], GetTextBounds(DROPDOWNBOX, bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + state*3)));

    if (editMode)
    {
        // Draw visible items
        for (int i = 0; i < itemCount; i++)
        {
            // Update item rectangle y position for next item
            if (direction == 0) itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));
            else itemBounds.y -= (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

            if (i == itemSelected)
            {
                GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_PRESSED)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED)));
                GuiDrawText(items[i], GetTextBounds(DROPDOWNBOX, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED)));
            }
            else if (i == itemFocused)
            {
                GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED)));
                GuiDrawText(items[i], GetTextBounds(DROPDOWNBOX, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_FOCUSED)));
            }
            else GuiDrawText(items[i], GetTextBounds(DROPDOWNBOX, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL)));
        }
    }

    if (!GuiGetStyle(DROPDOWNBOX, DROPDOWN_ARROW_HIDDEN))
    {
        // Draw arrows (using icon if available)
#if defined(RAYGUI_NO_ICONS)
        GuiDrawText("v", RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 2, 10, 10 },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));
#else
        GuiDrawText(direction? "#121#" : "#120#", RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 6, 10, 10 },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));   // ICON_ARROW_DOWN_FILL
#endif
    }
    //--------------------------------------------------------------------

    *active = itemSelected;

    // TODO: Use result to return more internal states: mouse-press out-of-bounds, mouse-press over selected-item...
    return result;   // Mouse click: result = 1
}

// Text Box control
// NOTE: Returns true on ENTER pressed (useful for data validation)
int GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode)
{
    #if !defined(RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN)
        #define RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN  40        // Frames to wait for autocursor movement
    #endif
    #if !defined(RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY)
        #define RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY      1        // Frames delay for autocursor movement
    #endif

    int result = 0;
    GuiState state = guiState;

    bool multiline = false;     // TODO: Consider multiline text input
    int wrapMode = GuiGetStyle(DEFAULT, TEXT_WRAP_MODE);

    Rectangle textBounds = GetTextBounds(TEXTBOX, bounds);
    int textLength = (int)strlen(text);     // Get current text length
    int thisCursorIndex = textBoxCursorIndex;
    if (thisCursorIndex > textLength) thisCursorIndex = textLength;
    int textWidth = GetTextWidth(text) - GetTextWidth(text + thisCursorIndex);
    int textIndexOffset = 0;    // Text index offset to start drawing in the box

    // Cursor rectangle
    // NOTE: Position X value should be updated
    Rectangle cursor = {
        textBounds.x + textWidth + GuiGetStyle(DEFAULT, TEXT_SPACING),
        textBounds.y + textBounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE),
        2,
        (float)GuiGetStyle(DEFAULT, TEXT_SIZE)*2
    };

    if (cursor.height >= bounds.height) cursor.height = bounds.height - GuiGetStyle(TEXTBOX, BORDER_WIDTH)*2;
    if (cursor.y < (bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH))) cursor.y = bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH);

    // Mouse cursor rectangle
    // NOTE: Initialized outside of screen
    Rectangle mouseCursor = cursor;
    mouseCursor.x = -1;
    mouseCursor.width = 1;

    // Auto-cursor movement logic
    // NOTE: Cursor moves automatically when key down after some time
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_BACKSPACE) || IsKeyDown(KEY_DELETE)) autoCursorCooldownCounter++;
    else
    {
        autoCursorCooldownCounter = 0;      // GLOBAL: Cursor cooldown counter
        autoCursorDelayCounter = 0;         // GLOBAL: Cursor delay counter
    }

    // Blink-cursor frame counter
    //if (!autoCursorMode) blinkCursorFrameCounter++;
    //else blinkCursorFrameCounter = 0;

    // Update control
    //--------------------------------------------------------------------
    // WARNING: Text editing is only supported under certain conditions:
    if ((state != STATE_DISABLED) &&                // Control not disabled
        !GuiGetStyle(TEXTBOX, TEXT_READONLY) &&     // TextBox not on read-only mode
        !guiLocked &&                               // Gui not locked
        !guiControlExclusiveMode &&                       // No gui slider on dragging
        (wrapMode == TEXT_WRAP_NONE))               // No wrap mode
    {
        Vector2 mousePosition = GetMousePosition();

        if (editMode)
        {
            state = STATE_PRESSED;

            if (textBoxCursorIndex > textLength) textBoxCursorIndex = textLength;

            // If text does not fit in the textbox and current cursor position is out of bounds,
            // we add an index offset to text for drawing only what requires depending on cursor
            while (textWidth >= textBounds.width)
            {
                int nextCodepointSize = 0;
                GetCodepointNext(text + textIndexOffset, &nextCodepointSize);

                textIndexOffset += nextCodepointSize;

                textWidth = GetTextWidth(text + textIndexOffset) - GetTextWidth(text + textBoxCursorIndex);
            }

            int codepoint = GetCharPressed();       // Get Unicode codepoint
            if (multiline && IsKeyPressed(KEY_ENTER)) codepoint = (int)'\n';

            // Encode codepoint as UTF-8
            int codepointSize = 0;
            const char *charEncoded = CodepointToUTF8(codepoint, &codepointSize);

            // Add codepoint to text, at current cursor position
            // NOTE: Make sure we do not overflow buffer size
            if (((multiline && (codepoint == (int)'\n')) || (codepoint >= 32)) && ((textLength + codepointSize) < textSize))
            {
                // Move forward data from cursor position
                for (int i = (textLength + codepointSize); i > textBoxCursorIndex; i--) text[i] = text[i - codepointSize];

                // Add new codepoint in current cursor position
                for (int i = 0; i < codepointSize; i++) text[textBoxCursorIndex + i] = charEncoded[i];

                textBoxCursorIndex += codepointSize;
                textLength += codepointSize;

                // Make sure text last character is EOL
                text[textLength] = '\0';
            }

            // Move cursor to start
            if ((textLength > 0) && IsKeyPressed(KEY_HOME)) textBoxCursorIndex = 0;

            // Move cursor to end
            if ((textLength > textBoxCursorIndex) && IsKeyPressed(KEY_END)) textBoxCursorIndex = textLength;

            // Delete codepoint from text, after current cursor position
            if ((textLength > textBoxCursorIndex) && (IsKeyPressed(KEY_DELETE) || (IsKeyDown(KEY_DELETE) && (autoCursorCooldownCounter >= RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN))))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_DELETE) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int nextCodepointSize = 0;
                    GetCodepointNext(text + textBoxCursorIndex, &nextCodepointSize);

                    // Move backward text from cursor position
                    for (int i = textBoxCursorIndex; i < textLength; i++) text[i] = text[i + nextCodepointSize];

                    textLength -= codepointSize;
                    if (textBoxCursorIndex > textLength) textBoxCursorIndex = textLength;

                    // Make sure text last character is EOL
                    text[textLength] = '\0';
                }
            }

            // Delete codepoint from text, before current cursor position
            if ((textLength > 0) && (IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && (autoCursorCooldownCounter >= RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN))))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_BACKSPACE) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int prevCodepointSize = 0;
                    GetCodepointPrevious(text + textBoxCursorIndex, &prevCodepointSize);

                    // Move backward text from cursor position
                    for (int i = (textBoxCursorIndex - prevCodepointSize); i < textLength; i++) text[i] = text[i + prevCodepointSize];

                    // TODO Check: >= cursor+codepointsize and <= length-codepointsize

                    // Prevent cursor index from decrementing past 0
                    if (textBoxCursorIndex > 0)
                    {
                        textBoxCursorIndex -= codepointSize;
                        textLength -= codepointSize;
                    }

                    // Make sure text last character is EOL
                    text[textLength] = '\0';
                }
            }

            // Move cursor position with keys
            if (IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && (autoCursorCooldownCounter > RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN)))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_LEFT) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int prevCodepointSize = 0;
                    GetCodepointPrevious(text + textBoxCursorIndex, &prevCodepointSize);

                    if (textBoxCursorIndex >= prevCodepointSize) textBoxCursorIndex -= prevCodepointSize;
                }
            }
            else if (IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && (autoCursorCooldownCounter > RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN)))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_RIGHT) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int nextCodepointSize = 0;
                    GetCodepointNext(text + textBoxCursorIndex, &nextCodepointSize);

                    if ((textBoxCursorIndex + nextCodepointSize) <= textLength) textBoxCursorIndex += nextCodepointSize;
                }
            }

            // Move cursor position with mouse
            if (CheckCollisionPointRec(mousePosition, textBounds))     // Mouse hover text
            {
                float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/(float)guiFont.baseSize;
                int codepointIndex = 0;
                float glyphWidth = 0.0f;
                float widthToMouseX = 0;
                int mouseCursorIndex = 0;

                for (int i = textIndexOffset; i < textLength; i++)
                {
                    codepoint = GetCodepointNext(&text[i], &codepointSize);
                    codepointIndex = GetGlyphIndex(guiFont, codepoint);

                    if (guiFont.glyphs[codepointIndex].advanceX == 0) glyphWidth = ((float)guiFont.recs[codepointIndex].width*scaleFactor);
                    else glyphWidth = ((float)guiFont.glyphs[codepointIndex].advanceX*scaleFactor);

                    if (mousePosition.x <= (textBounds.x + (widthToMouseX + glyphWidth/2)))
                    {
                        mouseCursor.x = textBounds.x + widthToMouseX;
                        mouseCursorIndex = i;
                        break;
                    }

                    widthToMouseX += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
                }

                // Check if mouse cursor is at the last position
                int textEndWidth = GetTextWidth(text + textIndexOffset);
                if (GetMousePosition().x >= (textBounds.x + textEndWidth - glyphWidth/2))
                {
                    mouseCursor.x = textBounds.x + textEndWidth;
                    mouseCursorIndex = textLength;
                }

                // Place cursor at required index on mouse click
                if ((mouseCursor.x >= 0) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    cursor.x = mouseCursor.x;
                    textBoxCursorIndex = mouseCursorIndex;
                }
            }
            else mouseCursor.x = -1;

            // Recalculate cursor position.y depending on textBoxCursorIndex
            cursor.x = bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + GetTextWidth(text + textIndexOffset) - GetTextWidth(text + textBoxCursorIndex) + GuiGetStyle(DEFAULT, TEXT_SPACING);
            //if (multiline) cursor.y = GetTextLines()

            // Finish text editing on ENTER or mouse click outside bounds
            if ((!multiline && IsKeyPressed(KEY_ENTER)) ||
                (!CheckCollisionPointRec(mousePosition, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                textBoxCursorIndex = 0;     // GLOBAL: Reset the shared cursor index
                result = 1;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousePosition, bounds))
            {
                state = STATE_FOCUSED;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    textBoxCursorIndex = textLength;   // GLOBAL: Place cursor index to the end of current text
                    result = 1;
                }
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_PRESSED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)));
    }
    else if (state == STATE_DISABLED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)));
    }
    else GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), BLANK);

    // Draw text considering index offset if required
    // NOTE: Text index offset depends on cursor position
    GuiDrawText(text + textIndexOffset, textBounds, GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TEXTBOX, TEXT + (state*3))));

    // Draw cursor
    if (editMode && !GuiGetStyle(TEXTBOX, TEXT_READONLY))
    {
        //if (autoCursorMode || ((blinkCursorFrameCounter/40)%2 == 0))
        GuiDrawRectangle(cursor, 0, BLANK, GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)));

        // Draw mouse position cursor (if required)
        if (mouseCursor.x >= 0) GuiDrawRectangle(mouseCursor, 0, BLANK, GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)));
    }
    else if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //--------------------------------------------------------------------

    return result;      // Mouse button pressed: result = 1
}

/*
// Text Box control with multiple lines and word-wrap
// NOTE: This text-box is readonly, no editing supported by default
bool GuiTextBoxMulti(Rectangle bounds, char *text, int textSize, bool editMode)
{
    bool pressed = false;

    GuiSetStyle(TEXTBOX, TEXT_READONLY, 1);
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);   // WARNING: If wrap mode enabled, text editing is not supported
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);

    // TODO: Implement methods to calculate cursor position properly
    pressed = GuiTextBox(bounds, text, textSize, editMode);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_NONE);
    GuiSetStyle(TEXTBOX, TEXT_READONLY, 0);

    return pressed;
}
*/

// Spinner control, returns selected value
int GuiSpinner(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
{
    int result = 1;
    GuiState state = guiState;

    int tempValue = *value;

    Rectangle spinner = { bounds.x + GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH) + GuiGetStyle(SPINNER, SPIN_BUTTON_SPACING), bounds.y,
                          bounds.width - 2*(GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH) + GuiGetStyle(SPINNER, SPIN_BUTTON_SPACING)), bounds.height };
    Rectangle leftButtonBound = { (float)bounds.x, (float)bounds.y, (float)GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.height };
    Rectangle rightButtonBound = { (float)bounds.x + bounds.width - GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.y, (float)GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.height };

    Rectangle textBounds = { 0 };
    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(SPINNER, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(SPINNER, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(SPINNER, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check spinner state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
        }
    }

#if defined(RAYGUI_NO_ICONS)
    if (GuiButton(leftButtonBound, "<")) tempValue--;
    if (GuiButton(rightButtonBound, ">")) tempValue++;
#else
    if (GuiButton(leftButtonBound, GuiIconText(ICON_ARROW_LEFT_FILL, NULL))) tempValue--;
    if (GuiButton(rightButtonBound, GuiIconText(ICON_ARROW_RIGHT_FILL, NULL))) tempValue++;
#endif

    if (!editMode)
    {
        if (tempValue < minValue) tempValue = minValue;
        if (tempValue > maxValue) tempValue = maxValue;
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    result = GuiValueBox(spinner, NULL, &tempValue, minValue, maxValue, editMode);

    // Draw value selector custom buttons
    // NOTE: BORDER_WIDTH and TEXT_ALIGNMENT forced values
    int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
    int tempTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, GuiGetStyle(SPINNER, BORDER_WIDTH));
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlign);
    GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);

    // Draw text label if provided
    GuiDrawText(text, textBounds, (GuiGetStyle(SPINNER, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    *value = tempValue;
    return result;
}

// Value Box control, updates input text with numbers
// NOTE: Requires static variables: frameCounter
int GuiValueBox(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
{
    #if !defined(RAYGUI_VALUEBOX_MAX_CHARS)
        #define RAYGUI_VALUEBOX_MAX_CHARS  32
    #endif

    int result = 0;
    GuiState state = guiState;

    char textValue[RAYGUI_VALUEBOX_MAX_CHARS + 1] = "\0";
    sprintf(textValue, "%i", *value);

    Rectangle textBounds = { 0 };
    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(VALUEBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(VALUEBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        bool valueHasChanged = false;

        if (editMode)
        {
            state = STATE_PRESSED;

            int keyCount = (int)strlen(textValue);

            // Only allow keys in range [48..57]
            if (keyCount < RAYGUI_VALUEBOX_MAX_CHARS)
            {
                if (GetTextWidth(textValue) < bounds.width)
                {
                    int key = GetCharPressed();
                    if ((key >= 48) && (key <= 57))
                    {
                        textValue[keyCount] = (char)key;
                        keyCount++;
                        valueHasChanged = true;
                    }
                }
            }

            // Delete text
            if (keyCount > 0)
            {
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    keyCount--;
                    textValue[keyCount] = '\0';
                    valueHasChanged = true;
                }
            }

            if (valueHasChanged) *value = TextToInteger(textValue);

            // NOTE: We are not clamp values until user input finishes
            //if (*value > maxValue) *value = maxValue;
            //else if (*value < minValue) *value = minValue;

            if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) || (!CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                if (*value > maxValue) *value = maxValue;
                else if (*value < minValue) *value = minValue;

                result = 1;
            }
        }
        else
        {
            if (*value > maxValue) *value = maxValue;
            else if (*value < minValue) *value = minValue;

            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                state = STATE_FOCUSED;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    Color baseColor = BLANK;
    if (state == STATE_PRESSED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED));
    else if (state == STATE_DISABLED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_DISABLED));

    GuiDrawRectangle(bounds, GuiGetStyle(VALUEBOX, BORDER_WIDTH), GetColor(GuiGetStyle(VALUEBOX, BORDER + (state*3))), baseColor);
    GuiDrawText(textValue, GetTextBounds(VALUEBOX, bounds), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(VALUEBOX, TEXT + (state*3))));

    // Draw cursor
    if (editMode)
    {
        // NOTE: ValueBox internal text is always centered
        Rectangle cursor = { bounds.x + GetTextWidth(textValue)/2 + bounds.width/2 + 1, bounds.y + 2*GuiGetStyle(VALUEBOX, BORDER_WIDTH), 4, bounds.height - 4*GuiGetStyle(VALUEBOX, BORDER_WIDTH) };
        GuiDrawRectangle(cursor, 0, BLANK, GetColor(GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED)));
    }

    // Draw text label if provided
    GuiDrawText(text, textBounds, (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Floating point Value Box control, updates input val_str with numbers
// NOTE: Requires static variables: frameCounter
int GuiValueBoxFloat(Rectangle bounds, const char *text, char *textValue, float *value, bool editMode)
{
    #if !defined(RAYGUI_VALUEBOX_MAX_CHARS)
        #define RAYGUI_VALUEBOX_MAX_CHARS  32
    #endif

    int result = 0;
    GuiState state = guiState;

    //char textValue[RAYGUI_VALUEBOX_MAX_CHARS + 1] = "\0";
    //sprintf(textValue, "%2.2f", *value);

    Rectangle textBounds = {0};
    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(VALUEBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(VALUEBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        bool valueHasChanged = false;

        if (editMode)
        {
            state = STATE_PRESSED;

            int keyCount = (int)strlen(textValue);

            // Only allow keys in range [48..57]
            if (keyCount < RAYGUI_VALUEBOX_MAX_CHARS)
            {
                if (GetTextWidth(textValue) < bounds.width)
                {
                    int key = GetCharPressed();
                    if (((key >= 48) && (key <= 57)) ||
                        (key == '.') ||
                        ((keyCount == 0) && (key == '+')) ||  // NOTE: Sign can only be in first position
                        ((keyCount == 0) && (key == '-')))
                    {
                        textValue[keyCount] = (char)key;
                        keyCount++;

                        valueHasChanged = true;
                    }
                }
            }

            // Pressed backspace
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (keyCount > 0)
                {
                    keyCount--;
                    textValue[keyCount] = '\0';
                    valueHasChanged = true;
                }
            }

            if (valueHasChanged) *value = TextToFloat(textValue);

            if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) || (!CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) result = 1;
        }
        else
        {
            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                state = STATE_FOCUSED;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    Color baseColor = BLANK;
    if (state == STATE_PRESSED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED));
    else if (state == STATE_DISABLED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_DISABLED));

    GuiDrawRectangle(bounds, GuiGetStyle(VALUEBOX, BORDER_WIDTH), GetColor(GuiGetStyle(VALUEBOX, BORDER + (state*3))), baseColor);
    GuiDrawText(textValue, GetTextBounds(VALUEBOX, bounds), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(VALUEBOX, TEXT + (state*3))));

    // Draw cursor
    if (editMode)
    {
        // NOTE: ValueBox internal text is always centered
        Rectangle cursor = {bounds.x + GetTextWidth(textValue)/2 + bounds.width/2 + 1,
                            bounds.y + 2*GuiGetStyle(VALUEBOX, BORDER_WIDTH), 4,
                            bounds.height - 4*GuiGetStyle(VALUEBOX, BORDER_WIDTH)};
        GuiDrawRectangle(cursor, 0, BLANK, GetColor(GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED)));
    }

    // Draw text label if provided
    GuiDrawText(text, textBounds,
                (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT,
                GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Slider control with pro parameters
// NOTE: Other GuiSlider*() controls use this one
RAYGUIAPI int GuiSliderPro(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue, int sliderWidth)
{
    int result = 0;
    GuiState state = guiState;

    float temp = (maxValue - minValue)/2.0f;
    if (value == NULL) value = &temp;
    float oldValue = *value;

    Rectangle slider = { bounds.x, bounds.y + GuiGetStyle(SLIDER, BORDER_WIDTH) + GuiGetStyle(SLIDER, SLIDER_PADDING),
                         0, bounds.height - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - 2*GuiGetStyle(SLIDER, SLIDER_PADDING) };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiControlExclusiveMode) // Allows to keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiControlExclusiveRec))
                {
                    state = STATE_PRESSED;
                    // Get equivalent value and slider position from mousePosition.x
                    *value = (maxValue - minValue)*((mousePoint.x - bounds.x - sliderWidth/2)/(bounds.width-sliderWidth)) + minValue;
                }
            }
            else
            {
                guiControlExclusiveMode = false;
                guiControlExclusiveRec = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiControlExclusiveMode = true;
                guiControlExclusiveRec = bounds; // Store bounds as an identifier when dragging starts

                if (!CheckCollisionPointRec(mousePoint, slider))
                {
                    // Get equivalent value and slider position from mousePosition.x
                    *value = (maxValue - minValue)*((mousePoint.x - bounds.x - sliderWidth/2)/(bounds.width-sliderWidth)) + minValue;
                }
            }
            else state = STATE_FOCUSED;
        }

        if (*value > maxValue) *value = maxValue;
        else if (*value < minValue) *value = minValue;
    }

    // Control value change check
    if (oldValue == *value) result = 0;
    else result = 1;

    // Slider bar limits check
    float sliderValue = (((*value - minValue)/(maxValue - minValue))*(bounds.width - sliderWidth - 2*GuiGetStyle(SLIDER, BORDER_WIDTH)));
    if (sliderWidth > 0)        // Slider
    {
        slider.x += sliderValue;
        slider.width = (float)sliderWidth;
        if (slider.x <= (bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH))) slider.x = bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH);
        else if ((slider.x + slider.width) >= (bounds.x + bounds.width)) slider.x = bounds.x + bounds.width - slider.width - GuiGetStyle(SLIDER, BORDER_WIDTH);
    }
    else if (sliderWidth == 0)  // SliderBar
    {
        slider.x += GuiGetStyle(SLIDER, BORDER_WIDTH);
        slider.width = sliderValue;
        if (slider.width > bounds.width) slider.width = bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH);
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(SLIDER, BORDER_WIDTH), GetColor(GuiGetStyle(SLIDER, BORDER + (state*3))), GetColor(GuiGetStyle(SLIDER, (state != STATE_DISABLED)?  BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)));

    // Draw slider internal bar (depends on state)
    if (state == STATE_NORMAL) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)));
    else if (state == STATE_FOCUSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, TEXT_COLOR_FOCUSED)));
    else if (state == STATE_PRESSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, TEXT_COLOR_PRESSED)));

    // Draw left/right text if provided
    if (textLeft != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textLeft);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x - textBounds.width - GuiGetStyle(SLIDER, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textLeft, textBounds, TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(SLIDER, TEXT + (state*3))));
    }

    if (textRight != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textRight);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(SLIDER, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textRight, textBounds, TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(SLIDER, TEXT + (state*3))));
    }
    //--------------------------------------------------------------------

    return result;
}

// Slider control extended, returns selected value and has text
int GuiSlider(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue)
{
    return GuiSliderPro(bounds, textLeft, textRight, value, minValue, maxValue, GuiGetStyle(SLIDER, SLIDER_WIDTH));
}

// Slider Bar control extended, returns selected value
int GuiSliderBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue)
{
    return GuiSliderPro(bounds, textLeft, textRight, value, minValue, maxValue, 0);
}

// Progress Bar control extended, shows current progress value
int GuiProgressBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue)
{
    int result = 0;
    GuiState state = guiState;

    float temp = (maxValue - minValue)/2.0f;
    if (value == NULL) value = &temp;

    // Progress bar
    Rectangle progress = { bounds.x + GuiGetStyle(PROGRESSBAR, BORDER_WIDTH),
                           bounds.y + GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) + GuiGetStyle(PROGRESSBAR, PROGRESS_PADDING), 0,
                           bounds.height - 2*GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) - 2*GuiGetStyle(PROGRESSBAR, PROGRESS_PADDING) };

    // Update control
    //--------------------------------------------------------------------
    if (*value > maxValue) *value = maxValue;

    // WARNING: Working with floats could lead to rounding issues
    if ((state != STATE_DISABLED)) progress.width = (float)(*value/(maxValue - minValue))*bounds.width - ((*value >= maxValue)? (float)(2*GuiGetStyle(PROGRESSBAR, BORDER_WIDTH)) : 0.0f);
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_DISABLED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(PROGRESSBAR, BORDER + (state*3))), BLANK);
    }
    else
    {
        if (*value > minValue)
        {
            // Draw progress bar with colored border, more visual
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y, (int)progress.width + (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height - 2 }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height - 1, (int)progress.width + (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
        }
        else GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));

        if (*value >= maxValue) GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + progress.width + 1, bounds.y, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
        else
        {
            // Draw borders not yet reached by value
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + (int)progress.width + 1, bounds.y, bounds.width - (int)progress.width - 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + (int)progress.width + 1, bounds.y + bounds.height - 1, bounds.width - (int)progress.width - 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - 1, bounds.y + 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height - 2 }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));
        }

        // Draw slider internal progress bar (depends on state)
        GuiDrawRectangle(progress, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BASE_COLOR_PRESSED)));
    }

    // Draw left/right text if provided
    if (textLeft != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textLeft);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x - textBounds.width - GuiGetStyle(PROGRESSBAR, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textLeft, textBounds, TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(PROGRESSBAR, TEXT + (state*3))));
    }

    if (textRight != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textRight);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(PROGRESSBAR, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textRight, textBounds, TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(PROGRESSBAR, TEXT + (state*3))));
    }
    //--------------------------------------------------------------------

    return result;
}

// Status Bar control
int GuiStatusBar(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER + (state*3))), GetColor(GuiGetStyle(STATUSBAR, BASE + (state*3))));
    GuiDrawText(text, GetTextBounds(STATUSBAR, bounds), GuiGetStyle(STATUSBAR, TEXT_ALIGNMENT), GetColor(GuiGetStyle(STATUSBAR, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Dummy rectangle control, intended for placeholding
int GuiDummyRec(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state != STATE_DISABLED)? BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)));
    GuiDrawText(text, GetTextBounds(DEFAULT, bounds), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(BUTTON, (state != STATE_DISABLED)? TEXT_COLOR_NORMAL : TEXT_COLOR_DISABLED)));
    //------------------------------------------------------------------

    return result;
}

// List View control
int GuiListView(Rectangle bounds, const char *text, int *scrollIndex, int *active)
{
    int result = 0;
    int itemCount = 0;
    const char **items = NULL;

    if (text != NULL) items = GuiTextSplit(text, ';', &itemCount, NULL);

    result = GuiListViewEx(bounds, items, itemCount, scrollIndex, active, NULL);

    return result;
}

// List View control with extended parameters
int GuiListViewEx(Rectangle bounds, const char **text, int count, int *scrollIndex, int *active, int *focus)
{
    int result = 0;
    GuiState state = guiState;

    int itemFocused = (focus == NULL)? -1 : *focus;
    int itemSelected = (active == NULL)? -1 : *active;

    // Check if we need a scroll bar
    bool useScrollBar = false;
    if ((GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING))*count > bounds.height) useScrollBar = true;

    // Define base item rectangle [0]
    Rectangle itemBounds = { 0 };
    itemBounds.x = bounds.x + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING);
    itemBounds.y = bounds.y + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING) + GuiGetStyle(DEFAULT, BORDER_WIDTH);
    itemBounds.width = bounds.width - 2*GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING) - GuiGetStyle(DEFAULT, BORDER_WIDTH);
    itemBounds.height = (float)GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT);
    if (useScrollBar) itemBounds.width -= GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);

    // Get items on the list
    int visibleItems = (int)bounds.height/(GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING));
    if (visibleItems > count) visibleItems = count;

    int startIndex = (scrollIndex == NULL)? 0 : *scrollIndex;
    if ((startIndex < 0) || (startIndex > (count - visibleItems))) startIndex = 0;
    int endIndex = startIndex + visibleItems;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check mouse inside list view
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            state = STATE_FOCUSED;

            // Check focused and selected item
            for (int i = 0; i < visibleItems; i++)
            {
                if (CheckCollisionPointRec(mousePoint, itemBounds))
                {
                    itemFocused = startIndex + i;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        if (itemSelected == (startIndex + i)) itemSelected = -1;
                        else itemSelected = startIndex + i;
                    }
                    break;
                }

                // Update item rectangle y position for next item
                itemBounds.y += (GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING));
            }

            if (useScrollBar)
            {
                int wheelMove = (int)GetMouseWheelMove();
                startIndex -= wheelMove;

                if (startIndex < 0) startIndex = 0;
                else if (startIndex > (count - visibleItems)) startIndex = count - visibleItems;

                endIndex = startIndex + visibleItems;
                if (endIndex > count) endIndex = count;
            }
        }
        else itemFocused = -1;

        // Reset item rectangle y to [0]
        itemBounds.y = bounds.y + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING) + GuiGetStyle(DEFAULT, BORDER_WIDTH);
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(DEFAULT, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER + state*3)), GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));     // Draw background

    // Draw visible items
    for (int i = 0; ((i < visibleItems) && (text != NULL)); i++)
    {
        GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_NORMAL)), BLANK);

        if (state == STATE_DISABLED)
        {
            if ((startIndex + i) == itemSelected) GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_DISABLED)), GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_DISABLED)));

            GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_DISABLED)));
        }
        else
        {
            if (((startIndex + i) == itemSelected) && (active != NULL))
            {
                // Draw item selected
                GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_PRESSED)), GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_PRESSED)));
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_PRESSED)));
            }
            else if (((startIndex + i) == itemFocused)) // && (focus != NULL))  // NOTE: We want items focused, despite not returned!
            {
                // Draw item focused
                GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_FOCUSED)), GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_FOCUSED)));
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_FOCUSED)));
            }
            else
            {
                // Draw item normal
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_NORMAL)));
            }
        }

        // Update item rectangle y position for next item
        itemBounds.y += (GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING));
    }

    if (useScrollBar)
    {
        Rectangle scrollBarBounds = {
            bounds.x + bounds.width - GuiGetStyle(LISTVIEW, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH),
            bounds.y + GuiGetStyle(LISTVIEW, BORDER_WIDTH), (float)GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH),
            bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH)
        };

        // Calculate percentage of visible items and apply same percentage to scrollbar
        float percentVisible = (float)(endIndex - startIndex)/count;
        float sliderSize = bounds.height*percentVisible;

        int prevSliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);   // Save default slider size
        int prevScrollSpeed = GuiGetStyle(SCROLLBAR, SCROLL_SPEED); // Save default scroll speed
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)sliderSize);            // Change slider size
        GuiSetStyle(SCROLLBAR, SCROLL_SPEED, count - visibleItems); // Change scroll speed

        startIndex = GuiScrollBar(scrollBarBounds, startIndex, 0, count - visibleItems);

        GuiSetStyle(SCROLLBAR, SCROLL_SPEED, prevScrollSpeed); // Reset scroll speed to default
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, prevSliderSize); // Reset slider size to default
    }
    //--------------------------------------------------------------------

    if (active != NULL) *active = itemSelected;
    if (focus != NULL) *focus = itemFocused;
    if (scrollIndex != NULL) *scrollIndex = startIndex;

    return result;
}

// Color Panel control - Color (RGBA) variant.
int GuiColorPanel(Rectangle bounds, const char *text, Color *color)
{
    int result = 0;

    Vector3 vcolor = { (float)color->r/255.0f, (float)color->g/255.0f, (float)color->b/255.0f };
    Vector3 hsv = ConvertRGBtoHSV(vcolor);
    Vector3 prevHsv = hsv; // workaround to see if GuiColorPanelHSV modifies the hsv.

    GuiColorPanelHSV(bounds, text, &hsv);

    // Check if the hsv was changed, only then change the color.
    // This is required, because the Color->HSV->Color conversion has precision errors.
    // Thus the assignment from HSV to Color should only be made, if the HSV has a new user-entered value.
    // Otherwise GuiColorPanel would often modify it's color without user input.
    // TODO: GuiColorPanelHSV could return 1 if the slider was dragged, to simplify this check.
    if (hsv.x != prevHsv.x || hsv.y != prevHsv.y || hsv.z != prevHsv.z)
    {
        Vector3 rgb = ConvertHSVtoRGB(hsv);

        // NOTE: Vector3ToColor() only available on raylib 1.8.1
        *color = RAYGUI_CLITERAL(Color){ (unsigned char)(255.0f*rgb.x),
                            (unsigned char)(255.0f*rgb.y),
                            (unsigned char)(255.0f*rgb.z),
                            color->a };
    }
    return result;
}

//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleCherry();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define CHERRY_STYLE_PROPS_COUNT  17

// Custom style name: Cherry
RAYGUIAPI const GuiStyleProp cherryStyleProps[CHERRY_STYLE_PROPS_COUNT] = {
    { 0, 0, 0xda5757ff },    // DEFAULT_BORDER_COLOR_NORMAL
    { 0, 1, 0x753233ff },    // DEFAULT_BASE_COLOR_NORMAL
    { 0, 2, 0xe17373ff },    // DEFAULT_TEXT_COLOR_NORMAL
    { 0, 3, 0xfaaa97ff },    // DEFAULT_BORDER_COLOR_FOCUSED
    { 0, 4, 0xe06262ff },    // DEFAULT_BASE_COLOR_FOCUSED
    { 0, 5, 0xfdb4aaff },    // DEFAULT_TEXT_COLOR_FOCUSED
    { 0, 6, 0xe03c46ff },    // DEFAULT_BORDER_COLOR_PRESSED
    { 0, 7, 0x5b1e20ff },    // DEFAULT_BASE_COLOR_PRESSED
    { 0, 8, 0xc2474fff },    // DEFAULT_TEXT_COLOR_PRESSED
    { 0, 9, 0xa19292ff },    // DEFAULT_BORDER_COLOR_DISABLED
    { 0, 10, 0x706060ff },    // DEFAULT_BASE_COLOR_DISABLED
    { 0, 11, 0x9e8585ff },    // DEFAULT_TEXT_COLOR_DISABLED
    { 0, 16, 0x0000000f },    // DEFAULT_TEXT_SIZE
    { 0, 17, 0x00000000 },    // DEFAULT_TEXT_SPACING
    { 0, 18, 0xfb8170ff },    // DEFAULT_LINE_COLOR
    { 0, 19, 0x3a1720ff },    // DEFAULT_BACKGROUND_COLOR
    { 0, 20, 0x00000016 },    // DEFAULT_TEXT_LINE_SPACING
};

// WARNING: This style uses a custom font: "Westington.ttf" (size: 15, spacing: 0)

#define CHERRY_STYLE_FONT_ATLAS_COMP_SIZE 2821

// Font atlas image pixels data: DEFLATE compressed
RAYGUIAPI unsigned char cherryFontData[CHERRY_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0x59, 0x92, 0xdc, 0xba, 0x11, 0x05, 0x50, 0xee, 0x7f, 0xd3, 0xd7, 0xe1, 0x70, 0x84, 0xed, 0x27, 0xa9, 0x09, 0x20,
    0x91, 0x20, 0xab, 0x5b, 0x47, 0xe7, 0xaf, 0xa1, 0x1a, 0x38, 0x24, 0x26, 0x16, 0x12, 0xb9, 0x00, 0x00, 0x00, 0x00, 0xf2,
    0xef, 0x7f, 0x5f, 0xfd, 0xf5, 0xcf, 0x65, 0xff, 0xff, 0xaa, 0xdc, 0xbe, 0x73, 0xfd, 0xb3, 0xb3, 0xf8, 0xae, 0x59, 0xf8,
    0x7f, 0xf9, 0xf2, 0xbb, 0x64, 0xfa, 0x33, 0xd7, 0x8f, 0x7b, 0xfd, 0x58, 0xef, 0xce, 0xfd, 0xfa, 0x31, 0x5c, 0x37, 0x9f,
    0xbf, 0xf6, 0xdd, 0x2a, 0xaf, 0xb9, 0x2f, 0x59, 0xbf, 0x7b, 0x3a, 0xae, 0xc8, 0xdc, 0x35, 0xb8, 0x3f, 0xa7, 0xd7, 0xf0,
    0x95, 0xeb, 0xef, 0x9c, 0xe1, 0x59, 0x1c, 0x7f, 0xab, 0xb5, 0x1a, 0xe0, 0xeb, 0xbf, 0x8e, 0xee, 0xa6, 0x14, 0xee, 0xab,
    0xff, 0xbd, 0x22, 0x8b, 0x65, 0xa3, 0xd7, 0xec, 0xdd, 0x57, 0x59, 0xa8, 0xa5, 0x2a, 0x57, 0x21, 0x8b, 0xe7, 0x61, 0xf5,
    0xdc, 0xa5, 0x54, 0xff, 0x8c, 0xbe, 0x6d, 0x5a, 0x8e, 0xe7, 0xae, 0x24, 0xd3, 0x7f, 0x1d, 0xbf, 0xa2, 0xf2, 0x6e, 0x33,
    0xf7, 0xd5, 0xdd, 0xab, 0xfb, 0xdf, 0x39, 0x53, 0xf7, 0xef, 0xd9, 0xf8, 0x1f, 0xd5, 0x97, 0xf7, 0xe7, 0xfb, 0xee, 0x28,
    0xf2, 0xdf, 0xde, 0x45, 0xed, 0xbe, 0xdb, 0x89, 0xf4, 0xdc, 0xdc, 0x9d, 0x29, 0x9e, 0x8d, 0xfa, 0xff, 0xbb, 0x8f, 0x97,
    0x14, 0x7b, 0x5e, 0x59, 0xb8, 0x4f, 0xb3, 0x54, 0xeb, 0xcd, 0xf4, 0x26, 0x2a, 0x35, 0xdd, 0x6e, 0x2f, 0x6a, 0xbf, 0xa7,
    0xfb, 0xf5, 0x35, 0xb8, 0x3b, 0xdf, 0x99, 0x3c, 0x82, 0x94, 0xe2, 0xff, 0x5a, 0xac, 0xb1, 0x9f, 0x8a, 0xff, 0x71, 0xac,
    0x8c, 0xe2, 0x29, 0xc3, 0x2b, 0x9c, 0x96, 0xef, 0x9f, 0x52, 0x8d, 0x76, 0x6a, 0x9c, 0x55, 0xbf, 0x7f, 0x66, 0xcf, 0x5e,
    0x06, 0xa3, 0xb9, 0xee, 0x5e, 0xe8, 0x7c, 0xaf, 0x2f, 0xc5, 0xda, 0x75, 0xa5, 0x8f, 0x9f, 0xad, 0xde, 0xfa, 0x28, 0x7e,
    0xb3, 0x5c, 0x17, 0x67, 0xa2, 0x3f, 0x74, 0xa2, 0x4f, 0xf2, 0x09, 0xf1, 0x9f, 0xc2, 0x08, 0xae, 0x5a, 0xc7, 0xa7, 0xa9,
    0x6d, 0xce, 0xa1, 0xfa, 0xb5, 0x3b, 0xfe, 0xd7, 0xcb, 0xc6, 0x91, 0x91, 0xd6, 0x71, 0x58, 0x4a, 0x23, 0xe9, 0x7a, 0x1b,
    0xbe, 0xfb, 0xda, 0xbb, 0xb9, 0xa2, 0x4c, 0xd4, 0x9e, 0x59, 0x9e, 0x01, 0xd8, 0x89, 0xff, 0xfe, 0xf1, 0x7f, 0x4a, 0xaf,
    0x98, 0xab, 0x8d, 0x6b, 0x35, 0x71, 0x16, 0xbe, 0x59, 0x9a, 0x6a, 0xb6, 0x1c, 0x9d, 0x65, 0xbd, 0x1a, 0xe3, 0xbf, 0x36,
    0x72, 0x4a, 0xdb, 0xdc, 0x44, 0x7d, 0xec, 0x96, 0xa6, 0xfb, 0x35, 0xad, 0xaf, 0xb8, 0x8f, 0xff, 0xbb, 0xf6, 0x7f, 0xbf,
    0xff, 0xff, 0x7e, 0xfb, 0xdf, 0x3d, 0xfa, 0x9f, 0x9f, 0xc5, 0x18, 0x9f, 0xd3, 0xe7, 0xe2, 0xff, 0x64, 0xdf, 0xbf, 0x36,
    0x96, 0x4f, 0x71, 0x9e, 0x32, 0x4d, 0x23, 0xf6, 0x94, 0x47, 0xe7, 0x69, 0x99, 0xd5, 0xe9, 0xab, 0xab, 0x53, 0xfe, 0x8c,
    0xfa, 0x3c, 0xf6, 0x4f, 0x1d, 0xff, 0x67, 0xe9, 0xd9, 0xc0, 0xb8, 0x6f, 0x75, 0x77, 0x66, 0xd2, 0x3e, 0xff, 0xf7, 0xde,
    0x53, 0xd6, 0xb5, 0x5a, 0x27, 0x85, 0x31, 0x76, 0x65, 0xd6, 0xbc, 0xfa, 0x34, 0xb1, 0xf3, 0xd9, 0xe4, 0xfb, 0xd7, 0xa9,
    0x3a, 0xd7, 0x98, 0x72, 0xed, 0x9e, 0x89, 0x67, 0x3f, 0xeb, 0xb3, 0x0a, 0x4f, 0xf5, 0xa6, 0x66, 0xe6, 0x62, 0xae, 0xe2,
    0xbc, 0x69, 0x0a, 0x3d, 0x9c, 0x4f, 0x8f, 0xff, 0x4f, 0xaa, 0x89, 0x3e, 0xf1, 0xf7, 0x27, 0x69, 0x1e, 0xe9, 0x77, 0x45,
    0x40, 0x26, 0x46, 0xe2, 0x57, 0xa1, 0x9e, 0xfe, 0x0e, 0xcf, 0xff, 0xc7, 0xf1, 0x5f, 0x1d, 0xff, 0x67, 0xea, 0xf7, 0x18,
    0x6b, 0xcf, 0x01, 0xf2, 0xe1, 0xad, 0xbf, 0x1a, 0xa0, 0x67, 0x4e, 0xfe, 0xe9, 0xfa, 0x39, 0x87, 0xc7, 0x85, 0x7f, 0xe3,
    0x75, 0x05, 0xf7, 0x0f, 0x20, 0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x5e, 0x37,
    0xb8, 0xbe, 0x16, 0xb1, 0x3b, 0x4b, 0x6b, 0xe5, 0xfd, 0x7e, 0x2f, 0x1d, 0xbd, 0x47, 0xf5, 0x68, 0x2b, 0xb9, 0xa0, 0x33,
    0xf5, 0xdd, 0xb3, 0xbc, 0xa2, 0x73, 0x74, 0x34, 0xf9, 0xc7, 0x1a, 0xd5, 0x99, 0x73, 0xba, 0x73, 0x5d, 0xc6, 0x99, 0x77,
    0x57, 0xf2, 0xfb, 0xae, 0xad, 0x06, 0x5e, 0x5d, 0x21, 0xd0, 0x99, 0xed, 0x60, 0xf6, 0xfa, 0x56, 0xee, 0xa8, 0x95, 0x4f,
    0xdb, 0x59, 0x8b, 0x78, 0x9f, 0x81, 0xf9, 0x1a, 0x66, 0x88, 0xee, 0xca, 0x51, 0x97, 0x8d, 0x35, 0xfb, 0xbf, 0xdf, 0xc9,
    0x95, 0x55, 0xdb, 0xe3, 0x7b, 0xaa, 0x92, 0x77, 0xa9, 0x9a, 0xb3, 0x39, 0x2d, 0x9f, 0x3b, 0xf3, 0xcd, 0xe6, 0x32, 0xfa,
    0xd4, 0x33, 0xb6, 0x5f, 0x8b, 0xfb, 0x30, 0xac, 0xad, 0xe5, 0xce, 0xf2, 0x5a, 0xf8, 0xb4, 0xe5, 0x8b, 0x48, 0xf1, 0x3d,
    0x6b, 0xf7, 0xda, 0x99, 0xec, 0x3f, 0xd5, 0x2c, 0x59, 0x67, 0xe2, 0x3f, 0xe5, 0xef, 0x92, 0xdb, 0x7b, 0xe2, 0x1a, 0x66,
    0x28, 0xaa, 0x66, 0x5e, 0xca, 0x46, 0xbd, 0x96, 0x8d, 0xb5, 0xee, 0x5d, 0xf1, 0xbf, 0x13, 0xe1, 0xb3, 0xf9, 0x7b, 0xd3,
    0xb2, 0xba, 0x7f, 0x25, 0x1b, 0x7a, 0xb5, 0x1f, 0xdc, 0x19, 0x21, 0x77, 0xc7, 0x9e, 0x8d, 0x33, 0x73, 0x26, 0xfe, 0xf3,
    0x01, 0xf1, 0xbf, 0x53, 0x17, 0x7d, 0xcf, 0xf8, 0x4f, 0x43, 0xed, 0x9c, 0xd2, 0x15, 0xfa, 0x3d, 0x76, 0x2b, 0x7b, 0x59,
    0x5c, 0xe5, 0x8c, 0xce, 0x69, 0x6e, 0x8f, 0x3a, 0xd6, 0x14, 0x3f, 0x19, 0xff, 0xeb, 0xf5, 0xcd, 0xa9, 0xf8, 0xbf, 0xdb,
    0xad, 0x23, 0xa5, 0xdd, 0x9e, 0xfa, 0xf6, 0xc1, 0x5a, 0xa9, 0x8b, 0x32, 0x9d, 0x5b, 0xb4, 0x12, 0xff, 0x19, 0xee, 0xa0,
    0x51, 0xd9, 0xb7, 0xa0, 0x9a, 0x0d, 0xb9, 0x23, 0xfe, 0xe7, 0xf3, 0x9b, 0x67, 0xb2, 0xee, 0xed, 0x88, 0xf3, 0x77, 0xe2,
    0x7f, 0x74, 0x05, 0x53, 0xcc, 0xd6, 0xb7, 0x9e, 0xe5, 0x7b, 0xfc, 0x69, 0xbd, 0x59, 0xd4, 0x67, 0xf6, 0x9a, 0xc9, 0x37,
    0x68, 0xff, 0xcf, 0xc7, 0x7f, 0xe5, 0x5d, 0x67, 0xe7, 0x66, 0xf2, 0x5a, 0xfc, 0x67, 0xbb, 0x85, 0x4f, 0xf3, 0xe8, 0xf2,
    0x8d, 0xbc, 0x5f, 0x33, 0xf3, 0x05, 0xb5, 0x18, 0x4f, 0xeb, 0x78, 0xa3, 0xf6, 0x69, 0x27, 0x7a, 0x98, 0xeb, 0xf1, 0x9f,
    0xa3, 0xe3, 0xff, 0x95, 0x4f, 0xa8, 0xf7, 0xff, 0xef, 0x3e, 0xbb, 0x77, 0xfe, 0xaf, 0x6b, 0xee, 0x69, 0xa6, 0xed, 0x4e,
    0x43, 0x2f, 0xe2, 0x7c, 0xcd, 0x90, 0xd7, 0x9e, 0x91, 0x55, 0x3e, 0xff, 0x44, 0x44, 0x56, 0x63, 0xab, 0x1a, 0xff, 0xbd,
    0xf3, 0xff, 0x1d, 0xf3, 0xcc, 0x73, 0x47, 0x9f, 0x42, 0xa4, 0xa6, 0x38, 0xbe, 0xca, 0xd6, 0x0c, 0x79, 0x36, 0xe2, 0x3f,
    0x0d, 0xf1, 0x7f, 0xf7, 0xfc, 0x6e, 0x37, 0xfe, 0x33, 0x78, 0xbe, 0x91, 0x6f, 0x13, 0xff, 0x29, 0xc5, 0x7f, 0x9a, 0x23,
    0x32, 0xe5, 0xd8, 0xba, 0x96, 0x9f, 0x4f, 0x9e, 0x79, 0xfe, 0x3f, 0x7e, 0xca, 0xba, 0xf3, 0x24, 0x79, 0xf6, 0x3b, 0xd5,
    0x7f, 0xed, 0x30, 0x3a, 0x4b, 0x59, 0x8e, 0xb0, 0x94, 0x72, 0xf3, 0xcf, 0x8f, 0xf9, 0xb2, 0xd9, 0x4a, 0x8d, 0x6b, 0x88,
    0x6a, 0x4e, 0xea, 0xd5, 0x7d, 0xd5, 0xf3, 0xfa, 0xbe, 0x36, 0xcf, 0xbc, 0x72, 0x67, 0xaf, 0x06, 0x39, 0x89, 0xf9, 0x19,
    0xbf, 0x49, 0x73, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0xd6, 0x60,
    0xa6, 0x94, 0xd5, 0x6b, 0xf6, 0x7f, 0x55, 0x73, 0x90, 0xe5, 0x36, 0xcb, 0xf4, 0xda, 0x5a, 0xdf, 0x0c, 0x8f, 0xb1, 0x63,
    0xa5, 0xf4, 0xd9, 0x2c, 0xdc, 0xf3, 0xe7, 0xb6, 0x76, 0xf5, 0xb3, 0x98, 0xcf, 0x32, 0xed, 0x2b, 0xd3, 0xc7, 0xeb, 0xad,
    0xef, 0xd7, 0xd0, 0xd7, 0x5f, 0x77, 0x15, 0x56, 0x38, 0xaf, 0x5e, 0xad, 0x99, 0x5c, 0xf1, 0x6f, 0xd4, 0x15, 0xd7, 0x44,
    0x86, 0xbb, 0x9d, 0x1c, 0xfa, 0x77, 0xe7, 0xfe, 0xeb, 0xfb, 0xf5, 0xee, 0x7b, 0xf5, 0xe5, 0x2d, 0xbc, 0xb6, 0x33, 0xe8,
    0x5e, 0x8f, 0x64, 0xe1, 0xce, 0xe2, 0x7d, 0x38, 0xfb, 0x3f, 0x47, 0x59, 0x04, 0x53, 0xc8, 0xf4, 0x71, 0xee, 0x78, 0x67,
    0xf2, 0x3f, 0x8d, 0xf2, 0x3c, 0xfe, 0xf9, 0xef, 0x2b, 0xf7, 0x53, 0x2d, 0x67, 0x72, 0x06, 0xb9, 0x24, 0x7b, 0xd7, 0x5a,
    0xe7, 0xf6, 0x5e, 0x5d, 0xef, 0x47, 0x9c, 0xb9, 0xa2, 0xf5, 0xf8, 0x5f, 0x6f, 0x77, 0x4e, 0xc5, 0xff, 0x7e, 0x16, 0xde,
    0x9d, 0x1c, 0x3d, 0xfb, 0x7f, 0xbf, 0x6f, 0xfd, 0xaf, 0xe5, 0xde, 0xd9, 0xb5, 0x95, 0xeb, 0x76, 0x9c, 0x09, 0x67, 0x9c,
    0x61, 0x6a, 0xf5, 0x38, 0xae, 0xc5, 0xfc, 0xce, 0x9f, 0x13, 0xff, 0x57, 0x29, 0xfe, 0xaf, 0x0f, 0x8a, 0xff, 0xbb, 0x76,
    0xe7, 0x2a, 0xee, 0x27, 0x72, 0xd7, 0x33, 0xbb, 0x1a, 0xdb, 0x9c, 0xdf, 0x6b, 0xd5, 0x94, 0xfa, 0xc3, 0x29, 0x67, 0x4f,
    0x7c, 0x2b, 0x07, 0xd7, 0x78, 0x77, 0x93, 0x71, 0xdf, 0xbf, 0x56, 0x53, 0xd6, 0x32, 0x31, 0xdd, 0x8f, 0x28, 0xb2, 0xb4,
    0x3b, 0x41, 0x3d, 0x2f, 0x58, 0x0e, 0x64, 0x47, 0x59, 0xad, 0xe9, 0x53, 0xe8, 0xff, 0x5f, 0x5b, 0xfb, 0x48, 0x54, 0xf3,
    0x1e, 0xd7, 0x72, 0xac, 0xa7, 0xd0, 0xee, 0xcc, 0xc4, 0xdf, 0x5e, 0x16, 0xde, 0x4c, 0x8e, 0xb2, 0xae, 0xc5, 0x3a, 0xf2,
    0x6a, 0xab, 0x31, 0xd6, 0x7a, 0x98, 0xd9, 0x1a, 0x6d, 0xcd, 0xd4, 0x0d, 0x59, 0xdc, 0xab, 0xa1, 0xde, 0x5e, 0x54, 0xf2,
    0xc9, 0xd5, 0x6b, 0x80, 0xbc, 0x36, 0xff, 0x77, 0x15, 0x32, 0xd5, 0xaf, 0xe5, 0xe6, 0xab, 0xcc, 0xbd, 0x55, 0x8e, 0xaf,
    0x96, 0x73, 0x6f, 0x26, 0x6b, 0x7b, 0x4f, 0xfe, 0xe2, 0xd5, 0xf3, 0x73, 0x4d, 0xcd, 0x6a, 0xe5, 0xa5, 0xf6, 0x7f, 0x3d,
    0x02, 0xe6, 0x76, 0xa5, 0xdc, 0xe9, 0xd5, 0x54, 0x33, 0x76, 0xf7, 0xf7, 0x7e, 0xde, 0x9f, 0xff, 0x5f, 0xcb, 0xe0, 0xfd,
    0x59, 0xd9, 0x59, 0xd3, 0xf0, 0x3f, 0xfa, 0x3e, 0xeb, 0xed, 0x67, 0x38, 0x39, 0x5a, 0xfb, 0xf7, 0xc6, 0x7f, 0x8a, 0xe3,
    0xc9, 0xd9, 0x7a, 0x38, 0x4b, 0x39, 0x87, 0xc7, 0xf3, 0x5b, 0xe7, 0xc6, 0x3f, 0x9f, 0x78, 0x3f, 0xed, 0xe4, 0xb2, 0x7f,
    0xf6, 0x7b, 0xe6, 0xb1, 0x1e, 0xd3, 0x89, 0xd9, 0x97, 0xe7, 0xce, 0xc4, 0xda, 0x31, 0xa5, 0xf1, 0x8e, 0xad, 0xcd, 0xff,
    0x65, 0x63, 0x8e, 0x6f, 0x77, 0xbf, 0xde, 0x14, 0x66, 0x15, 0xff, 0x86, 0x5f, 0x0a, 0x7c, 0x3d, 0x03, 0x91, 0xbf, 0x22,
    0xf7, 0xf0, 0x4f, 0x3c, 0xc6, 0x8e, 0xe7, 0xff, 0xd5, 0x7e, 0x53, 0x6d, 0xe7, 0x80, 0xb9, 0x79, 0x9f, 0xca, 0xfc, 0x6e,
    0x75, 0xcc, 0x08, 0x54, 0x9e, 0x31, 0x57, 0x76, 0x0e, 0xe8, 0x18, 0xc1, 0x65, 0xf9, 0xf9, 0x3f, 0xf0, 0x53, 0x6a, 0xa2,
    0xda, 0xbc, 0x22, 0xf0, 0xfd, 0x47, 0x3b, 0x22, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
    0x9e, 0xbf, 0x88, 0x5e, 0x5f, 0xeb, 0xba, 0x9e, 0xeb, 0xb0, 0x9e, 0x71, 0x65, 0x76, 0x7d, 0xea, 0x55, 0xca, 0x1a, 0xb9,
    0x97, 0x9b, 0x66, 0x2e, 0x5b, 0x73, 0x3d, 0x5b, 0x5d, 0x35, 0x43, 0xd6, 0x5e, 0xe9, 0xf8, 0x95, 0xf5, 0x57, 0x5f, 0xd3,
    0xe7, 0x2d, 0xaf, 0x1c, 0x77, 0x16, 0x73, 0x18, 0xac, 0xdc, 0x53, 0xfd, 0xc7, 0xba, 0x77, 0x2e, 0xae, 0x62, 0x46, 0x9f,
    0x4a, 0xb6, 0xe3, 0x4a, 0xbc, 0xde, 0xe5, 0x3b, 0xcd, 0xe2, 0x67, 0x65, 0x39, 0xa7, 0xf3, 0xa8, 0x16, 0xc9, 0x60, 0xc5,
    0x68, 0x26, 0xd6, 0x9c, 0xee, 0xe5, 0x1b, 0xa8, 0x96, 0x8d, 0xaf, 0x7a, 0x3d, 0xcf, 0x49, 0x16, 0xeb, 0x8f, 0xb5, 0x1c,
    0x7d, 0x27, 0x8f, 0x7b, 0x7d, 0x0d, 0x73, 0x96, 0x73, 0x64, 0x5d, 0x8b, 0xad, 0x4d, 0x26, 0xda, 0x87, 0xea, 0xb9, 0x98,
    0x6d, 0x01, 0x57, 0xdb, 0xcd, 0xd5, 0x9a, 0xa4, 0x1a, 0xff, 0xd5, 0xcf, 0x3a, 0x13, 0xff, 0x77, 0xc7, 0x96, 0xdb, 0x9c,
    0x13, 0xd9, 0xcc, 0x6f, 0xf8, 0x6c, 0xaf, 0xb0, 0xa7, 0xfd, 0x4d, 0x71, 0x8d, 0xfe, 0xf9, 0x75, 0x81, 0xa7, 0x56, 0x07,
    0xd6, 0x73, 0x47, 0xbf, 0x71, 0x0f, 0xa4, 0x2d, 0x6b, 0xe4, 0x5c, 0x3e, 0xd6, 0xf5, 0xf8, 0x5f, 0x1d, 0x2f, 0xac, 0xe6,
    0x33, 0xcc, 0x44, 0xeb, 0x34, 0xdb, 0xfe, 0x67, 0xeb, 0xea, 0x3f, 0xdf, 0xff, 0xbf, 0x36, 0xda, 0xd0, 0xb9, 0xd1, 0xc1,
    0x6c, 0x1e, 0x8e, 0xd5, 0xbb, 0xa9, 0x5e, 0x3a, 0xce, 0x0d, 0xbc, 0xf7, 0xd7, 0xf1, 0xfd, 0x77, 0xf7, 0x4e, 0x95, 0x91,
    0x52, 0xb6, 0x33, 0x36, 0x65, 0xf9, 0x6c, 0xdc, 0x1f, 0xf5, 0x7a, 0xce, 0xe5, 0xd5, 0xf8, 0x1f, 0xd5, 0x26, 0x6b, 0x23,
    0xb8, 0x71, 0xfb, 0x3f, 0x93, 0x6b, 0x72, 0x6f, 0xb7, 0x8a, 0x1c, 0x89, 0xc3, 0x7a, 0xe9, 0xce, 0x4e, 0x27, 0x7f, 0xaa,
    0x13, 0x53, 0xae, 0x99, 0x72, 0xac, 0xf4, 0xc4, 0xf8, 0x7f, 0x9c, 0x71, 0x24, 0x85, 0x7c, 0x25, 0x3b, 0xa3, 0xa8, 0xdd,
    0xf6, 0xa5, 0xe3, 0xdd, 0xb2, 0xb5, 0x97, 0xc3, 0xb9, 0x6f, 0xdd, 0x31, 0xfe, 0x3f, 0x3b, 0xff, 0xb7, 0xbf, 0xe3, 0x58,
    0xcf, 0x6e, 0x45, 0xab, 0xb5, 0xd9, 0xaf, 0xe7, 0x22, 0x1f, 0xd2, 0xab, 0xa9, 0xd5, 0x0e, 0xfb, 0x7f, 0x1d, 0xcf, 0x59,
    0xe7, 0xa5, 0xf8, 0xef, 0x1d, 0x09, 0xe5, 0xc0, 0xfc, 0xe4, 0xf3, 0xd1, 0xbf, 0x32, 0xfe, 0xef, 0x98, 0x31, 0xab, 0xee,
    0x38, 0x72, 0xb6, 0xb4, 0xb2, 0xe3, 0xea, 0xfc, 0x3d, 0xf9, 0x44, 0x0b, 0xdf, 0xb5, 0x4f, 0xc2, 0x4f, 0x8d, 0xff, 0x37,
    0xf7, 0x18, 0xd9, 0x89, 0xff, 0xf3, 0x59, 0x9a, 0xf3, 0x87, 0x79, 0xbd, 0xdd, 0x4f, 0x1c, 0xed, 0x11, 0x9b, 0xf6, 0x38,
    0xdc, 0x8f, 0xe1, 0x9d, 0xd2, 0xb9, 0x7d, 0x78, 0x6a, 0x51, 0xba, 0x53, 0xfa, 0x56, 0xfb, 0x3f, 0x33, 0x0b, 0xf5, 0x74,
    0xfc, 0x77, 0xce, 0x86, 0xe6, 0x50, 0xeb, 0xb8, 0x3e, 0x9a, 0xe8, 0x9f, 0x1b, 0xed, 0xf8, 0xc4, 0x6a, 0xcb, 0xf0, 0x4e,
    0xff, 0xbf, 0xfe, 0xfb, 0x8b, 0x7f, 0xce, 0x66, 0xa5, 0x38, 0xab, 0xf5, 0x93, 0xc6, 0xff, 0xd7, 0xd4, 0xae, 0x51, 0x79,
    0x69, 0xfc, 0xff, 0x19, 0xaf, 0xff, 0x7b, 0x7f, 0x81, 0x75, 0x1d, 0x6e, 0x87, 0x9f, 0x2e, 0x3d, 0xdf, 0x63, 0xdc, 0x2b,
    0xcd, 0x2b, 0xed, 0xff, 0xdc, 0x48, 0xf4, 0xbd, 0xf8, 0xef, 0xea, 0x2f, 0xab, 0x09, 0x3a, 0xe7, 0x59, 0xde, 0x6d, 0xc3,
    0x6b, 0xbb, 0x38, 0xee, 0xfd, 0xda, 0x66, 0x76, 0x74, 0x50, 0x2b, 0xcd, 0xe1, 0xf1, 0xff, 0xee, 0x55, 0xae, 0x1c, 0xcf,
    0xa7, 0xc4, 0xbf, 0xbc, 0xea, 0xa7, 0xea, 0x8c, 0x4a, 0x1c, 0x3e, 0x15, 0xc3, 0xcf, 0xdf, 0x4d, 0x7b, 0xcf, 0xff, 0xd7,
    0xeb, 0xe8, 0xb5, 0xe7, 0xff, 0x3b, 0x2d, 0xc3, 0xd3, 0xcf, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0xef, 0xb7, 0xfe, 0x6c, 0x9c, 0x99, 0xa6, 0x9e, 0xd5, 0x7b, 0x37, 0x2b, 0xc0, 0xde, 0x11, 0xa5, 0xb4, 0xf6,
    0x6c, 0x36, 0xb7, 0x75, 0xf7, 0xfa, 0xb4, 0x7a, 0x7e, 0xf0, 0x6c, 0xac, 0x3a, 0xac, 0xff, 0xba, 0xfc, 0xd4, 0x71, 0x9e,
    0xca, 0xc2, 0x55, 0xb9, 0x1f, 0x9e, 0xbb, 0x96, 0x69, 0xbf, 0xc2, 0xb3, 0xef, 0x51, 0xcd, 0x05, 0x9d, 0xa5, 0x1c, 0x10,
    0xab, 0x71, 0x96, 0xcd, 0x0c, 0xf5, 0xf5, 0xb5, 0x67, 0xb3, 0xb5, 0x4e, 0x6f, 0xe6, 0xea, 0x6c, 0xe4, 0x87, 0xae, 0x9f,
    0xa3, 0x9d, 0xd5, 0xa5, 0xdd, 0xc7, 0xb9, 0x7e, 0xc5, 0x3a, 0x72, 0x73, 0x67, 0xe3, 0xcc, 0x67, 0xf9, 0xcc, 0x64, 0x50,
    0x3b, 0xe5, 0x91, 0x75, 0xa5, 0xbd, 0x99, 0x0b, 0x56, 0x57, 0x34, 0x3e, 0xb7, 0xd6, 0xb6, 0xbe, 0x0a, 0x6c, 0x37, 0xbf,
    0xd5, 0x73, 0xfd, 0xb7, 0x6a, 0x5f, 0xe4, 0xd7, 0xa3, 0xcc, 0x07, 0xe4, 0x74, 0x38, 0xbb, 0x9a, 0xed, 0xcc, 0x5e, 0x23,
    0xe7, 0xbf, 0xe1, 0xde, 0xde, 0x21, 0x3b, 0x6b, 0x08, 0x53, 0xec, 0x51, 0x8d, 0x32, 0x1a, 0xdc, 0xc7, 0xe7, 0x6e, 0xf9,
    0x5c, 0x0e, 0xb8, 0x6b, 0x22, 0x33, 0x6d, 0x0a, 0xd9, 0x2f, 0x32, 0x58, 0xd7, 0xde, 0x5d, 0x56, 0xbb, 0x47, 0x67, 0xf2,
    0x96, 0xe7, 0xc0, 0x1d, 0x75, 0xbd, 0x98, 0x9b, 0xeb, 0x5a, 0x6e, 0x93, 0x53, 0x6c, 0x7b, 0x3b, 0x4b, 0x3a, 0xea, 0xe7,
    0x6a, 0x2f, 0x6f, 0xa6, 0x6c, 0x3d, 0xa3, 0xe1, 0x4c, 0xcf, 0xf2, 0x99, 0xf2, 0xf5, 0x5c, 0xe6, 0xb3, 0xd9, 0x6f, 0xf2,
    0x58, 0x59, 0xca, 0x77, 0xdb, 0x35, 0x91, 0x87, 0xff, 0xd4, 0x5d, 0xf3, 0x7c, 0x36, 0x8b, 0x4a, 0x0d, 0x5a, 0xcb, 0x91,
    0xf8, 0x5c, 0xc9, 0x6c, 0xae, 0x83, 0x13, 0x57, 0xeb, 0x2a, 0xde, 0x39, 0x9f, 0x11, 0xff, 0xb9, 0xbd, 0xe6, 0xd7, 0x56,
    0xdd, 0xd6, 0xdd, 0x2e, 0xce, 0xb4, 0x99, 0xf5, 0x7a, 0xa3, 0xfa, 0xb9, 0xb5, 0xac, 0x34, 0x73, 0xd9, 0x79, 0xae, 0xe9,
    0x91, 0x7e, 0x65, 0x8f, 0xca, 0x95, 0x4f, 0xad, 0xb4, 0x03, 0x4f, 0x95, 0x54, 0x46, 0x30, 0x4f, 0xc5, 0xff, 0x67, 0xb4,
    0xef, 0xb5, 0xdd, 0x0d, 0xc7, 0xe3, 0xff, 0x94, 0x7b, 0x45, 0xfd, 0x65, 0x29, 0xcf, 0xde, 0x74, 0xcc, 0x0b, 0xa5, 0xb5,
    0xec, 0x6a, 0x9a, 0x9f, 0xaf, 0xec, 0x66, 0xf7, 0xb9, 0xf1, 0x5f, 0xdd, 0x3b, 0xe6, 0xad, 0xf8, 0xcf, 0xc7, 0xb7, 0xff,
    0xeb, 0xbb, 0x9b, 0xfe, 0xf9, 0x19, 0x55, 0x4a, 0x3d, 0x8b, 0xce, 0xb2, 0xab, 0xb8, 0xef, 0xda, 0x6e, 0xaf, 0xf9, 0xc4,
    0xf8, 0x66, 0x36, 0x27, 0xf0, 0x93, 0xf1, 0x5f, 0xb9, 0x2a, 0xbd, 0x25, 0xbb, 0x33, 0x43, 0x6f, 0xb4, 0xff, 0xf3, 0xcf,
    0x0d, 0xde, 0x8a, 0xff, 0xea, 0xf3, 0xff, 0x99, 0xbd, 0x3c, 0x4f, 0xb4, 0x8c, 0x95, 0x96, 0xfc, 0xec, 0x3e, 0x09, 0x27,
    0xc6, 0x39, 0xa7, 0xdb, 0xff, 0xca, 0x78, 0xe4, 0x1a, 0xee, 0xfc, 0x7e, 0xba, 0xe4, 0xda, 0x9e, 0xf9, 0xfd, 0xb4, 0x19,
    0x99, 0xa7, 0xc6, 0xf7, 0x9f, 0xb5, 0xdf, 0xea, 0xf5, 0xda, 0xf3, 0xb4, 0x9f, 0x70, 0xac, 0x1d, 0x33, 0x00, 0xcf, 0x3f,
    0x73, 0xfc, 0xb4, 0x73, 0x57, 0xdd, 0x89, 0xe5, 0xb9, 0xef, 0xd3, 0x51, 0x3e, 0xf3, 0xbd, 0xe5, 0x4c, 0xe5, 0x6f, 0xfa,
    0xf5, 0xef, 0x67, 0xe5, 0x10, 0x1e, 0xff, 0x86, 0x76, 0xaf, 0xfc, 0xb9, 0xdf, 0x4d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa9, 0xdf, 0xde, 0xd7, 0xd7, 0xbd, 0x57, 0xd7, 0x7c, 0xf7, 0x67, 0xe2, 0x8e, 0x35,
    0x3c, 0xd0, 0xb6, 0x16, 0x34, 0x93, 0xb9, 0xe0, 0xba, 0x63, 0x31, 0x5b, 0x99, 0x4d, 0x80, 0xf9, 0x36, 0xff, 0x8d, 0x75,
    0x7e, 0xd5, 0xcf, 0xcd, 0x30, 0xf3, 0x82, 0x55, 0x7c, 0x30, 0x1f, 0xff, 0x19, 0xe6, 0x32, 0xea, 0x2e, 0xbd, 0x26, 0xb2,
    0x0b, 0x5c, 0x85, 0xfc, 0x55, 0xd1, 0x47, 0x80, 0xa5, 0xf8, 0x9f, 0xc9, 0xc8, 0xfd, 0x49, 0xa5, 0x29, 0xef, 0x3e, 0x06,
    0xfc, 0x79, 0x94, 0xfd, 0xbd, 0xe2, 0x7f, 0x27, 0x7f, 0x3d, 0xf0, 0x7e, 0xfc, 0xa7, 0x58, 0x5a, 0xcf, 0x15, 0x0b, 0xfc,
    0xda, 0xfb, 0x9f, 0x99, 0xfd, 0x4f, 0x7b, 0xe9, 0xd5, 0xb0, 0xc7, 0x44, 0xf5, 0xa9, 0x24, 0xd0, 0xf1, 0x7c, 0x70, 0xa7,
    0xb4, 0xfe, 0x9b, 0x84, 0x99, 0xe7, 0x19, 0xae, 0x1c, 0x7c, 0xff, 0xe7, 0x92, 0xb5, 0xfa, 0xca, 0x19, 0x84, 0xef, 0xdf,
    0xef, 0x70, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x79, 0xff, 0xf9, 0xe7, 0x3c, 0x80, 0xf8, 0x07, 0xfe, 0xba, 0xf8, 0xff, 0x17 };

// Font glyphs rectangles data (on atlas)
RAYGUIAPI const Rectangle cherryFontRecs[189] = {
    { 4, 4, 5 , 15 },
    { 17, 4, 3 , 10 },
    { 28, 4, 5 , 2 },
    { 41, 4, 10 , 10 },
    { 59, 4, 7 , 11 },
    { 74, 4, 7 , 10 },
    { 89, 4, 8 , 10 },
    { 105, 4, 1 , 2 },
    { 114, 4, 3 , 10 },
    { 125, 4, 3 , 10 },
    { 136, 4, 3 , 3 },
    { 147, 4, 7 , 7 },
    { 162, 4, 2 , 2 },
    { 172, 4, 6 , 3 },
    { 186, 4, 1 , 1 },
    { 195, 4, 4 , 10 },
    { 207, 4, 5 , 8 },
    { 220, 4, 5 , 8 },
    { 233, 4, 5 , 8 },
    { 246, 4, 5 , 8 },
    { 259, 4, 5 , 8 },
    { 272, 4, 5 , 8 },
    { 285, 4, 5 , 8 },
    { 298, 4, 5 , 8 },
    { 311, 4, 5 , 8 },
    { 324, 4, 5 , 8 },
    { 337, 4, 3 , 10 },
    { 348, 4, 3 , 12 },
    { 359, 4, 6 , 7 },
    { 373, 4, 6 , 4 },
    { 387, 4, 6 , 7 },
    { 401, 4, 5 , 10 },
    { 414, 4, 8 , 10 },
    { 430, 4, 7 , 9 },
    { 445, 4, 7 , 9 },
    { 460, 4, 6 , 9 },
    { 474, 4, 6 , 9 },
    { 488, 4, 7 , 9 },
    { 4, 27, 6 , 9 },
    { 18, 27, 6 , 9 },
    { 32, 27, 7 , 9 },
    { 47, 27, 3 , 9 },
    { 58, 27, 6 , 9 },
    { 72, 27, 7 , 9 },
    { 87, 27, 7 , 9 },
    { 102, 27, 11 , 9 },
    { 121, 27, 8 , 9 },
    { 137, 27, 6 , 9 },
    { 151, 27, 6 , 9 },
    { 165, 27, 7 , 9 },
    { 180, 27, 7 , 9 },
    { 195, 27, 6 , 9 },
    { 209, 27, 7 , 9 },
    { 224, 27, 8 , 9 },
    { 240, 27, 9 , 9 },
    { 257, 27, 11 , 9 },
    { 276, 27, 7 , 9 },
    { 291, 27, 7 , 9 },
    { 306, 27, 7 , 9 },
    { 321, 27, 3 , 9 },
    { 332, 27, 4 , 10 },
    { 344, 27, 3 , 9 },
    { 355, 27, 3 , 3 },
    { 366, 27, 7 , 2 },
    { 381, 27, 2 , 2 },
    { 391, 27, 6 , 6 },
    { 405, 27, 6 , 9 },
    { 419, 27, 6 , 6 },
    { 433, 27, 6 , 9 },
    { 447, 27, 6 , 6 },
    { 461, 27, 5 , 9 },
    { 474, 27, 5 , 9 },
    { 487, 27, 7 , 9 },
    { 4, 50, 3 , 7 },
    { 15, 50, 3 , 8 },
    { 26, 50, 6 , 9 },
    { 40, 50, 4 , 9 },
    { 52, 50, 11 , 6 },
    { 71, 50, 7 , 6 },
    { 86, 50, 5 , 6 },
    { 99, 50, 6 , 8 },
    { 113, 50, 6 , 8 },
    { 127, 50, 5 , 6 },
    { 140, 50, 5 , 6 },
    { 153, 50, 5 , 9 },
    { 166, 50, 7 , 6 },
    { 181, 50, 7 , 6 },
    { 196, 50, 11 , 6 },
    { 215, 50, 7 , 6 },
    { 230, 50, 7 , 8 },
    { 245, 50, 6 , 6 },
    { 259, 50, 5 , 9 },
    { 272, 50, 1 , 9 },
    { 281, 50, 5 , 9 },
    { 294, 50, 7 , 3 },
    { 309, 50, 3 , 10 },
    { 320, 50, 7 , 10 },
    { 335, 50, 7 , 10 },
    { 350, 50, 6 , 9 },
    { 364, 50, 7 , 9 },
    { 379, 50, 6 , 12 },
    { 393, 50, 7 , 11 },
    { 408, 50, 5 , 9 },
    { 421, 50, 5 , 5 },
    { 434, 50, 4 , 5 },
    { 446, 50, 6 , 7 },
    { 460, 50, 6 , 3 },
    { 474, 50, 5 , 5 },
    { 487, 50, 6 , 1 },
    { 4, 73, 3 , 3 },
    { 15, 73, 7 , 10 },
    { 30, 73, 3 , 5 },
    { 41, 73, 3 , 5 },
    { 52, 73, 7 , 12 },
    { 67, 73, 6 , 8 },
    { 81, 73, 7 , 9 },
    { 96, 73, 2 , 3 },
    { 106, 73, 6 , 9 },
    { 120, 73, 3 , 5 },
    { 131, 73, 3 , 5 },
    { 142, 73, 6 , 7 },
    { 156, 73, 13 , 9 },
    { 177, 73, 10 , 6 },
    { 195, 73, 7 , 11 },
    { 210, 73, 5 , 10 },
    { 223, 73, 7 , 12 },
    { 238, 73, 7 , 12 },
    { 253, 73, 7 , 12 },
    { 268, 73, 7 , 12 },
    { 283, 73, 7 , 11 },
    { 298, 73, 7 , 11 },
    { 313, 73, 12 , 9 },
    { 333, 73, 6 , 11 },
    { 347, 73, 7 , 12 },
    { 362, 73, 7 , 12 },
    { 377, 73, 7 , 12 },
    { 392, 73, 7 , 11 },
    { 407, 73, 3 , 12 },
    { 418, 73, 3 , 12 },
    { 429, 73, 3 , 12 },
    { 440, 73, 3 , 11 },
    { 451, 73, 7 , 9 },
    { 466, 73, 8 , 12 },
    { 482, 73, 6 , 12 },
    { 496, 73, 6 , 12 },
    { 4, 96, 6 , 12 },
    { 18, 96, 6 , 12 },
    { 32, 96, 6 , 11 },
    { 46, 96, 5 , 5 },
    { 59, 96, 8 , 9 },
    { 75, 96, 8 , 12 },
    { 91, 96, 8 , 12 },
    { 107, 96, 8 , 12 },
    { 123, 96, 8 , 11 },
    { 139, 96, 7 , 12 },
    { 154, 96, 7 , 9 },
    { 169, 96, 8 , 9 },
    { 185, 96, 6 , 9 },
    { 199, 96, 6 , 9 },
    { 213, 96, 6 , 9 },
    { 227, 96, 6 , 9 },
    { 241, 96, 6 , 8 },
    { 255, 96, 6 , 10 },
    { 269, 96, 10 , 6 },
    { 287, 96, 6 , 8 },
    { 301, 96, 6 , 9 },
    { 315, 96, 6 , 9 },
    { 329, 96, 6 , 9 },
    { 343, 96, 6 , 8 },
    { 357, 96, 3 , 10 },
    { 368, 96, 3 , 10 },
    { 379, 96, 3 , 10 },
    { 390, 96, 3 , 9 },
    { 401, 96, 6 , 10 },
    { 415, 96, 7 , 9 },
    { 430, 96, 5 , 9 },
    { 443, 96, 5 , 9 },
    { 456, 96, 5 , 9 },
    { 469, 96, 5 , 9 },
    { 482, 96, 5 , 8 },
    { 495, 96, 7 , 9 },
    { 4, 119, 8 , 8 },
    { 20, 119, 7 , 9 },
    { 35, 119, 7 , 9 },
    { 50, 119, 7 , 9 },
    { 65, 119, 7 , 8 },
    { 80, 119, 7 , 11 },
    { 95, 119, 5 , 8 },
    { 108, 119, 7 , 10 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
RAYGUIAPI const GlyphInfo cherryFontGlyphs[189] = {
    { 32, 0, 12, 5, { 0 }},
    { 33, 0, 2, 4, { 0 }},
    { 34, 0, 2, 6, { 0 }},
    { 35, 0, 2, 11, { 0 }},
    { 36, 0, 2, 8, { 0 }},
    { 37, 0, 2, 8, { 0 }},
    { 38, 0, 2, 9, { 0 }},
    { 39, 0, 2, 2, { 0 }},
    { 40, 0, 2, 4, { 0 }},
    { 41, 0, 2, 4, { 0 }},
    { 42, 0, 2, 4, { 0 }},
    { 43, 0, 4, 8, { 0 }},
    { 44, 0, 11, 3, { 0 }},
    { 45, 0, 6, 7, { 0 }},
    { 46, 0, 11, 2, { 0 }},
    { 47, 0, 2, 5, { 0 }},
    { 48, 0, 4, 6, { 0 }},
    { 49, 0, 4, 6, { 0 }},
    { 50, 0, 4, 6, { 0 }},
    { 51, 0, 4, 6, { 0 }},
    { 52, 0, 4, 6, { 0 }},
    { 53, 0, 4, 6, { 0 }},
    { 54, 0, 4, 6, { 0 }},
    { 55, 0, 4, 6, { 0 }},
    { 56, 0, 4, 6, { 0 }},
    { 57, 0, 4, 6, { 0 }},
    { 58, 0, 2, 4, { 0 }},
    { 59, 0, 2, 4, { 0 }},
    { 60, 0, 4, 7, { 0 }},
    { 61, 0, 5, 7, { 0 }},
    { 62, 0, 4, 7, { 0 }},
    { 63, 0, 2, 6, { 0 }},
    { 64, 0, 2, 9, { 0 }},
    { 65, 0, 3, 8, { 0 }},
    { 66, 0, 3, 8, { 0 }},
    { 67, 0, 3, 7, { 0 }},
    { 68, 0, 3, 7, { 0 }},
    { 69, 0, 3, 8, { 0 }},
    { 70, 0, 3, 7, { 0 }},
    { 71, 0, 3, 7, { 0 }},
    { 72, 0, 3, 8, { 0 }},
    { 73, 0, 3, 4, { 0 }},
    { 74, 0, 3, 7, { 0 }},
    { 75, 0, 3, 8, { 0 }},
    { 76, 0, 3, 8, { 0 }},
    { 77, 0, 3, 12, { 0 }},
    { 78, 0, 3, 9, { 0 }},
    { 79, 0, 3, 7, { 0 }},
    { 80, 0, 3, 7, { 0 }},
    { 81, 0, 3, 8, { 0 }},
    { 82, 0, 3, 8, { 0 }},
    { 83, 0, 3, 7, { 0 }},
    { 84, 0, 3, 8, { 0 }},
    { 85, 0, 3, 9, { 0 }},
    { 86, 0, 3, 10, { 0 }},
    { 87, 0, 3, 12, { 0 }},
    { 88, 0, 3, 8, { 0 }},
    { 89, 0, 3, 8, { 0 }},
    { 90, 0, 3, 8, { 0 }},
    { 91, 0, 3, 4, { 0 }},
    { 92, 0, 2, 5, { 0 }},
    { 93, 0, 3, 4, { 0 }},
    { 94, 0, 3, 4, { 0 }},
    { 95, 0, 10, 8, { 0 }},
    { 96, 0, 2, 3, { 0 }},
    { 97, 0, 6, 7, { 0 }},
    { 98, 0, 3, 7, { 0 }},
    { 99, 0, 6, 7, { 0 }},
    { 100, 0, 3, 7, { 0 }},
    { 101, 0, 6, 7, { 0 }},
    { 102, 0, 3, 6, { 0 }},
    { 103, 0, 5, 6, { 0 }},
    { 104, 0, 3, 8, { 0 }},
    { 105, 0, 5, 4, { 0 }},
    { 106, 0, 5, 4, { 0 }},
    { 107, 0, 3, 7, { 0 }},
    { 108, 0, 3, 5, { 0 }},
    { 109, 0, 6, 12, { 0 }},
    { 110, 0, 6, 8, { 0 }},
    { 111, 0, 6, 6, { 0 }},
    { 112, 0, 6, 7, { 0 }},
    { 113, 0, 6, 7, { 0 }},
    { 114, 0, 6, 6, { 0 }},
    { 115, 0, 6, 6, { 0 }},
    { 116, 0, 3, 6, { 0 }},
    { 117, 0, 6, 8, { 0 }},
    { 118, 0, 6, 8, { 0 }},
    { 119, 0, 6, 12, { 0 }},
    { 120, 0, 6, 8, { 0 }},
    { 121, 0, 6, 8, { 0 }},
    { 122, 0, 6, 7, { 0 }},
    { 123, 0, 3, 6, { 0 }},
    { 124, 0, 3, 2, { 0 }},
    { 125, 0, 3, 6, { 0 }},
    { 126, 0, 6, 8, { 0 }},
    { 161, 0, 4, 4, { 0 }},
    { 162, 0, 4, 8, { 0 }},
    { 163, 0, 2, 8, { 0 }},
    { 8364, 0, 3, 7, { 0 }},
    { 165, 0, 3, 8, { 0 }},
    { 352, 0, 0, 7, { 0 }},
    { 167, 0, 2, 8, { 0 }},
    { 353, 0, 3, 6, { 0 }},
    { 169, 0, 1, 6, { 0 }},
    { 170, 0, 2, 5, { 0 }},
    { 171, 0, 5, 7, { 0 }},
    { 172, 0, 6, 7, { 0 }},
    { 174, 0, 1, 6, { 0 }},
    { 175, 0, 2, 7, { 0 }},
    { 176, 0, 2, 4, { 0 }},
    { 177, 0, 3, 8, { 0 }},
    { 178, 0, 2, 4, { 0 }},
    { 179, 0, 2, 4, { 0 }},
    { 381, 0, 0, 8, { 0 }},
    { 181, 0, 6, 7, { 0 }},
    { 182, 0, 3, 8, { 0 }},
    { 183, 0, 6, 3, { 0 }},
    { 382, 0, 3, 7, { 0 }},
    { 185, 0, 2, 4, { 0 }},
    { 186, 0, 2, 4, { 0 }},
    { 187, 0, 5, 7, { 0 }},
    { 338, 0, 3, 14, { 0 }},
    { 339, 0, 6, 11, { 0 }},
    { 376, 0, 1, 8, { 0 }},
    { 191, 0, 4, 6, { 0 }},
    { 192, 0, 0, 8, { 0 }},
    { 193, 0, 0, 8, { 0 }},
    { 194, 0, 0, 8, { 0 }},
    { 195, 0, 0, 8, { 0 }},
    { 196, 0, 1, 8, { 0 }},
    { 197, 0, 1, 8, { 0 }},
    { 198, 0, 3, 13, { 0 }},
    { 199, 0, 3, 7, { 0 }},
    { 200, 0, 0, 8, { 0 }},
    { 201, 0, 0, 8, { 0 }},
    { 202, 0, 0, 8, { 0 }},
    { 203, 0, 1, 8, { 0 }},
    { 204, 0, 0, 4, { 0 }},
    { 205, 0, 0, 4, { 0 }},
    { 206, 0, 0, 4, { 0 }},
    { 207, 0, 1, 4, { 0 }},
    { 208, 0, 3, 8, { 0 }},
    { 209, 0, 0, 9, { 0 }},
    { 210, 0, 0, 7, { 0 }},
    { 211, 0, 0, 7, { 0 }},
    { 212, 0, 0, 7, { 0 }},
    { 213, 0, 0, 7, { 0 }},
    { 214, 0, 1, 7, { 0 }},
    { 215, 1, 5, 7, { 0 }},
    { 216, 0, 3, 9, { 0 }},
    { 217, 0, 0, 9, { 0 }},
    { 218, 0, 0, 9, { 0 }},
    { 219, 0, 0, 9, { 0 }},
    { 220, 0, 1, 9, { 0 }},
    { 221, 0, 0, 8, { 0 }},
    { 222, 0, 2, 8, { 0 }},
    { 223, 0, 3, 9, { 0 }},
    { 224, 0, 3, 7, { 0 }},
    { 225, 0, 3, 7, { 0 }},
    { 226, 0, 3, 7, { 0 }},
    { 227, 0, 3, 7, { 0 }},
    { 228, 0, 4, 7, { 0 }},
    { 229, 0, 2, 7, { 0 }},
    { 230, 0, 6, 11, { 0 }},
    { 231, 0, 6, 7, { 0 }},
    { 232, 0, 3, 7, { 0 }},
    { 233, 0, 3, 7, { 0 }},
    { 234, 0, 3, 7, { 0 }},
    { 235, 0, 4, 7, { 0 }},
    { 236, 0, 2, 4, { 0 }},
    { 237, 0, 2, 4, { 0 }},
    { 238, 0, 2, 4, { 0 }},
    { 239, 0, 3, 4, { 0 }},
    { 240, 0, 2, 7, { 0 }},
    { 241, 0, 3, 8, { 0 }},
    { 242, 0, 3, 6, { 0 }},
    { 243, 0, 3, 6, { 0 }},
    { 244, 0, 3, 6, { 0 }},
    { 245, 0, 3, 6, { 0 }},
    { 246, 0, 4, 6, { 0 }},
    { 247, 0, 3, 8, { 0 }},
    { 248, 0, 4, 9, { 0 }},
    { 249, 0, 3, 8, { 0 }},
    { 250, 0, 3, 8, { 0 }},
    { 251, 0, 3, 8, { 0 }},
    { 252, 0, 4, 8, { 0 }},
    { 253, 0, 3, 8, { 0 }},
    { 254, 0, 4, 6, { 0 }},
    { 255, 0, 4, 8, { 0 }},
};

// Style loading function: Cherry
RAYGUIAPI void GuiLoadStyleCherry(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < CHERRY_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(cherryStyleProps[i].controlId, cherryStyleProps[i].propertyId, cherryStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int cherryFontDataSize = 0;
    unsigned char *data = DecompressData(cherryFontData, CHERRY_STYLE_FONT_ATLAS_COMP_SIZE, &cherryFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 15;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, cherryFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, cherryFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleTerminal();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define TERMINAL_STYLE_PROPS_COUNT  17

// Custom style name: Terminal
RAYGUIAPI const GuiStyleProp terminalStyleProps[TERMINAL_STYLE_PROPS_COUNT] = {
    { 0, 0, 0x1c8d00ff },    // DEFAULT_BORDER_COLOR_NORMAL
    { 0, 1, 0x161313ff },    // DEFAULT_BASE_COLOR_NORMAL
    { 0, 2, 0x38f620ff },    // DEFAULT_TEXT_COLOR_NORMAL
    { 0, 3, 0xc3fbc6ff },    // DEFAULT_BORDER_COLOR_FOCUSED
    { 0, 4, 0x43bf2eff },    // DEFAULT_BASE_COLOR_FOCUSED
    { 0, 5, 0xdcfadcff },    // DEFAULT_TEXT_COLOR_FOCUSED
    { 0, 6, 0x1f5b19ff },    // DEFAULT_BORDER_COLOR_PRESSED
    { 0, 7, 0x43ff28ff },    // DEFAULT_BASE_COLOR_PRESSED
    { 0, 8, 0x1e6f15ff },    // DEFAULT_TEXT_COLOR_PRESSED
    { 0, 9, 0x223b22ff },    // DEFAULT_BORDER_COLOR_DISABLED
    { 0, 10, 0x182c18ff },    // DEFAULT_BASE_COLOR_DISABLED
    { 0, 11, 0x244125ff },    // DEFAULT_TEXT_COLOR_DISABLED
    { 0, 16, 0x00000010 },    // DEFAULT_TEXT_SIZE
    { 0, 17, 0x00000000 },    // DEFAULT_TEXT_SPACING
    { 0, 18, 0xe6fce3ff },    // DEFAULT_LINE_COLOR
    { 0, 19, 0x0c1505ff },    // DEFAULT_BACKGROUND_COLOR
    { 0, 20, 0x00000018 },    // DEFAULT_TEXT_LINE_SPACING
};

// WARNING: This style uses a custom font: "Mecha.ttf" (size: 16, spacing: 0)

#define TERMINAL_STYLE_FONT_ATLAS_COMP_SIZE 1860

// Font atlas image pixels data: DEFLATE compressed
RAYGUIAPI unsigned char terminalFontData[TERMINAL_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0x41, 0x92, 0xa4, 0x36, 0x10, 0x05, 0x50, 0xee, 0x7f, 0xe9, 0xf4, 0x62, 0x62, 0x16, 0x76, 0xb8, 0x1b, 0x94, 0x4a,
    0x89, 0x04, 0x9e, 0x5f, 0x78, 0xd3, 0xd5, 0x53, 0x4d, 0x01, 0xbf, 0x24, 0x84, 0x94, 0xc4, 0x01, 0x00, 0x00, 0x00, 0x7c,
    0x5e, 0xfc, 0xef, 0x4f, 0xe2, 0xc7, 0xdf, 0x8c, 0xcb, 0xef, 0xf3, 0xe7, 0xa7, 0xf1, 0xe3, 0x5f, 0xf9, 0xfb, 0xdf, 0x95,
    0x77, 0xba, 0xfe, 0x5b, 0x31, 0xb4, 0x75, 0x73, 0x5b, 0x95, 0x7b, 0x9f, 0xd1, 0xdf, 0xfe, 0x7d, 0x7b, 0xaa, 0xde, 0xad,
    0xf6, 0x95, 0xb1, 0xb3, 0x23, 0xbf, 0xe7, 0xae, 0x6e, 0x61, 0x6c, 0xdf, 0x2b, 0xc7, 0xa6, 0x7d, 0x1c, 0x0d, 0xf2, 0x7f,
    0x7e, 0xcc, 0x46, 0xf2, 0x14, 0xe9, 0xf4, 0x8e, 0x7f, 0x3b, 0xad, 0xfc, 0x0e, 0x1d, 0xdd, 0xc6, 0xdc, 0x3e, 0x89, 0x92,
    0xf7, 0x9f, 0xf9, 0x3b, 0x51, 0xb6, 0xd7, 0x72, 0xff, 0x26, 0x86, 0xdb, 0x88, 0xf9, 0x4f, 0x78, 0xbe, 0x8f, 0x63, 0xd1,
    0x71, 0xef, 0x99, 0xff, 0xfc, 0x51, 0xcb, 0x9f, 0x29, 0x57, 0xb7, 0x3c, 0xd7, 0xa6, 0xaf, 0x3a, 0x27, 0xe5, 0xff, 0xec,
    0x9b, 0xfa, 0xe7, 0x16, 0xb4, 0xa2, 0xdd, 0x90, 0xff, 0x5c, 0x06, 0x62, 0x22, 0x47, 0xbb, 0xf2, 0x5f, 0xdb, 0xd6, 0xc8,
    0xff, 0x33, 0xda, 0xff, 0xb3, 0x6d, 0xff, 0xf7, 0x79, 0x2b, 0xff, 0xd9, 0xa3, 0x90, 0x6d, 0xff, 0x63, 0x7a, 0xfb, 0x3b,
    0xe7, 0x7f, 0x74, 0xdc, 0x43, 0xfe, 0xcf, 0xaf, 0xe8, 0x73, 0xbf, 0x7d, 0xb6, 0x27, 0xe4, 0x7f, 0x5d, 0xfe, 0x7f, 0xeb,
    0xb3, 0x9d, 0xf5, 0xf4, 0x76, 0xe4, 0xff, 0xd8, 0x9e, 0xff, 0xb3, 0xeb, 0xa8, 0xeb, 0xfb, 0x62, 0xc7, 0x08, 0xd4, 0x91,
    0x1c, 0xdb, 0x89, 0xc1, 0x0c, 0xdf, 0xd3, 0x0b, 0x3b, 0xcb, 0x7f, 0x66, 0x4f, 0x66, 0xf2, 0x7f, 0x76, 0x5c, 0x8e, 0x5f,
    0x7a, 0x30, 0xab, 0xf6, 0x7e, 0x45, 0xfe, 0x67, 0x46, 0xe4, 0x9e, 0x9d, 0xff, 0x38, 0xd9, 0x57, 0x31, 0x31, 0xbe, 0xb9,
    0xb3, 0xcf, 0x30, 0xd3, 0x4f, 0xeb, 0x7b, 0x1c, 0xde, 0xd3, 0xff, 0x8f, 0xd6, 0xed, 0xbf, 0xfc, 0xcf, 0x5d, 0xff, 0xbf,
    0x2d, 0xff, 0xb1, 0xfd, 0x58, 0xc5, 0x85, 0x33, 0x56, 0xfe, 0xe7, 0xf6, 0xf0, 0x79, 0x8f, 0x23, 0x16, 0x5d, 0xbf, 0x74,
    0xcd, 0x7f, 0xee, 0xd3, 0x7d, 0xb1, 0xfd, 0x8f, 0x1b, 0x8f, 0x8f, 0xfc, 0xaf, 0xd8, 0x9b, 0x23, 0x77, 0xd8, 0x66, 0xe7,
    0x2f, 0xc8, 0xbf, 0xfc, 0xcb, 0x7f, 0xef, 0xfe, 0x7f, 0x0c, 0x8e, 0xdc, 0xc6, 0xe3, 0xc7, 0xff, 0xe2, 0x52, 0x6f, 0x69,
    0x7e, 0xb6, 0xe0, 0x78, 0x9f, 0x2b, 0xf7, 0x6e, 0xf9, 0xd9, 0x75, 0x4f, 0xb9, 0xfe, 0xdf, 0x39, 0x93, 0xf2, 0x28, 0x99,
    0x87, 0xb2, 0x7e, 0xfe, 0xdf, 0x33, 0xe7, 0x28, 0x77, 0xcf, 0x3f, 0xb0, 0x7a, 0x95, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x65, 0x0d, 0x4d, 0x94, 0xad, 0x7b, 0x8c, 0x16, 0x95, 0xe7, 0xf7, 0xd4, 0x58,
    0xbf, 0xb6, 0x87, 0xaa, 0xde, 0x71, 0xec, 0x59, 0x02, 0x63, 0xeb, 0x30, 0x73, 0x2b, 0xff, 0x56, 0xaf, 0x6e, 0x9b, 0x59,
    0xdd, 0x58, 0xb3, 0x65, 0x15, 0xb5, 0x2d, 0xee, 0x49, 0xc7, 0xca, 0x75, 0xe2, 0x71, 0x69, 0x75, 0x70, 0x6d, 0x92, 0x56,
    0xaf, 0xd8, 0xce, 0xac, 0x7d, 0xdf, 0x99, 0xff, 0xb1, 0x33, 0x70, 0xfc, 0x73, 0x46, 0x62, 0x35, 0x55, 0x6d, 0x0d, 0x98,
    0x63, 0xe9, 0xb9, 0x19, 0x8b, 0xd6, 0x90, 0x8d, 0xef, 0x83, 0x3d, 0xe9, 0x90, 0xff, 0x2f, 0xe7, 0x3f, 0x57, 0x23, 0x7b,
    0xc7, 0xb3, 0x50, 0xe4, 0x5f, 0xfe, 0xe5, 0xff, 0xad, 0xf9, 0x8f, 0xe2, 0xfe, 0x77, 0xbe, 0x5a, 0x47, 0xcd, 0x95, 0x55,
    0x6d, 0x4a, 0xe4, 0x5f, 0xfe, 0x7f, 0xef, 0x6d, 0xc7, 0xc5, 0x56, 0x6c, 0xa4, 0xc5, 0xdb, 0x59, 0xb3, 0x64, 0x5f, 0x15,
    0xad, 0xd1, 0x6f, 0x93, 0x28, 0x4c, 0xf0, 0x57, 0xf2, 0x9f, 0xaf, 0x7b, 0xbe, 0x67, 0xdc, 0x2b, 0xb3, 0xe5, 0x99, 0xda,
    0x57, 0xf5, 0xf9, 0x3f, 0x7b, 0x1e, 0x42, 0xe6, 0xfb, 0xe9, 0x5b, 0xf9, 0x1f, 0xfd, 0x6c, 0xf2, 0x7f, 0x6c, 0x7e, 0xb6,
    0xcc, 0xfd, 0x35, 0x16, 0x23, 0xd9, 0xd2, 0x57, 0x6d, 0x75, 0xa4, 0x46, 0xdb, 0xaa, 0x7e, 0x9e, 0xab, 0xd2, 0xf8, 0xde,
    0xfc, 0x47, 0xc1, 0x7d, 0xae, 0xb9, 0x56, 0x52, 0xfe, 0xe5, 0xff, 0x98, 0xac, 0xc0, 0xdb, 0x3d, 0xff, 0x2b, 0xae, 0xbf,
    0x2b, 0x9f, 0xe6, 0xfa, 0xcc, 0xf6, 0xff, 0x59, 0xf7, 0xff, 0xbe, 0x92, 0xff, 0xb3, 0x63, 0x79, 0x77, 0xfe, 0x3b, 0xd5,
    0x4c, 0xcd, 0x8c, 0x30, 0xce, 0xfc, 0x9b, 0x8e, 0xf9, 0xdf, 0x35, 0x9f, 0x47, 0xfe, 0x77, 0xe5, 0xff, 0xe7, 0xa7, 0x8d,
    0xcb, 0x7f, 0xaf, 0xfc, 0xaf, 0xeb, 0xff, 0x3f, 0x3b, 0xff, 0xeb, 0x7a, 0x5f, 0xab, 0xfb, 0x73, 0xb5, 0x5b, 0x9e, 0x99,
    0x01, 0xf7, 0xdb, 0xfc, 0xbb, 0x48, 0x57, 0x6d, 0xaf, 0x98, 0x87, 0x37, 0x33, 0x3b, 0x68, 0xf7, 0x95, 0x41, 0xf5, 0xbf,
    0xa9, 0x1f, 0xb3, 0xe8, 0x9b, 0x7f, 0x78, 0x46, 0xfe, 0xbf, 0xb0, 0xaf, 0x71, 0x3c, 0xee, 0x69, 0x59, 0x57, 0xfe, 0x3e,
    0xce, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0xd3, 0x2a, 0xac, 0xaa, 0xba, 0x8d, 0x3b,
    0xab, 0x65, 0xe4, 0xd6, 0xf9, 0xc5, 0xe5, 0xba, 0x2e, 0x99, 0x55, 0xe4, 0x23, 0x6b, 0xf9, 0x6a, 0xd6, 0x34, 0xe6, 0xcf,
    0x81, 0x48, 0xad, 0x96, 0x3b, 0x3f, 0x53, 0x32, 0x35, 0xec, 0xd7, 0xcd, 0xb6, 0xff, 0xbd, 0x7e, 0x43, 0xbe, 0x92, 0xc8,
    0xaa, 0xf5, 0x05, 0x75, 0xf5, 0x9f, 0x8e, 0x82, 0xd5, 0x8a, 0xd9, 0x35, 0xf4, 0xf7, 0xe6, 0x7f, 0x74, 0xf5, 0xdb, 0xf5,
    0x9f, 0x55, 0xd4, 0x5b, 0x89, 0xc4, 0xca, 0xbc, 0xca, 0xfc, 0x57, 0x9c, 0xe3, 0x51, 0xf6, 0xbe, 0xb1, 0xb8, 0x4e, 0xc1,
    0x95, 0x56, 0xe2, 0xd9, 0x6b, 0xa2, 0xe2, 0xd5, 0xf9, 0xaf, 0xad, 0x96, 0x5b, 0xff, 0x9d, 0x90, 0xaf, 0x4f, 0x70, 0x5f,
    0xfe, 0xff, 0x6e, 0x57, 0x75, 0xfe, 0x33, 0xef, 0x5b, 0x53, 0xd9, 0x6a, 0x2e, 0xff, 0x3d, 0x56, 0xf6, 0xc5, 0xf2, 0x9e,
    0x47, 0x26, 0xff, 0x63, 0xfd, 0xb6, 0x15, 0xf9, 0xcf, 0x57, 0xe6, 0x39, 0x36, 0x65, 0xfd, 0x28, 0x4e, 0xf9, 0xfa, 0xfc,
    0xd7, 0x9f, 0x69, 0xf1, 0x9f, 0xff, 0xbb, 0xac, 0xb6, 0x7d, 0x4b, 0xfe, 0x73, 0xd5, 0x69, 0x66, 0xf3, 0x9f, 0xbd, 0xe2,
    0xbd, 0xbb, 0xfd, 0x5f, 0x73, 0x4d, 0xb0, 0x3e, 0xff, 0x91, 0xaa, 0x0c, 0xde, 0x27, 0xff, 0xb1, 0x20, 0xb9, 0xf2, 0x9f,
    0x6d, 0x03, 0xdf, 0x90, 0xff, 0xea, 0xe7, 0x7c, 0x74, 0xc8, 0x7f, 0x6e, 0xc4, 0x2b, 0x4a, 0x73, 0x1a, 0xed, 0xdb, 0xff,
    0x63, 0xdb, 0xf8, 0x5f, 0xff, 0xfc, 0x47, 0x49, 0xbf, 0xf0, 0xce, 0xfc, 0xd7, 0x57, 0xcb, 0xcd, 0x56, 0x0d, 0xbc, 0x3e,
    0xd6, 0xd7, 0xa7, 0xfd, 0xaf, 0xbf, 0x1e, 0x5e, 0xd1, 0xfe, 0x1f, 0x0b, 0xee, 0x52, 0xac, 0xbf, 0xaa, 0xee, 0x9f, 0xff,
    0xaa, 0xeb, 0xc2, 0xcc, 0x73, 0x93, 0x46, 0xef, 0xff, 0xc5, 0xe9, 0x59, 0x3c, 0x7a, 0xe7, 0x68, 0x3c, 0x4f, 0x31, 0xf8,
    0xf4, 0xa2, 0xfe, 0xd7, 0xff, 0x95, 0xed, 0xff, 0x9a, 0x51, 0xef, 0xae, 0xa3, 0xe9, 0xd7, 0x9e, 0x2f, 0xde, 0x63, 0x8b,
    0xf3, 0x4f, 0xdc, 0x8a, 0xed, 0x57, 0x56, 0x7d, 0x8e, 0xf1, 0xfc, 0x67, 0x5f, 0x93, 0xff, 0x8a, 0x8a, 0xd3, 0xf5, 0xed,
    0xe1, 0xf7, 0xe6, 0xbc, 0xcc, 0xdc, 0x75, 0xef, 0xb1, 0xb5, 0xd5, 0xe7, 0xc5, 0x3b, 0xf2, 0x5f, 0xf7, 0xd9, 0x63, 0xc9,
    0xb7, 0x4a, 0xa6, 0x96, 0x77, 0x2c, 0x9f, 0x0f, 0x23, 0xff, 0xcf, 0xcd, 0x7f, 0xdc, 0x70, 0xcf, 0xf0, 0x58, 0x3c, 0x1e,
    0xdd, 0x61, 0x0f, 0xef, 0xcd, 0x3f, 0xfd, 0xce, 0x88, 0xee, 0xf9, 0x5f, 0x3f, 0x2f, 0xf4, 0xcb, 0xed, 0x81, 0x2a, 0xf9,
    0x3c, 0xf1, 0x5a, 0x56, 0xfe, 0x73, 0xfd, 0x96, 0xd9, 0xf9, 0xff, 0x20, 0xff, 0xdf, 0xdd, 0xeb, 0xd0, 0x7f, 0x76, 0x03,
    0xf2, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xfb, 0x67, 0xbb, 0x45, 0x6a, 0x7d, 0x5b, 0x0c, 0x56,
    0x32, 0x88, 0x74, 0xa5, 0xf8, 0x6c, 0x7d, 0x8c, 0x18, 0x5c, 0xf7, 0x35, 0x5f, 0x37, 0xf8, 0xfa, 0xf3, 0x15, 0x66, 0xd6,
    0x0f, 0xae, 0x3f, 0x8e, 0x75, 0x15, 0xd6, 0xeb, 0x8f, 0x52, 0xe6, 0xef, 0x47, 0x79, 0xed, 0xb5, 0xfe, 0x99, 0x99, 0xdf,
    0xcf, 0x99, 0x95, 0xbe, 0xb5, 0x67, 0xd4, 0xb5, 0x63, 0x5b, 0xb7, 0xba, 0x33, 0x26, 0x92, 0x3e, 0x3e, 0x4f, 0xfb, 0xf7,
    0xaa, 0x9f, 0x5d, 0x8f, 0x63, 0x2e, 0x4b, 0x51, 0xba, 0x06, 0x37, 0xf7, 0xd7, 0x9f, 0xb6, 0xaf, 0xaf, 0x57, 0x01, 0xd8,
    0xdd, 0x5f, 0xc8, 0x3f, 0x35, 0x28, 0x6e, 0x9f, 0x8b, 0xbf, 0xae, 0xa2, 0x4e, 0x5d, 0xad, 0xe2, 0xbb, 0x8f, 0xe3, 0x71,
    0xfa, 0x0c, 0x93, 0x68, 0xb8, 0xbe, 0xe5, 0xac, 0xff, 0x18, 0x2d, 0xf7, 0x75, 0x5c, 0xee, 0x23, 0xe6, 0xf3, 0x9f, 0x7f,
    0x3e, 0x53, 0x14, 0x57, 0xe6, 0xcd, 0x57, 0xc7, 0xac, 0xde, 0xfa, 0x7c, 0xfb, 0x9f, 0xb9, 0x52, 0x88, 0xe5, 0x9f, 0xaa,
    0xf6, 0x3b, 0x39, 0x4e, 0x6a, 0xb9, 0xae, 0x3e, 0x2b, 0x56, 0xbc, 0x12, 0x1b, 0xce, 0xad, 0xdc, 0xf5, 0xde, 0xb5, 0x33,
    0x64, 0x26, 0xff, 0x7d, 0x8f, 0xcd, 0xd1, 0xe6, 0x95, 0xdd, 0xf9, 0x7f, 0xee, 0x2b, 0xef, 0xc9, 0x7f, 0x9f, 0xb3, 0xef,
    0xb7, 0xcf, 0x33, 0xfb, 0xac, 0xc2, 0x0e, 0x47, 0xe0, 0xac, 0x4e, 0xec, 0x5b, 0xf3, 0x1f, 0x3f, 0x8e, 0x0c, 0xf6, 0xff,
    0x4e, 0xfe, 0xb9, 0x5f, 0xf0, 0xb4, 0xfc, 0x57, 0xb6, 0xcb, 0x15, 0x6d, 0xf9, 0xde, 0x6b, 0x66, 0xed, 0xbf, 0xf6, 0xff,
    0xdb, 0xf9, 0xbf, 0xff, 0x2a, 0x6c, 0x6f, 0x66, 0xfa, 0xed, 0xb3, 0x3e, 0xf9, 0x1f, 0x19, 0xf3, 0x92, 0xff, 0xb7, 0xe7,
    0xff, 0x09, 0xdf, 0x4d, 0xb3, 0xf7, 0xff, 0xde, 0x31, 0xfe, 0x57, 0xf5, 0x8a, 0xf6, 0x7f, 0x6e, 0x8c, 0xfd, 0xee, 0x6d,
    0xcb, 0x1f, 0xf3, 0xb7, 0x1c, 0x85, 0xd1, 0x3e, 0x80, 0xfe, 0xbf, 0xfc, 0xbf, 0x27, 0xff, 0x47, 0x7a, 0xf6, 0xc1, 0xb3,
    0xae, 0x33, 0x46, 0x8e, 0xc7, 0x73, 0xf2, 0x5f, 0x79, 0xc7, 0xee, 0x29, 0xe3, 0x7f, 0x4f, 0xfb, 0x66, 0x78, 0x63, 0xfe,
    0xef, 0xbd, 0xd7, 0xda, 0x63, 0xe6, 0xef, 0x33, 0xfb, 0x32, 0x4f, 0xce, 0xff, 0xb3, 0xfb, 0x64, 0xc7, 0x6d, 0x57, 0x85,
    0x51, 0xb8, 0xa7, 0x79, 0x46, 0xfe, 0x63, 0x53, 0xfe, 0x47, 0xc6, 0x06, 0xde, 0x9d, 0xff, 0xf1, 0xd6, 0x2a, 0x6e, 0x1f,
    0x63, 0x96, 0xff, 0xbb, 0xf3, 0xbf, 0x7f, 0xeb, 0x77, 0xf5, 0xc4, 0x62, 0x49, 0xd2, 0xc7, 0x8f, 0xc9, 0x13, 0xfb, 0xa4,
    0x3b, 0xb7, 0x4d, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xef, 0xcc, 0x00, 0x1e, 0x7b, 0xb5,
    0x43, 0x05, 0xfd, 0x23, 0x5d, 0x6d, 0xbd, 0xee, 0xf3, 0x1e, 0x89, 0xaa, 0xee, 0x99, 0x6d, 0xe8, 0x5c, 0x41, 0xdf, 0xac,
    0xdb, 0xb7, 0x7e, 0x03, 0xf4, 0xae, 0xa0, 0x7f, 0x5e, 0xe5, 0x63, 0xfd, 0xe7, 0xad, 0x9c, 0x3f, 0xdf, 0xbb, 0x82, 0x7e,
    0xbf, 0xb5, 0xab, 0xf4, 0xfb, 0x26, 0xd9, 0x59, 0x41, 0x7f, 0xbc, 0xbf, 0x71, 0x6c, 0xdc, 0xae, 0x6c, 0x35, 0x83, 0xae,
    0x15, 0xf4, 0xbb, 0xaf, 0x05, 0xe3, 0xfe, 0xb5, 0x59, 0x63, 0xcf, 0xb4, 0xa8, 0xaa, 0xa0, 0xf1, 0xb4, 0x0a, 0xfa, 0x67,
    0x19, 0x7b, 0x52, 0x9d, 0x6c, 0xf9, 0xb7, 0xfe, 0xf7, 0xbe, 0xfc, 0xf7, 0xad, 0x93, 0x91, 0xad, 0x52, 0xd3, 0xbb, 0x82,
    0xa6, 0xfc, 0xcb, 0xbf, 0xfc, 0xaf, 0xaa, 0x91, 0x20, 0xff, 0x3c, 0x3d, 0xff, 0x3b, 0x2b, 0xe8, 0xa8, 0xa0, 0xb9, 0xbb,
    0x22, 0xd0, 0x21, 0xff, 0xae, 0xff, 0x1f, 0xf1, 0x04, 0x3d, 0xf9, 0x5f, 0xf3, 0x8a, 0xfc, 0x7f, 0xfd, 0xde, 0x9f, 0x0a,
    0xda, 0x2b, 0xf3, 0x1f, 0x4d, 0xc7, 0xff, 0xf6, 0xdf, 0xf1, 0xe1, 0x89, 0xf9, 0xd7, 0xff, 0xcf, 0xe7, 0xe2, 0xfe, 0x0a,
    0x9a, 0xf9, 0x34, 0xcb, 0xbf, 0xfe, 0x7f, 0xc5, 0xfc, 0x9f, 0x9a, 0xb6, 0x47, 0x05, 0xcd, 0xb9, 0x6d, 0x93, 0x7f, 0xd0,
    0x2b, 0xd4, 0xff, 0x07, 0xbd, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0xf9, 0xf3, 0x9f, 0xfd, 0x00, 0xf2, 0x0f, 0x7c, 0x2e, 0xff, 0xff, 0x00 };

// Font glyphs rectangles data (on atlas)
RAYGUIAPI const Rectangle terminalFontRecs[189] = {
    { 4, 4, 4 , 16 },
    { 16, 4, 1 , 11 },
    { 25, 4, 3 , 3 },
    { 36, 4, 6 , 11 },
    { 50, 4, 5 , 11 },
    { 63, 4, 5 , 11 },
    { 76, 4, 5 , 11 },
    { 89, 4, 1 , 2 },
    { 98, 4, 2 , 13 },
    { 108, 4, 2 , 13 },
    { 118, 4, 3 , 3 },
    { 129, 4, 5 , 5 },
    { 142, 4, 1 , 3 },
    { 151, 4, 5 , 1 },
    { 164, 4, 1 , 1 },
    { 173, 4, 6 , 12 },
    { 187, 4, 5 , 11 },
    { 200, 4, 2 , 11 },
    { 210, 4, 5 , 11 },
    { 223, 4, 5 , 11 },
    { 236, 4, 5 , 11 },
    { 249, 4, 5 , 11 },
    { 262, 4, 5 , 11 },
    { 275, 4, 5 , 11 },
    { 288, 4, 5 , 11 },
    { 301, 4, 5 , 11 },
    { 314, 4, 1 , 8 },
    { 323, 4, 1 , 10 },
    { 332, 4, 4 , 5 },
    { 344, 4, 5 , 3 },
    { 357, 4, 4 , 5 },
    { 369, 4, 5 , 11 },
    { 382, 4, 11 , 11 },
    { 401, 4, 5 , 11 },
    { 414, 4, 5 , 11 },
    { 427, 4, 5 , 11 },
    { 440, 4, 5 , 11 },
    { 453, 4, 5 , 11 },
    { 466, 4, 5 , 11 },
    { 479, 4, 5 , 11 },
    { 492, 4, 5 , 11 },
    { 4, 28, 1 , 11 },
    { 13, 28, 5 , 11 },
    { 26, 28, 5 , 11 },
    { 39, 28, 5 , 11 },
    { 52, 28, 7 , 11 },
    { 67, 28, 5 , 11 },
    { 80, 28, 5 , 11 },
    { 93, 28, 5 , 11 },
    { 106, 28, 5 , 13 },
    { 119, 28, 5 , 11 },
    { 132, 28, 5 , 11 },
    { 145, 28, 5 , 11 },
    { 158, 28, 5 , 11 },
    { 171, 28, 5 , 11 },
    { 184, 28, 7 , 11 },
    { 199, 28, 5 , 11 },
    { 212, 28, 5 , 11 },
    { 225, 28, 5 , 11 },
    { 238, 28, 2 , 13 },
    { 248, 28, 6 , 12 },
    { 262, 28, 2 , 13 },
    { 272, 28, 5 , 4 },
    { 285, 28, 5 , 1 },
    { 298, 28, 2 , 2 },
    { 308, 28, 5 , 8 },
    { 321, 28, 5 , 11 },
    { 334, 28, 5 , 8 },
    { 347, 28, 5 , 11 },
    { 360, 28, 5 , 8 },
    { 373, 28, 4 , 11 },
    { 385, 28, 5 , 10 },
    { 398, 28, 5 , 11 },
    { 411, 28, 1 , 11 },
    { 420, 28, 1 , 13 },
    { 429, 28, 5 , 11 },
    { 442, 28, 1 , 11 },
    { 451, 28, 7 , 8 },
    { 466, 28, 5 , 8 },
    { 479, 28, 5 , 8 },
    { 492, 28, 5 , 10 },
    { 4, 52, 5 , 10 },
    { 17, 52, 4 , 8 },
    { 29, 52, 5 , 8 },
    { 42, 52, 3 , 11 },
    { 53, 52, 5 , 8 },
    { 66, 52, 5 , 8 },
    { 79, 52, 7 , 8 },
    { 94, 52, 5 , 8 },
    { 107, 52, 5 , 10 },
    { 120, 52, 5 , 8 },
    { 133, 52, 3 , 13 },
    { 144, 52, 1 , 15 },
    { 153, 52, 3 , 13 },
    { 164, 52, 5 , 3 },
    { 177, 52, 1 , 11 },
    { 186, 52, 5 , 11 },
    { 199, 52, 5 , 10 },
    { 212, 52, 5 , 10 },
    { 225, 52, 5 , 10 },
    { 238, 52, 0 , 0 },
    { 246, 52, 0 , 0 },
    { 254, 52, 0 , 0 },
    { 262, 52, 7 , 8 },
    { 277, 52, 0 , 0 },
    { 285, 52, 0 , 0 },
    { 293, 52, 5 , 3 },
    { 306, 52, 7 , 8 },
    { 321, 52, 5 , 1 },
    { 334, 52, 3 , 3 },
    { 345, 52, 5 , 7 },
    { 358, 52, 0 , 0 },
    { 366, 52, 0 , 0 },
    { 374, 52, 0 , 0 },
    { 382, 52, 5 , 10 },
    { 395, 52, 7 , 11 },
    { 410, 52, 1 , 1 },
    { 419, 52, 0 , 0 },
    { 427, 52, 0 , 0 },
    { 435, 52, 0 , 0 },
    { 443, 52, 0 , 0 },
    { 451, 52, 0 , 0 },
    { 459, 52, 0 , 0 },
    { 467, 52, 5 , 13 },
    { 480, 52, 5 , 11 },
    { 493, 52, 5 , 14 },
    { 4, 76, 5 , 14 },
    { 17, 76, 5 , 14 },
    { 30, 76, 5 , 14 },
    { 43, 76, 5 , 13 },
    { 56, 76, 5 , 13 },
    { 69, 76, 9 , 11 },
    { 86, 76, 5 , 13 },
    { 99, 76, 5 , 14 },
    { 112, 76, 5 , 14 },
    { 125, 76, 5 , 14 },
    { 138, 76, 5 , 13 },
    { 151, 76, 2 , 14 },
    { 161, 76, 2 , 14 },
    { 171, 76, 3 , 14 },
    { 182, 76, 3 , 13 },
    { 193, 76, 5 , 11 },
    { 206, 76, 5 , 14 },
    { 219, 76, 5 , 14 },
    { 232, 76, 5 , 14 },
    { 245, 76, 5 , 14 },
    { 258, 76, 5 , 14 },
    { 271, 76, 5 , 13 },
    { 284, 76, 5 , 5 },
    { 297, 76, 5 , 13 },
    { 310, 76, 5 , 14 },
    { 323, 76, 5 , 14 },
    { 336, 76, 5 , 14 },
    { 349, 76, 5 , 13 },
    { 362, 76, 5 , 14 },
    { 375, 76, 5 , 11 },
    { 388, 76, 5 , 11 },
    { 401, 76, 5 , 11 },
    { 414, 76, 5 , 11 },
    { 427, 76, 5 , 11 },
    { 440, 76, 5 , 11 },
    { 453, 76, 5 , 10 },
    { 466, 76, 5 , 10 },
    { 479, 76, 9 , 8 },
    { 496, 76, 5 , 10 },
    { 4, 100, 5 , 11 },
    { 17, 100, 5 , 11 },
    { 30, 100, 5 , 11 },
    { 43, 100, 5 , 10 },
    { 56, 100, 2 , 11 },
    { 66, 100, 2 , 11 },
    { 76, 100, 3 , 11 },
    { 87, 100, 3 , 10 },
    { 98, 100, 5 , 11 },
    { 111, 100, 5 , 11 },
    { 124, 100, 5 , 11 },
    { 137, 100, 5 , 11 },
    { 150, 100, 5 , 11 },
    { 163, 100, 5 , 11 },
    { 176, 100, 5 , 10 },
    { 189, 100, 5 , 5 },
    { 202, 100, 5 , 10 },
    { 215, 100, 5 , 11 },
    { 228, 100, 5 , 11 },
    { 241, 100, 5 , 11 },
    { 254, 100, 5 , 10 },
    { 267, 100, 5 , 13 },
    { 280, 100, 4 , 8 },
    { 292, 100, 5 , 12 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
RAYGUIAPI const GlyphInfo terminalFontGlyphs[189] = {
    { 32, 0, 14, 4, { 0 }},
    { 33, 1, 3, 3, { 0 }},
    { 34, 1, 3, 5, { 0 }},
    { 35, 1, 3, 8, { 0 }},
    { 36, 1, 3, 7, { 0 }},
    { 37, 1, 3, 7, { 0 }},
    { 38, 1, 3, 7, { 0 }},
    { 39, 1, 3, 3, { 0 }},
    { 40, 1, 2, 4, { 0 }},
    { 41, 1, 2, 4, { 0 }},
    { 42, 1, 3, 5, { 0 }},
    { 43, 1, 7, 7, { 0 }},
    { 44, 1, 13, 3, { 0 }},
    { 45, 1, 9, 7, { 0 }},
    { 46, 1, 13, 3, { 0 }},
    { 47, 1, 2, 8, { 0 }},
    { 48, 1, 3, 7, { 0 }},
    { 49, 1, 3, 4, { 0 }},
    { 50, 1, 3, 7, { 0 }},
    { 51, 1, 3, 7, { 0 }},
    { 52, 1, 3, 7, { 0 }},
    { 53, 1, 3, 7, { 0 }},
    { 54, 1, 3, 7, { 0 }},
    { 55, 1, 3, 7, { 0 }},
    { 56, 1, 3, 7, { 0 }},
    { 57, 1, 3, 7, { 0 }},
    { 58, 1, 6, 3, { 0 }},
    { 59, 1, 6, 3, { 0 }},
    { 60, 1, 7, 6, { 0 }},
    { 61, 1, 8, 7, { 0 }},
    { 62, 1, 7, 6, { 0 }},
    { 63, 1, 3, 7, { 0 }},
    { 64, 2, 3, 15, { 0 }},
    { 65, 1, 3, 7, { 0 }},
    { 66, 1, 3, 7, { 0 }},
    { 67, 1, 3, 7, { 0 }},
    { 68, 1, 3, 7, { 0 }},
    { 69, 1, 3, 7, { 0 }},
    { 70, 1, 3, 7, { 0 }},
    { 71, 1, 3, 7, { 0 }},
    { 72, 1, 3, 7, { 0 }},
    { 73, 1, 3, 3, { 0 }},
    { 74, 1, 3, 7, { 0 }},
    { 75, 1, 3, 7, { 0 }},
    { 76, 1, 3, 7, { 0 }},
    { 77, 1, 3, 9, { 0 }},
    { 78, 1, 3, 7, { 0 }},
    { 79, 1, 3, 7, { 0 }},
    { 80, 1, 3, 7, { 0 }},
    { 81, 1, 3, 7, { 0 }},
    { 82, 1, 3, 7, { 0 }},
    { 83, 1, 3, 7, { 0 }},
    { 84, 1, 3, 7, { 0 }},
    { 85, 1, 3, 7, { 0 }},
    { 86, 1, 3, 7, { 0 }},
    { 87, 1, 3, 9, { 0 }},
    { 88, 1, 3, 7, { 0 }},
    { 89, 1, 3, 7, { 0 }},
    { 90, 1, 3, 7, { 0 }},
    { 91, 1, 2, 4, { 0 }},
    { 92, 1, 2, 8, { 0 }},
    { 93, 1, 2, 4, { 0 }},
    { 94, 1, 3, 7, { 0 }},
    { 95, 1, 15, 7, { 0 }},
    { 96, 1, 0, 4, { 0 }},
    { 97, 1, 6, 7, { 0 }},
    { 98, 1, 3, 7, { 0 }},
    { 99, 1, 6, 7, { 0 }},
    { 100, 1, 3, 7, { 0 }},
    { 101, 1, 6, 7, { 0 }},
    { 102, 1, 3, 6, { 0 }},
    { 103, 1, 6, 7, { 0 }},
    { 104, 1, 3, 7, { 0 }},
    { 105, 1, 3, 3, { 0 }},
    { 106, 1, 3, 3, { 0 }},
    { 107, 1, 3, 7, { 0 }},
    { 108, 1, 3, 3, { 0 }},
    { 109, 1, 6, 9, { 0 }},
    { 110, 1, 6, 7, { 0 }},
    { 111, 1, 6, 7, { 0 }},
    { 112, 1, 6, 7, { 0 }},
    { 113, 1, 6, 7, { 0 }},
    { 114, 1, 6, 6, { 0 }},
    { 115, 1, 6, 7, { 0 }},
    { 116, 1, 3, 5, { 0 }},
    { 117, 1, 6, 7, { 0 }},
    { 118, 1, 6, 7, { 0 }},
    { 119, 1, 6, 9, { 0 }},
    { 120, 1, 6, 7, { 0 }},
    { 121, 1, 6, 7, { 0 }},
    { 122, 1, 6, 7, { 0 }},
    { 123, 1, 2, 5, { 0 }},
    { 124, 1, 1, 3, { 0 }},
    { 125, 1, 2, 5, { 0 }},
    { 126, 1, 8, 7, { 0 }},
    { 161, 1, 3, 3, { 0 }},
    { 162, 1, 3, 7, { 0 }},
    { 163, 1, 3, 7, { 0 }},
    { 8364, 1, 3, 7, { 0 }},
    { 165, 1, 3, 7, { 0 }},
    { 352, 0, 14, 4, { 0 }},
    { 167, 0, 14, 4, { 0 }},
    { 353, 0, 14, 4, { 0 }},
    { 169, 1, 3, 9, { 0 }},
    { 170, 0, 14, 4, { 0 }},
    { 171, 0, 14, 4, { 0 }},
    { 172, 1, 8, 7, { 0 }},
    { 174, 1, 3, 9, { 0 }},
    { 175, 1, 1, 7, { 0 }},
    { 176, 1, 0, 5, { 0 }},
    { 177, 1, 7, 7, { 0 }},
    { 178, 0, 14, 4, { 0 }},
    { 179, 0, 14, 4, { 0 }},
    { 381, 0, 14, 4, { 0 }},
    { 181, 1, 6, 7, { 0 }},
    { 182, 1, 3, 9, { 0 }},
    { 183, 1, 8, 3, { 0 }},
    { 382, 0, 14, 4, { 0 }},
    { 185, 0, 14, 4, { 0 }},
    { 186, 0, 14, 4, { 0 }},
    { 187, 0, 14, 4, { 0 }},
    { 338, 0, 14, 4, { 0 }},
    { 339, 0, 14, 4, { 0 }},
    { 376, 1, 1, 7, { 0 }},
    { 191, 1, 3, 7, { 0 }},
    { 192, 1, 0, 7, { 0 }},
    { 193, 1, 0, 7, { 0 }},
    { 194, 1, 0, 7, { 0 }},
    { 195, 1, 0, 7, { 0 }},
    { 196, 1, 1, 7, { 0 }},
    { 197, 1, 1, 7, { 0 }},
    { 198, 1, 3, 11, { 0 }},
    { 199, 1, 3, 7, { 0 }},
    { 200, 1, 0, 7, { 0 }},
    { 201, 1, 0, 7, { 0 }},
    { 202, 1, 0, 7, { 0 }},
    { 203, 1, 1, 7, { 0 }},
    { 204, 0, 0, 3, { 0 }},
    { 205, 1, 0, 3, { 0 }},
    { 206, 0, 0, 3, { 0 }},
    { 207, 0, 1, 3, { 0 }},
    { 208, 1, 3, 7, { 0 }},
    { 209, 1, 0, 7, { 0 }},
    { 210, 1, 0, 7, { 0 }},
    { 211, 1, 0, 7, { 0 }},
    { 212, 1, 0, 7, { 0 }},
    { 213, 1, 0, 7, { 0 }},
    { 214, 1, 1, 7, { 0 }},
    { 215, 1, 7, 7, { 0 }},
    { 216, 1, 2, 7, { 0 }},
    { 217, 1, 0, 7, { 0 }},
    { 218, 1, 0, 7, { 0 }},
    { 219, 1, 0, 7, { 0 }},
    { 220, 1, 1, 7, { 0 }},
    { 221, 1, 0, 7, { 0 }},
    { 222, 1, 3, 7, { 0 }},
    { 223, 1, 3, 7, { 0 }},
    { 224, 1, 3, 7, { 0 }},
    { 225, 1, 3, 7, { 0 }},
    { 226, 1, 3, 7, { 0 }},
    { 227, 1, 3, 7, { 0 }},
    { 228, 1, 4, 7, { 0 }},
    { 229, 1, 4, 7, { 0 }},
    { 230, 1, 6, 11, { 0 }},
    { 231, 1, 6, 7, { 0 }},
    { 232, 1, 3, 7, { 0 }},
    { 233, 1, 3, 7, { 0 }},
    { 234, 1, 3, 7, { 0 }},
    { 235, 1, 4, 7, { 0 }},
    { 236, 0, 3, 3, { 0 }},
    { 237, 1, 3, 3, { 0 }},
    { 238, 0, 3, 3, { 0 }},
    { 239, 0, 4, 3, { 0 }},
    { 240, 1, 3, 7, { 0 }},
    { 241, 1, 3, 7, { 0 }},
    { 242, 1, 3, 7, { 0 }},
    { 243, 1, 3, 7, { 0 }},
    { 244, 1, 3, 7, { 0 }},
    { 245, 1, 3, 7, { 0 }},
    { 246, 1, 4, 7, { 0 }},
    { 247, 1, 7, 7, { 0 }},
    { 248, 1, 5, 7, { 0 }},
    { 249, 1, 3, 7, { 0 }},
    { 250, 1, 3, 7, { 0 }},
    { 251, 1, 3, 7, { 0 }},
    { 252, 1, 4, 7, { 0 }},
    { 253, 1, 3, 7, { 0 }},
    { 254, 1, 6, 6, { 0 }},
    { 255, 1, 4, 7, { 0 }},
};

// Style loading function: Terminal
RAYGUIAPI void GuiLoadStyleTerminal(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < TERMINAL_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(terminalStyleProps[i].controlId, terminalStyleProps[i].propertyId, terminalStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int terminalFontDataSize = 0;
    unsigned char *data = DecompressData(terminalFontData, TERMINAL_STYLE_FONT_ATLAS_COMP_SIZE, &terminalFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 16;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, terminalFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, terminalFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleSunny();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define SUNNY_STYLE_PROPS_COUNT  33

// Custom style name: Sunny
RAYGUIAPI const GuiStyleProp sunnyStyleProps[SUNNY_STYLE_PROPS_COUNT] = {
    { 0, 0, 0x9c760aff },    // DEFAULT_BORDER_COLOR_NORMAL
    { 0, 1, 0x594006ff },    // DEFAULT_BASE_COLOR_NORMAL
    { 0, 2, 0xf6d519ff },    // DEFAULT_TEXT_COLOR_NORMAL
    { 0, 3, 0xf6ee89ff },    // DEFAULT_BORDER_COLOR_FOCUSED
    { 0, 4, 0xf5f3d1ff },    // DEFAULT_BASE_COLOR_FOCUSED
    { 0, 5, 0xf4cd19ff },    // DEFAULT_TEXT_COLOR_FOCUSED
    { 0, 6, 0xf7e580ff },    // DEFAULT_BORDER_COLOR_PRESSED
    { 0, 7, 0xf7f2c1ff },    // DEFAULT_BASE_COLOR_PRESSED
    { 0, 8, 0x52470aff },    // DEFAULT_TEXT_COLOR_PRESSED
    { 0, 9, 0xc0be92ff },    // DEFAULT_BORDER_COLOR_DISABLED
    { 0, 10, 0xd3d3a1ff },    // DEFAULT_BASE_COLOR_DISABLED
    { 0, 11, 0xbcbc89ff },    // DEFAULT_TEXT_COLOR_DISABLED
    { 0, 16, 0x00000010 },    // DEFAULT_TEXT_SIZE
    { 0, 17, 0x00000000 },    // DEFAULT_TEXT_SPACING
    { 0, 18, 0x725706ff },    // DEFAULT_LINE_COLOR
    { 0, 19, 0xf0be4bff },    // DEFAULT_BACKGROUND_COLOR
    { 0, 20, 0x00000018 },    // DEFAULT_TEXT_LINE_SPACING
    { 1, 2, 0x504506ff },    // LABEL_TEXT_COLOR_NORMAL
    { 1, 5, 0xfdeb9bff },    // LABEL_TEXT_COLOR_FOCUSED
    { 1, 8, 0xf5e8a4ff },    // LABEL_TEXT_COLOR_PRESSED
    { 2, 2, 0xebc21fff },    // BUTTON_TEXT_COLOR_NORMAL
    { 3, 2, 0xebc21fff },    // TOGGLE_TEXT_COLOR_NORMAL
    { 4, 2, 0x81700fff },    // SLIDER_TEXT_COLOR_NORMAL
    { 4, 5, 0xf4e49aff },    // SLIDER_TEXT_COLOR_FOCUSED
    { 7, 2, 0xebc21fff },    // COMBOBOX_TEXT_COLOR_NORMAL
    { 8, 2, 0xefd87bff },    // DROPDOWNBOX_TEXT_COLOR_NORMAL
    { 8, 5, 0xd4b219ff },    // DROPDOWNBOX_TEXT_COLOR_FOCUSED
    { 9, 2, 0x7a680bff },    // TEXTBOX_TEXT_COLOR_NORMAL
    { 9, 5, 0xad931fff },    // TEXTBOX_TEXT_COLOR_FOCUSED
    { 10, 2, 0x62570eff },    // VALUEBOX_TEXT_COLOR_NORMAL
    { 10, 5, 0xf2df88ff },    // VALUEBOX_TEXT_COLOR_FOCUSED
    { 12, 2, 0xf4e798ff },    // LISTVIEW_TEXT_COLOR_NORMAL
    { 15, 2, 0xebc21fff },    // STATUSBAR_TEXT_COLOR_NORMAL
};

// WARNING: This style uses a custom font: "GenericMobileSystemNuevo.ttf" (size: 16, spacing: 0)

#define SUNNY_STYLE_FONT_ATLAS_COMP_SIZE 2462

// Font atlas image pixels data: DEFLATE compressed
RAYGUIAPI unsigned char sunnyFontData[SUNNY_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0x59, 0xb2, 0xeb, 0x34, 0x10, 0x00, 0x50, 0x6d, 0x83, 0xfd, 0xef, 0x8d, 0x6d, 0x88, 0xa2, 0x28, 0x0a, 0xb8, 0x8f,
    0x58, 0x52, 0x77, 0xcb, 0x53, 0x0e, 0xa7, 0xf8, 0xb9, 0x7e, 0x49, 0x6c, 0xd9, 0x6d, 0x0d, 0xb6, 0x5a, 0xbd, 0x01, 0x00,
    0x00, 0x00, 0x5f, 0xef, 0xcf, 0xff, 0xfe, 0xff, 0x6f, 0xff, 0xb7, 0xe5, 0x78, 0xdb, 0xdf, 0xff, 0x62, 0xb4, 0xa5, 0x7f,
    0xdc, 0x93, 0x7e, 0xb0, 0x6d, 0xee, 0xb7, 0x62, 0xfb, 0xd5, 0x97, 0xca, 0xa6, 0x1f, 0x94, 0x5a, 0x4b, 0xff, 0xbd, 0x1d,
    0x96, 0x7d, 0x0f, 0x94, 0x5e, 0x3b, 0xdc, 0xe7, 0xcf, 0x9f, 0x1c, 0x6f, 0xa9, 0xdc, 0xcf, 0x95, 0x32, 0xad, 0x3c, 0x3f,
    0x2b, 0x25, 0xd8, 0x96, 0xb7, 0x1e, 0x97, 0x54, 0x9b, 0x2a, 0xc7, 0xf1, 0x37, 0xf7, 0x9b, 0xc4, 0xff, 0xf1, 0xd1, 0xfc,
    0x1d, 0x83, 0x47, 0x67, 0x7f, 0xfe, 0xda, 0x68, 0xd3, 0xf7, 0x84, 0x7f, 0x7e, 0xbb, 0x2e, 0x42, 0xb3, 0xdf, 0xd2, 0x87,
    0x77, 0xc2, 0x95, 0x12, 0x3a, 0x2e, 0xbd, 0x16, 0x8a, 0x89, 0xd1, 0x77, 0x46, 0xf6, 0x65, 0x66, 0xdb, 0xdc, 0xfe, 0xc5,
    0xae, 0xf9, 0xf1, 0xf7, 0x1d, 0x5f, 0x69, 0x47, 0x67, 0x78, 0x74, 0xe5, 0x1e, 0x1f, 0xdf, 0xfa, 0x59, 0xcd, 0x44, 0xfd,
    0x9e, 0xf8, 0xef, 0xff, 0x8a, 0xb3, 0x1e, 0x8c, 0xc1, 0x7c, 0xdd, 0xb0, 0x5e, 0x2b, 0x1c, 0xdf, 0xb3, 0xf6, 0x95, 0xf0,
    0x51, 0x39, 0xf5, 0xc0, 0x35, 0xbc, 0x7e, 0xce, 0xa2, 0xbf, 0xb3, 0x23, 0xfe, 0x2b, 0xa2, 0xb8, 0x4d, 0xd4, 0x88, 0x91,
    0x36, 0xc5, 0x71, 0x8b, 0x33, 0x1f, 0xe1, 0xa3, 0x7d, 0x89, 0xb4, 0x67, 0xf2, 0x65, 0xb9, 0xda, 0xe6, 0xd8, 0x17, 0xff,
    0x15, 0xf5, 0xed, 0x7a, 0x5d, 0xb2, 0xaf, 0x7c, 0x67, 0xea, 0xeb, 0xfb, 0xc7, 0xff, 0xfa, 0xb1, 0x55, 0xc4, 0x7f, 0xcd,
    0x59, 0x59, 0x8f, 0xff, 0x2e, 0xfe, 0xa7, 0xeb, 0xff, 0xcf, 0xa5, 0xde, 0x83, 0xfd, 0xaf, 0x7c, 0xaf, 0xbd, 0xaa, 0x9d,
    0x7f, 0x6d, 0xfc, 0x8f, 0xfa, 0x83, 0x6d, 0x39, 0xfe, 0x63, 0xbd, 0xcf, 0x48, 0x6f, 0xa3, 0xaa, 0xfd, 0xbf, 0xb7, 0xf5,
    0x70, 0xdc, 0x7b, 0x8c, 0x44, 0xf8, 0xda, 0x98, 0x47, 0xcd, 0x37, 0x5f, 0x11, 0xff, 0x6d, 0x62, 0x6f, 0xfa, 0xb0, 0xff,
    0x3f, 0x7b, 0x87, 0x5f, 0x8b, 0xff, 0xba, 0xba, 0xe3, 0xca, 0xf8, 0xef, 0xc1, 0x11, 0xb1, 0xea, 0x7e, 0x7c, 0xfc, 0xd8,
    0xd6, 0xc6, 0xe7, 0xce, 0xeb, 0xff, 0x8f, 0xda, 0x81, 0x99, 0xf8, 0xff, 0x96, 0xf6, 0xff, 0x4c, 0x4b, 0x7b, 0x54, 0x8a,
    0xf3, 0xf7, 0xb3, 0x2b, 0x6a, 0xf3, 0x6b, 0xe3, 0xbf, 0x4f, 0x8d, 0x61, 0xdf, 0x3b, 0xfe, 0xdb, 0x72, 0x2f, 0xaf, 0xf2,
    0xda, 0x8d, 0x5e, 0x51, 0xa3, 0x2b, 0x77, 0xe6, 0xae, 0x76, 0xcf, 0xf8, 0x8f, 0x5d, 0xcf, 0x7d, 0xd8, 0xca, 0xef, 0x89,
    0xf8, 0x6f, 0xaf, 0x89, 0xff, 0x1e, 0x1c, 0xed, 0xf8, 0xdc, 0xde, 0x8b, 0x8d, 0xc7, 0x45, 0x5a, 0xf2, 0x99, 0x36, 0x4a,
    0x55, 0xfc, 0xdf, 0xe3, 0x4c, 0xaa, 0xff, 0x2b, 0xc6, 0xff, 0xdb, 0x61, 0x0b, 0xff, 0xbb, 0xe2, 0xff, 0xf8, 0xf9, 0x5f,
    0xbb, 0xfd, 0xb3, 0xba, 0x68, 0xfc, 0xd7, 0x8c, 0xff, 0x9f, 0x7f, 0xb7, 0x9e, 0xe9, 0xb9, 0xe6, 0xe2, 0xff, 0x4e, 0xcf,
    0xff, 0xf6, 0xc6, 0xff, 0xe8, 0xa9, 0xf7, 0x9b, 0xda, 0xff, 0x95, 0xe3, 0xf8, 0xf1, 0x77, 0x6e, 0xf6, 0xbd, 0x53, 0xd4,
    0x42, 0x6f, 0x72, 0xdd, 0x33, 0xfa, 0xa3, 0xcf, 0x81, 0x72, 0xa3, 0x74, 0x57, 0xbf, 0xff, 0x53, 0x5f, 0x82, 0xe7, 0xed,
    0x4f, 0xbf, 0x71, 0x9b, 0x91, 0x77, 0xbd, 0x25, 0xeb, 0xea, 0x3a, 0xf3, 0xf9, 0x6c, 0xfc, 0xb7, 0x9c, 0x1f, 0x76, 0xd6,
    0x6c, 0x4a, 0xe2, 0x19, 0x63, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xe4, 0xf9,
    0x0b, 0x91, 0xb9, 0x86, 0xd1, 0x3c, 0x04, 0xb5, 0xf9, 0x10, 0xfa, 0x8f, 0x3c, 0xac, 0xab, 0xdf, 0x3d, 0x3b, 0xd7, 0x32,
    0x33, 0x4f, 0xb3, 0xf6, 0xb3, 0x55, 0x99, 0x25, 0x6a, 0x67, 0x9e, 0x8f, 0xf3, 0xdd, 0xac, 0x66, 0x6b, 0x8e, 0x65, 0x41,
    0x5b, 0xcf, 0x69, 0x9e, 0xc9, 0xc3, 0x57, 0x3d, 0xb7, 0xba, 0x26, 0x3f, 0xda, 0xfc, 0x9c, 0xf7, 0x9a, 0x4c, 0x23, 0x75,
    0xf1, 0xbf, 0x7e, 0x1e, 0xfa, 0x8f, 0xfd, 0xe8, 0x17, 0x64, 0x62, 0xa8, 0xdf, 0x3a, 0xf7, 0xf7, 0x3b, 0xc5, 0xff, 0xec,
    0x3d, 0x38, 0x7b, 0xf5, 0xc6, 0x67, 0xdc, 0x8d, 0xb2, 0x4f, 0x5c, 0x9f, 0x5b, 0xa5, 0x15, 0xe5, 0x47, 0xad, 0x8f, 0xff,
    0x76, 0x52, 0xfd, 0x1f, 0x8d, 0xff, 0x3e, 0x5c, 0xcd, 0xe6, 0xbe, 0xf1, 0xbf, 0x9e, 0x77, 0x22, 0xfb, 0xab, 0xb9, 0x95,
    0x1c, 0xe2, 0xb9, 0xc6, 0xf3, 0xf1, 0x9f, 0xcb, 0x14, 0x74, 0x6e, 0xdd, 0x7a, 0x7d, 0xfc, 0x47, 0x4b, 0xe4, 0xaa, 0xf8,
    0x8f, 0x5f, 0x25, 0x7d, 0x98, 0xcd, 0xfa, 0x8e, 0xf1, 0xdf, 0xbf, 0x3e, 0xfe, 0xeb, 0x73, 0xf9, 0x46, 0x23, 0x20, 0x92,
    0x07, 0x7d, 0xa6, 0x5f, 0xbd, 0xda, 0xb3, 0x8b, 0xf7, 0xbb, 0xa2, 0xf5, 0xff, 0xe8, 0xf3, 0x33, 0x47, 0x72, 0x75, 0xfc,
    0xb7, 0x61, 0x84, 0xf7, 0x44, 0x94, 0xc6, 0x47, 0x40, 0x32, 0x39, 0xaf, 0xae, 0x89, 0xff, 0x16, 0x5a, 0x31, 0x30, 0x5e,
    0xcf, 0xdf, 0x3d, 0xfe, 0xab, 0xfb, 0x06, 0xd1, 0x11, 0x83, 0xea, 0x7d, 0xbc, 0x5f, 0xff, 0xbf, 0x17, 0xb4, 0x12, 0x77,
    0xc4, 0x7f, 0xb6, 0x7c, 0xa2, 0xd1, 0x14, 0x8b, 0xff, 0xf9, 0xda, 0x3d, 0xb6, 0x9a, 0x40, 0x6c, 0xdb, 0x7b, 0xe3, 0xbf,
    0x36, 0x7f, 0xea, 0xfa, 0x58, 0xf1, 0x9b, 0xe2, 0x3f, 0xda, 0xff, 0x9f, 0xef, 0x71, 0xdd, 0x2b, 0xfe, 0xdb, 0xc4, 0xfa,
    0x3c, 0x67, 0xc7, 0xff, 0x28, 0xdb, 0xea, 0xdb, 0xe2, 0x3f, 0x1a, 0x5b, 0xd1, 0xf5, 0x8c, 0x5a, 0xb8, 0xa6, 0x7b, 0x7b,
    0xfd, 0xdf, 0x5e, 0x1a, 0xff, 0xf1, 0xf5, 0x65, 0x9e, 0x15, 0xff, 0xd5, 0xad, 0x86, 0xeb, 0xf3, 0xea, 0x45, 0x47, 0x1b,
    0xe3, 0xa3, 0x94, 0xef, 0x6e, 0xff, 0xf7, 0x0d, 0xad, 0xf0, 0xec, 0xf3, 0xff, 0xab, 0xe2, 0x7f, 0xd7, 0xf3, 0xbf, 0x36,
    0xbd, 0xe6, 0xdf, 0xae, 0xbd, 0x12, 0xff, 0xd5, 0xf1, 0x5f, 0xb9, 0x76, 0xe2, 0xda, 0xbf, 0xa8, 0x7e, 0xff, 0xa7, 0x7e,
    0x24, 0x6d, 0xff, 0xfb, 0x3f, 0x7b, 0xca, 0xa7, 0x4d, 0xad, 0x98, 0xb6, 0xfe, 0xab, 0x6d, 0x72, 0x55, 0xe3, 0xd8, 0x3e,
    0xd7, 0xbf, 0x7d, 0x73, 0xf7, 0xf8, 0x8f, 0xbd, 0x6d, 0x10, 0xfd, 0x64, 0xfc, 0x13, 0xf7, 0x59, 0x0b, 0xec, 0x3b, 0xdf,
    0xa2, 0x54, 0x0a, 0xd5, 0x25, 0xa4, 0x54, 0x71, 0x07, 0x78, 0xd7, 0x7b, 0xe6, 0xbb, 0xfe, 0x3d, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf7, 0xcd, 0x5e, 0x99, 0xcd, 0xb9, 0xfd, 0xeb, 0x5f, 0x7b, 0x68, 0xbe,
    0x6b, 0x4f, 0x7e, 0xae, 0x2f, 0xe7, 0xc1, 0x58, 0x9b, 0x77, 0x5a, 0x91, 0x83, 0xa1, 0x3a, 0x8f, 0xd4, 0xb8, 0x5c, 0x66,
    0xcf, 0xf4, 0x78, 0x26, 0xf1, 0xea, 0x15, 0x12, 0x99, 0xd7, 0x93, 0xc9, 0xa8, 0x93, 0x59, 0x4b, 0x20, 0x9e, 0x09, 0xb1,
    0x2d, 0x7e, 0x6b, 0xec, 0xac, 0xfd, 0x3a, 0xbf, 0x3a, 0x9a, 0xef, 0xb3, 0x26, 0xfe, 0x63, 0xb9, 0x25, 0xaf, 0xc8, 0xb4,
    0x16, 0xbf, 0x9a, 0x6a, 0xf7, 0xb0, 0xbe, 0x54, 0xe6, 0xf6, 0x36, 0xf7, 0xf7, 0xcc, 0x9c, 0xfe, 0x5c, 0xee, 0x81, 0xd5,
    0x92, 0xe8, 0x53, 0xd9, 0x52, 0x56, 0x3f, 0xf9, 0xf9, 0x73, 0xc7, 0xdb, 0xfa, 0x54, 0xe6, 0x88, 0xd5, 0x6f, 0xfd, 0x6d,
    0xf0, 0xf9, 0x7e, 0x78, 0xe7, 0xad, 0x9d, 0xd1, 0xfa, 0x94, 0xf8, 0x5f, 0xbf, 0x77, 0x9f, 0x1d, 0xff, 0xf1, 0xfb, 0x54,
    0x2b, 0xcc, 0xd2, 0xb3, 0xbe, 0x36, 0x46, 0x64, 0xe6, 0x6e, 0x3e, 0x47, 0xd0, 0x19, 0x19, 0x35, 0x7e, 0x8d, 0xa6, 0x95,
    0xb3, 0x36, 0x17, 0xfd, 0x9f, 0xe2, 0xf7, 0xa8, 0xb5, 0xb5, 0x2f, 0xfe, 0x6b, 0x32, 0xf2, 0x3d, 0x27, 0xfe, 0x67, 0x7a,
    0x1b, 0xe7, 0xc5, 0xff, 0xf8, 0xef, 0x75, 0x59, 0xb5, 0x2b, 0x73, 0xed, 0x8c, 0x72, 0x2d, 0xb7, 0xd0, 0x19, 0xe9, 0xc1,
    0xfb, 0x4a, 0x2f, 0xce, 0x67, 0xde, 0x7f, 0xfc, 0x3f, 0x5b, 0x53, 0xcf, 0xc6, 0xff, 0xfa, 0x2f, 0xfe, 0x36, 0xd1, 0x7e,
    0xa8, 0x6e, 0xff, 0xaf, 0x5c, 0x2f, 0x4f, 0x8a, 0xff, 0x36, 0x95, 0x8b, 0xec, 0xde, 0xf5, 0x7f, 0x65, 0x4e, 0xad, 0xfa,
    0xf8, 0x8f, 0x45, 0x78, 0xa4, 0x26, 0x9f, 0x5f, 0xf1, 0x60, 0xed, 0xbc, 0x8d, 0x5b, 0xf9, 0x3d, 0xd1, 0xfe, 0xdf, 0x11,
    0xff, 0xd5, 0xe3, 0x7f, 0xf1, 0x35, 0x3c, 0xf6, 0x8c, 0x74, 0x45, 0xc6, 0xff, 0xda, 0x09, 0xfd, 0xff, 0x51, 0x3b, 0x31,
    0x5a, 0x2a, 0xf5, 0x6b, 0x53, 0x55, 0x66, 0xdb, 0x5e, 0x8d, 0xff, 0x4c, 0xfd, 0x1f, 0x1d, 0x87, 0xc8, 0xb4, 0x10, 0xdb,
    0x60, 0xe4, 0x60, 0xf4, 0x9b, 0xeb, 0x59, 0x43, 0x73, 0xed, 0xff, 0x1d, 0xf1, 0x7f, 0x56, 0xff, 0xbf, 0x25, 0x47, 0x35,
    0xeb, 0xda, 0xff, 0x2d, 0xd1, 0xca, 0xcc, 0xdc, 0x6d, 0x22, 0xa5, 0xbd, 0x5e, 0x66, 0x57, 0xd6, 0xff, 0xb9, 0xd5, 0xe6,
    0xda, 0xb6, 0x7c, 0xa7, 0x91, 0xf6, 0xff, 0x9e, 0xa7, 0x70, 0xcf, 0xae, 0xff, 0xcf, 0x7c, 0x3a, 0xb9, 0x73, 0x7f, 0xee,
    0x93, 0x77, 0x6e, 0x4f, 0x1e, 0xe9, 0x3b, 0xf4, 0xff, 0x23, 0xe3, 0xff, 0xd1, 0x9c, 0xc7, 0xe3, 0xda, 0xb8, 0x0f, 0x46,
    0xd3, 0xbe, 0x35, 0xfe, 0xef, 0x91, 0x9b, 0xf9, 0x7b, 0xe3, 0xbf, 0xa5, 0xf2, 0x4f, 0x5f, 0x11, 0xff, 0x73, 0x2d, 0xab,
    0xd8, 0x8a, 0x88, 0xb9, 0x15, 0x93, 0xa3, 0x7d, 0xba, 0x76, 0xea, 0x95, 0x9e, 0x79, 0xfe, 0xb7, 0xe7, 0xfd, 0x9f, 0xda,
    0x37, 0x0d, 0x9e, 0xfb, 0x3e, 0xd4, 0x3b, 0x8e, 0xe0, 0x8c, 0xfb, 0xf9, 0x78, 0xd5, 0x82, 0xbe, 0xe9, 0x98, 0x62, 0x4f,
    0x75, 0xa3, 0xcf, 0x89, 0xf7, 0xdd, 0xef, 0xa3, 0xef, 0xff, 0x44, 0xcb, 0x54, 0xa6, 0xe5, 0x6f, 0xb9, 0x83, 0x5d, 0x19,
    0xff, 0xfb, 0x5b, 0x49, 0x55, 0xef, 0x30, 0x7c, 0xdb, 0x95, 0xa2, 0x64, 0xbe, 0xe5, 0xbc, 0xae, 0x8f, 0xf4, 0xbe, 0xa5,
    0x24, 0x5c, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3b, 0xe7, 0x8a, 0xf5, 0xb2,
    0x4c, 0x0b, 0xb3, 0x79, 0xae, 0xfa, 0xc2, 0xcc, 0xcd, 0x36, 0xcc, 0xdd, 0x9c, 0xfd, 0x9e, 0x9f, 0xab, 0x1a, 0xf4, 0xc1,
    0xbf, 0x68, 0x81, 0x3d, 0xed, 0xe1, 0x79, 0xc8, 0xeb, 0x6b, 0x34, 0xfc, 0xdc, 0xe3, 0x96, 0x28, 0x91, 0x36, 0x31, 0x4f,
    0x75, 0x6d, 0xae, 0xde, 0xee, 0x63, 0xae, 0xc9, 0x96, 0x1e, 0xc9, 0xfb, 0x37, 0x9b, 0xbd, 0x64, 0x47, 0xa9, 0xe4, 0x66,
    0xbb, 0xf7, 0xa5, 0xbf, 0xc7, 0x72, 0x3f, 0xf4, 0xc3, 0x18, 0xaa, 0xb9, 0x3f, 0xcd, 0x65, 0xe8, 0x5b, 0x9f, 0xb3, 0x3e,
    0x97, 0xdf, 0xb3, 0x6d, 0xcc, 0x89, 0xbd, 0x7e, 0xb5, 0xc7, 0x23, 0x25, 0xb7, 0x75, 0x6e, 0xae, 0xfe, 0x8e, 0x63, 0x8e,
    0xe7, 0xb3, 0x9d, 0xff, 0x9e, 0x4c, 0xed, 0x37, 0x9e, 0x85, 0x99, 0xa9, 0x27, 0xda, 0xd4, 0xdd, 0x7a, 0x2d, 0xca, 0xa3,
    0x77, 0x99, 0xf5, 0xf8, 0xaf, 0x99, 0x83, 0xde, 0x87, 0xc7, 0x13, 0x5b, 0x81, 0x63, 0x26, 0xbf, 0xf7, 0xfb, 0x5a, 0x8b,
    0xf9, 0xf8, 0xff, 0xd6, 0x36, 0xf6, 0xfd, 0x4a, 0xa0, 0x2f, 0xd6, 0xf1, 0x9f, 0x23, 0x6c, 0xa6, 0x15, 0x5f, 0x13, 0xff,
    0xeb, 0xf7, 0xad, 0xfd, 0xf1, 0xdf, 0x43, 0xdf, 0x10, 0x6d, 0xcf, 0xde, 0x73, 0x6b, 0x2e, 0xfe, 0xc7, 0x57, 0xd0, 0x9e,
    0xad, 0x55, 0xab, 0x28, 0xe5, 0x72, 0x94, 0xb7, 0x60, 0xef, 0x21, 0x9b, 0x11, 0x68, 0x35, 0x1b, 0x5b, 0x0f, 0xf5, 0x46,
    0xf6, 0xd4, 0xff, 0xf9, 0xfe, 0xff, 0x7c, 0xbb, 0x2d, 0x3e, 0x7e, 0x90, 0xb9, 0x96, 0x76, 0xb4, 0xc3, 0xcf, 0xd8, 0x1a,
    0xc9, 0x1b, 0x77, 0xd5, 0xd6, 0xdd, 0xf1, 0xdf, 0xff, 0xb3, 0x8e, 0xe2, 0x51, 0x44, 0xad, 0xdf, 0x1d, 0xee, 0xd7, 0xb2,
    0x88, 0x8f, 0xff, 0xad, 0x1e, 0x43, 0xc5, 0x11, 0xcf, 0xf5, 0x0b, 0x67, 0xa3, 0xb4, 0xae, 0xfe, 0xcf, 0x9c, 0xf5, 0x2b,
    0xb7, 0x66, 0x46, 0xf7, 0xde, 0x1a, 0xff, 0x73, 0xe5, 0x76, 0x45, 0xfc, 0x57, 0xe7, 0x13, 0x9c, 0x69, 0x4d, 0x44, 0xb3,
    0x34, 0x5f, 0xd1, 0x97, 0x9c, 0xcd, 0xce, 0x9f, 0x5b, 0x7f, 0xf6, 0x4d, 0xf1, 0xdf, 0x8a, 0xda, 0xff, 0x67, 0x6e, 0xbd,
    0x47, 0xfb, 0xbf, 0xbf, 0xa0, 0xfe, 0xcf, 0x3e, 0xd7, 0x99, 0x3f, 0x86, 0x73, 0x8e, 0x36, 0x9e, 0x81, 0x3f, 0xf3, 0x04,
    0xe3, 0xca, 0xbe, 0x70, 0x76, 0x6b, 0x36, 0xfe, 0xbf, 0xb5, 0xfe, 0x7f, 0x43, 0xfc, 0xe7, 0x9f, 0x65, 0xaf, 0x8c, 0xff,
    0x9d, 0x75, 0x44, 0xf9, 0xf6, 0x51, 0xee, 0xd9, 0xff, 0x3b, 0xeb, 0xff, 0x9a, 0x35, 0xe7, 0x9f, 0x16, 0xff, 0x33, 0xa3,
    0x4c, 0x4f, 0x8d, 0xff, 0x8a, 0x18, 0xa9, 0x1a, 0xff, 0xaf, 0xbe, 0x03, 0xb4, 0xad, 0x99, 0xeb, 0x9f, 0x17, 0xe1, 0xe3,
    0xf8, 0x8f, 0xbc, 0xff, 0xf3, 0xee, 0xf8, 0x9f, 0x7b, 0x37, 0xe6, 0xd9, 0xf1, 0xdf, 0x93, 0x2b, 0xd6, 0xbc, 0xf3, 0xf9,
    0x70, 0x6c, 0xfd, 0xe9, 0xe7, 0xc5, 0x7f, 0xfc, 0xed, 0x8c, 0xb7, 0x3f, 0xff, 0xcb, 0xbd, 0x1d, 0xb0, 0xf7, 0xf9, 0xdf,
    0x13, 0xde, 0x2b, 0xe1, 0x4d, 0xf7, 0x42, 0xe7, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xa0, 0x6a, 0x46, 0x46, 0x3c, 0x67, 0xcf, 0xdc, 0x4c, 0xb1, 0x6b, 0x72, 0xdf, 0x47, 0x32, 0x15, 0xcc, 0xec, 0x4f, 0x3c,
    0xff, 0x41, 0x24, 0xdb, 0xfe, 0xd1, 0xb9, 0x89, 0xe6, 0x1c, 0xcf, 0xe4, 0x32, 0xda, 0x75, 0x8c, 0xeb, 0x59, 0xe1, 0xe7,
    0x7f, 0x61, 0x2e, 0xf7, 0x63, 0x2c, 0x2e, 0xee, 0x71, 0xae, 0x33, 0xf3, 0xee, 0x76, 0x67, 0x8a, 0xef, 0xc9, 0x5c, 0xef,
    0xf5, 0x7b, 0x95, 0xcd, 0xd3, 0x1f, 0xdb, 0xe7, 0x48, 0x09, 0xf7, 0xc4, 0x4a, 0x2c, 0x6d, 0xe2, 0x4a, 0xaa, 0xce, 0xef,
    0x93, 0x59, 0x99, 0x63, 0xfe, 0x8e, 0x5c, 0x99, 0xb3, 0xbf, 0x25, 0xf2, 0xf5, 0xdf, 0xe5, 0x5c, 0xdf, 0xb5, 0x65, 0x71,
    0x9c, 0xf1, 0xe8, 0xbe, 0xad, 0xa4, 0x1e, 0xca, 0x6e, 0xfa, 0x9c, 0x7c, 0x04, 0xb1, 0x3a, 0xf7, 0xbd, 0xed, 0xe2, 0xf7,
    0x1f, 0x73, 0x26, 0xb7, 0xc0, 0x4c, 0x1d, 0xde, 0x02, 0xf1, 0x7f, 0xc7, 0xfc, 0x55, 0xb9, 0xcc, 0x57, 0x33, 0xa5, 0x78,
    0xd6, 0xb6, 0x23, 0xbf, 0x07, 0x56, 0x62, 0xbb, 0xf2, 0x18, 0xf7, 0xe7, 0xec, 0x38, 0xce, 0x26, 0xf2, 0xe4, 0x73, 0xdd,
    0xb6, 0xe7, 0x82, 0x89, 0xd6, 0xff, 0x4f, 0x8d, 0xff, 0xd1, 0x6a, 0x04, 0x9f, 0xd7, 0x25, 0x3b, 0x6f, 0x5b, 0xc5, 0x5a,
    0x06, 0xab, 0x3d, 0xa0, 0x1d, 0xdb, 0xae, 0xcc, 0xd9, 0x75, 0xf6, 0xb1, 0xc6, 0xcb, 0xe8, 0xca, 0xf8, 0x9f, 0xe9, 0xd7,
    0x7d, 0x5b, 0xfd, 0xdf, 0x2f, 0xdf, 0x36, 0x7f, 0x17, 0xbb, 0xfb, 0xb5, 0xbd, 0x7f, 0xcd, 0x8e, 0x27, 0xc4, 0xf8, 0x5d,
    0xe3, 0xff, 0xaa, 0x8c, 0xc7, 0x35, 0x2b, 0x2d, 0x45, 0xfb, 0xff, 0x4f, 0xb8, 0x26, 0x7a, 0x72, 0x2d, 0xe3, 0xd8, 0x0a,
    0xce, 0xd5, 0xdb, 0xc4, 0xff, 0xdd, 0xeb, 0xff, 0x99, 0xb1, 0x95, 0xfb, 0xe5, 0xaf, 0xcc, 0xae, 0xd3, 0xfb, 0xee, 0xeb,
    0xa5, 0x85, 0x9e, 0x79, 0xed, 0xd8, 0x26, 0xfe, 0xc5, 0xbf, 0xf8, 0xbf, 0xe2, 0x7a, 0xe9, 0x5f, 0xd1, 0xff, 0x17, 0xff,
    0xe2, 0x3f, 0xf2, 0xcc, 0xec, 0xfe, 0xfd, 0xff, 0x5d, 0x4f, 0x2a, 0xdf, 0x36, 0xfe, 0x1f, 0x7d, 0xda, 0xfb, 0xa4, 0xf1,
    0xff, 0xdd, 0xcf, 0xff, 0xae, 0xf8, 0xdd, 0xdc, 0x3e, 0xbf, 0xff, 0x19, 0xf7, 0x3b, 0xf2, 0xc5, 0xf3, 0xfc, 0xeb, 0x50,
    0x19, 0x80, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe6, 0xe7, 0x00, 0xaf, 0xbe,
    0xa7, 0x7f, 0x66, 0xbe, 0xf7, 0xc8, 0x9c, 0xcf, 0xb9, 0xef, 0x04, 0xf1, 0x1f, 0x9b, 0xa7, 0x73, 0x66, 0xbe, 0xf7, 0xfc,
    0xdc, 0x7c, 0xe7, 0x1a, 0xde, 0x37, 0x53, 0x5c, 0x6c, 0x43, 0x2e, 0xc2, 0xa3, 0x39, 0x3a, 0xef, 0xb2, 0xed, 0x1d, 0x2b,
    0x73, 0xc0, 0xbd, 0x6a, 0xd5, 0xa7, 0xc4, 0x7f, 0xdb, 0x96, 0xef, 0x12, 0xbe, 0xb1, 0xff, 0x2f, 0xfe, 0x41, 0xfc, 0x8b,
    0x7f, 0xd0, 0xfe, 0x17, 0xff, 0x60, 0xfc, 0xff, 0xaa, 0x58, 0x9d, 0x59, 0x27, 0xe6, 0xc9, 0x4f, 0x31, 0xe0, 0x7e, 0x77,
    0x80, 0xf8, 0xdd, 0xa1, 0x7a, 0x5b, 0x1b, 0xde, 0x37, 0x3c, 0x23, 0x84, 0xb3, 0xe2, 0xff, 0x9a, 0x51, 0x8a, 0xd5, 0xf8,
    0xd7, 0xfe, 0x87, 0x37, 0xf5, 0x46, 0x22, 0xef, 0xff, 0x89, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xe9, 0xaf, 0xff, 0x94, 0x03, 0x88, 0x7f, 0xe0, 0xeb, 0xe2, 0xff,
    0x0f };

// Font glyphs rectangles data (on atlas)
RAYGUIAPI const Rectangle sunnyFontRecs[189] = {
    { 4, 4, 4 , 16 },
    { 16, 4, 2 , 10 },
    { 26, 4, 5 , 3 },
    { 39, 4, 7 , 10 },
    { 54, 4, 7 , 13 },
    { 69, 4, 7 , 10 },
    { 84, 4, 7 , 10 },
    { 99, 4, 2 , 3 },
    { 109, 4, 3 , 12 },
    { 120, 4, 3 , 12 },
    { 131, 4, 5 , 6 },
    { 144, 4, 6 , 5 },
    { 158, 4, 2 , 4 },
    { 168, 4, 5 , 1 },
    { 181, 4, 2 , 2 },
    { 191, 4, 4 , 10 },
    { 203, 4, 6 , 10 },
    { 217, 4, 4 , 10 },
    { 229, 4, 6 , 10 },
    { 243, 4, 6 , 10 },
    { 257, 4, 6 , 10 },
    { 271, 4, 6 , 10 },
    { 285, 4, 6 , 10 },
    { 299, 4, 6 , 10 },
    { 313, 4, 6 , 10 },
    { 327, 4, 6 , 10 },
    { 341, 4, 2 , 6 },
    { 351, 4, 2 , 8 },
    { 361, 4, 7 , 7 },
    { 376, 4, 5 , 3 },
    { 389, 4, 7 , 7 },
    { 404, 4, 6 , 10 },
    { 418, 4, 7 , 12 },
    { 433, 4, 7 , 10 },
    { 448, 4, 7 , 10 },
    { 463, 4, 7 , 10 },
    { 478, 4, 7 , 10 },
    { 493, 4, 7 , 10 },
    { 4, 28, 7 , 10 },
    { 19, 28, 7 , 10 },
    { 34, 28, 7 , 10 },
    { 49, 28, 2 , 10 },
    { 59, 28, 5 , 10 },
    { 72, 28, 7 , 10 },
    { 87, 28, 6 , 10 },
    { 101, 28, 9 , 10 },
    { 118, 28, 7 , 10 },
    { 133, 28, 7 , 10 },
    { 148, 28, 7 , 10 },
    { 163, 28, 7 , 12 },
    { 178, 28, 7 , 10 },
    { 193, 28, 7 , 10 },
    { 208, 28, 6 , 10 },
    { 222, 28, 7 , 10 },
    { 237, 28, 7 , 10 },
    { 252, 28, 8 , 10 },
    { 268, 28, 7 , 10 },
    { 283, 28, 6 , 10 },
    { 297, 28, 7 , 10 },
    { 312, 28, 4 , 12 },
    { 324, 28, 4 , 10 },
    { 336, 28, 4 , 12 },
    { 348, 28, 6 , 3 },
    { 362, 28, 7 , 1 },
    { 377, 28, 4 , 3 },
    { 389, 28, 6 , 7 },
    { 403, 28, 6 , 10 },
    { 417, 28, 6 , 7 },
    { 431, 28, 6 , 10 },
    { 445, 28, 6 , 7 },
    { 459, 28, 4 , 10 },
    { 471, 28, 6 , 9 },
    { 485, 28, 6 , 10 },
    { 499, 28, 2 , 10 },
    { 4, 52, 5 , 12 },
    { 17, 52, 6 , 10 },
    { 31, 52, 3 , 10 },
    { 42, 52, 8 , 7 },
    { 58, 52, 6 , 7 },
    { 72, 52, 6 , 7 },
    { 86, 52, 6 , 9 },
    { 100, 52, 6 , 9 },
    { 114, 52, 5 , 7 },
    { 127, 52, 6 , 7 },
    { 141, 52, 4 , 10 },
    { 153, 52, 6 , 7 },
    { 167, 52, 6 , 7 },
    { 181, 52, 8 , 7 },
    { 197, 52, 6 , 7 },
    { 211, 52, 6 , 9 },
    { 225, 52, 6 , 7 },
    { 239, 52, 5 , 12 },
    { 252, 52, 2 , 12 },
    { 262, 52, 5 , 12 },
    { 275, 52, 7 , 3 },
    { 290, 52, 2 , 9 },
    { 300, 52, 6 , 11 },
    { 314, 52, 7 , 10 },
    { 329, 52, 7 , 9 },
    { 344, 52, 6 , 10 },
    { 358, 52, 7 , 11 },
    { 373, 52, 6 , 12 },
    { 387, 52, 6 , 10 },
    { 401, 52, 7 , 10 },
    { 416, 52, 5 , 5 },
    { 429, 52, 7 , 6 },
    { 444, 52, 6 , 3 },
    { 458, 52, 7 , 10 },
    { 473, 52, 9 , 10 },
    { 490, 52, 4 , 4 },
    { 4, 76, 6 , 7 },
    { 18, 76, 4 , 5 },
    { 30, 76, 4 , 5 },
    { 42, 76, 7 , 11 },
    { 57, 76, 6 , 9 },
    { 71, 76, 7 , 12 },
    { 86, 76, 2 , 2 },
    { 96, 76, 6 , 10 },
    { 110, 76, 3 , 5 },
    { 121, 76, 4 , 5 },
    { 133, 76, 7 , 6 },
    { 148, 76, 9 , 10 },
    { 165, 76, 8 , 7 },
    { 181, 76, 6 , 11 },
    { 195, 76, 6 , 11 },
    { 209, 76, 7 , 11 },
    { 224, 76, 7 , 11 },
    { 239, 76, 7 , 11 },
    { 254, 76, 7 , 11 },
    { 269, 76, 7 , 11 },
    { 284, 76, 7 , 11 },
    { 299, 76, 9 , 10 },
    { 316, 76, 7 , 12 },
    { 331, 76, 7 , 11 },
    { 346, 76, 7 , 11 },
    { 361, 76, 7 , 11 },
    { 376, 76, 7 , 11 },
    { 391, 76, 3 , 11 },
    { 402, 76, 3 , 11 },
    { 413, 76, 5 , 11 },
    { 426, 76, 5 , 11 },
    { 439, 76, 8 , 10 },
    { 455, 76, 7 , 11 },
    { 470, 76, 7 , 11 },
    { 485, 76, 7 , 11 },
    { 4, 100, 7 , 11 },
    { 19, 100, 7 , 11 },
    { 34, 100, 7 , 11 },
    { 49, 100, 7 , 7 },
    { 64, 100, 7 , 13 },
    { 79, 100, 7 , 11 },
    { 94, 100, 7 , 11 },
    { 109, 100, 7 , 11 },
    { 124, 100, 7 , 11 },
    { 139, 100, 6 , 11 },
    { 153, 100, 7 , 10 },
    { 168, 100, 7 , 10 },
    { 183, 100, 6 , 10 },
    { 197, 100, 6 , 10 },
    { 211, 100, 6 , 10 },
    { 225, 100, 6 , 10 },
    { 239, 100, 6 , 10 },
    { 253, 100, 6 , 11 },
    { 267, 100, 8 , 7 },
    { 283, 100, 6 , 9 },
    { 297, 100, 6 , 10 },
    { 311, 100, 6 , 10 },
    { 325, 100, 6 , 10 },
    { 339, 100, 6 , 10 },
    { 353, 100, 3 , 10 },
    { 364, 100, 3 , 10 },
    { 375, 100, 5 , 10 },
    { 388, 100, 5 , 10 },
    { 401, 100, 6 , 10 },
    { 415, 100, 6 , 10 },
    { 429, 100, 6 , 10 },
    { 443, 100, 6 , 10 },
    { 457, 100, 6 , 10 },
    { 471, 100, 6 , 10 },
    { 485, 100, 6 , 10 },
    { 4, 124, 6 , 7 },
    { 18, 124, 7 , 11 },
    { 33, 124, 6 , 10 },
    { 47, 124, 6 , 10 },
    { 61, 124, 6 , 10 },
    { 75, 124, 6 , 10 },
    { 89, 124, 6 , 12 },
    { 103, 124, 6 , 12 },
    { 117, 124, 6 , 12 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
RAYGUIAPI const GlyphInfo sunnyFontGlyphs[189] = {
    { 32, 0, 12, 4, { 0 }},
    { 33, 0, 2, 3, { 0 }},
    { 34, 0, 2, 6, { 0 }},
    { 35, 0, 2, 8, { 0 }},
    { 36, 0, 1, 8, { 0 }},
    { 37, 0, 2, 8, { 0 }},
    { 38, 0, 2, 8, { 0 }},
    { 39, 0, 2, 3, { 0 }},
    { 40, 0, 2, 4, { 0 }},
    { 41, 0, 2, 4, { 0 }},
    { 42, 0, 4, 6, { 0 }},
    { 43, 0, 6, 7, { 0 }},
    { 44, 0, 10, 3, { 0 }},
    { 45, 0, 8, 6, { 0 }},
    { 46, 0, 10, 3, { 0 }},
    { 47, 0, 2, 5, { 0 }},
    { 48, 0, 2, 7, { 0 }},
    { 49, 0, 2, 7, { 0 }},
    { 50, 0, 2, 7, { 0 }},
    { 51, 0, 2, 7, { 0 }},
    { 52, 0, 2, 7, { 0 }},
    { 53, 0, 2, 7, { 0 }},
    { 54, 0, 2, 7, { 0 }},
    { 55, 0, 2, 7, { 0 }},
    { 56, 0, 2, 7, { 0 }},
    { 57, 0, 2, 7, { 0 }},
    { 58, 0, 4, 3, { 0 }},
    { 59, 0, 4, 3, { 0 }},
    { 60, 0, 4, 8, { 0 }},
    { 61, 0, 6, 6, { 0 }},
    { 62, 0, 4, 8, { 0 }},
    { 63, 0, 2, 7, { 0 }},
    { 64, 0, 2, 8, { 0 }},
    { 65, 0, 2, 8, { 0 }},
    { 66, 0, 2, 8, { 0 }},
    { 67, 0, 2, 8, { 0 }},
    { 68, 0, 2, 8, { 0 }},
    { 69, 0, 2, 8, { 0 }},
    { 70, 0, 2, 8, { 0 }},
    { 71, 0, 2, 8, { 0 }},
    { 72, 0, 2, 8, { 0 }},
    { 73, 0, 2, 3, { 0 }},
    { 74, 0, 2, 6, { 0 }},
    { 75, 0, 2, 8, { 0 }},
    { 76, 0, 2, 7, { 0 }},
    { 77, 0, 2, 10, { 0 }},
    { 78, 0, 2, 8, { 0 }},
    { 79, 0, 2, 8, { 0 }},
    { 80, 0, 2, 8, { 0 }},
    { 81, 0, 2, 8, { 0 }},
    { 82, 0, 2, 8, { 0 }},
    { 83, 0, 2, 8, { 0 }},
    { 84, 0, 2, 7, { 0 }},
    { 85, 0, 2, 8, { 0 }},
    { 86, 0, 2, 8, { 0 }},
    { 87, 0, 2, 9, { 0 }},
    { 88, 0, 2, 8, { 0 }},
    { 89, 0, 2, 7, { 0 }},
    { 90, 0, 2, 8, { 0 }},
    { 91, 0, 2, 5, { 0 }},
    { 92, 0, 2, 5, { 0 }},
    { 93, 0, 2, 5, { 0 }},
    { 94, 0, 2, 7, { 0 }},
    { 95, 0, 14, 8, { 0 }},
    { 96, 0, 2, 5, { 0 }},
    { 97, 0, 5, 7, { 0 }},
    { 98, 0, 2, 7, { 0 }},
    { 99, 0, 5, 7, { 0 }},
    { 100, 0, 2, 7, { 0 }},
    { 101, 0, 5, 7, { 0 }},
    { 102, 0, 2, 5, { 0 }},
    { 103, 0, 5, 7, { 0 }},
    { 104, 0, 2, 7, { 0 }},
    { 105, 0, 2, 3, { 0 }},
    { 106, 0, 2, 6, { 0 }},
    { 107, 0, 2, 7, { 0 }},
    { 108, 0, 2, 4, { 0 }},
    { 109, 0, 5, 9, { 0 }},
    { 110, 0, 5, 7, { 0 }},
    { 111, 0, 5, 7, { 0 }},
    { 112, 0, 5, 7, { 0 }},
    { 113, 0, 5, 7, { 0 }},
    { 114, 0, 5, 6, { 0 }},
    { 115, 0, 5, 7, { 0 }},
    { 116, 0, 2, 5, { 0 }},
    { 117, 0, 5, 7, { 0 }},
    { 118, 0, 5, 7, { 0 }},
    { 119, 0, 5, 9, { 0 }},
    { 120, 0, 5, 7, { 0 }},
    { 121, 0, 5, 7, { 0 }},
    { 122, 0, 5, 7, { 0 }},
    { 123, 0, 2, 6, { 0 }},
    { 124, 0, 2, 3, { 0 }},
    { 125, 0, 2, 6, { 0 }},
    { 126, 0, 6, 8, { 0 }},
    { 161, 0, 5, 3, { 0 }},
    { 162, 0, 3, 7, { 0 }},
    { 163, 0, 2, 8, { 0 }},
    { 8364, 0, 3, 8, { 0 }},
    { 165, 0, 2, 7, { 0 }},
    { 352, 0, 1, 8, { 0 }},
    { 167, 0, 2, 7, { 0 }},
    { 353, 0, 2, 7, { 0 }},
    { 169, 0, 2, 8, { 0 }},
    { 170, 0, 2, 6, { 0 }},
    { 171, 0, 6, 8, { 0 }},
    { 172, 0, 7, 7, { 0 }},
    { 174, 0, 2, 8, { 0 }},
    { 175, 1, 3, 14, { 0 }},
    { 176, 0, 2, 5, { 0 }},
    { 177, 0, 4, 7, { 0 }},
    { 178, 0, 2, 5, { 0 }},
    { 179, 0, 2, 5, { 0 }},
    { 381, 0, 1, 8, { 0 }},
    { 181, 0, 5, 7, { 0 }},
    { 182, 0, 2, 8, { 0 }},
    { 183, 0, 6, 3, { 0 }},
    { 382, 0, 2, 7, { 0 }},
    { 185, 0, 2, 4, { 0 }},
    { 186, 0, 2, 5, { 0 }},
    { 187, 0, 6, 8, { 0 }},
    { 338, 0, 2, 10, { 0 }},
    { 339, 0, 5, 9, { 0 }},
    { 376, 0, 1, 7, { 0 }},
    { 191, 0, 3, 7, { 0 }},
    { 192, 0, 1, 8, { 0 }},
    { 193, 0, 1, 8, { 0 }},
    { 194, 0, 1, 8, { 0 }},
    { 195, 0, 1, 8, { 0 }},
    { 196, 0, 1, 8, { 0 }},
    { 197, 0, 1, 8, { 0 }},
    { 198, 0, 2, 10, { 0 }},
    { 199, 0, 2, 8, { 0 }},
    { 200, 0, 1, 8, { 0 }},
    { 201, 0, 1, 8, { 0 }},
    { 202, 0, 1, 8, { 0 }},
    { 203, 0, 1, 8, { 0 }},
    { 204, 0, 1, 4, { 0 }},
    { 205, 0, 1, 4, { 0 }},
    { 206, 0, 1, 6, { 0 }},
    { 207, 0, 1, 6, { 0 }},
    { 208, 0, 2, 9, { 0 }},
    { 209, 0, 1, 8, { 0 }},
    { 210, 0, 1, 8, { 0 }},
    { 211, 0, 1, 8, { 0 }},
    { 212, 0, 1, 8, { 0 }},
    { 213, 0, 1, 8, { 0 }},
    { 214, 0, 1, 8, { 0 }},
    { 215, 0, 5, 8, { 0 }},
    { 216, 0, 1, 8, { 0 }},
    { 217, 0, 1, 8, { 0 }},
    { 218, 0, 1, 8, { 0 }},
    { 219, 0, 1, 8, { 0 }},
    { 220, 0, 1, 8, { 0 }},
    { 221, 0, 1, 7, { 0 }},
    { 222, 0, 2, 8, { 0 }},
    { 223, 0, 2, 8, { 0 }},
    { 224, 0, 2, 7, { 0 }},
    { 225, 0, 2, 7, { 0 }},
    { 226, 0, 2, 7, { 0 }},
    { 227, 0, 2, 7, { 0 }},
    { 228, 0, 2, 7, { 0 }},
    { 229, 0, 1, 7, { 0 }},
    { 230, 0, 5, 9, { 0 }},
    { 231, 0, 5, 7, { 0 }},
    { 232, 0, 2, 7, { 0 }},
    { 233, 0, 2, 7, { 0 }},
    { 234, 0, 2, 7, { 0 }},
    { 235, 0, 2, 7, { 0 }},
    { 236, 0, 2, 4, { 0 }},
    { 237, 0, 2, 4, { 0 }},
    { 238, 0, 2, 6, { 0 }},
    { 239, 0, 2, 6, { 0 }},
    { 240, 0, 2, 7, { 0 }},
    { 241, 0, 2, 7, { 0 }},
    { 242, 0, 2, 7, { 0 }},
    { 243, 0, 2, 7, { 0 }},
    { 244, 0, 2, 7, { 0 }},
    { 245, 0, 2, 7, { 0 }},
    { 246, 0, 2, 7, { 0 }},
    { 247, 0, 4, 7, { 0 }},
    { 248, 0, 3, 8, { 0 }},
    { 249, 0, 2, 7, { 0 }},
    { 250, 0, 2, 7, { 0 }},
    { 251, 0, 2, 7, { 0 }},
    { 252, 0, 2, 7, { 0 }},
    { 253, 0, 2, 7, { 0 }},
    { 254, 0, 2, 7, { 0 }},
    { 255, 0, 2, 7, { 0 }},
};

// Style loading function: Sunny
RAYGUIAPI void GuiLoadStyleSunny(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < SUNNY_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(sunnyStyleProps[i].controlId, sunnyStyleProps[i].propertyId, sunnyStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int sunnyFontDataSize = 0;
    unsigned char *data = DecompressData(sunnyFontData, SUNNY_STYLE_FONT_ATLAS_COMP_SIZE, &sunnyFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 16;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, sunnyFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, sunnyFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleCandy();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define CANDY_STYLE_PROPS_COUNT  17

// Custom style name: Candy
RAYGUIAPI const GuiStyleProp candyStyleProps[CANDY_STYLE_PROPS_COUNT] = {
    { 0, 0, 0xe58b68ff },    // DEFAULT_BORDER_COLOR_NORMAL
    { 0, 1, 0xfeda96ff },    // DEFAULT_BASE_COLOR_NORMAL
    { 0, 2, 0xe59b5fff },    // DEFAULT_TEXT_COLOR_NORMAL
    { 0, 3, 0xee813fff },    // DEFAULT_BORDER_COLOR_FOCUSED
    { 0, 4, 0xfcd85bff },    // DEFAULT_BASE_COLOR_FOCUSED
    { 0, 5, 0xfc6955ff },    // DEFAULT_TEXT_COLOR_FOCUSED
    { 0, 6, 0xb34848ff },    // DEFAULT_BORDER_COLOR_PRESSED
    { 0, 7, 0xeb7272ff },    // DEFAULT_BASE_COLOR_PRESSED
    { 0, 8, 0xbd4a4aff },    // DEFAULT_TEXT_COLOR_PRESSED
    { 0, 9, 0x94795dff },    // DEFAULT_BORDER_COLOR_DISABLED
    { 0, 10, 0xc2a37aff },    // DEFAULT_BASE_COLOR_DISABLED
    { 0, 11, 0x9c8369ff },    // DEFAULT_TEXT_COLOR_DISABLED
    { 0, 16, 0x0000000f },    // DEFAULT_TEXT_SIZE
    { 0, 17, 0x00000000 },    // DEFAULT_TEXT_SPACING
    { 0, 18, 0xd77575ff },    // DEFAULT_LINE_COLOR
    { 0, 19, 0xfff5e1ff },    // DEFAULT_BACKGROUND_COLOR
    { 0, 20, 0x00000016 },    // DEFAULT_TEXT_LINE_SPACING
};

// WARNING: This style uses a custom font: "v5easter.ttf" (size: 15, spacing: 0)

#define CANDY_STYLE_FONT_ATLAS_COMP_SIZE 2260

// Font atlas image pixels data: DEFLATE compressed
RAYGUIAPI unsigned char candyFontData[CANDY_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0x4d, 0x92, 0xd4, 0x38, 0x10, 0x06, 0x50, 0xdd, 0x80, 0xfb, 0xcc, 0x61, 0xe6, 0x4a, 0x73, 0x45, 0x8e, 0xc0, 0x86,
    0x25, 0xa1, 0x89, 0x61, 0x88, 0x20, 0x80, 0x2e, 0x4b, 0x4a, 0xa5, 0x6c, 0xd9, 0xf5, 0x78, 0x1b, 0xa2, 0xdd, 0x5d, 0x55,
    0xfe, 0xf9, 0x2c, 0xc9, 0x2e, 0xa5, 0x6b, 0x01, 0x00, 0x00, 0x00, 0xde, 0xdc, 0x7f, 0xff, 0xfe, 0xfc, 0x59, 0xf9, 0xe0,
    0xa7, 0x3f, 0x7f, 0xb7, 0x7e, 0xb8, 0xf4, 0x68, 0x49, 0xfc, 0x2f, 0x8f, 0x3f, 0x49, 0xdf, 0x2b, 0x94, 0x83, 0x25, 0xed,
    0xcf, 0xfc, 0xfb, 0xcf, 0x6a, 0xca, 0xbb, 0xbf, 0x7a, 0x9d, 0xd6, 0x27, 0x8a, 0x2c, 0x3b, 0x7a, 0xd5, 0x7a, 0xb0, 0x4e,
    0xa5, 0x63, 0x49, 0x0d, 0xad, 0x5d, 0x9d, 0xde, 0xab, 0x63, 0x7b, 0xa7, 0xbd, 0xfe, 0x65, 0x78, 0x1d, 0xcb, 0x82, 0x65,
    0x75, 0x72, 0x1f, 0xaf, 0xcc, 0xff, 0xef, 0xff, 0x32, 0xf3, 0x7f, 0xfc, 0x9a, 0x33, 0xc7, 0xc4, 0xcf, 0xb5, 0xa9, 0x09,
    0xf9, 0x1f, 0x3f, 0x8b, 0x1c, 0xaf, 0xc1, 0xeb, 0xa3, 0xb8, 0x0c, 0xff, 0x4d, 0xfb, 0xfc, 0x9a, 0xf9, 0xaa, 0x2b, 0x97,
    0xf5, 0xa5, 0x76, 0xfe, 0x38, 0x6f, 0x7d, 0x9a, 0xb9, 0x7d, 0x78, 0xe6, 0xb2, 0xdc, 0xfc, 0xb7, 0x7e, 0x37, 0x7a, 0x66,
    0x8f, 0xf7, 0x0e, 0x7a, 0x5b, 0xf3, 0x3a, 0x74, 0xec, 0xb7, 0x5b, 0xb1, 0x9c, 0x2d, 0x3c, 0xba, 0xc6, 0xc7, 0xe7, 0x92,
    0xd6, 0xd2, 0xf9, 0x36, 0x77, 0xe4, 0xd8, 0xb8, 0xe2, 0x28, 0x6e, 0xef, 0xcd, 0x8c, 0x76, 0xa9, 0xfe, 0xb2, 0xb5, 0xe5,
    0xbf, 0xf7, 0xa8, 0xad, 0x8b, 0x5b, 0xda, 0x32, 0xdc, 0xd2, 0xc6, 0xfa, 0x2c, 0x79, 0x5b, 0x38, 0x2b, 0xff, 0x75, 0x22,
    0xfd, 0x25, 0xd0, 0xe3, 0x3c, 0x3e, 0x6b, 0xb7, 0x3e, 0x7f, 0xf4, 0x28, 0xc9, 0x3d, 0x66, 0x67, 0xf3, 0x7f, 0xdc, 0x1f,
    0x2e, 0x87, 0xe7, 0x9b, 0xf3, 0x7a, 0x5c, 0x3b, 0xe5, 0x3f, 0xda, 0x43, 0xee, 0x1f, 0xdd, 0xe4, 0xb5, 0xff, 0xeb, 0xf3,
    0xdf, 0x4a, 0x64, 0x1d, 0x1c, 0xa9, 0xe6, 0xb6, 0xc6, 0x3d, 0xe3, 0xe9, 0xe8, 0x19, 0x7d, 0x7c, 0x8c, 0x7b, 0x7e, 0xff,
    0xbf, 0x3d, 0x6e, 0xac, 0xcb, 0x46, 0x4e, 0xf9, 0x09, 0xdb, 0xb7, 0xff, 0xdf, 0xd7, 0x2f, 0x58, 0xd3, 0xff, 0x6f, 0x8d,
    0xff, 0xcb, 0xe9, 0xad, 0x7f, 0x2b, 0x59, 0xf1, 0xbd, 0x5f, 0x43, 0x3d, 0xa0, 0xb9, 0xab, 0x0a, 0xb1, 0xfd, 0x39, 0x97,
    0xff, 0x15, 0x7d, 0x80, 0xd1, 0xd1, 0xdf, 0x33, 0xfa, 0xff, 0x91, 0x2d, 0x18, 0xbd, 0xce, 0x9c, 0xb5, 0x66, 0xe3, 0xd7,
    0xf9, 0xaf, 0xdb, 0x62, 0x79, 0xe7, 0xf9, 0xd9, 0x2d, 0x98, 0xff, 0xaa, 0xb1, 0x5e, 0xf5, 0xe8, 0x51, 0x5c, 0x2f, 0xbd,
    0x37, 0x56, 0x6f, 0x71, 0xe5, 0xf4, 0x8c, 0xf6, 0xbf, 0x75, 0xfd, 0x7f, 0xe6, 0xea, 0xc0, 0xd8, 0x9d, 0x8d, 0xba, 0xe1,
    0xb1, 0x22, 0xff, 0x77, 0x69, 0xc5, 0xb2, 0x7a, 0xd5, 0xae, 0xff, 0x45, 0xee, 0x69, 0x67, 0xf4, 0xe7, 0xee, 0x92, 0xff,
    0xda, 0x71, 0x36, 0xed, 0x5f, 0x12, 0xbf, 0xe3, 0xbe, 0xea, 0x5b, 0x05, 0xad, 0xbb, 0x8d, 0xe7, 0xdd, 0xc5, 0xde, 0x31,
    0xff, 0xf7, 0xb8, 0xff, 0xbf, 0xc7, 0xb6, 0x5c, 0x3b, 0x92, 0xdb, 0x31, 0xff, 0xf0, 0x1e, 0xdf, 0x26, 0x3c, 0xef, 0x5b,
    0x8b, 0xf2, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xdd, 0x67, 0xe7, 0xc4, 0x2b,
    0x44, 0x8e, 0xcd, 0xb9, 0xa9, 0x4b, 0xea, 0x22, 0xc5, 0xe6, 0x46, 0x9f, 0x5b, 0xc3, 0x39, 0x5a, 0x35, 0x7b, 0x6e, 0x5e,
    0xf8, 0xeb, 0x7a, 0x42, 0x33, 0xaf, 0x7b, 0x3c, 0x17, 0x79, 0x64, 0x5e, 0x76, 0xb4, 0xd6, 0xf9, 0x68, 0x4d, 0xa0, 0x48,
    0xed, 0xa4, 0xd9, 0xda, 0xe9, 0xe3, 0x6b, 0x1c, 0x99, 0xc1, 0x3a, 0x3b, 0xe7, 0xed, 0xac, 0x8a, 0xcd, 0xd1, 0x9c, 0xf6,
    0x55, 0xbd, 0xad, 0xc3, 0x95, 0xea, 0x76, 0xab, 0xd3, 0x90, 0x5d, 0x35, 0x63, 0x65, 0xfe, 0x4b, 0x57, 0x6d, 0xec, 0xbe,
    0xf9, 0xce, 0x35, 0x71, 0x46, 0xfb, 0x79, 0x35, 0xf7, 0xca, 0xf4, 0x3e, 0x19, 0x6d, 0xa9, 0x9e, 0x9f, 0xff, 0xe8, 0xba,
    0xb7, 0xb6, 0x6a, 0xdd, 0xb6, 0x8a, 0xfd, 0x95, 0xf9, 0x8f, 0xf6, 0xed, 0x8e, 0x5b, 0xbe, 0xf1, 0x8a, 0x07, 0xe7, 0x54,
    0xb4, 0xa9, 0x1b, 0xe5, 0x7f, 0xfc, 0xdd, 0x56, 0xce, 0x61, 0x9f, 0x6b, 0x7b, 0x6b, 0x5a, 0xfe, 0xe3, 0x7d, 0x9f, 0xdd,
    0xda, 0xf8, 0xdc, 0xaa, 0x39, 0xeb, 0xf2, 0x5f, 0xd2, 0xab, 0x32, 0xed, 0x99, 0xff, 0x9e, 0xf6, 0x21, 0x32, 0x36, 0x18,
    0xff, 0xcb, 0xe8, 0x19, 0x77, 0xf4, 0xdd, 0x22, 0xed, 0xff, 0xf1, 0x5a, 0xde, 0xb9, 0xfd, 0xdf, 0x25, 0xff, 0x65, 0xab,
    0xfe, 0xff, 0xec, 0xd1, 0x38, 0x96, 0xf3, 0xeb, 0x6a, 0xb3, 0xed, 0xfe, 0x24, 0xa3, 0x91, 0x2d, 0x3e, 0x5b, 0x1d, 0x39,
    0x7a, 0xdd, 0x21, 0xf3, 0x89, 0x4d, 0xf7, 0xba, 0xc6, 0x55, 0xde, 0x72, 0xfc, 0x1f, 0xbf, 0xee, 0x2b, 0xff, 0xb3, 0xfd,
    0xe9, 0x72, 0x41, 0xfe, 0x57, 0x6d, 0xbb, 0x3b, 0xe7, 0x7f, 0xa6, 0xe7, 0x77, 0x56, 0xfe, 0xcb, 0xcb, 0xa7, 0x50, 0xcd,
    0xe6, 0x3f, 0xfe, 0x6c, 0x9c, 0xd1, 0x9c, 0xd7, 0x0b, 0xef, 0x6f, 0xad, 0x38, 0x07, 0xc6, 0xef, 0x37, 0x14, 0xf9, 0xbf,
    0x55, 0xfe, 0xaf, 0x6f, 0xff, 0x7b, 0x9f, 0xad, 0xf8, 0xde, 0xf9, 0x2f, 0x53, 0xd7, 0xf8, 0x56, 0xfc, 0xdd, 0x6e, 0xfd,
    0xff, 0xec, 0x73, 0xe7, 0xb9, 0xe7, 0xd3, 0x75, 0x7d, 0xdc, 0xd8, 0xf5, 0xa4, 0xd8, 0xd3, 0x0c, 0x62, 0x55, 0xb3, 0xd7,
    0x1c, 0x53, 0x25, 0xfc, 0x0c, 0xaf, 0x32, 0x7c, 0x06, 0x90, 0xff, 0x5f, 0x97, 0xac, 0x7b, 0x3a, 0x32, 0xac, 0xce, 0xd3,
    0xbd, 0xf2, 0x3f, 0xf7, 0x4d, 0x83, 0xfc, 0xa7, 0x0d, 0xd6, 0x0b, 0xda, 0x7f, 0x58, 0x7b, 0x97, 0x3d, 0xef, 0xf7, 0xc9,
    0xe8, 0xd3, 0x03, 0xef, 0xdb, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x38, 0x57,
    0xe0, 0xf5, 0x2c, 0xcf, 0xe3, 0x2a, 0x18, 0x25, 0x58, 0x05, 0xbb, 0x74, 0x54, 0x4e, 0xae, 0xc1, 0xf7, 0x1b, 0x99, 0xa5,
    0xbc, 0x5b, 0x2d, 0x82, 0xd1, 0xfa, 0x88, 0x23, 0xb5, 0xb1, 0xcb, 0x70, 0x9d, 0xaf, 0x68, 0x15, 0x9b, 0x32, 0x59, 0x73,
    0x34, 0x3e, 0xff, 0x2e, 0x3a, 0x83, 0xfa, 0x68, 0x7f, 0x7e, 0xaa, 0xdf, 0xea, 0x97, 0xef, 0xbe, 0xd6, 0x7f, 0x86, 0xdf,
    0xb5, 0x76, 0xcd, 0xf7, 0x9b, 0xdd, 0x27, 0x3d, 0x5b, 0x28, 0x92, 0xff, 0x1d, 0xab, 0xee, 0x65, 0x55, 0xf9, 0xb8, 0x53,
    0xbd, 0xed, 0x91, 0x3a, 0xbc, 0xed, 0x7a, 0x1e, 0xf3, 0xf9, 0xef, 0xad, 0x29, 0x50, 0x27, 0xab, 0x7e, 0x8e, 0x9e, 0x2b,
    0x8e, 0xb7, 0xcb, 0xd1, 0xdc, 0xe4, 0xd7, 0xad, 0xdc, 0xa7, 0xfa, 0xf9, 0xc7, 0xff, 0xfe, 0xfe, 0x30, 0xff, 0x35, 0x3c,
    0x1b, 0xa7, 0xfd, 0x89, 0x32, 0xe7, 0x57, 0xbf, 0x47, 0xfe, 0xc7, 0xcf, 0x99, 0x77, 0xa8, 0xb7, 0x5d, 0x12, 0x7e, 0x9a,
    0x39, 0xf3, 0x3d, 0x5a, 0x53, 0x24, 0x9a, 0xff, 0x91, 0x34, 0xbd, 0x3e, 0xcb, 0x95, 0x8e, 0x0a, 0x6a, 0xa3, 0xf9, 0x6f,
    0xbd, 0x6b, 0x24, 0xff, 0x75, 0xb8, 0xf6, 0x4a, 0xff, 0xb6, 0x1c, 0x6b, 0x5d, 0xf6, 0xcc, 0x7f, 0xa4, 0xc7, 0x59, 0x4f,
    0xcc, 0xff, 0x5c, 0x8d, 0xaf, 0x72, 0x7a, 0xfe, 0x73, 0x9f, 0x53, 0xd1, 0x37, 0x92, 0xe9, 0x4d, 0x76, 0xff, 0xef, 0xd6,
    0x46, 0x5b, 0xbc, 0x2e, 0xff, 0xf5, 0x83, 0xf7, 0x5f, 0xd5, 0xfe, 0xc7, 0xeb, 0xab, 0xd6, 0x83, 0x4f, 0xf8, 0x84, 0xfc,
    0x97, 0x4d, 0xfa, 0xff, 0xf9, 0xcf, 0x52, 0xdb, 0x33, 0xff, 0xe3, 0x3d, 0xae, 0xf1, 0x76, 0x7d, 0xac, 0xaf, 0xd0, 0x1e,
    0x01, 0xd4, 0xc0, 0x19, 0xa0, 0x27, 0xff, 0xb1, 0xf6, 0x3f, 0x3a, 0xfe, 0x8f, 0x27, 0xa3, 0x3e, 0xb6, 0xfd, 0xcf, 0x1d,
    0xff, 0xaf, 0xaa, 0x45, 0x98, 0xfb, 0xbc, 0xad, 0xac, 0x5a, 0x9b, 0xb1, 0xbe, 0x56, 0xb4, 0xfd, 0x6f, 0xf5, 0x0e, 0x66,
    0xf2, 0x5f, 0x26, 0x93, 0xf8, 0xf1, 0xf5, 0xbf, 0x55, 0xed, 0xff, 0xf9, 0xd7, 0xff, 0x6b, 0x20, 0x21, 0xab, 0xaa, 0x6e,
    0xd6, 0xe1, 0x6b, 0x31, 0xb3, 0x55, 0xd5, 0xfa, 0xef, 0x34, 0xf4, 0x9c, 0x9b, 0x5f, 0xdf, 0xa5, 0xe8, 0xd9, 0x2a, 0x19,
    0xa3, 0xbe, 0x5d, 0xfb, 0xff, 0xd7, 0xb4, 0xff, 0x3d, 0xad, 0x7f, 0xa4, 0x0e, 0xd9, 0xba, 0xeb, 0x7f, 0xd1, 0xf1, 0xd3,
    0xdc, 0xfd, 0xbf, 0xeb, 0xeb, 0x08, 0x66, 0x5f, 0xf9, 0x39, 0xf3, 0xb5, 0xae, 0xfa, 0x44, 0x3b, 0xf6, 0xff, 0xe3, 0x63,
    0xab, 0x91, 0x51, 0x7d, 0xde, 0xfd, 0xbf, 0x98, 0x56, 0xfe, 0xd7, 0xbc, 0xeb, 0xba, 0x63, 0x4e, 0xfe, 0xf7, 0xdc, 0x07,
    0x7b, 0xe5, 0xbf, 0xf7, 0x29, 0xe6, 0x35, 0xfd, 0x4e, 0xfc, 0x6e, 0x7b, 0xb0, 0x9d, 0xff, 0x7b, 0x7d, 0xff, 0xe7, 0xfc,
    0x27, 0x8f, 0x5e, 0x9d, 0xff, 0xdd, 0xce, 0x00, 0x59, 0x4f, 0xbc, 0x2e, 0x49, 0x7b, 0x32, 0x72, 0x17, 0x6f, 0xf7, 0xf3,
    0xa5, 0xfc, 0xf3, 0xfc, 0xef, 0x6f, 0xd6, 0xc7, 0xf7, 0x9b, 0xae, 0xef, 0x5b, 0xcb, 0x3f, 0xbc, 0xaf, 0xf6, 0xf7, 0x7f,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0x9e, 0x0f, 0x58, 0x27, 0xe7, 0x0a, 0xc6, 0x2a,
    0x0d, 0x95, 0xe6, 0x8c, 0xd3, 0x1a, 0xfe, 0x9d, 0x91, 0x75, 0x89, 0xd7, 0xb0, 0x88, 0xd4, 0xe0, 0x2e, 0x8d, 0xba, 0x35,
    0x39, 0xdb, 0xe6, 0x9c, 0xf5, 0x6c, 0xd7, 0xda, 0x1b, 0x3b, 0x2e, 0x46, 0xb6, 0x54, 0xbb, 0x86, 0xca, 0x79, 0xdb, 0x21,
    0xba, 0xbf, 0x6b, 0x63, 0x76, 0x77, 0x6d, 0x54, 0xae, 0xc9, 0xa9, 0x07, 0x50, 0x1b, 0x75, 0x54, 0xe2, 0xf3, 0xd5, 0x46,
    0xcf, 0x00, 0xf5, 0x8f, 0xda, 0x4e, 0x35, 0xf4, 0x3b, 0xa3, 0x7b, 0x3d, 0xb6, 0xac, 0xa7, 0xaa, 0x63, 0x5e, 0xa5, 0xe8,
    0xc8, 0xb6, 0x59, 0xbf, 0x9e, 0xe3, 0xc7, 0x45, 0x4d, 0x7a, 0x87, 0xde, 0xba, 0x24, 0xfb, 0xef, 0xef, 0x76, 0x05, 0xd9,
    0xba, 0xac, 0xfd, 0xcf, 0xc8, 0xff, 0x58, 0x0d, 0xd6, 0x91, 0x3a, 0x63, 0x99, 0xf9, 0xdf, 0xab, 0x3f, 0x16, 0x7f, 0x1e,
    0x44, 0x56, 0xc5, 0xfd, 0x7b, 0xcc, 0xda, 0x8d, 0x3f, 0x99, 0xe0, 0x3e, 0xfb, 0x7b, 0xf5, 0xfa, 0xf5, 0x9c, 0x29, 0xe7,
    0xea, 0x42, 0x5c, 0x95, 0xff, 0xbc, 0xfe, 0x7f, 0xb4, 0x3f, 0x1d, 0x79, 0x26, 0xd2, 0x6c, 0x2d, 0xc5, 0x99, 0x27, 0x8b,
    0x9c, 0xbd, 0x2e, 0x73, 0xa3, 0x82, 0x9e, 0x31, 0xeb, 0x0e, 0xdb, 0x61, 0x66, 0x1b, 0x95, 0xe0, 0x93, 0x17, 0xb2, 0xf2,
    0x9f, 0x51, 0xcf, 0x74, 0x7c, 0xfc, 0xd7, 0xde, 0x56, 0xb9, 0xe7, 0xc5, 0xd8, 0xeb, 0xed, 0xba, 0xac, 0x4e, 0x8c, 0x63,
    0xae, 0x7d, 0x92, 0x51, 0x4e, 0xfe, 0xfb, 0x7b, 0xb5, 0xbb, 0xef, 0xd3, 0xd2, 0x31, 0x4e, 0xde, 0x3f, 0xff, 0xa3, 0x9f,
    0x25, 0xb3, 0x8f, 0x3b, 0x5b, 0x5d, 0xfa, 0x9a, 0x65, 0xf3, 0x4f, 0x2a, 0xd9, 0xfd, 0xd8, 0xbe, 0x3a, 0xff, 0x6b, 0x9e,
    0x06, 0x93, 0xbd, 0x8d, 0x5a, 0xa3, 0x86, 0xac, 0xf6, 0xbf, 0x35, 0x02, 0xa9, 0x5b, 0x8c, 0x0b, 0xfb, 0xb2, 0x3d, 0x7e,
    0xfd, 0x7f, 0xee, 0x58, 0x59, 0xb1, 0x6c, 0xfe, 0xb8, 0xdd, 0x37, 0xe3, 0xbb, 0xe4, 0xff, 0x0e, 0xe7, 0xc8, 0x78, 0xab,
    0x9d, 0xd1, 0xf7, 0x3d, 0xbf, 0xfd, 0xcf, 0xc8, 0x7f, 0xfe, 0x15, 0xd0, 0x3b, 0x2e, 0x5b, 0x9f, 0xd5, 0xd9, 0x6b, 0x70,
    0xf2, 0x2f, 0xff, 0xf1, 0xbe, 0x4a, 0x4d, 0xaf, 0x70, 0xb9, 0xe2, 0xe9, 0x60, 0x57, 0x8d, 0xff, 0x73, 0xaf, 0x43, 0xdd,
    0xb5, 0xff, 0x7f, 0xf7, 0x7d, 0x3a, 0xd3, 0x6b, 0xa7, 0xbc, 0x55, 0xdf, 0x60, 0xf6, 0x6c, 0xff, 0xb4, 0xfc, 0x3f, 0x7b,
    0x9f, 0x4a, 0xbd, 0xfc, 0xe7, 0x9e, 0x01, 0xd6, 0x65, 0x7c, 0x24, 0xbb, 0xef, 0x97, 0xff, 0xbb, 0x3c, 0x35, 0x48, 0xfe,
    0x9f, 0x31, 0xe6, 0x3b, 0xf7, 0x7b, 0x0e, 0x39, 0xdf, 0xff, 0x2b, 0x27, 0x3e, 0x01, 0xef, 0xec, 0xfb, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x33, 0x7e, 0x33, 0x2b, 0x3d, 0x1f, 0xfd, 0xe5, 0xa7, 0xfa,
    0xad, 0x7e, 0xf9, 0xee, 0x6b, 0xfd, 0xe7, 0xb0, 0xce, 0xc9, 0x19, 0x6b, 0xa1, 0x2e, 0xf7, 0xda, 0xba, 0xdc, 0xc7, 0x7b,
    0xfb, 0x2e, 0x55, 0xb9, 0x7b, 0x8f, 0xd9, 0x3a, 0xb8, 0x85, 0x57, 0xa6, 0x2f, 0xab, 0x9e, 0x43, 0x74, 0xee, 0xe1, 0xab,
    0xd7, 0xfc, 0x54, 0x3f, 0xff, 0xf8, 0xdf, 0xdf, 0x1f, 0x6e, 0xcb, 0xbe, 0x7a, 0x60, 0x79, 0x6b, 0xa1, 0x2e, 0xf7, 0xca,
    0xba, 0xdc, 0x7d, 0x7b, 0x7b, 0xf7, 0xaa, 0xdc, 0xfd, 0x6b, 0x51, 0x37, 0x49, 0xdf, 0xb5, 0xb3, 0xe4, 0xe2, 0xdb, 0x72,
    0xaf, 0x1a, 0xce, 0x35, 0x5c, 0x5b, 0x73, 0xae, 0x2e, 0xef, 0xf9, 0xeb, 0xb2, 0xaa, 0x87, 0xd9, 0x4a, 0xce, 0x6e, 0x47,
    0x74, 0x6c, 0x2d, 0xae, 0xd9, 0x9b, 0xfd, 0xef, 0xb8, 0xd3, 0xec, 0xe7, 0x99, 0xfc, 0xef, 0x54, 0xa7, 0xb9, 0xbd, 0xbd,
    0xe3, 0xcf, 0x51, 0x7a, 0x4e, 0x5d, 0xee, 0xd6, 0xde, 0x2e, 0x27, 0xcf, 0xbc, 0x8d, 0x6d, 0xa1, 0xfe, 0x3e, 0xeb, 0x1e,
    0x75, 0x96, 0xee, 0x9e, 0xff, 0x9d, 0xb6, 0x63, 0x0d, 0xd6, 0xe5, 0xdf, 0xab, 0xf6, 0xf6, 0xae, 0xf9, 0xbf, 0x47, 0xa5,
    0x95, 0x9e, 0xfc, 0xef, 0x76, 0xdc, 0x3e, 0xb1, 0xfd, 0x2f, 0x1d, 0xd5, 0xee, 0x33, 0x97, 0xe5, 0xd4, 0x8c, 0x7f, 0xe7,
    0xba, 0xbc, 0xef, 0x93, 0xff, 0x22, 0xff, 0xa7, 0xf4, 0xff, 0xcf, 0x5b, 0x36, 0xdf, 0x37, 0x58, 0x71, 0xfd, 0xff, 0x5e,
    0x75, 0x79, 0xdf, 0xa9, 0xfd, 0xdf, 0xa7, 0xce, 0xe2, 0x33, 0xf3, 0xbf, 0x63, 0x7b, 0xaa, 0x2e, 0xa7, 0xfc, 0xd7, 0xcd,
    0xd2, 0xb7, 0xcb, 0x11, 0x15, 0x3f, 0x22, 0x76, 0xaa, 0x57, 0x9d, 0x7d, 0x6d, 0xb0, 0xff, 0x0e, 0xfb, 0x13, 0xea, 0x72,
    0xde, 0x27, 0xff, 0x33, 0xd7, 0xff, 0xcf, 0x4f, 0x5f, 0x19, 0x7a, 0x0e, 0xf8, 0x15, 0x67, 0xa0, 0xd8, 0xfd, 0xff, 0xb3,
    0xaf, 0x8b, 0x67, 0x3d, 0x97, 0x28, 0xf7, 0x3b, 0x59, 0xcf, 0xa9, 0xcb, 0x79, 0x9f, 0xbb, 0x3d, 0x33, 0xf7, 0xff, 0xfb,
    0xee, 0x2c, 0x96, 0xe1, 0x1c, 0x67, 0x3c, 0xeb, 0xbf, 0x5c, 0x54, 0xc5, 0x38, 0xfa, 0xfd, 0x9f, 0x3b, 0x7d, 0xa3, 0x52,
    0x7d, 0xd7, 0xd5, 0xc9, 0xb1, 0x16, 0xeb, 0x8f, 0xd6, 0x35, 0x55, 0x8c, 0x5f, 0xe7, 0xbf, 0xf5, 0x5d, 0x4a, 0x9e, 0x94,
    0xff, 0x27, 0xec, 0x6d, 0xc7, 0x2c, 0xac, 0xf4, 0x97, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xd0, 0x3d, 0x43, 0xf3, 0xf8, 0xbb, 0xcd, 0x3b, 0xd7, 0x20, 0x05, 0x66, 0xe7, 0xe3, 0x3c, 0x63, 0x3e, 0x1e, 0x90, 0x3d,
    0xb7, 0x71, 0xaf, 0x7a, 0xdc, 0x40, 0xee, 0xfc, 0x7f, 0xf9, 0x07, 0xf9, 0x97, 0x7f, 0x90, 0x7f, 0xf9, 0x07, 0xf9, 0xff,
    0xf9, 0x97, 0x55, 0xfe, 0xe1, 0xe6, 0xf9, 0xaf, 0x4b, 0x6a, 0x9b, 0x03, 0x77, 0x39, 0x03, 0x44, 0xf2, 0x5f, 0xf4, 0xff,
    0xe1, 0xb1, 0xdf, 0x01, 0x30, 0xfe, 0x07, 0xe3, 0x7f, 0xf9, 0x87, 0x77, 0xcc, 0x7f, 0xeb, 0xfb, 0xbf, 0x46, 0xff, 0xf0,
    0x8c, 0xf1, 0xbf, 0x2d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0xf2, 0xff, 0x3f, 0xdb, 0x01, 0xe4, 0x1f, 0x78, 0xbb, 0xfc, 0xff, 0x0b };

// Font glyphs rectangles data (on atlas)
RAYGUIAPI const Rectangle candyFontRecs[189] = {
    { 4, 4, 3 , 15 },
    { 15, 4, 2 , 9 },
    { 25, 4, 3 , 2 },
    { 36, 4, 8 , 9 },
    { 52, 4, 6 , 11 },
    { 66, 4, 7 , 9 },
    { 81, 4, 7 , 9 },
    { 96, 4, 1 , 2 },
    { 105, 4, 3 , 11 },
    { 116, 4, 3 , 11 },
    { 127, 4, 7 , 7 },
    { 142, 4, 6 , 5 },
    { 156, 4, 2 , 3 },
    { 166, 4, 5 , 1 },
    { 179, 4, 2 , 2 },
    { 189, 4, 5 , 10 },
    { 202, 4, 6 , 9 },
    { 216, 4, 4 , 9 },
    { 228, 4, 6 , 9 },
    { 242, 4, 6 , 9 },
    { 256, 4, 7 , 9 },
    { 271, 4, 6 , 9 },
    { 285, 4, 6 , 9 },
    { 299, 4, 6 , 9 },
    { 313, 4, 6 , 9 },
    { 327, 4, 6 , 9 },
    { 341, 4, 2 , 6 },
    { 351, 4, 2 , 7 },
    { 361, 4, 4 , 6 },
    { 373, 4, 5 , 3 },
    { 386, 4, 4 , 6 },
    { 398, 4, 6 , 9 },
    { 412, 4, 8 , 7 },
    { 428, 4, 6 , 9 },
    { 442, 4, 6 , 9 },
    { 456, 4, 6 , 9 },
    { 470, 4, 6 , 9 },
    { 484, 4, 6 , 9 },
    { 4, 27, 6 , 9 },
    { 18, 27, 6 , 9 },
    { 32, 27, 6 , 9 },
    { 46, 27, 2 , 9 },
    { 56, 27, 6 , 9 },
    { 70, 27, 6 , 9 },
    { 84, 27, 6 , 9 },
    { 98, 27, 8 , 9 },
    { 114, 27, 6 , 9 },
    { 128, 27, 6 , 9 },
    { 142, 27, 6 , 9 },
    { 156, 27, 6 , 9 },
    { 170, 27, 6 , 9 },
    { 184, 27, 6 , 9 },
    { 198, 27, 6 , 9 },
    { 212, 27, 6 , 9 },
    { 226, 27, 6 , 9 },
    { 240, 27, 8 , 9 },
    { 256, 27, 6 , 9 },
    { 270, 27, 6 , 9 },
    { 284, 27, 6 , 9 },
    { 298, 27, 3 , 11 },
    { 309, 27, 5 , 10 },
    { 322, 27, 3 , 11 },
    { 333, 27, 6 , 4 },
    { 347, 27, 6 , 1 },
    { 361, 27, 3 , 2 },
    { 372, 27, 6 , 7 },
    { 386, 27, 6 , 10 },
    { 400, 27, 6 , 7 },
    { 414, 27, 6 , 10 },
    { 428, 27, 6 , 7 },
    { 442, 27, 5 , 10 },
    { 455, 27, 7 , 10 },
    { 470, 27, 6 , 10 },
    { 484, 27, 2 , 10 },
    { 494, 27, 2 , 12 },
    { 4, 50, 6 , 10 },
    { 18, 50, 3 , 10 },
    { 29, 50, 8 , 7 },
    { 45, 50, 6 , 7 },
    { 59, 50, 6 , 7 },
    { 73, 50, 6 , 10 },
    { 87, 50, 6 , 10 },
    { 101, 50, 6 , 7 },
    { 115, 50, 6 , 7 },
    { 129, 50, 3 , 10 },
    { 140, 50, 6 , 7 },
    { 154, 50, 6 , 7 },
    { 168, 50, 8 , 7 },
    { 184, 50, 6 , 7 },
    { 198, 50, 6 , 10 },
    { 212, 50, 6 , 7 },
    { 226, 50, 4 , 11 },
    { 238, 50, 1 , 11 },
    { 247, 50, 4 , 11 },
    { 259, 50, 6 , 2 },
    { 273, 50, 2 , 10 },
    { 283, 50, 6 , 11 },
    { 297, 50, 8 , 9 },
    { 313, 50, 7 , 9 },
    { 328, 50, 6 , 9 },
    { 342, 50, 0 , 0 },
    { 350, 50, 6 , 11 },
    { 364, 50, 0 , 0 },
    { 372, 50, 8 , 8 },
    { 388, 50, 5 , 7 },
    { 401, 50, 7 , 6 },
    { 416, 50, 7 , 12 },
    { 431, 50, 8 , 8 },
    { 447, 50, 6 , 1 },
    { 461, 50, 4 , 5 },
    { 473, 50, 6 , 7 },
    { 487, 50, 3 , 5 },
    { 498, 50, 3 , 5 },
    { 4, 73, 0 , 0 },
    { 12, 73, 6 , 9 },
    { 26, 73, 6 , 9 },
    { 40, 73, 4 , 4 },
    { 52, 73, 0 , 0 },
    { 60, 73, 2 , 5 },
    { 70, 73, 4 , 7 },
    { 82, 73, 7 , 6 },
    { 97, 73, 10 , 9 },
    { 115, 73, 10 , 7 },
    { 133, 73, 0 , 0 },
    { 141, 73, 6 , 9 },
    { 155, 73, 6 , 12 },
    { 169, 73, 6 , 12 },
    { 183, 73, 6 , 12 },
    { 197, 73, 6 , 12 },
    { 211, 73, 6 , 11 },
    { 225, 73, 6 , 12 },
    { 239, 73, 10 , 9 },
    { 257, 73, 6 , 12 },
    { 271, 73, 6 , 12 },
    { 285, 73, 6 , 12 },
    { 299, 73, 6 , 12 },
    { 313, 73, 6 , 11 },
    { 327, 73, 3 , 12 },
    { 338, 73, 3 , 12 },
    { 349, 73, 4 , 12 },
    { 361, 73, 4 , 11 },
    { 373, 73, 8 , 9 },
    { 389, 73, 6 , 12 },
    { 403, 73, 6 , 12 },
    { 417, 73, 6 , 12 },
    { 431, 73, 6 , 12 },
    { 445, 73, 6 , 12 },
    { 459, 73, 6 , 11 },
    { 473, 73, 5 , 5 },
    { 486, 73, 9 , 10 },
    { 4, 96, 6 , 12 },
    { 18, 96, 6 , 12 },
    { 32, 96, 6 , 12 },
    { 46, 96, 6 , 11 },
    { 60, 96, 6 , 12 },
    { 74, 96, 7 , 12 },
    { 89, 96, 6 , 10 },
    { 103, 96, 6 , 10 },
    { 117, 96, 6 , 10 },
    { 131, 96, 6 , 10 },
    { 145, 96, 6 , 10 },
    { 159, 96, 6 , 9 },
    { 173, 96, 6 , 11 },
    { 187, 96, 10 , 7 },
    { 205, 96, 6 , 10 },
    { 219, 96, 6 , 10 },
    { 233, 96, 6 , 10 },
    { 247, 96, 6 , 10 },
    { 261, 96, 6 , 9 },
    { 275, 96, 3 , 10 },
    { 286, 96, 3 , 10 },
    { 297, 96, 4 , 10 },
    { 309, 96, 4 , 9 },
    { 321, 96, 7 , 12 },
    { 336, 96, 6 , 13 },
    { 350, 96, 6 , 10 },
    { 364, 96, 6 , 10 },
    { 378, 96, 6 , 10 },
    { 392, 96, 6 , 10 },
    { 406, 96, 6 , 9 },
    { 420, 96, 7 , 12 },
    { 435, 96, 8 , 8 },
    { 451, 96, 6 , 10 },
    { 465, 96, 6 , 10 },
    { 479, 96, 6 , 10 },
    { 493, 96, 6 , 9 },
    { 4, 119, 6 , 13 },
    { 18, 119, 7 , 12 },
    { 33, 119, 6 , 12 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
RAYGUIAPI const GlyphInfo candyFontGlyphs[189] = {
    { 32, 0, 12, 3, { 0 }},
    { 33, 0, 3, 3, { 0 }},
    { 34, 0, 2, 4, { 0 }},
    { 35, 0, 3, 9, { 0 }},
    { 36, 0, 2, 7, { 0 }},
    { 37, 0, 3, 8, { 0 }},
    { 38, 0, 3, 8, { 0 }},
    { 39, 0, 2, 2, { 0 }},
    { 40, 1, 2, 5, { 0 }},
    { 41, 1, 2, 5, { 0 }},
    { 42, 0, 4, 8, { 0 }},
    { 43, 0, 6, 7, { 0 }},
    { 44, 0, 10, 3, { 0 }},
    { 45, 0, 7, 6, { 0 }},
    { 46, 0, 10, 3, { 0 }},
    { 47, 1, 3, 7, { 0 }},
    { 48, 0, 3, 7, { 0 }},
    { 49, 0, 3, 5, { 0 }},
    { 50, 0, 3, 7, { 0 }},
    { 51, 0, 3, 7, { 0 }},
    { 52, 0, 3, 8, { 0 }},
    { 53, 0, 3, 7, { 0 }},
    { 54, 0, 3, 7, { 0 }},
    { 55, 0, 3, 7, { 0 }},
    { 56, 0, 3, 7, { 0 }},
    { 57, 0, 3, 7, { 0 }},
    { 58, 0, 6, 3, { 0 }},
    { 59, 0, 6, 3, { 0 }},
    { 60, 1, 5, 6, { 0 }},
    { 61, 1, 7, 7, { 0 }},
    { 62, 1, 5, 6, { 0 }},
    { 63, 0, 3, 7, { 0 }},
    { 64, 0, 4, 9, { 0 }},
    { 65, 0, 3, 7, { 0 }},
    { 66, 0, 3, 7, { 0 }},
    { 67, 0, 3, 7, { 0 }},
    { 68, 0, 3, 7, { 0 }},
    { 69, 0, 3, 7, { 0 }},
    { 70, 0, 3, 7, { 0 }},
    { 71, 0, 3, 7, { 0 }},
    { 72, 0, 3, 7, { 0 }},
    { 73, 0, 3, 3, { 0 }},
    { 74, 0, 3, 7, { 0 }},
    { 75, 0, 3, 7, { 0 }},
    { 76, 0, 3, 7, { 0 }},
    { 77, 0, 3, 9, { 0 }},
    { 78, 0, 3, 7, { 0 }},
    { 79, 0, 3, 7, { 0 }},
    { 80, 0, 3, 7, { 0 }},
    { 81, 0, 3, 7, { 0 }},
    { 82, 0, 3, 7, { 0 }},
    { 83, 0, 3, 7, { 0 }},
    { 84, 0, 3, 7, { 0 }},
    { 85, 0, 3, 7, { 0 }},
    { 86, 0, 3, 7, { 0 }},
    { 87, 0, 3, 9, { 0 }},
    { 88, 0, 3, 7, { 0 }},
    { 89, 0, 3, 7, { 0 }},
    { 90, 0, 3, 7, { 0 }},
    { 91, 1, 2, 5, { 0 }},
    { 92, 1, 3, 7, { 0 }},
    { 93, 1, 2, 5, { 0 }},
    { 94, 0, 3, 7, { 0 }},
    { 95, 0, 11, 7, { 0 }},
    { 96, 0, 0, 4, { 0 }},
    { 97, 0, 5, 7, { 0 }},
    { 98, 0, 2, 7, { 0 }},
    { 99, 0, 5, 7, { 0 }},
    { 100, 0, 2, 7, { 0 }},
    { 101, 0, 5, 7, { 0 }},
    { 102, 0, 2, 6, { 0 }},
    { 103, 0, 5, 7, { 0 }},
    { 104, 0, 2, 7, { 0 }},
    { 105, 0, 2, 3, { 0 }},
    { 106, 0, 2, 3, { 0 }},
    { 107, 0, 2, 7, { 0 }},
    { 108, 0, 2, 4, { 0 }},
    { 109, 0, 5, 9, { 0 }},
    { 110, 0, 5, 7, { 0 }},
    { 111, 0, 5, 7, { 0 }},
    { 112, 0, 5, 7, { 0 }},
    { 113, 0, 5, 7, { 0 }},
    { 114, 0, 5, 7, { 0 }},
    { 115, 0, 5, 7, { 0 }},
    { 116, 0, 2, 4, { 0 }},
    { 117, 0, 5, 7, { 0 }},
    { 118, 0, 5, 7, { 0 }},
    { 119, 0, 5, 9, { 0 }},
    { 120, 0, 5, 7, { 0 }},
    { 121, 0, 5, 7, { 0 }},
    { 122, 0, 5, 7, { 0 }},
    { 123, 1, 2, 6, { 0 }},
    { 124, 1, 2, 3, { 0 }},
    { 125, 1, 2, 6, { 0 }},
    { 126, 0, 0, 7, { 0 }},
    { 161, 0, 3, 3, { 0 }},
    { 162, 0, 2, 7, { 0 }},
    { 163, 0, 3, 9, { 0 }},
    { 8364, 0, 3, 8, { 0 }},
    { 165, 0, 3, 7, { 0 }},
    { 352, 0, 12, 1, { 0 }},
    { 167, 0, 2, 7, { 0 }},
    { 353, 0, 12, 1, { 0 }},
    { 169, 0, 0, 9, { 0 }},
    { 170, 0, 0, 6, { 0 }},
    { 171, 1, 5, 9, { 0 }},
    { 172, 0, 0, 7, { 0 }},
    { 174, 0, 0, 9, { 0 }},
    { 175, 0, 0, 7, { 0 }},
    { 176, 0, 0, 5, { 0 }},
    { 177, 0, 4, 7, { 0 }},
    { 178, 0, 0, 4, { 0 }},
    { 179, 0, 0, 4, { 0 }},
    { 381, 0, 12, 1, { 0 }},
    { 181, 0, 5, 7, { 0 }},
    { 182, 0, 3, 7, { 0 }},
    { 183, 0, 6, 5, { 0 }},
    { 382, 0, 12, 1, { 0 }},
    { 185, 0, 0, 3, { 0 }},
    { 186, 0, 0, 5, { 0 }},
    { 187, 1, 5, 9, { 0 }},
    { 338, 0, 3, 11, { 0 }},
    { 339, 0, 5, 11, { 0 }},
    { 376, 0, 12, 1, { 0 }},
    { 191, 0, 4, 7, { 0 }},
    { 192, 0, 0, 7, { 0 }},
    { 193, 0, 0, 7, { 0 }},
    { 194, 0, 0, 7, { 0 }},
    { 195, 0, 0, 7, { 0 }},
    { 196, 0, 1, 7, { 0 }},
    { 197, 0, 0, 7, { 0 }},
    { 198, 0, 3, 11, { 0 }},
    { 199, 0, 3, 7, { 0 }},
    { 200, 0, 0, 7, { 0 }},
    { 201, 0, 0, 7, { 0 }},
    { 202, 0, 0, 7, { 0 }},
    { 203, 0, 1, 7, { 0 }},
    { 204, -1, 0, 3, { 0 }},
    { 205, 0, 0, 3, { 0 }},
    { 206, -1, 0, 3, { 0 }},
    { 207, -1, 1, 3, { 0 }},
    { 208, 0, 3, 9, { 0 }},
    { 209, 0, 0, 7, { 0 }},
    { 210, 0, 0, 7, { 0 }},
    { 211, 0, 0, 7, { 0 }},
    { 212, 0, 0, 7, { 0 }},
    { 213, 0, 0, 7, { 0 }},
    { 214, 0, 1, 7, { 0 }},
    { 215, 0, 7, 6, { 0 }},
    { 216, 0, 3, 10, { 0 }},
    { 217, 0, 0, 7, { 0 }},
    { 218, 0, 0, 7, { 0 }},
    { 219, 0, 0, 7, { 0 }},
    { 220, 0, 1, 7, { 0 }},
    { 221, 0, 0, 7, { 0 }},
    { 222, 0, 0, 7, { 0 }},
    { 223, 0, 3, 7, { 0 }},
    { 224, 0, 2, 7, { 0 }},
    { 225, 0, 2, 7, { 0 }},
    { 226, 0, 2, 7, { 0 }},
    { 227, 0, 2, 7, { 0 }},
    { 228, 0, 3, 7, { 0 }},
    { 229, 0, 1, 7, { 0 }},
    { 230, 0, 5, 11, { 0 }},
    { 231, 0, 5, 7, { 0 }},
    { 232, 0, 2, 7, { 0 }},
    { 233, 0, 2, 7, { 0 }},
    { 234, 0, 2, 7, { 0 }},
    { 235, 0, 3, 7, { 0 }},
    { 236, 0, 2, 4, { 0 }},
    { 237, 0, 2, 4, { 0 }},
    { 238, 0, 2, 4, { 0 }},
    { 239, 0, 3, 4, { 0 }},
    { 240, 0, 0, 7, { 0 }},
    { 241, 0, 2, 7, { 0 }},
    { 242, 0, 2, 7, { 0 }},
    { 243, 0, 2, 7, { 0 }},
    { 244, 0, 2, 7, { 0 }},
    { 245, 0, 2, 7, { 0 }},
    { 246, 0, 3, 7, { 0 }},
    { 247, 0, 0, 7, { 0 }},
    { 248, 0, 5, 9, { 0 }},
    { 249, 0, 2, 7, { 0 }},
    { 250, 0, 2, 7, { 0 }},
    { 251, 0, 2, 7, { 0 }},
    { 252, 0, 3, 7, { 0 }},
    { 253, 0, 2, 7, { 0 }},
    { 254, 0, 0, 7, { 0 }},
    { 255, 0, 3, 7, { 0 }},
};

// Style loading function: Candy
RAYGUIAPI void GuiLoadStyleCandy(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < CANDY_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(candyStyleProps[i].controlId, candyStyleProps[i].propertyId, candyStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int candyFontDataSize = 0;
    unsigned char *data = DecompressData(candyFontData, CANDY_STYLE_FONT_ATLAS_COMP_SIZE, &candyFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 15;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, candyFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, candyFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleAshes();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define ASHES_STYLE_PROPS_COUNT  16

// Custom style name: Ashes
RAYGUIAPI const GuiStyleProp ashesStyleProps[ASHES_STYLE_PROPS_COUNT] = {
    { 0, 0, 0xf0f0f0ff },    // DEFAULT_BORDER_COLOR_NORMAL
    { 0, 1, 0x868686ff },    // DEFAULT_BASE_COLOR_NORMAL
    { 0, 2, 0xe6e6e6ff },    // DEFAULT_TEXT_COLOR_NORMAL
    { 0, 3, 0x929999ff },    // DEFAULT_BORDER_COLOR_FOCUSED
    { 0, 4, 0xeaeaeaff },    // DEFAULT_BASE_COLOR_FOCUSED
    { 0, 5, 0x98a1a8ff },    // DEFAULT_TEXT_COLOR_FOCUSED
    { 0, 6, 0x3f3f3fff },    // DEFAULT_BORDER_COLOR_PRESSED
    { 0, 7, 0xf6f6f6ff },    // DEFAULT_BASE_COLOR_PRESSED
    { 0, 8, 0x414141ff },    // DEFAULT_TEXT_COLOR_PRESSED
    { 0, 9, 0x8b8b8bff },    // DEFAULT_BORDER_COLOR_DISABLED
    { 0, 10, 0x777777ff },    // DEFAULT_BASE_COLOR_DISABLED
    { 0, 11, 0x959595ff },    // DEFAULT_TEXT_COLOR_DISABLED
    { 0, 16, 0x00000010 },    // DEFAULT_TEXT_SIZE
    { 0, 18, 0x9dadb1ff },    // DEFAULT_LINE_COLOR
    { 0, 19, 0x6b6b6bff },    // DEFAULT_BACKGROUND_COLOR
    { 0, 20, 0x00000018 },    // DEFAULT_TEXT_LINE_SPACING
};

// WARNING: This style uses a custom font: "v5loxical.ttf" (size: 16, spacing: 1)

#define ASHES_STYLE_FONT_ATLAS_COMP_SIZE 2042

// Font atlas image pixels data: DEFLATE compressed
RAYGUIAPI unsigned char ashesFontData[ASHES_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0xd1, 0xb2, 0xab, 0x36, 0x0c, 0x05, 0x50, 0xff, 0xff, 0x4f, 0xab, 0x0f, 0x9d, 0x4e, 0xdb, 0x69, 0x13, 0x90, 0x2c,
    0x83, 0x21, 0xeb, 0xae, 0xb7, 0x9c, 0xdc, 0x40, 0x0c, 0x1b, 0x63, 0x88, 0x45, 0x0c, 0x00, 0x00, 0x00, 0xe0, 0xe7, 0xc5,
    0xff, 0xbe, 0x12, 0x1f, 0xdf, 0x19, 0xa7, 0x3f, 0xe7, 0xdf, 0xaf, 0xc7, 0x87, 0xbf, 0xe6, 0x3e, 0x2f, 0x4e, 0x2f, 0xb7,
    0xf2, 0x97, 0xf8, 0xdf, 0xf5, 0x8b, 0xe9, 0xa5, 0xc6, 0xe9, 0x6f, 0xf3, 0xed, 0xbd, 0xdf, 0x96, 0xf9, 0xe9, 0x93, 0x22,
    0xb1, 0x8c, 0xec, 0xfb, 0x3f, 0x7f, 0xce, 0xd5, 0x2d, 0x1e, 0x5f, 0xfe, 0xf2, 0xfd, 0xaf, 0xd9, 0xd7, 0xe3, 0xe4, 0xab,
    0xdf, 0xfe, 0x12, 0x07, 0x7f, 0x39, 0x9b, 0xbb, 0x78, 0x40, 0xfe, 0xe3, 0xcb, 0xe7, 0x7d, 0xff, 0xc4, 0x98, 0x38, 0x26,
    0x9c, 0x5b, 0xee, 0x95, 0xed, 0xf9, 0x69, 0x4f, 0xcf, 0x6d, 0xeb, 0xef, 0x6d, 0x9c, 0xcb, 0x5c, 0x34, 0xac, 0xff, 0x6c,
    0xab, 0x45, 0xc3, 0xbb, 0xe3, 0xc4, 0xde, 0x92, 0xd9, 0x9b, 0xb3, 0x69, 0xab, 0xbd, 0x7f, 0x34, 0x6c, 0xaf, 0x6b, 0xf3,
    0xff, 0xcf, 0x7f, 0x23, 0x75, 0x14, 0x1c, 0x4d, 0xf9, 0xcf, 0xf7, 0xb9, 0xdf, 0x8e, 0xbe, 0x71, 0xd1, 0xd9, 0xd4, 0xb9,
    0x3d, 0xb4, 0xfe, 0x7a, 0xdf, 0xa7, 0x5f, 0x9d, 0xff, 0x68, 0xfc, 0x1f, 0x47, 0x6b, 0x17, 0xf2, 0x3f, 0xbd, 0x15, 0xa2,
    0xd8, 0xdb, 0x56, 0x7b, 0xa2, 0xd9, 0xad, 0x1f, 0x8b, 0x7b, 0xfa, 0x55, 0x09, 0xad, 0x1d, 0xcd, 0x22, 0xf9, 0x7f, 0x72,
    0x7d, 0x6c, 0x4c, 0x6f, 0xbd, 0xf3, 0xfb, 0x44, 0x7e, 0xfd, 0x8e, 0xf7, 0x00, 0xf9, 0xbf, 0x22, 0xff, 0x95, 0xe4, 0xc6,
    0x92, 0xa3, 0x7f, 0xf5, 0xca, 0xc0, 0x8a, 0xb6, 0xcc, 0xe5, 0x3f, 0x1a, 0x5b, 0x20, 0xdb, 0x6f, 0xe7, 0xd7, 0xff, 0xec,
    0x48, 0x3d, 0x6e, 0xec, 0xff, 0xbf, 0x1d, 0x1d, 0x33, 0x47, 0xcd, 0x6c, 0x3a, 0x73, 0xcb, 0xfd, 0x7b, 0x5d, 0xe7, 0xaf,
    0x88, 0xac, 0xc8, 0x7f, 0xf5, 0xfc, 0xbf, 0x72, 0x86, 0x3a, 0x36, 0xc9, 0xf3, 0xf5, 0xf9, 0x8f, 0xf6, 0x16, 0x58, 0x99,
    0xff, 0xb3, 0x6d, 0x1f, 0x0b, 0xc7, 0xff, 0xb9, 0xab, 0xab, 0xfb, 0xf7, 0xff, 0xf7, 0x5f, 0xff, 0x1b, 0xe5, 0xeb, 0x46,
    0xd9, 0x6b, 0x80, 0x71, 0xea, 0xec, 0xe1, 0x29, 0xf9, 0xef, 0x38, 0xfe, 0x55, 0x47, 0x4a, 0x1d, 0x57, 0xb8, 0xb2, 0xf9,
    0x8f, 0x86, 0xb6, 0x8f, 0xa5, 0x47, 0xff, 0x4a, 0xaf, 0x24, 0xff, 0xbd, 0xe3, 0xff, 0x38, 0x71, 0x06, 0x18, 0xaf, 0xe8,
    0xff, 0xf3, 0xa3, 0x9c, 0x68, 0xb8, 0x07, 0x57, 0xb9, 0xbb, 0x9a, 0xed, 0x31, 0x2b, 0xf7, 0x24, 0x77, 0x68, 0x77, 0xfd,
    0xff, 0x75, 0xdb, 0xa0, 0xb6, 0x2f, 0xfc, 0xf5, 0xff, 0x9e, 0xdf, 0xff, 0xef, 0xb4, 0x35, 0x90, 0xff, 0xab, 0xf7, 0xdb,
    0xb8, 0xb5, 0x7f, 0x95, 0x7f, 0xbf, 0x60, 0xeb, 0xca, 0xff, 0xda, 0xdf, 0xff, 0xc4, 0x41, 0x6f, 0x38, 0x1e, 0xd9, 0xff,
    0x5f, 0xb3, 0x36, 0x21, 0x79, 0xe0, 0x58, 0x2f, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xfc, 0xd0, 0xec, 0x99, 0xcc, 0xec, 0xe5, 0x6a, 0x15, 0xe0, 0x8e, 0xf7, 0x57, 0x97, 0x10, 0x6d, 0xf5, 0x1f, 0xfb,
    0xaa, 0x49, 0x7e, 0x9e, 0x4d, 0x9a, 0x99, 0xf7, 0x9c, 0xab, 0x6f, 0x9e, 0xab, 0x24, 0x17, 0xc9, 0x9a, 0x6d, 0x91, 0xac,
    0x5b, 0x9c, 0xdd, 0x8e, 0xf3, 0xb3, 0xec, 0xe7, 0xfe, 0xff, 0xdf, 0xb5, 0x2a, 0xfa, 0xf6, 0xe5, 0xd1, 0xd4, 0x32, 0xd7,
    0xe5, 0x7f, 0xb4, 0xd7, 0x54, 0xcd, 0x57, 0xa8, 0xca, 0xd7, 0xc7, 0x1f, 0x9b, 0xe5, 0x3f, 0xb3, 0x3f, 0x66, 0xeb, 0xfe,
    0x44, 0x61, 0x56, 0x64, 0x4c, 0xcf, 0xb7, 0x8c, 0x85, 0xfb, 0x5d, 0x57, 0x05, 0xc9, 0x31, 0x99, 0xff, 0x3b, 0xf7, 0xfd,
    0x4a, 0x6d, 0x46, 0xf9, 0x7f, 0x43, 0xfe, 0x2b, 0xd5, 0x3d, 0x3b, 0xe6, 0x45, 0x77, 0xd5, 0xc7, 0x8d, 0xf4, 0x79, 0x51,
    0x66, 0x7f, 0x88, 0xa9, 0xca, 0xea, 0x91, 0x3e, 0x0e, 0xef, 0x98, 0x7f, 0xfd, 0x7f, 0xa5, 0x36, 0xde, 0xdd, 0xf9, 0x9f,
    0x7b, 0x22, 0xc0, 0xea, 0xea, 0x07, 0x6b, 0xfb, 0xa7, 0xde, 0xfe, 0x3f, 0x26, 0xf2, 0x9f, 0x7d, 0x86, 0xd9, 0x5e, 0xf9,
    0x9f, 0x3f, 0xff, 0xaf, 0x57, 0x3b, 0xef, 0x1a, 0x05, 0x8f, 0x86, 0xe3, 0x7d, 0xf5, 0x49, 0x82, 0x5d, 0xf9, 0x1f, 0x85,
    0xa7, 0xc0, 0xcd, 0x9e, 0x8f, 0x3e, 0x3b, 0xff, 0xb9, 0x2b, 0x11, 0xeb, 0xf2, 0xff, 0xec, 0xfe, 0x7f, 0xbc, 0xe0, 0xfc,
    0xbf, 0x63, 0x7b, 0x8f, 0x4d, 0xcf, 0xff, 0xbb, 0x46, 0xd1, 0xef, 0xcb, 0x7f, 0xf6, 0x7b, 0x76, 0x7c, 0xfe, 0x5b, 0xcf,
    0xff, 0xe5, 0xff, 0xed, 0xf9, 0xbf, 0xeb, 0x49, 0x66, 0x7b, 0xe4, 0x3f, 0x9a, 0xaa, 0xef, 0xca, 0xff, 0x5b, 0xf3, 0xbf,
    0xeb, 0xf5, 0xbf, 0xe7, 0xe6, 0x7f, 0x94, 0xc6, 0x66, 0x73, 0xef, 0x7e, 0x42, 0xff, 0x3f, 0x0a, 0xd7, 0xac, 0xf2, 0x23,
    0xd9, 0xeb, 0xc6, 0xff, 0xf2, 0x7f, 0xee, 0xee, 0xd9, 0xdd, 0xf9, 0x5f, 0x73, 0xff, 0xab, 0xef, 0xbc, 0xa0, 0xe3, 0xdd,
    0x7b, 0x8f, 0xff, 0x57, 0xff, 0x16, 0x67, 0xed, 0xf2, 0x7b, 0xae, 0xb4, 0x45, 0xf9, 0xba, 0x7d, 0xcf, 0x6f, 0x49, 0xf2,
    0xd7, 0xdf, 0xb2, 0x47, 0xce, 0x38, 0x78, 0x7f, 0x34, 0xf5, 0x73, 0x95, 0xea, 0xf0, 0x3b, 0xe4, 0x3f, 0xfb, 0xd4, 0x9f,
    0xec, 0xbb, 0xf7, 0xc8, 0xff, 0xfa, 0x96, 0xbd, 0x3a, 0xff, 0x4f, 0xfc, 0xb5, 0xe2, 0xef, 0xb5, 0x42, 0x2c, 0x7e, 0xff,
    0x6f, 0xef, 0x0b, 0x7b, 0x3d, 0x31, 0xcb, 0x76, 0xe5, 0xca, 0xbe, 0x15, 0xad, 0x05, 0x8e, 0xac, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x9b, 0xfb, 0x70, 0x34, 0x7f, 0x32, 0x9a, 0xaa, 0x20, 0x64, 0xeb, 0xc0,
    0x8d, 0xd3, 0xb5, 0xf0, 0xa3, 0x54, 0x79, 0x76, 0xbe, 0xba, 0xc2, 0xf7, 0x19, 0xcd, 0x9f, 0xbf, 0x69, 0x14, 0x66, 0x09,
    0x67, 0x9f, 0x6d, 0x50, 0xa9, 0xad, 0x50, 0x5f, 0x56, 0x14, 0xab, 0x48, 0xce, 0xb7, 0x41, 0x14, 0x9e, 0xef, 0x70, 0xf4,
    0x5d, 0xf2, 0x5b, 0x21, 0xf3, 0x5a, 0x65, 0x6e, 0xf9, 0x38, 0xf1, 0xdc, 0x81, 0x38, 0x9c, 0xdf, 0xd0, 0x9b, 0xff, 0xd5,
    0x75, 0xb0, 0x2b, 0xb5, 0x3c, 0xfe, 0xfb, 0xda, 0x7d, 0x75, 0x27, 0xbe, 0x1d, 0x77, 0x2a, 0x55, 0x28, 0xb2, 0xc7, 0xe1,
    0xfc, 0x5e, 0x38, 0xb7, 0xac, 0x91, 0xda, 0x02, 0x95, 0x7a, 0xe6, 0x9d, 0x6d, 0x1a, 0x85, 0x9a, 0x17, 0xe7, 0x7b, 0x9f,
    0x28, 0xb6, 0xfd, 0x4c, 0xcd, 0x9d, 0x33, 0x33, 0x8a, 0xa3, 0xb0, 0xe7, 0x75, 0xd5, 0xc1, 0x8d, 0x96, 0x4c, 0x77, 0x54,
    0xc1, 0xca, 0xd5, 0xee, 0xcd, 0xce, 0x24, 0x8d, 0xc9, 0xfd, 0x38, 0xbf, 0x7f, 0x47, 0x4b, 0x35, 0xae, 0xe3, 0x65, 0xc5,
    0x97, 0x65, 0x65, 0xf2, 0x5f, 0xef, 0x4b, 0xd7, 0xe7, 0x3f, 0xbf, 0x06, 0x99, 0x0a, 0x52, 0x73, 0xbd, 0xe3, 0x7c, 0xfe,
    0x77, 0xed, 0xff, 0xef, 0xce, 0x7f, 0xbe, 0x06, 0xcc, 0xd1, 0x79, 0x77, 0x65, 0x2f, 0x8a, 0x52, 0x26, 0xb3, 0xfb, 0x60,
    0xbd, 0xff, 0x8f, 0xd2, 0x39, 0x55, 0xe7, 0xb6, 0xbd, 0x22, 0xff, 0x63, 0x61, 0xfe, 0xef, 0xee, 0xff, 0xf7, 0xcc, 0x7f,
    0x5f, 0x2d, 0xdf, 0xb9, 0xfc, 0xe7, 0x53, 0x3e, 0x8a, 0x6b, 0xde, 0x77, 0xfe, 0x1f, 0xe9, 0xf1, 0xd3, 0xcc, 0xb2, 0xa2,
    0xe5, 0x5c, 0x6b, 0x1c, 0x3c, 0x9f, 0xf0, 0xf9, 0xf9, 0x1f, 0xc5, 0x33, 0xe0, 0xa3, 0xa7, 0x61, 0xe4, 0xdb, 0x67, 0x76,
    0x84, 0xbb, 0xbe, 0x0a, 0xf2, 0xf5, 0xcf, 0xc1, 0x88, 0x86, 0xb3, 0xfc, 0xa3, 0xca, 0x85, 0xb1, 0x41, 0xfe, 0xeb, 0xd7,
    0x87, 0xbb, 0xfa, 0xff, 0xee, 0xeb, 0xd6, 0x3d, 0xf9, 0x1f, 0xa5, 0x71, 0x4c, 0x5f, 0x3b, 0x8f, 0xa9, 0xfe, 0x7f, 0xbc,
    0x2c, 0xff, 0xd7, 0x3f, 0x07, 0x23, 0x5a, 0x46, 0xf9, 0x51, 0x7c, 0xde, 0x50, 0xef, 0xf9, 0xff, 0xb7, 0xe7, 0xe2, 0x46,
    0xf9, 0xd9, 0x70, 0x6b, 0xf3, 0x7f, 0xee, 0x6a, 0xf5, 0xca, 0xfc, 0x0f, 0xf9, 0x3f, 0x79, 0x87, 0xeb, 0xe8, 0x5e, 0xc5,
    0x48, 0xdf, 0x75, 0x99, 0xbd, 0x77, 0x74, 0xe5, 0xf9, 0xff, 0x75, 0xfd, 0xff, 0x58, 0x72, 0xfd, 0x7f, 0x45, 0x3f, 0xfb,
    0xa4, 0xfe, 0xff, 0xfb, 0x9e, 0xfd, 0x9c, 0xfc, 0x9f, 0x49, 0xdc, 0x78, 0x44, 0x45, 0xd1, 0xae, 0x4a, 0xd0, 0xd7, 0x5e,
    0xff, 0xeb, 0xbe, 0x9f, 0xd1, 0x73, 0xfe, 0x9f, 0xef, 0x49, 0xe7, 0x46, 0xb9, 0x9d, 0xcb, 0xba, 0xee, 0xfc, 0xff, 0x1d,
    0xf9, 0xef, 0x59, 0xbb, 0xfb, 0x2b, 0x20, 0xee, 0x99, 0xff, 0x51, 0xfc, 0x7d, 0xcd, 0xbd, 0xe7, 0xff, 0x57, 0xe6, 0x6c,
    0x8f, 0x7d, 0xe6, 0xbd, 0xf9, 0x3f, 0xfa, 0xfd, 0xcf, 0x13, 0xb7, 0xdb, 0xec, 0xd1, 0x27, 0x16, 0xbd, 0xf7, 0xae, 0xb3,
    0xa3, 0x98, 0xba, 0x56, 0x21, 0xff, 0x9d, 0xf9, 0x7f, 0x53, 0xc2, 0x9e, 0x92, 0xff, 0x55, 0x47, 0x8c, 0xa7, 0x3c, 0x19,
    0x42, 0xfe, 0xaf, 0xcd, 0xff, 0xaf, 0x24, 0xec, 0xbd, 0xf9, 0x7f, 0xdf, 0xd1, 0x2c, 0x3b, 0x86, 0x8e, 0x0b, 0xc7, 0xdd,
    0x71, 0xe3, 0x18, 0xbf, 0xba, 0x5e, 0xd7, 0xac, 0xf3, 0xae, 0x2d, 0xf3, 0x84, 0xb5, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0x67, 0xb9, 0x44, 0xcb, 0x6c, 0xd9, 0x51, 0xaa, 0xc3, 0x9e, 0x9f, 0x0b, 0x99, 0xad,
    0x35, 0x3f, 0x5a, 0xea, 0x14, 0x57, 0xaa, 0xc5, 0x1e, 0xcf, 0x37, 0x8d, 0x64, 0x5b, 0xde, 0xb5, 0xad, 0xa2, 0xbc, 0xf4,
    0x95, 0xdb, 0xe4, 0xf3, 0xac, 0xa0, 0xfc, 0x93, 0x16, 0x2a, 0xd5, 0x33, 0xae, 0xcb, 0xc1, 0xea, 0xd9, 0xfc, 0xf9, 0xba,
    0xfc, 0xf3, 0x55, 0x4f, 0xfb, 0x9e, 0x14, 0xd0, 0xfd, 0x2d, 0x32, 0x75, 0x5e, 0xcf, 0xb5, 0x42, 0x34, 0xb5, 0xe5, 0x5d,
    0xdb, 0xea, 0x68, 0x8d, 0x62, 0xf9, 0xda, 0x76, 0xd5, 0xeb, 0x8e, 0x52, 0x3f, 0x78, 0x47, 0x0e, 0x2a, 0xfd, 0xd1, 0xfd,
    0xb3, 0x5c, 0xe7, 0xe7, 0xf7, 0xd6, 0xf3, 0xff, 0xd4, 0xf3, 0xb1, 0x67, 0x7c, 0xa7, 0xef, 0xb5, 0xb4, 0x76, 0xfa, 0x0e,
    0xd5, 0xfc, 0x77, 0x54, 0x97, 0xe9, 0x9c, 0xe7, 0x1e, 0x5f, 0x6a, 0x2e, 0x47, 0xeb, 0xf9, 0xff, 0x4e, 0xaf, 0x57, 0xf3,
    0x9f, 0x1f, 0x2f, 0xcc, 0x8e, 0x2f, 0xe6, 0x9f, 0x50, 0x94, 0xaf, 0xde, 0x79, 0xdf, 0xeb, 0xdd, 0xcf, 0x48, 0x5a, 0xfd,
    0x7a, 0x25, 0xff, 0xbb, 0xf6, 0x0d, 0x3d, 0xf9, 0x8f, 0x0d, 0xb7, 0x52, 0x7e, 0xf4, 0x36, 0x7f, 0x9e, 0x59, 0x3d, 0xbe,
    0x74, 0xe7, 0x7f, 0xc7, 0xb6, 0xcf, 0x8f, 0xff, 0x9f, 0x97, 0xff, 0x28, 0xf4, 0xc3, 0xf7, 0xf5, 0x83, 0x99, 0xa7, 0x58,
    0xd6, 0xc7, 0xf8, 0xfb, 0xef, 0x83, 0xf2, 0x7f, 0x77, 0xdb, 0x8f, 0x1f, 0xe8, 0xff, 0xf7, 0xd9, 0xef, 0xba, 0xc7, 0xc1,
    0xf2, 0x2f, 0xff, 0xf2, 0x1f, 0x27, 0xc6, 0x63, 0x7b, 0xbc, 0x7e, 0x5d, 0xfe, 0x73, 0x95, 0xf9, 0xd7, 0xbf, 0x5e, 0xcb,
    0xed, 0xda, 0x6d, 0x24, 0xff, 0xfb, 0xe5, 0x7f, 0xe6, 0x89, 0x61, 0x4f, 0x68, 0xf3, 0xde, 0xfb, 0x7f, 0xcf, 0x1a, 0x6b,
    0xea, 0xff, 0xe5, 0xbf, 0x7e, 0xff, 0xff, 0x1d, 0xf9, 0xef, 0xbe, 0x97, 0xf7, 0x94, 0xeb, 0x7f, 0xe3, 0xe0, 0xb9, 0x40,
    0x71, 0xcb, 0x73, 0xc4, 0xd6, 0xe4, 0xff, 0x19, 0xdb, 0xe4, 0x78, 0xaf, 0x7a, 0x52, 0xfe, 0x9f, 0xdf, 0xe6, 0x9d, 0xf7,
    0x25, 0xdf, 0xf1, 0xbd, 0x9f, 0x99, 0xff, 0x37, 0x6c, 0x93, 0xa3, 0x11, 0xd3, 0x9e, 0xfd, 0xff, 0xf3, 0x73, 0x10, 0x8b,
    0x7f, 0x83, 0xb2, 0x6f, 0xfe, 0xc7, 0x6b, 0xfa, 0xff, 0xe7, 0x6f, 0x93, 0xb8, 0xf1, 0x5b, 0xf4, 0xe6, 0x7f, 0xb7, 0xfb,
    0x7f, 0x77, 0xff, 0x06, 0xed, 0x69, 0xe7, 0x3d, 0xcf, 0xfb, 0xfd, 0x4f, 0x6d, 0x4c, 0xfd, 0xa4, 0x7d, 0xf1, 0xae, 0x3b,
    0xe7, 0x2a, 0xe2, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xd6, 0x55, 0xdf, 0x3c, 0x8a,
    0x33, 0xab, 0xce, 0xbd, 0x56, 0xa9, 0xe6, 0x1e, 0xcb, 0xbe, 0x6b, 0xa5, 0x36, 0x4b, 0xb4, 0xcd, 0x5d, 0xea, 0xda, 0x32,
    0x9f, 0x97, 0x1b, 0xc5, 0x2a, 0x2c, 0xbc, 0xeb, 0x08, 0xb0, 0xaa, 0x42, 0xfe, 0x68, 0xa8, 0x92, 0x5f, 0xab, 0xe6, 0x78,
    0x4f, 0x2d, 0xf7, 0xb9, 0x99, 0xb2, 0xeb, 0xb6, 0x4c, 0x6f, 0x9d, 0x7c, 0xde, 0x75, 0x04, 0x58, 0xf9, 0x1c, 0x91, 0x68,
    0xfc, 0xcc, 0xd8, 0xea, 0xbb, 0x8e, 0xaf, 0x33, 0xcf, 0x46, 0xa2, 0xf2, 0xc4, 0xfa, 0xb5, 0x8d, 0xd6, 0x96, 0x85, 0x3b,
    0x8e, 0x59, 0xcf, 0xde, 0x4b, 0xf7, 0xeb, 0x51, 0xe5, 0xff, 0xd7, 0x92, 0xb4, 0x7a, 0x8e, 0xf8, 0x6c, 0x5d, 0x8e, 0xfc,
    0x59, 0xea, 0x7e, 0x35, 0x1b, 0xa2, 0x30, 0xa7, 0x7f, 0x78, 0x4e, 0x06, 0x37, 0xf5, 0x3f, 0x9d, 0x75, 0x3b, 0xae, 0xcf,
    0xff, 0x5e, 0xaf, 0xd7, 0xfa, 0x7f, 0xf9, 0x47, 0xfe, 0xe5, 0x5f, 0xfe, 0x91, 0xff, 0xf7, 0xe6, 0x3f, 0xe4, 0x9f, 0x5b,
    0x47, 0xff, 0x2b, 0x9f, 0xdb, 0x21, 0xff, 0x73, 0x77, 0xff, 0xe4, 0x9f, 0x3b, 0xee, 0xfe, 0xf7, 0x55, 0xc8, 0x95, 0xff,
    0xda, 0xaf, 0xaf, 0xe4, 0x1f, 0xf9, 0x7f, 0x7b, 0xff, 0x2f, 0xff, 0xb8, 0xff, 0x77, 0x4d, 0xfe, 0xd5, 0xc9, 0xce, 0xfd,
    0x4a, 0x3a, 0xf7, 0x24, 0x44, 0xb8, 0xfe, 0xba, 0x85, 0xbd, 0xb1, 0xff, 0xb7, 0x99, 0x47, 0x2d, 0xab, 0xc5, 0xe1, 0x17,
    0xe6, 0x7f, 0xc9, 0x3f, 0xfc, 0xda, 0xc8, 0xaf, 0xfb, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xe3, 0xcf, 0x7f, 0xda, 0x01, 0xe4, 0x1f, 0xf8, 0xb9, 0xfc, 0xff,
    0x01 };

// Font glyphs rectangles data (on atlas)
RAYGUIAPI const Rectangle ashesFontRecs[189] = {
    { 4, 4, 4 , 16 },
    { 16, 4, 1 , 10 },
    { 25, 4, 3 , 3 },
    { 36, 4, 6 , 8 },
    { 50, 4, 5 , 11 },
    { 63, 4, 7 , 8 },
    { 78, 4, 6 , 9 },
    { 92, 4, 1 , 3 },
    { 101, 4, 3 , 12 },
    { 112, 4, 3 , 12 },
    { 123, 4, 5 , 5 },
    { 136, 4, 5 , 5 },
    { 149, 4, 2 , 2 },
    { 159, 4, 4 , 1 },
    { 171, 4, 1 , 1 },
    { 180, 4, 5 , 10 },
    { 193, 4, 4 , 8 },
    { 205, 4, 2 , 8 },
    { 215, 4, 4 , 8 },
    { 227, 4, 4 , 8 },
    { 239, 4, 6 , 8 },
    { 253, 4, 4 , 8 },
    { 265, 4, 4 , 8 },
    { 277, 4, 4 , 8 },
    { 289, 4, 4 , 8 },
    { 301, 4, 4 , 8 },
    { 313, 4, 1 , 5 },
    { 322, 4, 2 , 6 },
    { 332, 4, 4 , 7 },
    { 344, 4, 4 , 4 },
    { 356, 4, 4 , 7 },
    { 368, 4, 4 , 10 },
    { 380, 4, 8 , 7 },
    { 396, 4, 4 , 10 },
    { 408, 4, 4 , 10 },
    { 420, 4, 4 , 10 },
    { 432, 4, 4 , 10 },
    { 444, 4, 4 , 10 },
    { 456, 4, 5 , 10 },
    { 469, 4, 4 , 10 },
    { 481, 4, 4 , 10 },
    { 493, 4, 1 , 10 },
    { 4, 28, 3 , 10 },
    { 15, 28, 4 , 10 },
    { 27, 28, 4 , 10 },
    { 39, 28, 7 , 10 },
    { 54, 28, 4 , 10 },
    { 66, 28, 4 , 10 },
    { 78, 28, 4 , 10 },
    { 90, 28, 5 , 11 },
    { 103, 28, 4 , 10 },
    { 115, 28, 4 , 10 },
    { 127, 28, 5 , 10 },
    { 140, 28, 4 , 10 },
    { 152, 28, 4 , 10 },
    { 164, 28, 7 , 10 },
    { 179, 28, 4 , 10 },
    { 191, 28, 4 , 10 },
    { 203, 28, 4 , 10 },
    { 215, 28, 2 , 12 },
    { 225, 28, 5 , 10 },
    { 238, 28, 2 , 12 },
    { 248, 28, 5 , 3 },
    { 261, 28, 5 , 1 },
    { 274, 28, 2 , 2 },
    { 284, 28, 4 , 8 },
    { 296, 28, 4 , 10 },
    { 308, 28, 3 , 8 },
    { 319, 28, 4 , 10 },
    { 331, 28, 4 , 8 },
    { 343, 28, 3 , 10 },
    { 354, 28, 5 , 11 },
    { 367, 28, 4 , 10 },
    { 379, 28, 1 , 10 },
    { 388, 28, 3 , 13 },
    { 399, 28, 4 , 10 },
    { 411, 28, 2 , 10 },
    { 421, 28, 7 , 8 },
    { 436, 28, 4 , 8 },
    { 448, 28, 4 , 8 },
    { 460, 28, 4 , 11 },
    { 472, 28, 4 , 11 },
    { 484, 28, 3 , 8 },
    { 495, 28, 4 , 8 },
    { 4, 52, 3 , 10 },
    { 15, 52, 4 , 8 },
    { 27, 52, 5 , 8 },
    { 40, 52, 7 , 8 },
    { 55, 52, 4 , 8 },
    { 67, 52, 4 , 11 },
    { 79, 52, 4 , 8 },
    { 91, 52, 4 , 12 },
    { 103, 52, 1 , 10 },
    { 112, 52, 4 , 12 },
    { 124, 52, 4 , 2 },
    { 136, 52, 1 , 10 },
    { 145, 52, 4 , 12 },
    { 157, 52, 5 , 10 },
    { 170, 52, 5 , 10 },
    { 183, 52, 5 , 10 },
    { 196, 52, 6 , 13 },
    { 210, 52, 4 , 10 },
    { 222, 52, 6 , 13 },
    { 236, 52, 7 , 9 },
    { 251, 52, 3 , 7 },
    { 262, 52, 6 , 5 },
    { 276, 52, 6 , 13 },
    { 290, 52, 7 , 9 },
    { 305, 52, 4 , 1 },
    { 317, 52, 3 , 5 },
    { 328, 52, 5 , 7 },
    { 341, 52, 3 , 5 },
    { 352, 52, 6 , 13 },
    { 366, 52, 6 , 13 },
    { 380, 52, 4 , 11 },
    { 392, 52, 6 , 10 },
    { 406, 52, 3 , 3 },
    { 417, 52, 6 , 13 },
    { 431, 52, 2 , 5 },
    { 441, 52, 3 , 5 },
    { 452, 52, 6 , 5 },
    { 466, 52, 7 , 10 },
    { 481, 52, 7 , 8 },
    { 496, 52, 6 , 13 },
    { 4, 76, 4 , 10 },
    { 16, 76, 4 , 13 },
    { 28, 76, 4 , 13 },
    { 40, 76, 4 , 13 },
    { 52, 76, 4 , 13 },
    { 64, 76, 4 , 13 },
    { 76, 76, 4 , 13 },
    { 88, 76, 7 , 10 },
    { 103, 76, 4 , 13 },
    { 115, 76, 4 , 13 },
    { 127, 76, 4 , 13 },
    { 139, 76, 4 , 13 },
    { 151, 76, 4 , 13 },
    { 163, 76, 2 , 13 },
    { 173, 76, 2 , 13 },
    { 183, 76, 3 , 13 },
    { 194, 76, 3 , 13 },
    { 205, 76, 5 , 10 },
    { 218, 76, 4 , 13 },
    { 230, 76, 4 , 13 },
    { 242, 76, 4 , 13 },
    { 254, 76, 4 , 13 },
    { 266, 76, 4 , 13 },
    { 278, 76, 4 , 13 },
    { 290, 76, 5 , 5 },
    { 303, 76, 6 , 12 },
    { 317, 76, 4 , 13 },
    { 329, 76, 4 , 13 },
    { 341, 76, 4 , 13 },
    { 353, 76, 4 , 13 },
    { 365, 76, 4 , 13 },
    { 377, 76, 6 , 13 },
    { 391, 76, 5 , 12 },
    { 404, 76, 4 , 12 },
    { 416, 76, 4 , 12 },
    { 428, 76, 4 , 12 },
    { 440, 76, 4 , 12 },
    { 452, 76, 4 , 12 },
    { 464, 76, 4 , 12 },
    { 476, 76, 7 , 8 },
    { 491, 76, 3 , 11 },
    { 4, 100, 4 , 12 },
    { 16, 100, 4 , 12 },
    { 28, 100, 4 , 12 },
    { 40, 100, 4 , 12 },
    { 52, 100, 3 , 12 },
    { 63, 100, 3 , 12 },
    { 74, 100, 3 , 12 },
    { 85, 100, 3 , 12 },
    { 96, 100, 6 , 13 },
    { 110, 100, 4 , 12 },
    { 122, 100, 4 , 12 },
    { 134, 100, 4 , 12 },
    { 146, 100, 4 , 12 },
    { 158, 100, 4 , 12 },
    { 170, 100, 4 , 12 },
    { 182, 100, 5 , 5 },
    { 195, 100, 6 , 10 },
    { 209, 100, 4 , 12 },
    { 221, 100, 4 , 12 },
    { 233, 100, 4 , 12 },
    { 245, 100, 4 , 12 },
    { 257, 100, 4 , 15 },
    { 269, 100, 6 , 13 },
    { 283, 100, 4 , 15 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
RAYGUIAPI const GlyphInfo ashesFontGlyphs[189] = {
    { 32, 0, 13, 4, { 0 }},
    { 33, 0, 3, 4, { 0 }},
    { 34, 0, 1, 5, { 0 }},
    { 35, 0, 4, 8, { 0 }},
    { 36, 0, 2, 7, { 0 }},
    { 37, 0, 5, 9, { 0 }},
    { 38, 0, 4, 8, { 0 }},
    { 39, 0, 1, 3, { 0 }},
    { 40, 0, 2, 5, { 0 }},
    { 41, 0, 2, 5, { 0 }},
    { 42, 0, 3, 7, { 0 }},
    { 43, 0, 6, 7, { 0 }},
    { 44, 0, 12, 4, { 0 }},
    { 45, 0, 9, 5, { 0 }},
    { 46, 0, 12, 3, { 0 }},
    { 47, 0, 3, 7, { 0 }},
    { 48, 0, 5, 6, { 0 }},
    { 49, 0, 5, 4, { 0 }},
    { 50, 0, 5, 6, { 0 }},
    { 51, 0, 5, 6, { 0 }},
    { 52, 0, 5, 8, { 0 }},
    { 53, 0, 5, 6, { 0 }},
    { 54, 0, 5, 6, { 0 }},
    { 55, 0, 5, 6, { 0 }},
    { 56, 0, 5, 6, { 0 }},
    { 57, 0, 5, 6, { 0 }},
    { 58, 0, 8, 3, { 0 }},
    { 59, 0, 8, 4, { 0 }},
    { 60, 0, 5, 6, { 0 }},
    { 61, 0, 7, 7, { 0 }},
    { 62, 0, 5, 6, { 0 }},
    { 63, 1, 3, 7, { 0 }},
    { 64, 0, 5, 10, { 0 }},
    { 65, 0, 3, 6, { 0 }},
    { 66, 0, 3, 6, { 0 }},
    { 67, 0, 3, 6, { 0 }},
    { 68, 0, 3, 6, { 0 }},
    { 69, 0, 3, 6, { 0 }},
    { 70, 0, 3, 6, { 0 }},
    { 71, 0, 3, 6, { 0 }},
    { 72, 0, 3, 6, { 0 }},
    { 73, 0, 3, 3, { 0 }},
    { 74, 0, 3, 5, { 0 }},
    { 75, 0, 3, 6, { 0 }},
    { 76, 0, 3, 6, { 0 }},
    { 77, 0, 3, 9, { 0 }},
    { 78, 0, 3, 6, { 0 }},
    { 79, 0, 3, 6, { 0 }},
    { 80, 0, 3, 6, { 0 }},
    { 81, 0, 3, 7, { 0 }},
    { 82, 0, 3, 6, { 0 }},
    { 83, 0, 3, 6, { 0 }},
    { 84, 0, 3, 6, { 0 }},
    { 85, 0, 3, 6, { 0 }},
    { 86, 0, 3, 6, { 0 }},
    { 87, 0, 3, 9, { 0 }},
    { 88, 0, 3, 6, { 0 }},
    { 89, 0, 3, 6, { 0 }},
    { 90, 0, 3, 6, { 0 }},
    { 91, 0, 2, 4, { 0 }},
    { 92, 0, 3, 7, { 0 }},
    { 93, 0, 2, 4, { 0 }},
    { 94, 0, 3, 7, { 0 }},
    { 95, 0, 12, 7, { 0 }},
    { 96, 0, 1, 4, { 0 }},
    { 97, 0, 5, 6, { 0 }},
    { 98, 0, 3, 6, { 0 }},
    { 99, 0, 5, 5, { 0 }},
    { 100, 0, 3, 6, { 0 }},
    { 101, 0, 5, 6, { 0 }},
    { 102, 0, 3, 5, { 0 }},
    { 103, 0, 5, 6, { 0 }},
    { 104, 0, 3, 6, { 0 }},
    { 105, 0, 3, 3, { 0 }},
    { 106, 0, 3, 5, { 0 }},
    { 107, 0, 3, 6, { 0 }},
    { 108, 0, 3, 4, { 0 }},
    { 109, 0, 5, 9, { 0 }},
    { 110, 0, 5, 6, { 0 }},
    { 111, 0, 5, 6, { 0 }},
    { 112, 0, 5, 6, { 0 }},
    { 113, 0, 5, 6, { 0 }},
    { 114, 0, 5, 5, { 0 }},
    { 115, 0, 5, 6, { 0 }},
    { 116, 0, 3, 5, { 0 }},
    { 117, 0, 5, 6, { 0 }},
    { 118, 0, 5, 7, { 0 }},
    { 119, 0, 5, 9, { 0 }},
    { 120, 0, 5, 6, { 0 }},
    { 121, 0, 5, 6, { 0 }},
    { 122, 0, 5, 6, { 0 }},
    { 123, 0, 2, 6, { 0 }},
    { 124, 0, 3, 3, { 0 }},
    { 125, 0, 2, 6, { 0 }},
    { 126, 0, 1, 6, { 0 }},
    { 161, 0, 4, 4, { 0 }},
    { 162, 0, 3, 6, { 0 }},
    { 163, 0, 3, 7, { 0 }},
    { 8364, 0, 3, 7, { 0 }},
    { 165, 0, 3, 7, { 0 }},
    { 352, 1, 0, 8, { 0 }},
    { 167, 0, 3, 6, { 0 }},
    { 353, 1, 0, 8, { 0 }},
    { 169, 0, 1, 9, { 0 }},
    { 170, 0, 1, 5, { 0 }},
    { 171, 0, 6, 8, { 0 }},
    { 172, 1, 0, 8, { 0 }},
    { 174, 0, 1, 9, { 0 }},
    { 175, 0, 0, 6, { 0 }},
    { 176, 0, 1, 5, { 0 }},
    { 177, 0, 5, 7, { 0 }},
    { 178, 0, 1, 5, { 0 }},
    { 179, 1, 0, 8, { 0 }},
    { 381, 1, 0, 8, { 0 }},
    { 181, 0, 5, 6, { 0 }},
    { 182, 0, 3, 8, { 0 }},
    { 183, 0, 7, 5, { 0 }},
    { 382, 1, 0, 8, { 0 }},
    { 185, 0, 1, 4, { 0 }},
    { 186, 0, 1, 5, { 0 }},
    { 187, 0, 6, 8, { 0 }},
    { 338, 0, 3, 9, { 0 }},
    { 339, 0, 5, 9, { 0 }},
    { 376, 1, 0, 8, { 0 }},
    { 191, 0, 4, 6, { 0 }},
    { 192, 0, 0, 6, { 0 }},
    { 193, 0, 0, 6, { 0 }},
    { 194, 0, 0, 6, { 0 }},
    { 195, 0, 0, 6, { 0 }},
    { 196, 0, 0, 6, { 0 }},
    { 197, 0, 0, 6, { 0 }},
    { 198, 0, 3, 9, { 0 }},
    { 199, 0, 3, 6, { 0 }},
    { 200, 0, 0, 6, { 0 }},
    { 201, 0, 0, 6, { 0 }},
    { 202, 0, 0, 6, { 0 }},
    { 203, 0, 0, 6, { 0 }},
    { 204, 0, 0, 4, { 0 }},
    { 205, 0, 0, 4, { 0 }},
    { 206, 0, 0, 5, { 0 }},
    { 207, 0, 0, 5, { 0 }},
    { 208, 0, 3, 7, { 0 }},
    { 209, 0, 0, 6, { 0 }},
    { 210, 0, 0, 6, { 0 }},
    { 211, 0, 0, 6, { 0 }},
    { 212, 0, 0, 6, { 0 }},
    { 213, 0, 0, 6, { 0 }},
    { 214, 0, 0, 6, { 0 }},
    { 215, 0, 7, 7, { 0 }},
    { 216, 0, 2, 8, { 0 }},
    { 217, 0, 0, 6, { 0 }},
    { 218, 0, 0, 6, { 0 }},
    { 219, 0, 0, 6, { 0 }},
    { 220, 0, 0, 6, { 0 }},
    { 221, 0, 0, 6, { 0 }},
    { 222, 1, 0, 8, { 0 }},
    { 223, 0, 3, 7, { 0 }},
    { 224, 0, 1, 6, { 0 }},
    { 225, 0, 1, 6, { 0 }},
    { 226, 0, 1, 6, { 0 }},
    { 227, 0, 1, 6, { 0 }},
    { 228, 0, 1, 6, { 0 }},
    { 229, 0, 1, 6, { 0 }},
    { 230, 0, 5, 9, { 0 }},
    { 231, 0, 5, 5, { 0 }},
    { 232, 0, 1, 6, { 0 }},
    { 233, 0, 1, 6, { 0 }},
    { 234, 0, 1, 6, { 0 }},
    { 235, 0, 1, 6, { 0 }},
    { 236, 0, 1, 5, { 0 }},
    { 237, 0, 1, 5, { 0 }},
    { 238, 0, 1, 5, { 0 }},
    { 239, 0, 1, 5, { 0 }},
    { 240, 1, 0, 8, { 0 }},
    { 241, 0, 1, 6, { 0 }},
    { 242, 0, 1, 6, { 0 }},
    { 243, 0, 1, 6, { 0 }},
    { 244, 0, 1, 6, { 0 }},
    { 245, 0, 1, 6, { 0 }},
    { 246, 0, 1, 6, { 0 }},
    { 247, 0, 7, 7, { 0 }},
    { 248, 0, 4, 8, { 0 }},
    { 249, 0, 1, 6, { 0 }},
    { 250, 0, 1, 6, { 0 }},
    { 251, 0, 1, 6, { 0 }},
    { 252, 0, 1, 6, { 0 }},
    { 253, 0, 1, 6, { 0 }},
    { 254, 1, 0, 8, { 0 }},
    { 255, 0, 1, 6, { 0 }},
};

// Style loading function: Ashes
RAYGUIAPI void GuiLoadStyleAshes(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < ASHES_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(ashesStyleProps[i].controlId, ashesStyleProps[i].propertyId, ashesStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int ashesFontDataSize = 0;
    unsigned char *data = DecompressData(ashesFontData, ASHES_STYLE_FONT_ATLAS_COMP_SIZE, &ashesFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 16;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, ashesFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, ashesFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleEnefete();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2024 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define ENEFETE_STYLE_PROPS_COUNT  17

// Custom style name: Enefete
RAYGUIAPI const GuiStyleProp enefeteStyleProps[ENEFETE_STYLE_PROPS_COUNT] = {
    { 0, 0, 0x1980d5ff },    // DEFAULT_BORDER_COLOR_NORMAL
    { 0, 1, 0x4df3ebff },    // DEFAULT_BASE_COLOR_NORMAL
    { 0, 2, 0x103e60ff },    // DEFAULT_TEXT_COLOR_NORMAL
    { 0, 3, 0xe7e2f7ff },    // DEFAULT_BORDER_COLOR_FOCUSED
    { 0, 4, 0x23d4ddff },    // DEFAULT_BASE_COLOR_FOCUSED
    { 0, 5, 0xf1f1f1ff },    // DEFAULT_TEXT_COLOR_FOCUSED
    { 0, 6, 0x6413a6ff },    // DEFAULT_BORDER_COLOR_PRESSED
    { 0, 7, 0xea66d9ff },    // DEFAULT_BASE_COLOR_PRESSED
    { 0, 8, 0x9f00bbff },    // DEFAULT_TEXT_COLOR_PRESSED
    { 0, 9, 0x4b909eff },    // DEFAULT_BORDER_COLOR_DISABLED
    { 0, 10, 0x73c7d0ff },    // DEFAULT_BASE_COLOR_DISABLED
    { 0, 11, 0x448894ff },    // DEFAULT_TEXT_COLOR_DISABLED
    { 0, 16, 0x00000010 },    // DEFAULT_TEXT_SIZE
    { 0, 17, 0x00000000 },    // DEFAULT_TEXT_SPACING
    { 0, 18, 0x1d3f6cff },    // DEFAULT_LINE_COLOR
    { 0, 19, 0x29c9e5ff },    // DEFAULT_BACKGROUND_COLOR
    { 0, 20, 0x00000018 },    // DEFAULT_TEXT_LINE_SPACING
};

// WARNING: This style uses a custom font: "GenericMobileSystemNuevo.ttf" (size: 16, spacing: 0)

#define ENEFETE_STYLE_FONT_ATLAS_COMP_SIZE 2462

// Font atlas image pixels data: DEFLATE compressed
RAYGUIAPI unsigned char enefeteFontData[ENEFETE_STYLE_FONT_ATLAS_COMP_SIZE] = { 0xed,
    0xdd, 0x59, 0xb2, 0xeb, 0x34, 0x10, 0x00, 0x50, 0x6d, 0x83, 0xfd, 0xef, 0x8d, 0x6d, 0x88, 0xa2, 0x28, 0x0a, 0xb8, 0x8f,
    0x58, 0x52, 0x77, 0xcb, 0x53, 0x0e, 0xa7, 0xf8, 0xb9, 0x7e, 0x49, 0x6c, 0xd9, 0x6d, 0x0d, 0xb6, 0x5a, 0xbd, 0x01, 0x00,
    0x00, 0x00, 0x5f, 0xef, 0xcf, 0xff, 0xfe, 0xff, 0x6f, 0xff, 0xb7, 0xe5, 0x78, 0xdb, 0xdf, 0xff, 0x62, 0xb4, 0xa5, 0x7f,
    0xdc, 0x93, 0x7e, 0xb0, 0x6d, 0xee, 0xb7, 0x62, 0xfb, 0xd5, 0x97, 0xca, 0xa6, 0x1f, 0x94, 0x5a, 0x4b, 0xff, 0xbd, 0x1d,
    0x96, 0x7d, 0x0f, 0x94, 0x5e, 0x3b, 0xdc, 0xe7, 0xcf, 0x9f, 0x1c, 0x6f, 0xa9, 0xdc, 0xcf, 0x95, 0x32, 0xad, 0x3c, 0x3f,
    0x2b, 0x25, 0xd8, 0x96, 0xb7, 0x1e, 0x97, 0x54, 0x9b, 0x2a, 0xc7, 0xf1, 0x37, 0xf7, 0x9b, 0xc4, 0xff, 0xf1, 0xd1, 0xfc,
    0x1d, 0x83, 0x47, 0x67, 0x7f, 0xfe, 0xda, 0x68, 0xd3, 0xf7, 0x84, 0x7f, 0x7e, 0xbb, 0x2e, 0x42, 0xb3, 0xdf, 0xd2, 0x87,
    0x77, 0xc2, 0x95, 0x12, 0x3a, 0x2e, 0xbd, 0x16, 0x8a, 0x89, 0xd1, 0x77, 0x46, 0xf6, 0x65, 0x66, 0xdb, 0xdc, 0xfe, 0xc5,
    0xae, 0xf9, 0xf1, 0xf7, 0x1d, 0x5f, 0x69, 0x47, 0x67, 0x78, 0x74, 0xe5, 0x1e, 0x1f, 0xdf, 0xfa, 0x59, 0xcd, 0x44, 0xfd,
    0x9e, 0xf8, 0xef, 0xff, 0x8a, 0xb3, 0x1e, 0x8c, 0xc1, 0x7c, 0xdd, 0xb0, 0x5e, 0x2b, 0x1c, 0xdf, 0xb3, 0xf6, 0x95, 0xf0,
    0x51, 0x39, 0xf5, 0xc0, 0x35, 0xbc, 0x7e, 0xce, 0xa2, 0xbf, 0xb3, 0x23, 0xfe, 0x2b, 0xa2, 0xb8, 0x4d, 0xd4, 0x88, 0x91,
    0x36, 0xc5, 0x71, 0x8b, 0x33, 0x1f, 0xe1, 0xa3, 0x7d, 0x89, 0xb4, 0x67, 0xf2, 0x65, 0xb9, 0xda, 0xe6, 0xd8, 0x17, 0xff,
    0x15, 0xf5, 0xed, 0x7a, 0x5d, 0xb2, 0xaf, 0x7c, 0x67, 0xea, 0xeb, 0xfb, 0xc7, 0xff, 0xfa, 0xb1, 0x55, 0xc4, 0x7f, 0xcd,
    0x59, 0x59, 0x8f, 0xff, 0x2e, 0xfe, 0xa7, 0xeb, 0xff, 0xcf, 0xa5, 0xde, 0x83, 0xfd, 0xaf, 0x7c, 0xaf, 0xbd, 0xaa, 0x9d,
    0x7f, 0x6d, 0xfc, 0x8f, 0xfa, 0x83, 0x6d, 0x39, 0xfe, 0x63, 0xbd, 0xcf, 0x48, 0x6f, 0xa3, 0xaa, 0xfd, 0xbf, 0xb7, 0xf5,
    0x70, 0xdc, 0x7b, 0x8c, 0x44, 0xf8, 0xda, 0x98, 0x47, 0xcd, 0x37, 0x5f, 0x11, 0xff, 0x6d, 0x62, 0x6f, 0xfa, 0xb0, 0xff,
    0x3f, 0x7b, 0x87, 0x5f, 0x8b, 0xff, 0xba, 0xba, 0xe3, 0xca, 0xf8, 0xef, 0xc1, 0x11, 0xb1, 0xea, 0x7e, 0x7c, 0xfc, 0xd8,
    0xd6, 0xc6, 0xe7, 0xce, 0xeb, 0xff, 0x8f, 0xda, 0x81, 0x99, 0xf8, 0xff, 0x96, 0xf6, 0xff, 0x4c, 0x4b, 0x7b, 0x54, 0x8a,
    0xf3, 0xf7, 0xb3, 0x2b, 0x6a, 0xf3, 0x6b, 0xe3, 0xbf, 0x4f, 0x8d, 0x61, 0xdf, 0x3b, 0xfe, 0xdb, 0x72, 0x2f, 0xaf, 0xf2,
    0xda, 0x8d, 0x5e, 0x51, 0xa3, 0x2b, 0x77, 0xe6, 0xae, 0x76, 0xcf, 0xf8, 0x8f, 0x5d, 0xcf, 0x7d, 0xd8, 0xca, 0xef, 0x89,
    0xf8, 0x6f, 0xaf, 0x89, 0xff, 0x1e, 0x1c, 0xed, 0xf8, 0xdc, 0xde, 0x8b, 0x8d, 0xc7, 0x45, 0x5a, 0xf2, 0x99, 0x36, 0x4a,
    0x55, 0xfc, 0xdf, 0xe3, 0x4c, 0xaa, 0xff, 0x2b, 0xc6, 0xff, 0xdb, 0x61, 0x0b, 0xff, 0xbb, 0xe2, 0xff, 0xf8, 0xf9, 0x5f,
    0xbb, 0xfd, 0xb3, 0xba, 0x68, 0xfc, 0xd7, 0x8c, 0xff, 0x9f, 0x7f, 0xb7, 0x9e, 0xe9, 0xb9, 0xe6, 0xe2, 0xff, 0x4e, 0xcf,
    0xff, 0xf6, 0xc6, 0xff, 0xe8, 0xa9, 0xf7, 0x9b, 0xda, 0xff, 0x95, 0xe3, 0xf8, 0xf1, 0x77, 0x6e, 0xf6, 0xbd, 0x53, 0xd4,
    0x42, 0x6f, 0x72, 0xdd, 0x33, 0xfa, 0xa3, 0xcf, 0x81, 0x72, 0xa3, 0x74, 0x57, 0xbf, 0xff, 0x53, 0x5f, 0x82, 0xe7, 0xed,
    0x4f, 0xbf, 0x71, 0x9b, 0x91, 0x77, 0xbd, 0x25, 0xeb, 0xea, 0x3a, 0xf3, 0xf9, 0x6c, 0xfc, 0xb7, 0x9c, 0x1f, 0x76, 0xd6,
    0x6c, 0x4a, 0xe2, 0x19, 0x63, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xe4, 0xf9,
    0x0b, 0x91, 0xb9, 0x86, 0xd1, 0x3c, 0x04, 0xb5, 0xf9, 0x10, 0xfa, 0x8f, 0x3c, 0xac, 0xab, 0xdf, 0x3d, 0x3b, 0xd7, 0x32,
    0x33, 0x4f, 0xb3, 0xf6, 0xb3, 0x55, 0x99, 0x25, 0x6a, 0x67, 0x9e, 0x8f, 0xf3, 0xdd, 0xac, 0x66, 0x6b, 0x8e, 0x65, 0x41,
    0x5b, 0xcf, 0x69, 0x9e, 0xc9, 0xc3, 0x57, 0x3d, 0xb7, 0xba, 0x26, 0x3f, 0xda, 0xfc, 0x9c, 0xf7, 0x9a, 0x4c, 0x23, 0x75,
    0xf1, 0xbf, 0x7e, 0x1e, 0xfa, 0x8f, 0xfd, 0xe8, 0x17, 0x64, 0x62, 0xa8, 0xdf, 0x3a, 0xf7, 0xf7, 0x3b, 0xc5, 0xff, 0xec,
    0x3d, 0x38, 0x7b, 0xf5, 0xc6, 0x67, 0xdc, 0x8d, 0xb2, 0x4f, 0x5c, 0x9f, 0x5b, 0xa5, 0x15, 0xe5, 0x47, 0xad, 0x8f, 0xff,
    0x76, 0x52, 0xfd, 0x1f, 0x8d, 0xff, 0x3e, 0x5c, 0xcd, 0xe6, 0xbe, 0xf1, 0xbf, 0x9e, 0x77, 0x22, 0xfb, 0xab, 0xb9, 0x95,
    0x1c, 0xe2, 0xb9, 0xc6, 0xf3, 0xf1, 0x9f, 0xcb, 0x14, 0x74, 0x6e, 0xdd, 0x7a, 0x7d, 0xfc, 0x47, 0x4b, 0xe4, 0xaa, 0xf8,
    0x8f, 0x5f, 0x25, 0x7d, 0x98, 0xcd, 0xfa, 0x8e, 0xf1, 0xdf, 0xbf, 0x3e, 0xfe, 0xeb, 0x73, 0xf9, 0x46, 0x23, 0x20, 0x92,
    0x07, 0x7d, 0xa6, 0x5f, 0xbd, 0xda, 0xb3, 0x8b, 0xf7, 0xbb, 0xa2, 0xf5, 0xff, 0xe8, 0xf3, 0x33, 0x47, 0x72, 0x75, 0xfc,
    0xb7, 0x61, 0x84, 0xf7, 0x44, 0x94, 0xc6, 0x47, 0x40, 0x32, 0x39, 0xaf, 0xae, 0x89, 0xff, 0x16, 0x5a, 0x31, 0x30, 0x5e,
    0xcf, 0xdf, 0x3d, 0xfe, 0xab, 0xfb, 0x06, 0xd1, 0x11, 0x83, 0xea, 0x7d, 0xbc, 0x5f, 0xff, 0xbf, 0x17, 0xb4, 0x12, 0x77,
    0xc4, 0x7f, 0xb6, 0x7c, 0xa2, 0xd1, 0x14, 0x8b, 0xff, 0xf9, 0xda, 0x3d, 0xb6, 0x9a, 0x40, 0x6c, 0xdb, 0x7b, 0xe3, 0xbf,
    0x36, 0x7f, 0xea, 0xfa, 0x58, 0xf1, 0x9b, 0xe2, 0x3f, 0xda, 0xff, 0x9f, 0xef, 0x71, 0xdd, 0x2b, 0xfe, 0xdb, 0xc4, 0xfa,
    0x3c, 0x67, 0xc7, 0xff, 0x28, 0xdb, 0xea, 0xdb, 0xe2, 0x3f, 0x1a, 0x5b, 0xd1, 0xf5, 0x8c, 0x5a, 0xb8, 0xa6, 0x7b, 0x7b,
    0xfd, 0xdf, 0x5e, 0x1a, 0xff, 0xf1, 0xf5, 0x65, 0x9e, 0x15, 0xff, 0xd5, 0xad, 0x86, 0xeb, 0xf3, 0xea, 0x45, 0x47, 0x1b,
    0xe3, 0xa3, 0x94, 0xef, 0x6e, 0xff, 0xf7, 0x0d, 0xad, 0xf0, 0xec, 0xf3, 0xff, 0xab, 0xe2, 0x7f, 0xd7, 0xf3, 0xbf, 0x36,
    0xbd, 0xe6, 0xdf, 0xae, 0xbd, 0x12, 0xff, 0xd5, 0xf1, 0x5f, 0xb9, 0x76, 0xe2, 0xda, 0xbf, 0xa8, 0x7e, 0xff, 0xa7, 0x7e,
    0x24, 0x6d, 0xff, 0xfb, 0x3f, 0x7b, 0xca, 0xa7, 0x4d, 0xad, 0x98, 0xb6, 0xfe, 0xab, 0x6d, 0x72, 0x55, 0xe3, 0xd8, 0x3e,
    0xd7, 0xbf, 0x7d, 0x73, 0xf7, 0xf8, 0x8f, 0xbd, 0x6d, 0x10, 0xfd, 0x64, 0xfc, 0x13, 0xf7, 0x59, 0x0b, 0xec, 0x3b, 0xdf,
    0xa2, 0x54, 0x0a, 0xd5, 0x25, 0xa4, 0x54, 0x71, 0x07, 0x78, 0xd7, 0x7b, 0xe6, 0xbb, 0xfe, 0x3d, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf7, 0xcd, 0x5e, 0x99, 0xcd, 0xb9, 0xfd, 0xeb, 0x5f, 0x7b, 0x68, 0xbe,
    0x6b, 0x4f, 0x7e, 0xae, 0x2f, 0xe7, 0xc1, 0x58, 0x9b, 0x77, 0x5a, 0x91, 0x83, 0xa1, 0x3a, 0x8f, 0xd4, 0xb8, 0x5c, 0x66,
    0xcf, 0xf4, 0x78, 0x26, 0xf1, 0xea, 0x15, 0x12, 0x99, 0xd7, 0x93, 0xc9, 0xa8, 0x93, 0x59, 0x4b, 0x20, 0x9e, 0x09, 0xb1,
    0x2d, 0x7e, 0x6b, 0xec, 0xac, 0xfd, 0x3a, 0xbf, 0x3a, 0x9a, 0xef, 0xb3, 0x26, 0xfe, 0x63, 0xb9, 0x25, 0xaf, 0xc8, 0xb4,
    0x16, 0xbf, 0x9a, 0x6a, 0xf7, 0xb0, 0xbe, 0x54, 0xe6, 0xf6, 0x36, 0xf7, 0xf7, 0xcc, 0x9c, 0xfe, 0x5c, 0xee, 0x81, 0xd5,
    0x92, 0xe8, 0x53, 0xd9, 0x52, 0x56, 0x3f, 0xf9, 0xf9, 0x73, 0xc7, 0xdb, 0xfa, 0x54, 0xe6, 0x88, 0xd5, 0x6f, 0xfd, 0x6d,
    0xf0, 0xf9, 0x7e, 0x78, 0xe7, 0xad, 0x9d, 0xd1, 0xfa, 0x94, 0xf8, 0x5f, 0xbf, 0x77, 0x9f, 0x1d, 0xff, 0xf1, 0xfb, 0x54,
    0x2b, 0xcc, 0xd2, 0xb3, 0xbe, 0x36, 0x46, 0x64, 0xe6, 0x6e, 0x3e, 0x47, 0xd0, 0x19, 0x19, 0x35, 0x7e, 0x8d, 0xa6, 0x95,
    0xb3, 0x36, 0x17, 0xfd, 0x9f, 0xe2, 0xf7, 0xa8, 0xb5, 0xb5, 0x2f, 0xfe, 0x6b, 0x32, 0xf2, 0x3d, 0x27, 0xfe, 0x67, 0x7a,
    0x1b, 0xe7, 0xc5, 0xff, 0xf8, 0xef, 0x75, 0x59, 0xb5, 0x2b, 0x73, 0xed, 0x8c, 0x72, 0x2d, 0xb7, 0xd0, 0x19, 0xe9, 0xc1,
    0xfb, 0x4a, 0x2f, 0xce, 0x67, 0xde, 0x7f, 0xfc, 0x3f, 0x5b, 0x53, 0xcf, 0xc6, 0xff, 0xfa, 0x2f, 0xfe, 0x36, 0xd1, 0x7e,
    0xa8, 0x6e, 0xff, 0xaf, 0x5c, 0x2f, 0x4f, 0x8a, 0xff, 0x36, 0x95, 0x8b, 0xec, 0xde, 0xf5, 0x7f, 0x65, 0x4e, 0xad, 0xfa,
    0xf8, 0x8f, 0x45, 0x78, 0xa4, 0x26, 0x9f, 0x5f, 0xf1, 0x60, 0xed, 0xbc, 0x8d, 0x5b, 0xf9, 0x3d, 0xd1, 0xfe, 0xdf, 0x11,
    0xff, 0xd5, 0xe3, 0x7f, 0xf1, 0x35, 0x3c, 0xf6, 0x8c, 0x74, 0x45, 0xc6, 0xff, 0xda, 0x09, 0xfd, 0xff, 0x51, 0x3b, 0x31,
    0x5a, 0x2a, 0xf5, 0x6b, 0x53, 0x55, 0x66, 0xdb, 0x5e, 0x8d, 0xff, 0x4c, 0xfd, 0x1f, 0x1d, 0x87, 0xc8, 0xb4, 0x10, 0xdb,
    0x60, 0xe4, 0x60, 0xf4, 0x9b, 0xeb, 0x59, 0x43, 0x73, 0xed, 0xff, 0x1d, 0xf1, 0x7f, 0x56, 0xff, 0xbf, 0x25, 0x47, 0x35,
    0xeb, 0xda, 0xff, 0x2d, 0xd1, 0xca, 0xcc, 0xdc, 0x6d, 0x22, 0xa5, 0xbd, 0x5e, 0x66, 0x57, 0xd6, 0xff, 0xb9, 0xd5, 0xe6,
    0xda, 0xb6, 0x7c, 0xa7, 0x91, 0xf6, 0xff, 0x9e, 0xa7, 0x70, 0xcf, 0xae, 0xff, 0xcf, 0x7c, 0x3a, 0xb9, 0x73, 0x7f, 0xee,
    0x93, 0x77, 0x6e, 0x4f, 0x1e, 0xe9, 0x3b, 0xf4, 0xff, 0x23, 0xe3, 0xff, 0xd1, 0x9c, 0xc7, 0xe3, 0xda, 0xb8, 0x0f, 0x46,
    0xd3, 0xbe, 0x35, 0xfe, 0xef, 0x91, 0x9b, 0xf9, 0x7b, 0xe3, 0xbf, 0xa5, 0xf2, 0x4f, 0x5f, 0x11, 0xff, 0x73, 0x2d, 0xab,
    0xd8, 0x8a, 0x88, 0xb9, 0x15, 0x93, 0xa3, 0x7d, 0xba, 0x76, 0xea, 0x95, 0x9e, 0x79, 0xfe, 0xb7, 0xe7, 0xfd, 0x9f, 0xda,
    0x37, 0x0d, 0x9e, 0xfb, 0x3e, 0xd4, 0x3b, 0x8e, 0xe0, 0x8c, 0xfb, 0xf9, 0x78, 0xd5, 0x82, 0xbe, 0xe9, 0x98, 0x62, 0x4f,
    0x75, 0xa3, 0xcf, 0x89, 0xf7, 0xdd, 0xef, 0xa3, 0xef, 0xff, 0x44, 0xcb, 0x54, 0xa6, 0xe5, 0x6f, 0xb9, 0x83, 0x5d, 0x19,
    0xff, 0xfb, 0x5b, 0x49, 0x55, 0xef, 0x30, 0x7c, 0xdb, 0x95, 0xa2, 0x64, 0xbe, 0xe5, 0xbc, 0xae, 0x8f, 0xf4, 0xbe, 0xa5,
    0x24, 0x5c, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3b, 0xe7, 0x8a, 0xf5, 0xb2,
    0x4c, 0x0b, 0xb3, 0x79, 0xae, 0xfa, 0xc2, 0xcc, 0xcd, 0x36, 0xcc, 0xdd, 0x9c, 0xfd, 0x9e, 0x9f, 0xab, 0x1a, 0xf4, 0xc1,
    0xbf, 0x68, 0x81, 0x3d, 0xed, 0xe1, 0x79, 0xc8, 0xeb, 0x6b, 0x34, 0xfc, 0xdc, 0xe3, 0x96, 0x28, 0x91, 0x36, 0x31, 0x4f,
    0x75, 0x6d, 0xae, 0xde, 0xee, 0x63, 0xae, 0xc9, 0x96, 0x1e, 0xc9, 0xfb, 0x37, 0x9b, 0xbd, 0x64, 0x47, 0xa9, 0xe4, 0x66,
    0xbb, 0xf7, 0xa5, 0xbf, 0xc7, 0x72, 0x3f, 0xf4, 0xc3, 0x18, 0xaa, 0xb9, 0x3f, 0xcd, 0x65, 0xe8, 0x5b, 0x9f, 0xb3, 0x3e,
    0x97, 0xdf, 0xb3, 0x6d, 0xcc, 0x89, 0xbd, 0x7e, 0xb5, 0xc7, 0x23, 0x25, 0xb7, 0x75, 0x6e, 0xae, 0xfe, 0x8e, 0x63, 0x8e,
    0xe7, 0xb3, 0x9d, 0xff, 0x9e, 0x4c, 0xed, 0x37, 0x9e, 0x85, 0x99, 0xa9, 0x27, 0xda, 0xd4, 0xdd, 0x7a, 0x2d, 0xca, 0xa3,
    0x77, 0x99, 0xf5, 0xf8, 0xaf, 0x99, 0x83, 0xde, 0x87, 0xc7, 0x13, 0x5b, 0x81, 0x63, 0x26, 0xbf, 0xf7, 0xfb, 0x5a, 0x8b,
    0xf9, 0xf8, 0xff, 0xd6, 0x36, 0xf6, 0xfd, 0x4a, 0xa0, 0x2f, 0xd6, 0xf1, 0x9f, 0x23, 0x6c, 0xa6, 0x15, 0x5f, 0x13, 0xff,
    0xeb, 0xf7, 0xad, 0xfd, 0xf1, 0xdf, 0x43, 0xdf, 0x10, 0x6d, 0xcf, 0xde, 0x73, 0x6b, 0x2e, 0xfe, 0xc7, 0x57, 0xd0, 0x9e,
    0xad, 0x55, 0xab, 0x28, 0xe5, 0x72, 0x94, 0xb7, 0x60, 0xef, 0x21, 0x9b, 0x11, 0x68, 0x35, 0x1b, 0x5b, 0x0f, 0xf5, 0x46,
    0xf6, 0xd4, 0xff, 0xf9, 0xfe, 0xff, 0x7c, 0xbb, 0x2d, 0x3e, 0x7e, 0x90, 0xb9, 0x96, 0x76, 0xb4, 0xc3, 0xcf, 0xd8, 0x1a,
    0xc9, 0x1b, 0x77, 0xd5, 0xd6, 0xdd, 0xf1, 0xdf, 0xff, 0xb3, 0x8e, 0xe2, 0x51, 0x44, 0xad, 0xdf, 0x1d, 0xee, 0xd7, 0xb2,
    0x88, 0x8f, 0xff, 0xad, 0x1e, 0x43, 0xc5, 0x11, 0xcf, 0xf5, 0x0b, 0x67, 0xa3, 0xb4, 0xae, 0xfe, 0xcf, 0x9c, 0xf5, 0x2b,
    0xb7, 0x66, 0x46, 0xf7, 0xde, 0x1a, 0xff, 0x73, 0xe5, 0x76, 0x45, 0xfc, 0x57, 0xe7, 0x13, 0x9c, 0x69, 0x4d, 0x44, 0xb3,
    0x34, 0x5f, 0xd1, 0x97, 0x9c, 0xcd, 0xce, 0x9f, 0x5b, 0x7f, 0xf6, 0x4d, 0xf1, 0xdf, 0x8a, 0xda, 0xff, 0x67, 0x6e, 0xbd,
    0x47, 0xfb, 0xbf, 0xbf, 0xa0, 0xfe, 0xcf, 0x3e, 0xd7, 0x99, 0x3f, 0x86, 0x73, 0x8e, 0x36, 0x9e, 0x81, 0x3f, 0xf3, 0x04,
    0xe3, 0xca, 0xbe, 0x70, 0x76, 0x6b, 0x36, 0xfe, 0xbf, 0xb5, 0xfe, 0x7f, 0x43, 0xfc, 0xe7, 0x9f, 0x65, 0xaf, 0x8c, 0xff,
    0x9d, 0x75, 0x44, 0xf9, 0xf6, 0x51, 0xee, 0xd9, 0xff, 0x3b, 0xeb, 0xff, 0x9a, 0x35, 0xe7, 0x9f, 0x16, 0xff, 0x33, 0xa3,
    0x4c, 0x4f, 0x8d, 0xff, 0x8a, 0x18, 0xa9, 0x1a, 0xff, 0xaf, 0xbe, 0x03, 0xb4, 0xad, 0x99, 0xeb, 0x9f, 0x17, 0xe1, 0xe3,
    0xf8, 0x8f, 0xbc, 0xff, 0xf3, 0xee, 0xf8, 0x9f, 0x7b, 0x37, 0xe6, 0xd9, 0xf1, 0xdf, 0x93, 0x2b, 0xd6, 0xbc, 0xf3, 0xf9,
    0x70, 0x6c, 0xfd, 0xe9, 0xe7, 0xc5, 0x7f, 0xfc, 0xed, 0x8c, 0xb7, 0x3f, 0xff, 0xcb, 0xbd, 0x1d, 0xb0, 0xf7, 0xf9, 0xdf,
    0x13, 0xde, 0x2b, 0xe1, 0x4d, 0xf7, 0x42, 0xe7, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xa0, 0x6a, 0x46, 0x46, 0x3c, 0x67, 0xcf, 0xdc, 0x4c, 0xb1, 0x6b, 0x72, 0xdf, 0x47, 0x32, 0x15, 0xcc, 0xec, 0x4f, 0x3c,
    0xff, 0x41, 0x24, 0xdb, 0xfe, 0xd1, 0xb9, 0x89, 0xe6, 0x1c, 0xcf, 0xe4, 0x32, 0xda, 0x75, 0x8c, 0xeb, 0x59, 0xe1, 0xe7,
    0x7f, 0x61, 0x2e, 0xf7, 0x63, 0x2c, 0x2e, 0xee, 0x71, 0xae, 0x33, 0xf3, 0xee, 0x76, 0x67, 0x8a, 0xef, 0xc9, 0x5c, 0xef,
    0xf5, 0x7b, 0x95, 0xcd, 0xd3, 0x1f, 0xdb, 0xe7, 0x48, 0x09, 0xf7, 0xc4, 0x4a, 0x2c, 0x6d, 0xe2, 0x4a, 0xaa, 0xce, 0xef,
    0x93, 0x59, 0x99, 0x63, 0xfe, 0x8e, 0x5c, 0x99, 0xb3, 0xbf, 0x25, 0xf2, 0xf5, 0xdf, 0xe5, 0x5c, 0xdf, 0xb5, 0x65, 0x71,
    0x9c, 0xf1, 0xe8, 0xbe, 0xad, 0xa4, 0x1e, 0xca, 0x6e, 0xfa, 0x9c, 0x7c, 0x04, 0xb1, 0x3a, 0xf7, 0xbd, 0xed, 0xe2, 0xf7,
    0x1f, 0x73, 0x26, 0xb7, 0xc0, 0x4c, 0x1d, 0xde, 0x02, 0xf1, 0x7f, 0xc7, 0xfc, 0x55, 0xb9, 0xcc, 0x57, 0x33, 0xa5, 0x78,
    0xd6, 0xb6, 0x23, 0xbf, 0x07, 0x56, 0x62, 0xbb, 0xf2, 0x18, 0xf7, 0xe7, 0xec, 0x38, 0xce, 0x26, 0xf2, 0xe4, 0x73, 0xdd,
    0xb6, 0xe7, 0x82, 0x89, 0xd6, 0xff, 0x4f, 0x8d, 0xff, 0xd1, 0x6a, 0x04, 0x9f, 0xd7, 0x25, 0x3b, 0x6f, 0x5b, 0xc5, 0x5a,
    0x06, 0xab, 0x3d, 0xa0, 0x1d, 0xdb, 0xae, 0xcc, 0xd9, 0x75, 0xf6, 0xb1, 0xc6, 0xcb, 0xe8, 0xca, 0xf8, 0x9f, 0xe9, 0xd7,
    0x7d, 0x5b, 0xfd, 0xdf, 0x2f, 0xdf, 0x36, 0x7f, 0x17, 0xbb, 0xfb, 0xb5, 0xbd, 0x7f, 0xcd, 0x8e, 0x27, 0xc4, 0xf8, 0x5d,
    0xe3, 0xff, 0xaa, 0x8c, 0xc7, 0x35, 0x2b, 0x2d, 0x45, 0xfb, 0xff, 0x4f, 0xb8, 0x26, 0x7a, 0x72, 0x2d, 0xe3, 0xd8, 0x0a,
    0xce, 0xd5, 0xdb, 0xc4, 0xff, 0xdd, 0xeb, 0xff, 0x99, 0xb1, 0x95, 0xfb, 0xe5, 0xaf, 0xcc, 0xae, 0xd3, 0xfb, 0xee, 0xeb,
    0xa5, 0x85, 0x9e, 0x79, 0xed, 0xd8, 0x26, 0xfe, 0xc5, 0xbf, 0xf8, 0xbf, 0xe2, 0x7a, 0xe9, 0x5f, 0xd1, 0xff, 0x17, 0xff,
    0xe2, 0x3f, 0xf2, 0xcc, 0xec, 0xfe, 0xfd, 0xff, 0x5d, 0x4f, 0x2a, 0xdf, 0x36, 0xfe, 0x1f, 0x7d, 0xda, 0xfb, 0xa4, 0xf1,
    0xff, 0xdd, 0xcf, 0xff, 0xae, 0xf8, 0xdd, 0xdc, 0x3e, 0xbf, 0xff, 0x19, 0xf7, 0x3b, 0xf2, 0xc5, 0xf3, 0xfc, 0xeb, 0x50,
    0x19, 0x80, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe6, 0xe7, 0x00, 0xaf, 0xbe,
    0xa7, 0x7f, 0x66, 0xbe, 0xf7, 0xc8, 0x9c, 0xcf, 0xb9, 0xef, 0x04, 0xf1, 0x1f, 0x9b, 0xa7, 0x73, 0x66, 0xbe, 0xf7, 0xfc,
    0xdc, 0x7c, 0xe7, 0x1a, 0xde, 0x37, 0x53, 0x5c, 0x6c, 0x43, 0x2e, 0xc2, 0xa3, 0x39, 0x3a, 0xef, 0xb2, 0xed, 0x1d, 0x2b,
    0x73, 0xc0, 0xbd, 0x6a, 0xd5, 0xa7, 0xc4, 0x7f, 0xdb, 0x96, 0xef, 0x12, 0xbe, 0xb1, 0xff, 0x2f, 0xfe, 0x41, 0xfc, 0x8b,
    0x7f, 0xd0, 0xfe, 0x17, 0xff, 0x60, 0xfc, 0xff, 0xaa, 0x58, 0x9d, 0x59, 0x27, 0xe6, 0xc9, 0x4f, 0x31, 0xe0, 0x7e, 0x77,
    0x80, 0xf8, 0xdd, 0xa1, 0x7a, 0x5b, 0x1b, 0xde, 0x37, 0x3c, 0x23, 0x84, 0xb3, 0xe2, 0xff, 0x9a, 0x51, 0x8a, 0xd5, 0xf8,
    0xd7, 0xfe, 0x87, 0x37, 0xf5, 0x46, 0x22, 0xef, 0xff, 0x89, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0xe9, 0xaf, 0xff, 0x94, 0x03, 0x88, 0x7f, 0xe0, 0xeb, 0xe2, 0xff,
    0x0f };

// Font glyphs rectangles data (on atlas)
RAYGUIAPI const Rectangle enefeteFontRecs[189] = {
    { 4, 4, 4 , 16 },
    { 16, 4, 2 , 10 },
    { 26, 4, 5 , 3 },
    { 39, 4, 7 , 10 },
    { 54, 4, 7 , 13 },
    { 69, 4, 7 , 10 },
    { 84, 4, 7 , 10 },
    { 99, 4, 2 , 3 },
    { 109, 4, 3 , 12 },
    { 120, 4, 3 , 12 },
    { 131, 4, 5 , 6 },
    { 144, 4, 6 , 5 },
    { 158, 4, 2 , 4 },
    { 168, 4, 5 , 1 },
    { 181, 4, 2 , 2 },
    { 191, 4, 4 , 10 },
    { 203, 4, 6 , 10 },
    { 217, 4, 4 , 10 },
    { 229, 4, 6 , 10 },
    { 243, 4, 6 , 10 },
    { 257, 4, 6 , 10 },
    { 271, 4, 6 , 10 },
    { 285, 4, 6 , 10 },
    { 299, 4, 6 , 10 },
    { 313, 4, 6 , 10 },
    { 327, 4, 6 , 10 },
    { 341, 4, 2 , 6 },
    { 351, 4, 2 , 8 },
    { 361, 4, 7 , 7 },
    { 376, 4, 5 , 3 },
    { 389, 4, 7 , 7 },
    { 404, 4, 6 , 10 },
    { 418, 4, 7 , 12 },
    { 433, 4, 7 , 10 },
    { 448, 4, 7 , 10 },
    { 463, 4, 7 , 10 },
    { 478, 4, 7 , 10 },
    { 493, 4, 7 , 10 },
    { 4, 28, 7 , 10 },
    { 19, 28, 7 , 10 },
    { 34, 28, 7 , 10 },
    { 49, 28, 2 , 10 },
    { 59, 28, 5 , 10 },
    { 72, 28, 7 , 10 },
    { 87, 28, 6 , 10 },
    { 101, 28, 9 , 10 },
    { 118, 28, 7 , 10 },
    { 133, 28, 7 , 10 },
    { 148, 28, 7 , 10 },
    { 163, 28, 7 , 12 },
    { 178, 28, 7 , 10 },
    { 193, 28, 7 , 10 },
    { 208, 28, 6 , 10 },
    { 222, 28, 7 , 10 },
    { 237, 28, 7 , 10 },
    { 252, 28, 8 , 10 },
    { 268, 28, 7 , 10 },
    { 283, 28, 6 , 10 },
    { 297, 28, 7 , 10 },
    { 312, 28, 4 , 12 },
    { 324, 28, 4 , 10 },
    { 336, 28, 4 , 12 },
    { 348, 28, 6 , 3 },
    { 362, 28, 7 , 1 },
    { 377, 28, 4 , 3 },
    { 389, 28, 6 , 7 },
    { 403, 28, 6 , 10 },
    { 417, 28, 6 , 7 },
    { 431, 28, 6 , 10 },
    { 445, 28, 6 , 7 },
    { 459, 28, 4 , 10 },
    { 471, 28, 6 , 9 },
    { 485, 28, 6 , 10 },
    { 499, 28, 2 , 10 },
    { 4, 52, 5 , 12 },
    { 17, 52, 6 , 10 },
    { 31, 52, 3 , 10 },
    { 42, 52, 8 , 7 },
    { 58, 52, 6 , 7 },
    { 72, 52, 6 , 7 },
    { 86, 52, 6 , 9 },
    { 100, 52, 6 , 9 },
    { 114, 52, 5 , 7 },
    { 127, 52, 6 , 7 },
    { 141, 52, 4 , 10 },
    { 153, 52, 6 , 7 },
    { 167, 52, 6 , 7 },
    { 181, 52, 8 , 7 },
    { 197, 52, 6 , 7 },
    { 211, 52, 6 , 9 },
    { 225, 52, 6 , 7 },
    { 239, 52, 5 , 12 },
    { 252, 52, 2 , 12 },
    { 262, 52, 5 , 12 },
    { 275, 52, 7 , 3 },
    { 290, 52, 2 , 9 },
    { 300, 52, 6 , 11 },
    { 314, 52, 7 , 10 },
    { 329, 52, 7 , 9 },
    { 344, 52, 6 , 10 },
    { 358, 52, 7 , 11 },
    { 373, 52, 6 , 12 },
    { 387, 52, 6 , 10 },
    { 401, 52, 7 , 10 },
    { 416, 52, 5 , 5 },
    { 429, 52, 7 , 6 },
    { 444, 52, 6 , 3 },
    { 458, 52, 7 , 10 },
    { 473, 52, 9 , 10 },
    { 490, 52, 4 , 4 },
    { 4, 76, 6 , 7 },
    { 18, 76, 4 , 5 },
    { 30, 76, 4 , 5 },
    { 42, 76, 7 , 11 },
    { 57, 76, 6 , 9 },
    { 71, 76, 7 , 12 },
    { 86, 76, 2 , 2 },
    { 96, 76, 6 , 10 },
    { 110, 76, 3 , 5 },
    { 121, 76, 4 , 5 },
    { 133, 76, 7 , 6 },
    { 148, 76, 9 , 10 },
    { 165, 76, 8 , 7 },
    { 181, 76, 6 , 11 },
    { 195, 76, 6 , 11 },
    { 209, 76, 7 , 11 },
    { 224, 76, 7 , 11 },
    { 239, 76, 7 , 11 },
    { 254, 76, 7 , 11 },
    { 269, 76, 7 , 11 },
    { 284, 76, 7 , 11 },
    { 299, 76, 9 , 10 },
    { 316, 76, 7 , 12 },
    { 331, 76, 7 , 11 },
    { 346, 76, 7 , 11 },
    { 361, 76, 7 , 11 },
    { 376, 76, 7 , 11 },
    { 391, 76, 3 , 11 },
    { 402, 76, 3 , 11 },
    { 413, 76, 5 , 11 },
    { 426, 76, 5 , 11 },
    { 439, 76, 8 , 10 },
    { 455, 76, 7 , 11 },
    { 470, 76, 7 , 11 },
    { 485, 76, 7 , 11 },
    { 4, 100, 7 , 11 },
    { 19, 100, 7 , 11 },
    { 34, 100, 7 , 11 },
    { 49, 100, 7 , 7 },
    { 64, 100, 7 , 13 },
    { 79, 100, 7 , 11 },
    { 94, 100, 7 , 11 },
    { 109, 100, 7 , 11 },
    { 124, 100, 7 , 11 },
    { 139, 100, 6 , 11 },
    { 153, 100, 7 , 10 },
    { 168, 100, 7 , 10 },
    { 183, 100, 6 , 10 },
    { 197, 100, 6 , 10 },
    { 211, 100, 6 , 10 },
    { 225, 100, 6 , 10 },
    { 239, 100, 6 , 10 },
    { 253, 100, 6 , 11 },
    { 267, 100, 8 , 7 },
    { 283, 100, 6 , 9 },
    { 297, 100, 6 , 10 },
    { 311, 100, 6 , 10 },
    { 325, 100, 6 , 10 },
    { 339, 100, 6 , 10 },
    { 353, 100, 3 , 10 },
    { 364, 100, 3 , 10 },
    { 375, 100, 5 , 10 },
    { 388, 100, 5 , 10 },
    { 401, 100, 6 , 10 },
    { 415, 100, 6 , 10 },
    { 429, 100, 6 , 10 },
    { 443, 100, 6 , 10 },
    { 457, 100, 6 , 10 },
    { 471, 100, 6 , 10 },
    { 485, 100, 6 , 10 },
    { 4, 124, 6 , 7 },
    { 18, 124, 7 , 11 },
    { 33, 124, 6 , 10 },
    { 47, 124, 6 , 10 },
    { 61, 124, 6 , 10 },
    { 75, 124, 6 , 10 },
    { 89, 124, 6 , 12 },
    { 103, 124, 6 , 12 },
    { 117, 124, 6 , 12 },
};

// Font glyphs info data
// NOTE: No glyphs.image data provided
RAYGUIAPI const GlyphInfo enefeteFontGlyphs[189] = {
    { 32, 0, 12, 4, { 0 }},
    { 33, 0, 2, 3, { 0 }},
    { 34, 0, 2, 6, { 0 }},
    { 35, 0, 2, 8, { 0 }},
    { 36, 0, 1, 8, { 0 }},
    { 37, 0, 2, 8, { 0 }},
    { 38, 0, 2, 8, { 0 }},
    { 39, 0, 2, 3, { 0 }},
    { 40, 0, 2, 4, { 0 }},
    { 41, 0, 2, 4, { 0 }},
    { 42, 0, 4, 6, { 0 }},
    { 43, 0, 6, 7, { 0 }},
    { 44, 0, 10, 3, { 0 }},
    { 45, 0, 8, 6, { 0 }},
    { 46, 0, 10, 3, { 0 }},
    { 47, 0, 2, 5, { 0 }},
    { 48, 0, 2, 7, { 0 }},
    { 49, 0, 2, 7, { 0 }},
    { 50, 0, 2, 7, { 0 }},
    { 51, 0, 2, 7, { 0 }},
    { 52, 0, 2, 7, { 0 }},
    { 53, 0, 2, 7, { 0 }},
    { 54, 0, 2, 7, { 0 }},
    { 55, 0, 2, 7, { 0 }},
    { 56, 0, 2, 7, { 0 }},
    { 57, 0, 2, 7, { 0 }},
    { 58, 0, 4, 3, { 0 }},
    { 59, 0, 4, 3, { 0 }},
    { 60, 0, 4, 8, { 0 }},
    { 61, 0, 6, 6, { 0 }},
    { 62, 0, 4, 8, { 0 }},
    { 63, 0, 2, 7, { 0 }},
    { 64, 0, 2, 8, { 0 }},
    { 65, 0, 2, 8, { 0 }},
    { 66, 0, 2, 8, { 0 }},
    { 67, 0, 2, 8, { 0 }},
    { 68, 0, 2, 8, { 0 }},
    { 69, 0, 2, 8, { 0 }},
    { 70, 0, 2, 8, { 0 }},
    { 71, 0, 2, 8, { 0 }},
    { 72, 0, 2, 8, { 0 }},
    { 73, 0, 2, 3, { 0 }},
    { 74, 0, 2, 6, { 0 }},
    { 75, 0, 2, 8, { 0 }},
    { 76, 0, 2, 7, { 0 }},
    { 77, 0, 2, 10, { 0 }},
    { 78, 0, 2, 8, { 0 }},
    { 79, 0, 2, 8, { 0 }},
    { 80, 0, 2, 8, { 0 }},
    { 81, 0, 2, 8, { 0 }},
    { 82, 0, 2, 8, { 0 }},
    { 83, 0, 2, 8, { 0 }},
    { 84, 0, 2, 7, { 0 }},
    { 85, 0, 2, 8, { 0 }},
    { 86, 0, 2, 8, { 0 }},
    { 87, 0, 2, 9, { 0 }},
    { 88, 0, 2, 8, { 0 }},
    { 89, 0, 2, 7, { 0 }},
    { 90, 0, 2, 8, { 0 }},
    { 91, 0, 2, 5, { 0 }},
    { 92, 0, 2, 5, { 0 }},
    { 93, 0, 2, 5, { 0 }},
    { 94, 0, 2, 7, { 0 }},
    { 95, 0, 14, 8, { 0 }},
    { 96, 0, 2, 5, { 0 }},
    { 97, 0, 5, 7, { 0 }},
    { 98, 0, 2, 7, { 0 }},
    { 99, 0, 5, 7, { 0 }},
    { 100, 0, 2, 7, { 0 }},
    { 101, 0, 5, 7, { 0 }},
    { 102, 0, 2, 5, { 0 }},
    { 103, 0, 5, 7, { 0 }},
    { 104, 0, 2, 7, { 0 }},
    { 105, 0, 2, 3, { 0 }},
    { 106, 0, 2, 6, { 0 }},
    { 107, 0, 2, 7, { 0 }},
    { 108, 0, 2, 4, { 0 }},
    { 109, 0, 5, 9, { 0 }},
    { 110, 0, 5, 7, { 0 }},
    { 111, 0, 5, 7, { 0 }},
    { 112, 0, 5, 7, { 0 }},
    { 113, 0, 5, 7, { 0 }},
    { 114, 0, 5, 6, { 0 }},
    { 115, 0, 5, 7, { 0 }},
    { 116, 0, 2, 5, { 0 }},
    { 117, 0, 5, 7, { 0 }},
    { 118, 0, 5, 7, { 0 }},
    { 119, 0, 5, 9, { 0 }},
    { 120, 0, 5, 7, { 0 }},
    { 121, 0, 5, 7, { 0 }},
    { 122, 0, 5, 7, { 0 }},
    { 123, 0, 2, 6, { 0 }},
    { 124, 0, 2, 3, { 0 }},
    { 125, 0, 2, 6, { 0 }},
    { 126, 0, 6, 8, { 0 }},
    { 161, 0, 5, 3, { 0 }},
    { 162, 0, 3, 7, { 0 }},
    { 163, 0, 2, 8, { 0 }},
    { 8364, 0, 3, 8, { 0 }},
    { 165, 0, 2, 7, { 0 }},
    { 352, 0, 1, 8, { 0 }},
    { 167, 0, 2, 7, { 0 }},
    { 353, 0, 2, 7, { 0 }},
    { 169, 0, 2, 8, { 0 }},
    { 170, 0, 2, 6, { 0 }},
    { 171, 0, 6, 8, { 0 }},
    { 172, 0, 7, 7, { 0 }},
    { 174, 0, 2, 8, { 0 }},
    { 175, 1, 3, 14, { 0 }},
    { 176, 0, 2, 5, { 0 }},
    { 177, 0, 4, 7, { 0 }},
    { 178, 0, 2, 5, { 0 }},
    { 179, 0, 2, 5, { 0 }},
    { 381, 0, 1, 8, { 0 }},
    { 181, 0, 5, 7, { 0 }},
    { 182, 0, 2, 8, { 0 }},
    { 183, 0, 6, 3, { 0 }},
    { 382, 0, 2, 7, { 0 }},
    { 185, 0, 2, 4, { 0 }},
    { 186, 0, 2, 5, { 0 }},
    { 187, 0, 6, 8, { 0 }},
    { 338, 0, 2, 10, { 0 }},
    { 339, 0, 5, 9, { 0 }},
    { 376, 0, 1, 7, { 0 }},
    { 191, 0, 3, 7, { 0 }},
    { 192, 0, 1, 8, { 0 }},
    { 193, 0, 1, 8, { 0 }},
    { 194, 0, 1, 8, { 0 }},
    { 195, 0, 1, 8, { 0 }},
    { 196, 0, 1, 8, { 0 }},
    { 197, 0, 1, 8, { 0 }},
    { 198, 0, 2, 10, { 0 }},
    { 199, 0, 2, 8, { 0 }},
    { 200, 0, 1, 8, { 0 }},
    { 201, 0, 1, 8, { 0 }},
    { 202, 0, 1, 8, { 0 }},
    { 203, 0, 1, 8, { 0 }},
    { 204, 0, 1, 4, { 0 }},
    { 205, 0, 1, 4, { 0 }},
    { 206, 0, 1, 6, { 0 }},
    { 207, 0, 1, 6, { 0 }},
    { 208, 0, 2, 9, { 0 }},
    { 209, 0, 1, 8, { 0 }},
    { 210, 0, 1, 8, { 0 }},
    { 211, 0, 1, 8, { 0 }},
    { 212, 0, 1, 8, { 0 }},
    { 213, 0, 1, 8, { 0 }},
    { 214, 0, 1, 8, { 0 }},
    { 215, 0, 5, 8, { 0 }},
    { 216, 0, 1, 8, { 0 }},
    { 217, 0, 1, 8, { 0 }},
    { 218, 0, 1, 8, { 0 }},
    { 219, 0, 1, 8, { 0 }},
    { 220, 0, 1, 8, { 0 }},
    { 221, 0, 1, 7, { 0 }},
    { 222, 0, 2, 8, { 0 }},
    { 223, 0, 2, 8, { 0 }},
    { 224, 0, 2, 7, { 0 }},
    { 225, 0, 2, 7, { 0 }},
    { 226, 0, 2, 7, { 0 }},
    { 227, 0, 2, 7, { 0 }},
    { 228, 0, 2, 7, { 0 }},
    { 229, 0, 1, 7, { 0 }},
    { 230, 0, 5, 9, { 0 }},
    { 231, 0, 5, 7, { 0 }},
    { 232, 0, 2, 7, { 0 }},
    { 233, 0, 2, 7, { 0 }},
    { 234, 0, 2, 7, { 0 }},
    { 235, 0, 2, 7, { 0 }},
    { 236, 0, 2, 4, { 0 }},
    { 237, 0, 2, 4, { 0 }},
    { 238, 0, 2, 6, { 0 }},
    { 239, 0, 2, 6, { 0 }},
    { 240, 0, 2, 7, { 0 }},
    { 241, 0, 2, 7, { 0 }},
    { 242, 0, 2, 7, { 0 }},
    { 243, 0, 2, 7, { 0 }},
    { 244, 0, 2, 7, { 0 }},
    { 245, 0, 2, 7, { 0 }},
    { 246, 0, 2, 7, { 0 }},
    { 247, 0, 4, 7, { 0 }},
    { 248, 0, 3, 8, { 0 }},
    { 249, 0, 2, 7, { 0 }},
    { 250, 0, 2, 7, { 0 }},
    { 251, 0, 2, 7, { 0 }},
    { 252, 0, 2, 7, { 0 }},
    { 253, 0, 2, 7, { 0 }},
    { 254, 0, 2, 7, { 0 }},
    { 255, 0, 2, 7, { 0 }},
};

// Style loading function: Enefete
RAYGUIAPI void GuiLoadStyleEnefete(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < ENEFETE_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(enefeteStyleProps[i].controlId, enefeteStyleProps[i].propertyId, enefeteStyleProps[i].propertyValue);
    }

    // Custom font loading
    // NOTE: Compressed font image data (DEFLATE), it requires DecompressData() function
    int enefeteFontDataSize = 0;
    unsigned char *data = DecompressData(enefeteFontData, ENEFETE_STYLE_FONT_ATLAS_COMP_SIZE, &enefeteFontDataSize);
    Image imFont = { data, 512, 256, 1, 2 };

    Font font = { 0 };
    font.baseSize = 16;
    font.glyphCount = 189;

    // Load texture from image
    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed image data can be unloaded from memory

    // Copy char recs data from global fontRecs
    // NOTE: Required to avoid issues if trying to free font
    font.recs = (Rectangle *)RAYGUI_MALLOC(font.glyphCount*sizeof(Rectangle));
    memcpy(font.recs, enefeteFontRecs, font.glyphCount*sizeof(Rectangle));

    // Copy font char info data from global fontChars
    // NOTE: Required to avoid issues if trying to free font
    font.glyphs = (GlyphInfo *)RAYGUI_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    memcpy(font.glyphs, enefeteFontGlyphs, font.glyphCount*sizeof(GlyphInfo));

    GuiSetFont(font);

    // Setup a white rectangle on the font to be used on shapes drawing,
    // it makes possible to draw shapes and text (full UI) in a single draw call
    Rectangle fontWhiteRec = { 510, 254, 1, 1 };
    SetShapesTexture(font.texture, fontWhiteRec);

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}


// Color Bar Alpha control
// NOTE: Returns alpha value normalized [0..1]
int GuiColorBarAlpha(Rectangle bounds, const char *text, float *alpha)
{
    #if !defined(RAYGUI_COLORBARALPHA_CHECKED_SIZE)
        #define RAYGUI_COLORBARALPHA_CHECKED_SIZE   10
    #endif

    int result = 0;
    GuiState state = guiState;
    Rectangle selector = { (float)bounds.x + (*alpha)*bounds.width - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT)/2, (float)bounds.y - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW), (float)GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT), (float)bounds.height + GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW)*2 };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiControlExclusiveMode) // Allows to keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiControlExclusiveRec))
                {
                    state = STATE_PRESSED;

                    *alpha = (mousePoint.x - bounds.x)/bounds.width;
                    if (*alpha <= 0.0f) *alpha = 0.0f;
                    if (*alpha >= 1.0f) *alpha = 1.0f;
                }
            }
            else
            {
                guiControlExclusiveMode = false;
                guiControlExclusiveRec = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds) || CheckCollisionPointRec(mousePoint, selector))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiControlExclusiveMode = true;
                guiControlExclusiveRec = bounds; // Store bounds as an identifier when dragging starts

                *alpha = (mousePoint.x - bounds.x)/bounds.width;
                if (*alpha <= 0.0f) *alpha = 0.0f;
                if (*alpha >= 1.0f) *alpha = 1.0f;
                //selector.x = bounds.x + (int)(((alpha - 0)/(100 - 0))*(bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH))) - selector.width/2;
            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------

    // Draw alpha bar: checked background
    if (state != STATE_DISABLED)
    {
        int checksX = (int)bounds.width/RAYGUI_COLORBARALPHA_CHECKED_SIZE;
        int checksY = (int)bounds.height/RAYGUI_COLORBARALPHA_CHECKED_SIZE;

        for (int x = 0; x < checksX; x++)
        {
            for (int y = 0; y < checksY; y++)
            {
                Rectangle check = { bounds.x + x*RAYGUI_COLORBARALPHA_CHECKED_SIZE, bounds.y + y*RAYGUI_COLORBARALPHA_CHECKED_SIZE, RAYGUI_COLORBARALPHA_CHECKED_SIZE, RAYGUI_COLORBARALPHA_CHECKED_SIZE };
                GuiDrawRectangle(check, 0, BLANK, ((x + y)%2)? Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), 0.4f) : Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.4f));
            }
        }

        DrawRectangleGradientEx(bounds, RAYGUI_CLITERAL(Color){ 255, 255, 255, 0 }, RAYGUI_CLITERAL(Color){ 255, 255, 255, 0 }, Fade(RAYGUI_CLITERAL(Color){ 0, 0, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 0, 0, 255 }, guiAlpha));
    }
    else DrawRectangleGradientEx(bounds, Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), guiAlpha), Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), guiAlpha));

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);

    // Draw alpha bar: selector
    GuiDrawRectangle(selector, 0, BLANK, GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)));
    //--------------------------------------------------------------------

    return result;
}

// Color Bar Hue control
// Returns hue value normalized [0..1]
// NOTE: Other similar bars (for reference):
//      Color GuiColorBarSat() [WHITE->color]
//      Color GuiColorBarValue() [BLACK->color], HSV/HSL
//      float GuiColorBarLuminance() [BLACK->WHITE]
int GuiColorBarHue(Rectangle bounds, const char *text, float *hue)
{
    int result = 0;
    GuiState state = guiState;
    Rectangle selector = { (float)bounds.x - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW), (float)bounds.y + (*hue)/360.0f*bounds.height - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT)/2, (float)bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW)*2, (float)GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT) };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiControlExclusiveMode) // Allows to keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiControlExclusiveRec))
                {
                    state = STATE_PRESSED;

                    *hue = (mousePoint.y - bounds.y)*360/bounds.height;
                    if (*hue <= 0.0f) *hue = 0.0f;
                    if (*hue >= 359.0f) *hue = 359.0f;
                }
            }
            else
            {
                guiControlExclusiveMode = false;
                guiControlExclusiveRec = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds) || CheckCollisionPointRec(mousePoint, selector))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiControlExclusiveMode = true;
                guiControlExclusiveRec = bounds; // Store bounds as an identifier when dragging starts

                *hue = (mousePoint.y - bounds.y)*360/bounds.height;
                if (*hue <= 0.0f) *hue = 0.0f;
                if (*hue >= 359.0f) *hue = 359.0f;

            }
            else state = STATE_FOCUSED;

            /*if (IsKeyDown(KEY_UP))
            {
                hue -= 2.0f;
                if (hue <= 0.0f) hue = 0.0f;
            }
            else if (IsKeyDown(KEY_DOWN))
            {
                hue += 2.0f;
                if (hue >= 360.0f) hue = 360.0f;
            }*/
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state != STATE_DISABLED)
    {
        // Draw hue bar:color bars
        // TODO: Use directly DrawRectangleGradientEx(bounds, color1, color2, color2, color1);
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 255, 255, 0, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + bounds.height/6), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 255, 255, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 0, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 2*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 255, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 3*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 255, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 0, 255, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 4*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 0, 0, 255, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 255, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 5*(bounds.height/6)), (int)bounds.width, (int)(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 255, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 0, 255 }, guiAlpha));
    }
    else DrawRectangleGradientV((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height, Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), guiAlpha));

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);

    // Draw hue bar: selector
    GuiDrawRectangle(selector, 0, BLANK, GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)));
    //--------------------------------------------------------------------

    return result;
}

// Color Picker control
// NOTE: It's divided in multiple controls:
//      Color GuiColorPanel(Rectangle bounds, Color color)
//      float GuiColorBarAlpha(Rectangle bounds, float alpha)
//      float GuiColorBarHue(Rectangle bounds, float value)
// NOTE: bounds define GuiColorPanel() size
// NOTE: this picker converts RGB to HSV, which can cause the Hue control to jump. If you have this problem, consider using the HSV variant instead
int GuiColorPicker(Rectangle bounds, const char *text, Color *color)
{
    int result = 0;

    Color temp = { 200, 0, 0, 255 };
    if (color == NULL) color = &temp;

    GuiColorPanel(bounds, NULL, color);

    Rectangle boundsHue = { (float)bounds.x + bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_PADDING), (float)bounds.y, (float)GuiGetStyle(COLORPICKER, HUEBAR_WIDTH), (float)bounds.height };
    //Rectangle boundsAlpha = { bounds.x, bounds.y + bounds.height + GuiGetStyle(COLORPICKER, BARS_PADDING), bounds.width, GuiGetStyle(COLORPICKER, BARS_THICK) };

    // NOTE: this conversion can cause low hue-resolution, if the r, g and b value are very similar, which causes the hue bar to shift around when only the GuiColorPanel is used.
    Vector3 hsv = ConvertRGBtoHSV(RAYGUI_CLITERAL(Vector3){ (*color).r/255.0f, (*color).g/255.0f, (*color).b/255.0f });

    GuiColorBarHue(boundsHue, NULL, &hsv.x);

    //color.a = (unsigned char)(GuiColorBarAlpha(boundsAlpha, (float)color.a/255.0f)*255.0f);
    Vector3 rgb = ConvertHSVtoRGB(hsv);

    *color = RAYGUI_CLITERAL(Color){ (unsigned char)roundf(rgb.x*255.0f), (unsigned char)roundf(rgb.y*255.0f), (unsigned char)roundf(rgb.z*255.0f), (*color).a };

    return result;
}

// Color Picker control that avoids conversion to RGB and back to HSV on each call, thus avoiding jittering.
// The user can call ConvertHSVtoRGB() to convert *colorHsv value to RGB.
// NOTE: It's divided in multiple controls:
//      int GuiColorPanelHSV(Rectangle bounds, const char *text, Vector3 *colorHsv)
//      int GuiColorBarAlpha(Rectangle bounds, const char *text, float *alpha)
//      float GuiColorBarHue(Rectangle bounds, float value)
// NOTE: bounds define GuiColorPanelHSV() size
int GuiColorPickerHSV(Rectangle bounds, const char *text, Vector3 *colorHsv)
{
    int result = 0;

    Vector3 tempHsv = { 0 };

    if (colorHsv == NULL)
    {
        const Vector3 tempColor = { 200.0f/255.0f, 0.0f, 0.0f };
        tempHsv = ConvertRGBtoHSV(tempColor);
        colorHsv = &tempHsv;
    }

    GuiColorPanelHSV(bounds, NULL, colorHsv);

    const Rectangle boundsHue = { (float)bounds.x + bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_PADDING), (float)bounds.y, (float)GuiGetStyle(COLORPICKER, HUEBAR_WIDTH), (float)bounds.height };

    GuiColorBarHue(boundsHue, NULL, &colorHsv->x);

    return result;
}

// Color Panel control - HSV variant
int GuiColorPanelHSV(Rectangle bounds, const char *text, Vector3 *colorHsv)
{
    int result = 0;
    GuiState state = guiState;
    Vector2 pickerSelector = { 0 };

    const Color colWhite = { 255, 255, 255, 255 };
    const Color colBlack = { 0, 0, 0, 255 };

    pickerSelector.x = bounds.x + (float)colorHsv->y*bounds.width;            // HSV: Saturation
    pickerSelector.y = bounds.y + (1.0f - (float)colorHsv->z)*bounds.height;  // HSV: Value

    Vector3 maxHue = { colorHsv->x, 1.0f, 1.0f };
    Vector3 rgbHue = ConvertHSVtoRGB(maxHue);
    Color maxHueCol = { (unsigned char)(255.0f*rgbHue.x),
                      (unsigned char)(255.0f*rgbHue.y),
                      (unsigned char)(255.0f*rgbHue.z), 255 };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiControlExclusiveMode) // Allows to keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiControlExclusiveRec))
                {
                    pickerSelector = mousePoint;

                    if (pickerSelector.x < bounds.x) pickerSelector.x = bounds.x;
                    if (pickerSelector.x > bounds.x + bounds.width) pickerSelector.x = bounds.x + bounds.width;
                    if (pickerSelector.y < bounds.y) pickerSelector.y = bounds.y;
                    if (pickerSelector.y > bounds.y + bounds.height) pickerSelector.y = bounds.y + bounds.height;

                    // Calculate color from picker
                    Vector2 colorPick = { pickerSelector.x - bounds.x, pickerSelector.y - bounds.y };

                    colorPick.x /= (float)bounds.width;     // Get normalized value on x
                    colorPick.y /= (float)bounds.height;    // Get normalized value on y

                    colorHsv->y = colorPick.x;
                    colorHsv->z = 1.0f - colorPick.y;

                }
            }
            else
            {
                guiControlExclusiveMode = false;
                guiControlExclusiveRec = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiControlExclusiveMode = true;
                guiControlExclusiveRec = bounds;
                pickerSelector = mousePoint;

                // Calculate color from picker
                Vector2 colorPick = { pickerSelector.x - bounds.x, pickerSelector.y - bounds.y };

                colorPick.x /= (float)bounds.width;     // Get normalized value on x
                colorPick.y /= (float)bounds.height;    // Get normalized value on y

                colorHsv->y = colorPick.x;
                colorHsv->z = 1.0f - colorPick.y;
            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state != STATE_DISABLED)
    {
        DrawRectangleGradientEx(bounds, Fade(colWhite, guiAlpha), Fade(colWhite, guiAlpha), Fade(maxHueCol, guiAlpha), Fade(maxHueCol, guiAlpha));
        DrawRectangleGradientEx(bounds, Fade(colBlack, 0), Fade(colBlack, guiAlpha), Fade(colBlack, guiAlpha), Fade(colBlack, 0));

        // Draw color picker: selector
        Rectangle selector = { pickerSelector.x - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, pickerSelector.y - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE), (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE) };
        GuiDrawRectangle(selector, 0, BLANK, colWhite);
    }
    else
    {
        DrawRectangleGradientEx(bounds, Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), 0.6f), guiAlpha));
    }

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);
    //--------------------------------------------------------------------

    return result;
}

// Message Box control
int GuiMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons)
{
    #if !defined(RAYGUI_MESSAGEBOX_BUTTON_HEIGHT)
        #define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT    24
    #endif
    #if !defined(RAYGUI_MESSAGEBOX_BUTTON_PADDING)
        #define RAYGUI_MESSAGEBOX_BUTTON_PADDING   12
    #endif

    int result = -1;    // Returns clicked button from buttons list, 0 refers to closed window button

    int buttonCount = 0;
    const char **buttonsText = GuiTextSplit(buttons, ';', &buttonCount, NULL);
    Rectangle buttonBounds = { 0 };
    buttonBounds.x = bounds.x + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    buttonBounds.y = bounds.y + bounds.height - RAYGUI_MESSAGEBOX_BUTTON_HEIGHT - RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    buttonBounds.width = (bounds.width - RAYGUI_MESSAGEBOX_BUTTON_PADDING*(buttonCount + 1))/buttonCount;
    buttonBounds.height = RAYGUI_MESSAGEBOX_BUTTON_HEIGHT;

    int textWidth = GetTextWidth(message) + 2;

    Rectangle textBounds = { 0 };
    textBounds.x = bounds.x + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    textBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    textBounds.width = bounds.width - RAYGUI_MESSAGEBOX_BUTTON_PADDING*2;
    textBounds.height = bounds.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 3*RAYGUI_MESSAGEBOX_BUTTON_PADDING - RAYGUI_MESSAGEBOX_BUTTON_HEIGHT;

    // Draw control
    //--------------------------------------------------------------------
    if (GuiWindowBox(bounds, title)) result = 0;

    int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel(textBounds, message);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevTextAlignment);

    prevTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    for (int i = 0; i < buttonCount; i++)
    {
        if (GuiButton(buttonBounds, buttonsText[i])) result = i + 1;
        buttonBounds.x += (buttonBounds.width + RAYGUI_MESSAGEBOX_BUTTON_PADDING);
    }

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, prevTextAlignment);
    //--------------------------------------------------------------------

    return result;
}

// Text Input Box control, ask for text
int GuiTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool *secretViewActive)
{
    #if !defined(RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT)
        #define RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT      24
    #endif
    #if !defined(RAYGUI_TEXTINPUTBOX_BUTTON_PADDING)
        #define RAYGUI_TEXTINPUTBOX_BUTTON_PADDING     12
    #endif
    #if !defined(RAYGUI_TEXTINPUTBOX_HEIGHT)
        #define RAYGUI_TEXTINPUTBOX_HEIGHT             26
    #endif

    // Used to enable text edit mode
    // WARNING: No more than one GuiTextInputBox() should be open at the same time
    static bool textEditMode = false;

    int result = -1;

    int buttonCount = 0;
    const char **buttonsText = GuiTextSplit(buttons, ';', &buttonCount, NULL);
    Rectangle buttonBounds = { 0 };
    buttonBounds.x = bounds.x + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    buttonBounds.y = bounds.y + bounds.height - RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    buttonBounds.width = (bounds.width - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING*(buttonCount + 1))/buttonCount;
    buttonBounds.height = RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT;

    int messageInputHeight = (int)bounds.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - GuiGetStyle(STATUSBAR, BORDER_WIDTH) - RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT - 2*RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;

    Rectangle textBounds = { 0 };
    if (message != NULL)
    {
        int textSize = GetTextWidth(message) + 2;

        textBounds.x = bounds.x + bounds.width/2 - textSize/2;
        textBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + messageInputHeight/4 - (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        textBounds.width = (float)textSize;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    }

    Rectangle textBoxBounds = { 0 };
    textBoxBounds.x = bounds.x + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    textBoxBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - RAYGUI_TEXTINPUTBOX_HEIGHT/2;
    if (message == NULL) textBoxBounds.y = bounds.y + 24 + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    else textBoxBounds.y += (messageInputHeight/2 + messageInputHeight/4);
    textBoxBounds.width = bounds.width - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING*2;
    textBoxBounds.height = RAYGUI_TEXTINPUTBOX_HEIGHT;

    // Draw control
    //--------------------------------------------------------------------
    if (GuiWindowBox(bounds, title)) result = 0;

    // Draw message if available
    if (message != NULL)
    {
        int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiLabel(textBounds, message);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevTextAlignment);
    }

    if (secretViewActive != NULL)
    {
        static char stars[] = "****************";
        if (GuiTextBox(RAYGUI_CLITERAL(Rectangle){ textBoxBounds.x, textBoxBounds.y, textBoxBounds.width - 4 - RAYGUI_TEXTINPUTBOX_HEIGHT, textBoxBounds.height },
            ((*secretViewActive == 1) || textEditMode)? text : stars, textMaxSize, textEditMode)) textEditMode = !textEditMode;

        GuiToggle(RAYGUI_CLITERAL(Rectangle){ textBoxBounds.x + textBoxBounds.width - RAYGUI_TEXTINPUTBOX_HEIGHT, textBoxBounds.y, RAYGUI_TEXTINPUTBOX_HEIGHT, RAYGUI_TEXTINPUTBOX_HEIGHT }, (*secretViewActive == 1)? "#44#" : "#45#", secretViewActive);
    }
    else
    {
        if (GuiTextBox(textBoxBounds, text, textMaxSize, textEditMode)) textEditMode = !textEditMode;
    }

    int prevBtnTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    for (int i = 0; i < buttonCount; i++)
    {
        if (GuiButton(buttonBounds, buttonsText[i])) result = i + 1;
        buttonBounds.x += (buttonBounds.width + RAYGUI_MESSAGEBOX_BUTTON_PADDING);
    }

    if (result >= 0) textEditMode = false;

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, prevBtnTextAlignment);
    //--------------------------------------------------------------------

    return result;      // Result is the pressed button index
}

// Grid control
// NOTE: Returns grid mouse-hover selected cell
// About drawing lines at subpixel spacing, simple put, not easy solution:
// https://stackoverflow.com/questions/4435450/2d-opengl-drawing-lines-that-dont-exactly-fit-pixel-raster
int GuiGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 *mouseCell)
{
    // Grid lines alpha amount
    #if !defined(RAYGUI_GRID_ALPHA)
        #define RAYGUI_GRID_ALPHA    0.15f
    #endif

    int result = 0;
    GuiState state = guiState;

    Vector2 mousePoint = GetMousePosition();
    Vector2 currentMouseCell = { -1, -1 };

    float spaceWidth = spacing/(float)subdivs;
    int linesV = (int)(bounds.width/spaceWidth) + 1;
    int linesH = (int)(bounds.height/spaceWidth) + 1;

    int color = GuiGetStyle(DEFAULT, LINE_COLOR);

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            // NOTE: Cell values must be the upper left of the cell the mouse is in
            currentMouseCell.x = floorf((mousePoint.x - bounds.x)/spacing);
            currentMouseCell.y = floorf((mousePoint.y - bounds.y)/spacing);
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_DISABLED) color = GuiGetStyle(DEFAULT, BORDER_COLOR_DISABLED);

    if (subdivs > 0)
    {
        // Draw vertical grid lines
        for (int i = 0; i < linesV; i++)
        {
            Rectangle lineV = { bounds.x + spacing*i/subdivs, bounds.y, 1, bounds.height + 1 };
            GuiDrawRectangle(lineV, 0, BLANK, ((i%subdivs) == 0)? GuiFade(GetColor(color), RAYGUI_GRID_ALPHA*4) : GuiFade(GetColor(color), RAYGUI_GRID_ALPHA));
        }

        // Draw horizontal grid lines
        for (int i = 0; i < linesH; i++)
        {
            Rectangle lineH = { bounds.x, bounds.y + spacing*i/subdivs, bounds.width + 1, 1 };
            GuiDrawRectangle(lineH, 0, BLANK, ((i%subdivs) == 0)? GuiFade(GetColor(color), RAYGUI_GRID_ALPHA*4) : GuiFade(GetColor(color), RAYGUI_GRID_ALPHA));
        }
    }

    if (mouseCell != NULL) *mouseCell = currentMouseCell;
    return result;
}

//----------------------------------------------------------------------------------
// Tooltip management functions
// NOTE: Tooltips requires some global variables: tooltipPtr
//----------------------------------------------------------------------------------
// Enable gui tooltips (global state)
void GuiEnableTooltip(void) { guiTooltip = true; }

// Disable gui tooltips (global state)
void GuiDisableTooltip(void) { guiTooltip = false; }

// Set tooltip string
void GuiSetTooltip(const char *tooltip) { guiTooltipPtr = tooltip; }

//----------------------------------------------------------------------------------
// Styles loading functions
//----------------------------------------------------------------------------------

// Load raygui style file (.rgs)
// NOTE: By default a binary file is expected, that file could contain a custom font,
// in that case, custom font image atlas is GRAY+ALPHA and pixel data can be compressed (DEFLATE)
void GuiLoadStyle(const char *fileName)
{
    #define MAX_LINE_BUFFER_SIZE    256

    bool tryBinary = false;
    if (!guiStyleLoaded) GuiLoadStyleDefault();

    // Try reading the files as text file first
    FILE *rgsFile = fopen(fileName, "rt");

    if (rgsFile != NULL)
    {
        char buffer[MAX_LINE_BUFFER_SIZE] = { 0 };
        fgets(buffer, MAX_LINE_BUFFER_SIZE, rgsFile);

        if (buffer[0] == '#')
        {
            int controlId = 0;
            int propertyId = 0;
            unsigned int propertyValue = 0;

            while (!feof(rgsFile))
            {
                switch (buffer[0])
                {
                    case 'p':
                    {
                        // Style property: p <control_id> <property_id> <property_value> <property_name>

                        sscanf(buffer, "p %d %d 0x%x", &controlId, &propertyId, &propertyValue);
                        GuiSetStyle(controlId, propertyId, (int)propertyValue);

                    } break;
                    case 'f':
                    {
                        // Style font: f <gen_font_size> <charmap_file> <font_file>

                        int fontSize = 0;
                        char charmapFileName[256] = { 0 };
                        char fontFileName[256] = { 0 };
                        sscanf(buffer, "f %d %s %[^\r\n]s", &fontSize, charmapFileName, fontFileName);

                        Font font = { 0 };
                        int *codepoints = NULL;
                        int codepointCount = 0;

                        if (charmapFileName[0] != '0')
                        {
                            // Load text data from file
                            // NOTE: Expected an UTF-8 array of codepoints, no separation
                            char *textData = LoadFileText(TextFormat("%s/%s", GetDirectoryPath(fileName), charmapFileName));
                            codepoints = LoadCodepoints(textData, &codepointCount);
                            UnloadFileText(textData);
                        }

                        if (fontFileName[0] != '\0')
                        {
                            // In case a font is already loaded and it is not default internal font, unload it
                            if (font.texture.id != GetFontDefault().texture.id) UnloadTexture(font.texture);

                            if (codepointCount > 0) font = LoadFontEx(TextFormat("%s/%s", GetDirectoryPath(fileName), fontFileName), fontSize, codepoints, codepointCount);
                            else font = LoadFontEx(TextFormat("%s/%s", GetDirectoryPath(fileName), fontFileName), fontSize, NULL, 0);   // Default to 95 standard codepoints
                        }

                        // If font texture not properly loaded, revert to default font and size/spacing
                        if (font.texture.id == 0)
                        {
                            font = GetFontDefault();
                            GuiSetStyle(DEFAULT, TEXT_SIZE, 10);
                            GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
                        }

                        UnloadCodepoints(codepoints);

                        if ((font.texture.id > 0) && (font.glyphCount > 0)) GuiSetFont(font);

                    } break;
                    default: break;
                }

                fgets(buffer, MAX_LINE_BUFFER_SIZE, rgsFile);
            }
        }
        else tryBinary = true;

        fclose(rgsFile);
    }

    if (tryBinary)
    {
        rgsFile = fopen(fileName, "rb");

        if (rgsFile != NULL)
        {
            fseek(rgsFile, 0, SEEK_END);
            int fileDataSize = ftell(rgsFile);
            fseek(rgsFile, 0, SEEK_SET);

            if (fileDataSize > 0)
            {
                unsigned char *fileData = (unsigned char *)RAYGUI_MALLOC(fileDataSize*sizeof(unsigned char));
                fread(fileData, sizeof(unsigned char), fileDataSize, rgsFile);

                GuiLoadStyleFromMemory(fileData, fileDataSize);

                RAYGUI_FREE(fileData);
            }

            fclose(rgsFile);
        }
    }
}

// Load style default over global style
void GuiLoadStyleDefault(void)
{
    // We set this variable first to avoid cyclic function calls
    // when calling GuiSetStyle() and GuiGetStyle()
    guiStyleLoaded = true;

    // Initialize default LIGHT style property values
    // WARNING: Default value are applied to all controls on set but
    // they can be overwritten later on for every custom control
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x838383ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xc9c9c9ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x686868ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x5bb2d9ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0xc9effeff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x6c9bbcff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x0492c7ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x97e8ffff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0x368bafff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0xb5c1c2ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, 0xe6e9e9ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0xaeb7b8ff);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 0);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // Initialize default extended property values
    // NOTE: By default, extended property values are initialized to 0
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);                // DEFAULT, shared by all controls
    GuiSetStyle(DEFAULT, TEXT_SPACING, 1);              // DEFAULT, shared by all controls
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x90abb5ff);       // DEFAULT specific property
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xf5f5f5ff); // DEFAULT specific property
    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, 15);        // DEFAULT, 15 pixels between lines
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);   // DEFAULT, text aligned vertically to middle of text-bounds

    // Initialize control-specific property values
    // NOTE: Those properties are in default list but require specific values by control type
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
    GuiSetStyle(SLIDER, TEXT_PADDING, 4);
    GuiSetStyle(PROGRESSBAR, TEXT_PADDING, 4);
    GuiSetStyle(CHECKBOX, TEXT_PADDING, 4);
    GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 4);
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(VALUEBOX, TEXT_PADDING, 0);
    GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(SPINNER, TEXT_PADDING, 0);
    GuiSetStyle(SPINNER, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(STATUSBAR, TEXT_PADDING, 8);
    GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

    // Initialize extended property values
    // NOTE: By default, extended property values are initialized to 0
    GuiSetStyle(TOGGLE, GROUP_PADDING, 2);
    GuiSetStyle(SLIDER, SLIDER_WIDTH, 16);
    GuiSetStyle(SLIDER, SLIDER_PADDING, 1);
    GuiSetStyle(PROGRESSBAR, PROGRESS_PADDING, 1);
    GuiSetStyle(CHECKBOX, CHECK_PADDING, 1);
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, 32);
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_SPACING, 2);
    GuiSetStyle(DROPDOWNBOX, ARROW_PADDING, 16);
    GuiSetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING, 2);
    GuiSetStyle(SPINNER, SPIN_BUTTON_WIDTH, 24);
    GuiSetStyle(SPINNER, SPIN_BUTTON_SPACING, 2);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);
    GuiSetStyle(SCROLLBAR, ARROWS_VISIBLE, 0);
    GuiSetStyle(SCROLLBAR, ARROWS_SIZE, 6);
    GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING, 0);
    GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, 16);
    GuiSetStyle(SCROLLBAR, SCROLL_PADDING, 0);
    GuiSetStyle(SCROLLBAR, SCROLL_SPEED, 12);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, 28);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_SPACING, 2);
    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 12);
    GuiSetStyle(LISTVIEW, SCROLLBAR_SIDE, SCROLLBAR_RIGHT_SIDE);
    GuiSetStyle(COLORPICKER, COLOR_SELECTOR_SIZE, 8);
    GuiSetStyle(COLORPICKER, HUEBAR_WIDTH, 16);
    GuiSetStyle(COLORPICKER, HUEBAR_PADDING, 8);
    GuiSetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT, 8);
    GuiSetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW, 2);

    if (guiFont.texture.id != GetFontDefault().texture.id)
    {
        // Unload previous font texture
        UnloadTexture(guiFont.texture);
        RL_FREE(guiFont.recs);
        RL_FREE(guiFont.glyphs);
        guiFont.recs = NULL;
        guiFont.glyphs = NULL;

        // Setup default raylib font
        guiFont = GetFontDefault();

        // NOTE: Default raylib font character 95 is a white square
        Rectangle whiteChar = guiFont.recs[95];

        // NOTE: We set up a 1px padding on char rectangle to avoid pixel bleeding on MSAA filtering
        SetShapesTexture(guiFont.texture, RAYGUI_CLITERAL(Rectangle){ whiteChar.x + 1, whiteChar.y + 1, whiteChar.width - 2, whiteChar.height - 2 });
    }
}

// Get text with icon id prepended
// NOTE: Useful to add icons by name id (enum) instead of
// a number that can change between ricon versions
const char *GuiIconText(int iconId, const char *text)
{
#if defined(RAYGUI_NO_ICONS)
    return NULL;
#else
    static char buffer[1024] = { 0 };
    static char iconBuffer[16] = { 0 };

    if (text != NULL)
    {
        memset(buffer, 0, 1024);
        sprintf(buffer, "#%03i#", iconId);

        for (int i = 5; i < 1024; i++)
        {
            buffer[i] = text[i - 5];
            if (text[i - 5] == '\0') break;
        }

        return buffer;
    }
    else
    {
        sprintf(iconBuffer, "#%03i#", iconId);

        return iconBuffer;
    }
#endif
}

#if !defined(RAYGUI_NO_ICONS)
// Get full icons data pointer
unsigned int *GuiGetIcons(void) { return guiIconsPtr; }

// Load raygui icons file (.rgi)
// NOTE: In case nameIds are required, they can be requested with loadIconsName,
// they are returned as a guiIconsName[iconCount][RAYGUI_ICON_MAX_NAME_LENGTH],
// WARNING: guiIconsName[]][] memory should be manually freed!
char **GuiLoadIcons(const char *fileName, bool loadIconsName)
{
    // Style File Structure (.rgi)
    // ------------------------------------------------------
    // Offset  | Size    | Type       | Description
    // ------------------------------------------------------
    // 0       | 4       | char       | Signature: "rGI "
    // 4       | 2       | short      | Version: 100
    // 6       | 2       | short      | reserved

    // 8       | 2       | short      | Num icons (N)
    // 10      | 2       | short      | Icons size (Options: 16, 32, 64) (S)

    // Icons name id (32 bytes per name id)
    // foreach (icon)
    // {
    //   12+32*i  | 32   | char       | Icon NameId
    // }

    // Icons data: One bit per pixel, stored as unsigned int array (depends on icon size)
    // S*S pixels/32bit per unsigned int = K unsigned int per icon
    // foreach (icon)
    // {
    //   ...   | K       | unsigned int | Icon Data
    // }

    FILE *rgiFile = fopen(fileName, "rb");

    char **guiIconsName = NULL;

    if (rgiFile != NULL)
    {
        char signature[5] = { 0 };
        short version = 0;
        short reserved = 0;
        short iconCount = 0;
        short iconSize = 0;

        fread(signature, 1, 4, rgiFile);
        fread(&version, sizeof(short), 1, rgiFile);
        fread(&reserved, sizeof(short), 1, rgiFile);
        fread(&iconCount, sizeof(short), 1, rgiFile);
        fread(&iconSize, sizeof(short), 1, rgiFile);

        if ((signature[0] == 'r') &&
            (signature[1] == 'G') &&
            (signature[2] == 'I') &&
            (signature[3] == ' '))
        {
            if (loadIconsName)
            {
                guiIconsName = (char **)RAYGUI_MALLOC(iconCount*sizeof(char **));
                for (int i = 0; i < iconCount; i++)
                {
                    guiIconsName[i] = (char *)RAYGUI_MALLOC(RAYGUI_ICON_MAX_NAME_LENGTH);
                    fread(guiIconsName[i], 1, RAYGUI_ICON_MAX_NAME_LENGTH, rgiFile);
                }
            }
            else fseek(rgiFile, iconCount*RAYGUI_ICON_MAX_NAME_LENGTH, SEEK_CUR);

            // Read icons data directly over internal icons array
            fread(guiIconsPtr, sizeof(unsigned int), iconCount*(iconSize*iconSize/32), rgiFile);
        }

        fclose(rgiFile);
    }

    return guiIconsName;
}

// Draw selected icon using rectangles pixel-by-pixel
void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color)
{
    #define BIT_CHECK(a,b) ((a) & (1u<<(b)))

    for (int i = 0, y = 0; i < RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32; i++)
    {
        for (int k = 0; k < 32; k++)
        {
            if (BIT_CHECK(guiIconsPtr[iconId*RAYGUI_ICON_DATA_ELEMENTS + i], k))
            {
            #if !defined(RAYGUI_STANDALONE)
                GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ (float)posX + (k%RAYGUI_ICON_SIZE)*pixelSize, (float)posY + y*pixelSize, (float)pixelSize, (float)pixelSize }, 0, BLANK, color);
            #endif
            }

            if ((k == 15) || (k == 31)) y++;
        }
    }
}

// Set icon drawing size
void GuiSetIconScale(int scale)
{
    if (scale >= 1) guiIconScale = scale;
}

#endif      // !RAYGUI_NO_ICONS

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------

// Load style from memory
// WARNING: Binary files only
static void GuiLoadStyleFromMemory(const unsigned char *fileData, int dataSize)
{
    unsigned char *fileDataPtr = (unsigned char *)fileData;

    char signature[5] = { 0 };
    short version = 0;
    short reserved = 0;
    int propertyCount = 0;

    memcpy(signature, fileDataPtr, 4);
    memcpy(&version, fileDataPtr + 4, sizeof(short));
    memcpy(&reserved, fileDataPtr + 4 + 2, sizeof(short));
    memcpy(&propertyCount, fileDataPtr + 4 + 2 + 2, sizeof(int));
    fileDataPtr += 12;

    if ((signature[0] == 'r') &&
        (signature[1] == 'G') &&
        (signature[2] == 'S') &&
        (signature[3] == ' '))
    {
        short controlId = 0;
        short propertyId = 0;
        unsigned int propertyValue = 0;

        for (int i = 0; i < propertyCount; i++)
        {
            memcpy(&controlId, fileDataPtr, sizeof(short));
            memcpy(&propertyId, fileDataPtr + 2, sizeof(short));
            memcpy(&propertyValue, fileDataPtr + 2 + 2, sizeof(unsigned int));
            fileDataPtr += 8;

            if (controlId == 0) // DEFAULT control
            {
                // If a DEFAULT property is loaded, it is propagated to all controls
                // NOTE: All DEFAULT properties should be defined first in the file
                GuiSetStyle(0, (int)propertyId, propertyValue);

                if (propertyId < RAYGUI_MAX_PROPS_BASE) for (int i = 1; i < RAYGUI_MAX_CONTROLS; i++) GuiSetStyle(i, (int)propertyId, propertyValue);
            }
            else GuiSetStyle((int)controlId, (int)propertyId, propertyValue);
        }

        // Font loading is highly dependant on raylib API to load font data and image

#if !defined(RAYGUI_STANDALONE)
        // Load custom font if available
        int fontDataSize = 0;
        memcpy(&fontDataSize, fileDataPtr, sizeof(int));
        fileDataPtr += 4;

        if (fontDataSize > 0)
        {
            Font font = { 0 };
            int fontType = 0;   // 0-Normal, 1-SDF

            memcpy(&font.baseSize, fileDataPtr, sizeof(int));
            memcpy(&font.glyphCount, fileDataPtr + 4, sizeof(int));
            memcpy(&fontType, fileDataPtr + 4 + 4, sizeof(int));
            fileDataPtr += 12;

            // Load font white rectangle
            Rectangle fontWhiteRec = { 0 };
            memcpy(&fontWhiteRec, fileDataPtr, sizeof(Rectangle));
            fileDataPtr += 16;

            // Load font image parameters
            int fontImageUncompSize = 0;
            int fontImageCompSize = 0;
            memcpy(&fontImageUncompSize, fileDataPtr, sizeof(int));
            memcpy(&fontImageCompSize, fileDataPtr + 4, sizeof(int));
            fileDataPtr += 8;

            Image imFont = { 0 };
            imFont.mipmaps = 1;
            memcpy(&imFont.width, fileDataPtr, sizeof(int));
            memcpy(&imFont.height, fileDataPtr + 4, sizeof(int));
            memcpy(&imFont.format, fileDataPtr + 4 + 4, sizeof(int));
            fileDataPtr += 12;

            if ((fontImageCompSize > 0) && (fontImageCompSize != fontImageUncompSize))
            {
                // Compressed font atlas image data (DEFLATE), it requires DecompressData()
                int dataUncompSize = 0;
                unsigned char *compData = (unsigned char *)RAYGUI_MALLOC(fontImageCompSize);
                memcpy(compData, fileDataPtr, fontImageCompSize);
                fileDataPtr += fontImageCompSize;

                imFont.data = DecompressData(compData, fontImageCompSize, &dataUncompSize);

                // Security check, dataUncompSize must match the provided fontImageUncompSize
                if (dataUncompSize != fontImageUncompSize) RAYGUI_LOG("WARNING: Uncompressed font atlas image data could be corrupted");

                RAYGUI_FREE(compData);
            }
            else
            {
                // Font atlas image data is not compressed
                imFont.data = (unsigned char *)RAYGUI_MALLOC(fontImageUncompSize);
                memcpy(imFont.data, fileDataPtr, fontImageUncompSize);
                fileDataPtr += fontImageUncompSize;
            }

            if (font.texture.id != GetFontDefault().texture.id) UnloadTexture(font.texture);
            font.texture = LoadTextureFromImage(imFont);

            RAYGUI_FREE(imFont.data);

            // Validate font atlas texture was loaded correctly
            if (font.texture.id != 0)
            {
                // Load font recs data
                int recsDataSize = font.glyphCount*sizeof(Rectangle);
                int recsDataCompressedSize = 0;

                // WARNING: Version 400 adds the compression size parameter
                if (version >= 400)
                {
                    // RGS files version 400 support compressed recs data
                    memcpy(&recsDataCompressedSize, fileDataPtr, sizeof(int));
                    fileDataPtr += sizeof(int);
                }

                if ((recsDataCompressedSize > 0) && (recsDataCompressedSize != recsDataSize))
                {
                    // Recs data is compressed, uncompress it
                    unsigned char *recsDataCompressed = (unsigned char *)RAYGUI_MALLOC(recsDataCompressedSize);

                    memcpy(recsDataCompressed, fileDataPtr, recsDataCompressedSize);
                    fileDataPtr += recsDataCompressedSize;

                    int recsDataUncompSize = 0;
                    font.recs = (Rectangle *)DecompressData(recsDataCompressed, recsDataCompressedSize, &recsDataUncompSize);

                    // Security check, data uncompressed size must match the expected original data size
                    if (recsDataUncompSize != recsDataSize) RAYGUI_LOG("WARNING: Uncompressed font recs data could be corrupted");

                    RAYGUI_FREE(recsDataCompressed);
                }
                else
                {
                    // Recs data is uncompressed
                    font.recs = (Rectangle *)RAYGUI_CALLOC(font.glyphCount, sizeof(Rectangle));
                    for (int i = 0; i < font.glyphCount; i++)
                    {
                        memcpy(&font.recs[i], fileDataPtr, sizeof(Rectangle));
                        fileDataPtr += sizeof(Rectangle);
                    }
                }

                // Load font glyphs info data
                int glyphsDataSize = font.glyphCount*16;    // 16 bytes data per glyph
                int glyphsDataCompressedSize = 0;

                // WARNING: Version 400 adds the compression size parameter
                if (version >= 400)
                {
                    // RGS files version 400 support compressed glyphs data
                    memcpy(&glyphsDataCompressedSize, fileDataPtr, sizeof(int));
                    fileDataPtr += sizeof(int);
                }

                // Allocate required glyphs space to fill with data
                font.glyphs = (GlyphInfo *)RAYGUI_CALLOC(font.glyphCount, sizeof(GlyphInfo));

                if ((glyphsDataCompressedSize > 0) && (glyphsDataCompressedSize != glyphsDataSize))
                {
                    // Glyphs data is compressed, uncompress it
                    unsigned char *glypsDataCompressed = (unsigned char *)RAYGUI_MALLOC(glyphsDataCompressedSize);

                    memcpy(glypsDataCompressed, fileDataPtr, glyphsDataCompressedSize);
                    fileDataPtr += glyphsDataCompressedSize;

                    int glyphsDataUncompSize = 0;
                    unsigned char *glyphsDataUncomp = DecompressData(glypsDataCompressed, glyphsDataCompressedSize, &glyphsDataUncompSize);

                    // Security check, data uncompressed size must match the expected original data size
                    if (glyphsDataUncompSize != glyphsDataSize) RAYGUI_LOG("WARNING: Uncompressed font glyphs data could be corrupted");

                    unsigned char *glyphsDataUncompPtr = glyphsDataUncomp;

                    for (int i = 0; i < font.glyphCount; i++)
                    {
                        memcpy(&font.glyphs[i].value, glyphsDataUncompPtr, sizeof(int));
                        memcpy(&font.glyphs[i].offsetX, glyphsDataUncompPtr + 4, sizeof(int));
                        memcpy(&font.glyphs[i].offsetY, glyphsDataUncompPtr + 8, sizeof(int));
                        memcpy(&font.glyphs[i].advanceX, glyphsDataUncompPtr + 12, sizeof(int));
                        glyphsDataUncompPtr += 16;
                    }

                    RAYGUI_FREE(glypsDataCompressed);
                    RAYGUI_FREE(glyphsDataUncomp);
                }
                else
                {
                    // Glyphs data is uncompressed
                    for (int i = 0; i < font.glyphCount; i++)
                    {
                        memcpy(&font.glyphs[i].value, fileDataPtr, sizeof(int));
                        memcpy(&font.glyphs[i].offsetX, fileDataPtr + 4, sizeof(int));
                        memcpy(&font.glyphs[i].offsetY, fileDataPtr + 8, sizeof(int));
                        memcpy(&font.glyphs[i].advanceX, fileDataPtr + 12, sizeof(int));
                        fileDataPtr += 16;
                    }
                }
            }
            else font = GetFontDefault();   // Fallback in case of errors loading font atlas texture

            GuiSetFont(font);

            // Set font texture source rectangle to be used as white texture to draw shapes
            // NOTE: It makes possible to draw shapes and text (full UI) in a single draw call
            if ((fontWhiteRec.x > 0) &&
                (fontWhiteRec.y > 0) &&
                (fontWhiteRec.width > 0) &&
                (fontWhiteRec.height > 0)) SetShapesTexture(font.texture, fontWhiteRec);
        }
#endif
    }
}

// Gui get text width considering icon
static int GetTextWidth(const char *text)
{
    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    Vector2 textSize = { 0 };
    int textIconOffset = 0;

    if ((text != NULL) && (text[0] != '\0'))
    {
        if (text[0] == '#')
        {
            for (int i = 1; (i < 5) && (text[i] != '\0'); i++)
            {
                if (text[i] == '#')
                {
                    textIconOffset = i;
                    break;
                }
            }
        }

        text += textIconOffset;

        // Make sure guiFont is set, GuiGetStyle() initializes it lazynessly
        float fontSize = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);

        // Custom MeasureText() implementation
        if ((guiFont.texture.id > 0) && (text != NULL))
        {
            // Get size in bytes of text, considering end of line and line break
            int size = 0;
            for (int i = 0; i < MAX_LINE_BUFFER_SIZE; i++)
            {
                if ((text[i] != '\0') && (text[i] != '\n')) size++;
                else break;
            }

            float scaleFactor = fontSize/(float)guiFont.baseSize;
            textSize.y = (float)guiFont.baseSize*scaleFactor;
            float glyphWidth = 0.0f;

            for (int i = 0, codepointSize = 0; i < size; i += codepointSize)
            {
                int codepoint = GetCodepointNext(&text[i], &codepointSize);
                int codepointIndex = GetGlyphIndex(guiFont, codepoint);

                if (guiFont.glyphs[codepointIndex].advanceX == 0) glyphWidth = ((float)guiFont.recs[codepointIndex].width*scaleFactor);
                else glyphWidth = ((float)guiFont.glyphs[codepointIndex].advanceX*scaleFactor);

                textSize.x += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
            }
        }

        if (textIconOffset > 0) textSize.x += (RAYGUI_ICON_SIZE + ICON_TEXT_PADDING);
    }

    return (int)textSize.x;
}

// Get text bounds considering control bounds
static Rectangle GetTextBounds(int control, Rectangle bounds)
{
    Rectangle textBounds = bounds;

    textBounds.x = bounds.x + GuiGetStyle(control, BORDER_WIDTH);
    textBounds.y = bounds.y + GuiGetStyle(control, BORDER_WIDTH) + GuiGetStyle(control, TEXT_PADDING);
    textBounds.width = bounds.width - 2*GuiGetStyle(control, BORDER_WIDTH) - 2*GuiGetStyle(control, TEXT_PADDING);
    textBounds.height = bounds.height - 2*GuiGetStyle(control, BORDER_WIDTH) - 2*GuiGetStyle(control, TEXT_PADDING);    // NOTE: Text is processed line per line!

    // Depending on control, TEXT_PADDING and TEXT_ALIGNMENT properties could affect the text-bounds
    switch (control)
    {
        case COMBOBOX:
        case DROPDOWNBOX:
        case LISTVIEW:
            // TODO: Special cases (no label): COMBOBOX, DROPDOWNBOX, LISTVIEW
        case SLIDER:
        case CHECKBOX:
        case VALUEBOX:
        case SPINNER:
            // TODO: More special cases (label on side): SLIDER, CHECKBOX, VALUEBOX, SPINNER
        default:
        {
            // TODO: WARNING: TEXT_ALIGNMENT is already considered in GuiDrawText()
            if (GuiGetStyle(control, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT) textBounds.x -= GuiGetStyle(control, TEXT_PADDING);
            else textBounds.x += GuiGetStyle(control, TEXT_PADDING);
        }
        break;
    }

    return textBounds;
}

// Get text icon if provided and move text cursor
// NOTE: We support up to 999 values for iconId
static const char *GetTextIcon(const char *text, int *iconId)
{
#if !defined(RAYGUI_NO_ICONS)
    *iconId = -1;
    if (text[0] == '#')     // Maybe we have an icon!
    {
        char iconValue[4] = { 0 };  // Maximum length for icon value: 3 digits + '\0'

        int pos = 1;
        while ((pos < 4) && (text[pos] >= '0') && (text[pos] <= '9'))
        {
            iconValue[pos - 1] = text[pos];
            pos++;
        }

        if (text[pos] == '#')
        {
            *iconId = TextToInteger(iconValue);

            // Move text pointer after icon
            // WARNING: If only icon provided, it could point to EOL character: '\0'
            if (*iconId >= 0) text += (pos + 1);
        }
    }
#endif

    return text;
}

// Get text divided into lines (by line-breaks '\n')
RAYGUIAPI const char **GetTextLines(const char *text, int *count)
{
    #define RAYGUI_MAX_TEXT_LINES   128

    static const char *lines[RAYGUI_MAX_TEXT_LINES] = { 0 };
    for (int i = 0; i < RAYGUI_MAX_TEXT_LINES; i++) lines[i] = NULL;    // Init NULL pointers to substrings

    int textSize = (int)strlen(text);

    lines[0] = text;
    int len = 0;
    *count = 1;
    //int lineSize = 0;   // Stores current line size, not returned

    for (int i = 0, k = 0; (i < textSize) && (*count < RAYGUI_MAX_TEXT_LINES); i++)
    {
        if (text[i] == '\n')
        {
            //lineSize = len;
            k++;
            lines[k] = &text[i + 1];     // WARNING: next value is valid?
            len = 0;
            *count += 1;
        }
        else len++;
    }

    //lines[*count - 1].size = len;

    return lines;
}

// Get text width to next space for provided string
static float GetNextSpaceWidth(const char *text, int *nextSpaceIndex)
{
    float width = 0;
    int codepointByteCount = 0;
    int codepoint = 0;
    int index = 0;
    float glyphWidth = 0;
    float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/guiFont.baseSize;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] != ' ')
        {
            codepoint = GetCodepoint(&text[i], &codepointByteCount);
            index = GetGlyphIndex(guiFont, codepoint);
            glyphWidth = (guiFont.glyphs[index].advanceX == 0)? guiFont.recs[index].width*scaleFactor : guiFont.glyphs[index].advanceX*scaleFactor;
            width += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
        }
        else
        {
            *nextSpaceIndex = i;
            break;
        }
    }

    return width;
}

// Gui draw text using default font
static void GuiDrawText(const char *text, Rectangle textBounds, int alignment, Color tint)
{
    #define TEXT_VALIGN_PIXEL_OFFSET(h)  ((int)h%2)     // Vertical alignment for pixel perfect

    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    if ((text == NULL) || (text[0] == '\0')) return;    // Security check

    // PROCEDURE:
    //   - Text is processed line per line
    //   - For every line, horizontal alignment is defined
    //   - For all text, vertical alignment is defined (multiline text only)
    //   - For every line, wordwrap mode is checked (useful for GuitextBox(), read-only)

    // Get text lines (using '\n' as delimiter) to be processed individually
    // WARNING: We can't use GuiTextSplit() function because it can be already used
    // before the GuiDrawText() call and its buffer is static, it would be overriden :(
    int lineCount = 0;
    const char **lines = GetTextLines(text, &lineCount);

    // Text style variables
    //int alignment = GuiGetStyle(DEFAULT, TEXT_ALIGNMENT);
    int alignmentVertical = GuiGetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL);
    int wrapMode = GuiGetStyle(DEFAULT, TEXT_WRAP_MODE);    // Wrap-mode only available in read-only mode, no for text editing

    // TODO: WARNING: This totalHeight is not valid for vertical alignment in case of word-wrap
    float totalHeight = (float)(lineCount*GuiGetStyle(DEFAULT, TEXT_SIZE) + (lineCount - 1)*GuiGetStyle(DEFAULT, TEXT_SIZE)/2);
    float posOffsetY = 0.0f;

    for (int i = 0; i < lineCount; i++)
    {
        int iconId = 0;
        lines[i] = GetTextIcon(lines[i], &iconId);      // Check text for icon and move cursor

        // Get text position depending on alignment and iconId
        //---------------------------------------------------------------------------------
        Vector2 textBoundsPosition = { textBounds.x, textBounds.y };
        float textBoundsWidthOffset = 0.0f;

        // NOTE: We get text size after icon has been processed
        // WARNING: GetTextWidth() also processes text icon to get width! -> Really needed?
        int textSizeX = GetTextWidth(lines[i]);

        // If text requires an icon, add size to measure
        if (iconId >= 0)
        {
            textSizeX += RAYGUI_ICON_SIZE*guiIconScale;

            // WARNING: If only icon provided, text could be pointing to EOF character: '\0'
#if !defined(RAYGUI_NO_ICONS)
            if ((lines[i] != NULL) && (lines[i][0] != '\0')) textSizeX += ICON_TEXT_PADDING;
#endif
        }

        // Check guiTextAlign global variables
        switch (alignment)
        {
            case TEXT_ALIGN_LEFT: textBoundsPosition.x = textBounds.x; break;
            case TEXT_ALIGN_CENTER: textBoundsPosition.x = textBounds.x +  textBounds.width/2 - textSizeX/2; break;
            case TEXT_ALIGN_RIGHT: textBoundsPosition.x = textBounds.x + textBounds.width - textSizeX; break;
            default: break;
        }

        if (textSizeX > textBounds.width && (lines[i] != NULL) && (lines[i][0] != '\0')) textBoundsPosition.x = textBounds.x;

        switch (alignmentVertical)
        {
            // Only valid in case of wordWrap = 0;
            case TEXT_ALIGN_TOP: textBoundsPosition.y = textBounds.y + posOffsetY; break;
            case TEXT_ALIGN_MIDDLE: textBoundsPosition.y = textBounds.y + posOffsetY + textBounds.height/2 - totalHeight/2 + TEXT_VALIGN_PIXEL_OFFSET(textBounds.height); break;
            case TEXT_ALIGN_BOTTOM: textBoundsPosition.y = textBounds.y + posOffsetY + textBounds.height - totalHeight + TEXT_VALIGN_PIXEL_OFFSET(textBounds.height); break;
            default: break;
        }

        // NOTE: Make sure we get pixel-perfect coordinates,
        // In case of decimals we got weird text positioning
        textBoundsPosition.x = (float)((int)textBoundsPosition.x);
        textBoundsPosition.y = (float)((int)textBoundsPosition.y);
        //---------------------------------------------------------------------------------

        // Draw text (with icon if available)
        //---------------------------------------------------------------------------------
#if !defined(RAYGUI_NO_ICONS)
        if (iconId >= 0)
        {
            // NOTE: We consider icon height, probably different than text size
            GuiDrawIcon(iconId, (int)textBoundsPosition.x, (int)(textBounds.y + textBounds.height/2 - RAYGUI_ICON_SIZE*guiIconScale/2 + TEXT_VALIGN_PIXEL_OFFSET(textBounds.height)), guiIconScale, tint);
            textBoundsPosition.x += (float)(RAYGUI_ICON_SIZE*guiIconScale + ICON_TEXT_PADDING);
            textBoundsWidthOffset = (float)(RAYGUI_ICON_SIZE*guiIconScale + ICON_TEXT_PADDING);
        }
#endif
        // Get size in bytes of text,
        // considering end of line and line break
        int lineSize = 0;
        for (int c = 0; (lines[i][c] != '\0') && (lines[i][c] != '\n') && (lines[i][c] != '\r'); c++, lineSize++){ }
        float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/guiFont.baseSize;

        int lastSpaceIndex = 0;
        bool tempWrapCharMode = false;

        int textOffsetY = 0;
        float textOffsetX = 0.0f;
        float glyphWidth = 0;

        int ellipsisWidth = GetTextWidth("...");
        bool textOverflow = false;
        for (int c = 0, codepointSize = 0; c < lineSize; c += codepointSize)
        {
            int codepoint = GetCodepointNext(&lines[i][c], &codepointSize);
            int index = GetGlyphIndex(guiFont, codepoint);

            // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
            // but we need to draw all of the bad bytes using the '?' symbol moving one byte
            if (codepoint == 0x3f) codepointSize = 1; // TODO: Review not recognized codepoints size

            // Get glyph width to check if it goes out of bounds
            if (guiFont.glyphs[index].advanceX == 0) glyphWidth = ((float)guiFont.recs[index].width*scaleFactor);
            else glyphWidth = (float)guiFont.glyphs[index].advanceX*scaleFactor;

            // Wrap mode text measuring, to validate if
            // it can be drawn or a new line is required
            if (wrapMode == TEXT_WRAP_CHAR)
            {
                // Jump to next line if current character reach end of the box limits
                if ((textOffsetX + glyphWidth) > textBounds.width - textBoundsWidthOffset)
                {
                    textOffsetX = 0.0f;
                    textOffsetY += GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);

                    if (tempWrapCharMode)   // Wrap at char level when too long words
                    {
                        wrapMode = TEXT_WRAP_WORD;
                        tempWrapCharMode = false;
                    }
                }
            }
            else if (wrapMode == TEXT_WRAP_WORD)
            {
                if (codepoint == 32) lastSpaceIndex = c;

                // Get width to next space in line
                int nextSpaceIndex = 0;
                float nextSpaceWidth = GetNextSpaceWidth(lines[i] + c, &nextSpaceIndex);

                int nextSpaceIndex2 = 0;
                float nextWordSize = GetNextSpaceWidth(lines[i] + lastSpaceIndex + 1, &nextSpaceIndex2);

                if (nextWordSize > textBounds.width - textBoundsWidthOffset)
                {
                    // Considering the case the next word is longer than bounds
                    tempWrapCharMode = true;
                    wrapMode = TEXT_WRAP_CHAR;
                }
                else if ((textOffsetX + nextSpaceWidth) > textBounds.width - textBoundsWidthOffset)
                {
                    textOffsetX = 0.0f;
                    textOffsetY += GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);
                }
            }

            if (codepoint == '\n') break;   // WARNING: Lines are already processed manually, no need to keep drawing after this codepoint
            else
            {
                // TODO: There are multiple types of spaces in Unicode,
                // maybe it's a good idea to add support for more: http://jkorpela.fi/chars/spaces.html
                if ((codepoint != ' ') && (codepoint != '\t'))      // Do not draw codepoints with no glyph
                {
                    if (wrapMode == TEXT_WRAP_NONE)
                    {
                        // Draw only required text glyphs fitting the textBounds.width
                        if (textSizeX > textBounds.width)
                        {
                            if (textOffsetX <= (textBounds.width - glyphWidth - textBoundsWidthOffset - ellipsisWidth))
                            {
                                DrawTextCodepoint(guiFont, codepoint, RAYGUI_CLITERAL(Vector2){ textBoundsPosition.x + textOffsetX, textBoundsPosition.y + textOffsetY }, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), GuiFade(tint, guiAlpha));
                            }
                            else if (!textOverflow)
                            {
                                textOverflow = true;

                                for (int j = 0; j < ellipsisWidth; j += ellipsisWidth/3)
                                {
                                    DrawTextCodepoint(guiFont, '.', RAYGUI_CLITERAL(Vector2){ textBoundsPosition.x + textOffsetX + j, textBoundsPosition.y + textOffsetY }, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), GuiFade(tint, guiAlpha));
                                }
                            }
                        }
                        else
                        {
                            DrawTextCodepoint(guiFont, codepoint, RAYGUI_CLITERAL(Vector2){ textBoundsPosition.x + textOffsetX, textBoundsPosition.y + textOffsetY }, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), GuiFade(tint, guiAlpha));
                        }
                    }
                    else if ((wrapMode == TEXT_WRAP_CHAR) || (wrapMode == TEXT_WRAP_WORD))
                    {
                        // Draw only glyphs inside the bounds
                        if ((textBoundsPosition.y + textOffsetY) <= (textBounds.y + textBounds.height - GuiGetStyle(DEFAULT, TEXT_SIZE)))
                        {
                            DrawTextCodepoint(guiFont, codepoint, RAYGUI_CLITERAL(Vector2){ textBoundsPosition.x + textOffsetX, textBoundsPosition.y + textOffsetY }, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), GuiFade(tint, guiAlpha));
                        }
                    }
                }

                if (guiFont.glyphs[index].advanceX == 0) textOffsetX += ((float)guiFont.recs[index].width*scaleFactor + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
                else textOffsetX += ((float)guiFont.glyphs[index].advanceX*scaleFactor + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
            }
        }

        if (wrapMode == TEXT_WRAP_NONE) posOffsetY += (float)GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);
        else if ((wrapMode == TEXT_WRAP_CHAR) || (wrapMode == TEXT_WRAP_WORD)) posOffsetY += (textOffsetY + (float)GuiGetStyle(DEFAULT, TEXT_LINE_SPACING));
        //---------------------------------------------------------------------------------
    }

#if defined(RAYGUI_DEBUG_TEXT_BOUNDS)
    GuiDrawRectangle(textBounds, 0, WHITE, Fade(BLUE, 0.4f));
#endif
}

// Gui draw rectangle using default raygui plain style with borders
static void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color)
{
    if (color.a > 0)
    {
        // Draw rectangle filled with color
        DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height, GuiFade(color, guiAlpha));
    }

    if (borderWidth > 0)
    {
        // Draw rectangle border lines with color
        DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, borderWidth, GuiFade(borderColor, guiAlpha));
        DrawRectangle((int)rec.x, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2*borderWidth, GuiFade(borderColor, guiAlpha));
        DrawRectangle((int)rec.x + (int)rec.width - borderWidth, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2*borderWidth, GuiFade(borderColor, guiAlpha));
        DrawRectangle((int)rec.x, (int)rec.y + (int)rec.height - borderWidth, (int)rec.width, borderWidth, GuiFade(borderColor, guiAlpha));
    }

#if defined(RAYGUI_DEBUG_RECS_BOUNDS)
    DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height, Fade(RED, 0.4f));
#endif
}

// Draw tooltip using control bounds
static void GuiTooltip(Rectangle controlRec)
{
    if (!guiLocked && guiTooltip && (guiTooltipPtr != NULL) && !guiControlExclusiveMode)
    {
        Vector2 textSize = MeasureTextEx(GuiGetFont(), guiTooltipPtr, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING));

        if ((controlRec.x + textSize.x + 16) > GetScreenWidth()) controlRec.x -= (textSize.x + 16 - controlRec.width);

        GuiPanel(RAYGUI_CLITERAL(Rectangle){ controlRec.x, controlRec.y + controlRec.height + 4, textSize.x + 16, GuiGetStyle(DEFAULT, TEXT_SIZE) + 8.f }, NULL);

        int textPadding = GuiGetStyle(LABEL, TEXT_PADDING);
        int textAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_PADDING, 0);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiLabel(RAYGUI_CLITERAL(Rectangle){ controlRec.x, controlRec.y + controlRec.height + 4, textSize.x + 16, GuiGetStyle(DEFAULT, TEXT_SIZE) + 8.f }, guiTooltipPtr);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, textAlignment);
        GuiSetStyle(LABEL, TEXT_PADDING, textPadding);
    }
}

// Split controls text into multiple strings
// Also check for multiple columns (required by GuiToggleGroup())
static const char **GuiTextSplit(const char *text, char delimiter, int *count, int *textRow)
{
    // NOTE: Current implementation returns a copy of the provided string with '\0' (string end delimiter)
    // inserted between strings defined by "delimiter" parameter. No memory is dynamically allocated,
    // all used memory is static... it has some limitations:
    //      1. Maximum number of possible split strings is set by RAYGUI_TEXTSPLIT_MAX_ITEMS
    //      2. Maximum size of text to split is RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE
    // NOTE: Those definitions could be externally provided if required

    // TODO: HACK: GuiTextSplit() - Review how textRows are returned to user
    // textRow is an externally provided array of integers that stores row number for every splitted string

    #if !defined(RAYGUI_TEXTSPLIT_MAX_ITEMS)
        #define RAYGUI_TEXTSPLIT_MAX_ITEMS          128
    #endif
    #if !defined(RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE)
        #define RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE     1024
    #endif

    static const char *result[RAYGUI_TEXTSPLIT_MAX_ITEMS] = { NULL };   // String pointers array (points to buffer data)
    static char buffer[RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE] = { 0 };         // Buffer data (text input copy with '\0' added)
    memset(buffer, 0, RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE);

    result[0] = buffer;
    int counter = 1;

    if (textRow != NULL) textRow[0] = 0;

    // Count how many substrings we have on text and point to every one
    for (int i = 0; i < RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE; i++)
    {
        buffer[i] = text[i];
        if (buffer[i] == '\0') break;
        else if ((buffer[i] == delimiter) || (buffer[i] == '\n'))
        {
            result[counter] = buffer + i + 1;

            if (textRow != NULL)
            {
                if (buffer[i] == '\n') textRow[counter] = textRow[counter - 1] + 1;
                else textRow[counter] = textRow[counter - 1];
            }

            buffer[i] = '\0';   // Set an end of string at this point

            counter++;
            if (counter > RAYGUI_TEXTSPLIT_MAX_ITEMS) break;
        }
    }

    *count = counter;

    return result;
}

// Convert color data from RGB to HSV
// NOTE: Color data should be passed normalized
static Vector3 ConvertRGBtoHSV(Vector3 rgb)
{
    Vector3 hsv = { 0 };
    float min = 0.0f;
    float max = 0.0f;
    float delta = 0.0f;

    min = (rgb.x < rgb.y)? rgb.x : rgb.y;
    min = (min < rgb.z)? min  : rgb.z;

    max = (rgb.x > rgb.y)? rgb.x : rgb.y;
    max = (max > rgb.z)? max  : rgb.z;

    hsv.z = max;            // Value
    delta = max - min;

    if (delta < 0.00001f)
    {
        hsv.y = 0.0f;
        hsv.x = 0.0f;           // Undefined, maybe NAN?
        return hsv;
    }

    if (max > 0.0f)
    {
        // NOTE: If max is 0, this divide would cause a crash
        hsv.y = (delta/max);    // Saturation
    }
    else
    {
        // NOTE: If max is 0, then r = g = b = 0, s = 0, h is undefined
        hsv.y = 0.0f;
        hsv.x = 0.0f;           // Undefined, maybe NAN?
        return hsv;
    }

    // NOTE: Comparing float values could not work properly
    if (rgb.x >= max) hsv.x = (rgb.y - rgb.z)/delta;    // Between yellow & magenta
    else
    {
        if (rgb.y >= max) hsv.x = 2.0f + (rgb.z - rgb.x)/delta;  // Between cyan & yellow
        else hsv.x = 4.0f + (rgb.x - rgb.y)/delta;      // Between magenta & cyan
    }

    hsv.x *= 60.0f;     // Convert to degrees

    if (hsv.x < 0.0f) hsv.x += 360.0f;

    return hsv;
}

// Convert color data from HSV to RGB
// NOTE: Color data should be passed normalized
static Vector3 ConvertHSVtoRGB(Vector3 hsv)
{
    Vector3 rgb = { 0 };
    float hh = 0.0f, p = 0.0f, q = 0.0f, t = 0.0f, ff = 0.0f;
    long i = 0;

    // NOTE: Comparing float values could not work properly
    if (hsv.y <= 0.0f)
    {
        rgb.x = hsv.z;
        rgb.y = hsv.z;
        rgb.z = hsv.z;
        return rgb;
    }

    hh = hsv.x;
    if (hh >= 360.0f) hh = 0.0f;
    hh /= 60.0f;

    i = (long)hh;
    ff = hh - i;
    p = hsv.z*(1.0f - hsv.y);
    q = hsv.z*(1.0f - (hsv.y*ff));
    t = hsv.z*(1.0f - (hsv.y*(1.0f - ff)));

    switch (i)
    {
        case 0:
        {
            rgb.x = hsv.z;
            rgb.y = t;
            rgb.z = p;
        } break;
        case 1:
        {
            rgb.x = q;
            rgb.y = hsv.z;
            rgb.z = p;
        } break;
        case 2:
        {
            rgb.x = p;
            rgb.y = hsv.z;
            rgb.z = t;
        } break;
        case 3:
        {
            rgb.x = p;
            rgb.y = q;
            rgb.z = hsv.z;
        } break;
        case 4:
        {
            rgb.x = t;
            rgb.y = p;
            rgb.z = hsv.z;
        } break;
        case 5:
        default:
        {
            rgb.x = hsv.z;
            rgb.y = p;
            rgb.z = q;
        } break;
    }

    return rgb;
}

// Scroll bar control (used by GuiScrollPanel())
static int GuiScrollBar(Rectangle bounds, int value, int minValue, int maxValue)
{
    GuiState state = guiState;

    // Is the scrollbar horizontal or vertical?
    bool isVertical = (bounds.width > bounds.height)? false : true;

    // The size (width or height depending on scrollbar type) of the spinner buttons
    const int spinnerSize = GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE)?
        (isVertical? (int)bounds.width - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH) :
        (int)bounds.height - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH)) : 0;

    // Arrow buttons [<] [>] [∧] [∨]
    Rectangle arrowUpLeft = { 0 };
    Rectangle arrowDownRight = { 0 };

    // Actual area of the scrollbar excluding the arrow buttons
    Rectangle scrollbar = { 0 };

    // Slider bar that moves     --[///]-----
    Rectangle slider = { 0 };

    // Normalize value
    if (value > maxValue) value = maxValue;
    if (value < minValue) value = minValue;

    int valueRange = maxValue - minValue;
    if (valueRange <= 0) valueRange = 1;

    int sliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);
    if (sliderSize < 1) sliderSize = 1;  // TODO: Consider a minimum slider size

    // Calculate rectangles for all of the components
    arrowUpLeft = RAYGUI_CLITERAL(Rectangle){
        (float)bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH),
        (float)bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH),
        (float)spinnerSize, (float)spinnerSize };

    if (isVertical)
    {
        arrowDownRight = RAYGUI_CLITERAL(Rectangle){ (float)bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)bounds.y + bounds.height - spinnerSize - GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };
        scrollbar = RAYGUI_CLITERAL(Rectangle){ bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING), arrowUpLeft.y + arrowUpLeft.height, bounds.width - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING)), bounds.height - arrowUpLeft.height - arrowDownRight.height - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH) };

        // Make sure the slider won't get outside of the scrollbar
        sliderSize = (sliderSize >= scrollbar.height)? ((int)scrollbar.height - 2) : sliderSize;
        slider = RAYGUI_CLITERAL(Rectangle){
            bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING),
            scrollbar.y + (int)(((float)(value - minValue)/valueRange)*(scrollbar.height - sliderSize)),
            bounds.width - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING)),
            (float)sliderSize };
    }
    else    // horizontal
    {
        arrowDownRight = RAYGUI_CLITERAL(Rectangle){ (float)bounds.x + bounds.width - spinnerSize - GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };
        scrollbar = RAYGUI_CLITERAL(Rectangle){ arrowUpLeft.x + arrowUpLeft.width, bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING), bounds.width - arrowUpLeft.width - arrowDownRight.width - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH), bounds.height - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING)) };

        // Make sure the slider won't get outside of the scrollbar
        sliderSize = (sliderSize >= scrollbar.width)? ((int)scrollbar.width - 2) : sliderSize;
        slider = RAYGUI_CLITERAL(Rectangle){
            scrollbar.x + (int)(((float)(value - minValue)/valueRange)*(scrollbar.width - sliderSize)),
            bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING),
            (float)sliderSize,
            bounds.height - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING)) };
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiControlExclusiveMode) // Allows to keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
                !CheckCollisionPointRec(mousePoint, arrowUpLeft) &&
                !CheckCollisionPointRec(mousePoint, arrowDownRight))
            {
                if (CHECK_BOUNDS_ID(bounds, guiControlExclusiveRec))
                {
                    state = STATE_PRESSED;

                    if (isVertical) value = (int)(((float)(mousePoint.y - scrollbar.y - slider.height/2)*valueRange)/(scrollbar.height - slider.height) + minValue);
                    else value = (int)(((float)(mousePoint.x - scrollbar.x - slider.width/2)*valueRange)/(scrollbar.width - slider.width) + minValue);
                }
            }
            else
            {
                guiControlExclusiveMode = false;
                guiControlExclusiveRec = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds))
        {
            state = STATE_FOCUSED;

            // Handle mouse wheel
            int wheel = (int)GetMouseWheelMove();
            if (wheel != 0) value += wheel;

            // Handle mouse button down
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                guiControlExclusiveMode = true;
                guiControlExclusiveRec = bounds; // Store bounds as an identifier when dragging starts

                // Check arrows click
                if (CheckCollisionPointRec(mousePoint, arrowUpLeft)) value -= valueRange/GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                else if (CheckCollisionPointRec(mousePoint, arrowDownRight)) value += valueRange/GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                else if (!CheckCollisionPointRec(mousePoint, slider))
                {
                    // If click on scrollbar position but not on slider, place slider directly on that position
                    if (isVertical) value = (int)(((float)(mousePoint.y - scrollbar.y - slider.height/2)*valueRange)/(scrollbar.height - slider.height) + minValue);
                    else value = (int)(((float)(mousePoint.x - scrollbar.x - slider.width/2)*valueRange)/(scrollbar.width - slider.width) + minValue);
                }

                state = STATE_PRESSED;
            }

            // Keyboard control on mouse hover scrollbar
            /*
            if (isVertical)
            {
                if (IsKeyDown(KEY_DOWN)) value += 5;
                else if (IsKeyDown(KEY_UP)) value -= 5;
            }
            else
            {
                if (IsKeyDown(KEY_RIGHT)) value += 5;
                else if (IsKeyDown(KEY_LEFT)) value -= 5;
            }
            */
        }

        // Normalize value
        if (value > maxValue) value = maxValue;
        if (value < minValue) value = minValue;
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(SCROLLBAR, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER + state*3)), GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_DISABLED)));   // Draw the background

    GuiDrawRectangle(scrollbar, 0, BLANK, GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL)));     // Draw the scrollbar active area background
    GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BORDER + state*3)));         // Draw the slider bar

    // Draw arrows (using icon if available)
    if (GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE))
    {
#if defined(RAYGUI_NO_ICONS)
        GuiDrawText(isVertical? "^" : "<",
            RAYGUI_CLITERAL(Rectangle){ arrowUpLeft.x, arrowUpLeft.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));
        GuiDrawText(isVertical? "v" : ">",
            RAYGUI_CLITERAL(Rectangle){ arrowDownRight.x, arrowDownRight.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));
#else
        GuiDrawText(isVertical? "#121#" : "#118#",
            RAYGUI_CLITERAL(Rectangle){ arrowUpLeft.x, arrowUpLeft.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(SCROLLBAR, TEXT + state*3)));   // ICON_ARROW_UP_FILL / ICON_ARROW_LEFT_FILL
        GuiDrawText(isVertical? "#120#" : "#119#",
            RAYGUI_CLITERAL(Rectangle){ arrowDownRight.x, arrowDownRight.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(SCROLLBAR, TEXT + state*3)));   // ICON_ARROW_DOWN_FILL / ICON_ARROW_RIGHT_FILL
#endif
    }
    //--------------------------------------------------------------------

    return value;
}

// Color fade-in or fade-out, alpha goes from 0.0f to 1.0f
// WARNING: It multiplies current alpha by alpha scale factor
static Color GuiFade(Color color, float alpha)
{
    if (alpha < 0.0f) alpha = 0.0f;
    else if (alpha > 1.0f) alpha = 1.0f;

    Color result = { color.r, color.g, color.b, (unsigned char)(color.a*alpha) };

    return result;
}

#if defined(RAYGUI_STANDALONE)
// Returns a Color struct from hexadecimal value
static Color GetColor(int hexValue)
{
    Color color;

    color.r = (unsigned char)(hexValue >> 24) & 0xFF;
    color.g = (unsigned char)(hexValue >> 16) & 0xFF;
    color.b = (unsigned char)(hexValue >> 8) & 0xFF;
    color.a = (unsigned char)hexValue & 0xFF;

    return color;
}

// Returns hexadecimal value for a Color
static int ColorToInt(Color color)
{
    return (((int)color.r << 24) | ((int)color.g << 16) | ((int)color.b << 8) | (int)color.a);
}

// Check if point is inside rectangle
static bool CheckCollisionPointRec(Vector2 point, Rectangle rec)
{
    bool collision = false;

    if ((point.x >= rec.x) && (point.x <= (rec.x + rec.width)) &&
        (point.y >= rec.y) && (point.y <= (rec.y + rec.height))) collision = true;

    return collision;
}

// Formatting of text with variables to 'embed'
static const char *TextFormat(const char *text, ...)
{
    #if !defined(RAYGUI_TEXTFORMAT_MAX_SIZE)
        #define RAYGUI_TEXTFORMAT_MAX_SIZE   256
    #endif

    static char buffer[RAYGUI_TEXTFORMAT_MAX_SIZE];

    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);

    return buffer;
}

// Draw rectangle with vertical gradient fill color
// NOTE: This function is only used by GuiColorPicker()
static void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2)
{
    Rectangle bounds = { (float)posX, (float)posY, (float)width, (float)height };
    DrawRectangleGradientEx(bounds, color1, color2, color2, color1);
}

// Split string into multiple strings
const char **TextSplit(const char *text, char delimiter, int *count)
{
    // NOTE: Current implementation returns a copy of the provided string with '\0' (string end delimiter)
    // inserted between strings defined by "delimiter" parameter. No memory is dynamically allocated,
    // all used memory is static... it has some limitations:
    //      1. Maximum number of possible split strings is set by RAYGUI_TEXTSPLIT_MAX_ITEMS
    //      2. Maximum size of text to split is RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE

    #if !defined(RAYGUI_TEXTSPLIT_MAX_ITEMS)
        #define RAYGUI_TEXTSPLIT_MAX_ITEMS          128
    #endif
    #if !defined(RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE)
        #define RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE      1024
    #endif

    static const char *result[RAYGUI_TEXTSPLIT_MAX_ITEMS] = { NULL };
    static char buffer[RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE] = { 0 };
    memset(buffer, 0, RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE);

    result[0] = buffer;
    int counter = 0;

    if (text != NULL)
    {
        counter = 1;

        // Count how many substrings we have on text and point to every one
        for (int i = 0; i < RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE; i++)
        {
            buffer[i] = text[i];
            if (buffer[i] == '\0') break;
            else if (buffer[i] == delimiter)
            {
                buffer[i] = '\0';   // Set an end of string at this point
                result[counter] = buffer + i + 1;
                counter++;

                if (counter == RAYGUI_TEXTSPLIT_MAX_ITEMS) break;
            }
        }
    }

    *count = counter;
    return result;
}

// Get integer value from text
// NOTE: This function replaces atoi() [stdlib.h]
static int TextToInteger(const char *text)
{
    int value = 0;
    int sign = 1;

    if ((text[0] == '+') || (text[0] == '-'))
    {
        if (text[0] == '-') sign = -1;
        text++;
    }

    for (int i = 0; ((text[i] >= '0') && (text[i] <= '9')); ++i) value = value*10 + (int)(text[i] - '0');

    return value*sign;
}

// Get float value from text
// NOTE: This function replaces atof() [stdlib.h]
// WARNING: Only '.' character is understood as decimal point
static float TextToFloat(const char *text)
{
    float value = 0.0f;
    float sign = 1.0f;

    if ((text[0] == '+') || (text[0] == '-'))
    {
        if (text[0] == '-') sign = -1.0f;
        text++;
    }

    int i = 0;
    for (; ((text[i] >= '0') && (text[i] <= '9')); i++) value = value*10.0f + (float)(text[i] - '0');

    if (text[i++] != '.') value *= sign;
    else
    {
        float divisor = 10.0f;
        for (; ((text[i] >= '0') && (text[i] <= '9')); i++)
        {
            value += ((float)(text[i] - '0'))/divisor;
            divisor = divisor*10.0f;
        }
    }

    return value;
}

// Encode codepoint into UTF-8 text (char array size returned as parameter)
static const char *CodepointToUTF8(int codepoint, int *byteSize)
{
    static char utf8[6] = { 0 };
    int size = 0;

    if (codepoint <= 0x7f)
    {
        utf8[0] = (char)codepoint;
        size = 1;
    }
    else if (codepoint <= 0x7ff)
    {
        utf8[0] = (char)(((codepoint >> 6) & 0x1f) | 0xc0);
        utf8[1] = (char)((codepoint & 0x3f) | 0x80);
        size = 2;
    }
    else if (codepoint <= 0xffff)
    {
        utf8[0] = (char)(((codepoint >> 12) & 0x0f) | 0xe0);
        utf8[1] = (char)(((codepoint >>  6) & 0x3f) | 0x80);
        utf8[2] = (char)((codepoint & 0x3f) | 0x80);
        size = 3;
    }
    else if (codepoint <= 0x10ffff)
    {
        utf8[0] = (char)(((codepoint >> 18) & 0x07) | 0xf0);
        utf8[1] = (char)(((codepoint >> 12) & 0x3f) | 0x80);
        utf8[2] = (char)(((codepoint >>  6) & 0x3f) | 0x80);
        utf8[3] = (char)((codepoint & 0x3f) | 0x80);
        size = 4;
    }

    *byteSize = size;

    return utf8;
}

// Get next codepoint in a UTF-8 encoded text, scanning until '\0' is found
// When a invalid UTF-8 byte is encountered we exit as soon as possible and a '?'(0x3f) codepoint is returned
// Total number of bytes processed are returned as a parameter
// NOTE: the standard says U+FFFD should be returned in case of errors
// but that character is not supported by the default font in raylib
static int GetCodepointNext(const char *text, int *codepointSize)
{
    const char *ptr = text;
    int codepoint = 0x3f;       // Codepoint (defaults to '?')
    *codepointSize = 1;

    // Get current codepoint and bytes processed
    if (0xf0 == (0xf8 & ptr[0]))
    {
        // 4 byte UTF-8 codepoint
        if (((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80) || ((ptr[3] & 0xC0) ^ 0x80)) { return codepoint; } //10xxxxxx checks
        codepoint = ((0x07 & ptr[0]) << 18) | ((0x3f & ptr[1]) << 12) | ((0x3f & ptr[2]) << 6) | (0x3f & ptr[3]);
        *codepointSize = 4;
    }
    else if (0xe0 == (0xf0 & ptr[0]))
    {
        // 3 byte UTF-8 codepoint */
        if (((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80)) { return codepoint; } //10xxxxxx checks
        codepoint = ((0x0f & ptr[0]) << 12) | ((0x3f & ptr[1]) << 6) | (0x3f & ptr[2]);
        *codepointSize = 3;
    }
    else if (0xc0 == (0xe0 & ptr[0]))
    {
        // 2 byte UTF-8 codepoint
        if ((ptr[1] & 0xC0) ^ 0x80) { return codepoint; } //10xxxxxx checks
        codepoint = ((0x1f & ptr[0]) << 6) | (0x3f & ptr[1]);
        *codepointSize = 2;
    }
    else if (0x00 == (0x80 & ptr[0]))
    {
        // 1 byte UTF-8 codepoint
        codepoint = ptr[0];
        *codepointSize = 1;
    }

    return codepoint;
}
#endif      // RAYGUI_STANDALONE

#endif      // RAYGUI_IMPLEMENTATION
