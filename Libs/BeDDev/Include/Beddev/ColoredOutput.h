#pragma once

#include <iostream>
#include <ostream>
#include <string>

#if defined(__APPLE__)
#   include <TargetConditionals.h>
#   if TARGET_OS_OSX == 1
#       define BEDDEV_PLATFORM_MAC
#   elif TARGET_OS_IPHONE == 1
#       define BEDDEV_PLATFORM_IPHONE
#   endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
#   define BEDDEV_PLATFORM_LINUX
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER) || defined(__MINGW32__)
#   define BEDDEV_PLATFORM_WINDOWS
#endif

#if defined(BEDDEV_PLATFORM_WINDOWS)

#if !defined(NOMINMAX) && !defined(BEDDEV_CONFIG_NO_NOMINMAX)
#  define BEDDEV_DEFINED_NOMINMAX
#  define NOMINMAX
#endif
#if !defined(WIN32_LEAN_AND_MEAN) && !defined(BEDDEV_CONFIG_NO_WIN32_LEAN_AND_MEAN)
#  define BEDDEV_DEFINED_WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif

#ifdef __AFXDLL
#include <AfxWin.h>
#else
#include <windows.h>
#endif

#ifdef BEDDEV_DEFINED_NOMINMAX
#  undef NOMINMAX
#endif
#ifdef BEDDEV_DEFINED_WIN32_LEAN_AND_MEAN
#  undef WIN32_LEAN_AND_MEAN
#endif

#endif // defined(BEDDEV_PLATFORM_WINDOWS)

#define BEDDEV_ERROR(msg) throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + std::string(msg))

namespace TestingImpl
{

enum ColorCode
{
    None = 0,

    White,
    Red,
    Green,
    Blue,
    Cyan,
    Yellow,
    Grey,

    Bright = 0x10,

    BrightRed = Bright | Red,
    BrightGreen = Bright | Green,
    LightGrey = Bright | Grey,
    BrightWhite = Bright | White,
    BrightYellow = Bright | Yellow,

    // By intention
    FileName = LightGrey,
    Warning = BrightYellow,
    ResultError = BrightRed,
    ResultSuccess = BrightGreen,
    ResultExpectedFailure = Warning,

    Error = BrightRed,
    Success = Green,

    OriginalExpression = Cyan,
    ReconstructedExpression = BrightYellow,

    SecondaryText = LightGrey,
    Headers = White
};

class IColourImpl
{
public:
    virtual ~IColourImpl() = default;
    virtual void use(ColorCode colourCode) = 0;
};

IColourImpl* platformColourInstance();

struct Colour
{
    Colour(ColorCode colourCode)
    {
        use(colourCode);
    }

    Colour(Colour&& rhs) noexcept
    {
        m_moved = rhs.m_moved;
        rhs.m_moved = true;
    }

    Colour& operator=(Colour&& rhs) noexcept
    {
        m_moved = rhs.m_moved;
        rhs.m_moved = true;
        return *this;
    }

    ~Colour()
    {
        if (!m_moved) use(None);
    }

    void use(ColorCode colourCode)
    {
        static IColourImpl* impl = platformColourInstance();
        impl->use(colourCode);
    }

private:
    bool m_moved = false;
};

inline std::ostream& operator<<(std::ostream& os, Colour const&) { return os; }

struct NoColourImpl : IColourImpl
{
    void use(ColorCode) {}

    static IColourImpl* instance() {
        static NoColourImpl s_instance;
        return &s_instance;
    }
};

#if defined(BEDDEV_PLATFORM_WINDOWS)
class Win32ColourImpl : public IColourImpl
{
public:
    Win32ColourImpl() : stdoutHandle(GetStdHandle(STD_OUTPUT_HANDLE))
    {
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(stdoutHandle, &csbiInfo);
        originalForegroundAttributes = csbiInfo.wAttributes & ~(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        originalBackgroundAttributes = csbiInfo.wAttributes & ~(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    void use(ColorCode code) override
    {
        switch (code)
        {
        case None:      return setTextAttribute(originalForegroundAttributes);
        case White:     return setTextAttribute(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        case Red:       return setTextAttribute(FOREGROUND_RED);
        case Green:     return setTextAttribute(FOREGROUND_GREEN);
        case Blue:      return setTextAttribute(FOREGROUND_BLUE);
        case Cyan:      return setTextAttribute(FOREGROUND_BLUE | FOREGROUND_GREEN);
        case Yellow:    return setTextAttribute(FOREGROUND_RED | FOREGROUND_GREEN);
        case Grey:      return setTextAttribute(0);

        case LightGrey:     return setTextAttribute(FOREGROUND_INTENSITY);
        case BrightRed:     return setTextAttribute(FOREGROUND_INTENSITY | FOREGROUND_RED);
        case BrightGreen:   return setTextAttribute(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        case BrightWhite:   return setTextAttribute(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        case BrightYellow:  return setTextAttribute(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

        case Bright: BEDDEV_ERROR("Internal test error: not a colour");

        default:
            BEDDEV_ERROR("Unknown colour requested");
        }
    }

private:
    void setTextAttribute(WORD _textAttribute)
    {
        SetConsoleTextAttribute(stdoutHandle, _textAttribute | originalBackgroundAttributes);
    }

    HANDLE stdoutHandle;
    WORD originalForegroundAttributes;
    WORD originalBackgroundAttributes;
};
#endif

class PosixColourImpl : public IColourImpl
{
public:
    void use(ColorCode _colourCode) override
    {
        switch (_colourCode) {
        case None:
        case White:     return setColour("[0m");
        case Red:       return setColour("[0;31m");
        case Green:     return setColour("[0;32m");
        case Blue:      return setColour("[0;34m");
        case Cyan:      return setColour("[0;36m");
        case Yellow:    return setColour("[0;33m");
        case Grey:      return setColour("[1;30m");

        case LightGrey:     return setColour("[0;37m");
        case BrightRed:     return setColour("[1;31m");
        case BrightGreen:   return setColour("[1;32m");
        case BrightWhite:   return setColour("[1;37m");
        case BrightYellow:  return setColour("[1;33m");

        case Bright: BEDDEV_ERROR("not a colour");
        default: BEDDEV_ERROR("Unknown colour requested");
        }
    }

    static IColourImpl* instance()
    {
        static PosixColourImpl s_instance;
        return &s_instance;
    }

private:
    void setColour(const char* escapeCode)
    {
        // TODO: get current std::ostream
        //getCurrentContext().getConfig()->stream()
        std::cout << '\033' << escapeCode;
    }
};

inline IColourImpl* platformColourInstance()
{
#if defined(BEDDEV_PLATFORM_WINDOWS)
    static Win32ColourImpl s_instance;
#else
    static PosixColourImpl s_instance;
#endif

    // TODO: read from settings
    bool useColor = true;

    return useColor ? &s_instance : NoColourImpl::instance();
}

}
