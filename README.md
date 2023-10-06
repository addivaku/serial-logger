# serial-logger

![IDE](https://img.shields.io/badge/IDE-VS%20Code-blue?logo=visual-studio-code)
![cpp](https://img.shields.io/badge/Language-C%2B%2B-%2300599C?logo=C%2B%2B)
![Framework](https://img.shields.io/badge/Framework-Arduino-%2300979D?logo=arduino)
![PlatformStm](https://img.shields.io/badge/Platform-STM32-%2303234B?logo=stmicroelectronics)
![PlatformAvr](https://img.shields.io/badge/Platform-Atmel%20AVR-red)

A simple and lightweight logging tool for the `Arduino` framework. Designed as
`Platform IO` library, it provides the user with the ability to output logging
details via the serial console. The user can decide about the log level and
which serial instance to use. The library allows filtering messages by severity.

## Features

The logger prints arbitrary log details to the serial output. The output adheres
to the following pattern:

```log
[<log level>] <log string>
```

The user can choose between the following severity levels:

- `debug`
- `info`
- `warning`
- `error`
- `critical`

The listing starts at the most verbose level and ends with the most quiet one.
A level includes the subsequent levels, too. This means that if the user chooses
`warning`, messages flagged with `error` and `critical` are printed as well.
Messages flagged with `info` or `debug`, however, are dropped.

If the log output is not or no longer needed, the user can disable it easily.
The user can also enable colorized messages if the terminal supports it. Please
see the sections [Memory optimization](#memory-optimization) and
[Colorization](#colorization) for further details.

## Integration

This section demonstrates how to use the library.

### IDE integration

The library can be integrated in any `Platform IO` project. Just add this
library to your project's `lib` folder and add the following include:

```cpp
#include <serial-logger.hpp>
```

### Interface usage

The module is wrapped in the `logging` namespace. The most important classes are

1. the `logging::Level` class which defines the log levels (=severity) and
2. the `logging::Logger` class which defines the logging interface.

The logger is implemented as a pure static class. You need to assign it a
pointer to an instance of the `HardwareSerial` class via the
`logging::Logger::setSerial` interface. If you set `nullptr` (default case),
all log requests are dropped.

To set the log level, you can use the `logging::Logger::setLevel` interface.
Note that the default level is `kInfo`. Use `logging::Logger::loglevel` to print
the current log level configuration to the logs.

The following _pseudo code_ shows two different possibilities to issue a
message:

1. `logging::Logger::log(<level>, <message>)`
2. `logging::Logger::<level>(<message>)`

While the first option is pretty straightforward, the second one requires most
probably an explanation. The `Logger` class exposes one method for each severity
that issues a message with the respective severity. They are basically wrappers
for the `logging::Logger::log(...)` method.

The interface the logging methods expose is pretty similar to that of Arduino's
`HardwareSeria` class. It uses a template definition so that you are able to
pass every data type you want to print:

```cpp
template <classname T>
static void logging::Logger::log(logging::Level level, T data);

template <classname T>
static void logging::Logger::<level>(T data);
```

Please note that the `data` is forward to `HardwareSerial`'s `println` method.
This means that if `HardwareSerial` cannot print it, this library cannot either.

However, the library provides advanced methods that allows the user to use the
well-known `printf` syntax to issue a log message:

```C++
static void logging::Logger::log(logging::Level level, const char * fmt, ...);
static void logging::Logger::<level>(const char * fmt, ...);
```

:warning: Please see the limitations that affect this way of logging:

- printing floating point numbers like `float` und `double` is memory-consuming
  and disabled by default (in platformio). If you have the resources, you can
  modify your project's `platformio.ini` with

  ```text
  [env:stm32]
    build_flags = -D PIO_FRAMEWORK_ARDUINO_NANOLIB_FLOAT_PRINTF
  # or
  [env:avr]
    build_flags = -Wl,-u,vfprintf -lprintf_flt -lm
  ```

  See the [PlatformIO Community post] / [Github Thread] for details.

  :rotating_light: If you use float strings without the above addition, your
  program might hang!

- `%lu`, `%llu`, and similar identifiers for huge integers are not working as
  used from other libraries.

[PlatformIO Community post]: https://community.platformio.org/t/how-to-make-stm32duino-support-sprintf-under-arduino-frame/17402/2
[Github Thread]: https://github.com/platformio/platformio-vscode-ide/issues/1385

### Memory optimization

It is known that exhaustive logging is expensive and memory consuming. This
applies to embedded devices in particular. Setting the pointer to the
`HardwareSerial` instance to `nullptr` disables logging, but does not disable
the internals so that the memory footprint stays very high.

This can be resolved by defining the build flag

```
LOGGER_VOID_ALL
```

If defined, the preprocessor voids all internals, but keeps the interface up.
Note that this also disables logging!

### Colorization

Per default, the logger does not colorize the log messages. If you use a console
that supports [ANSI coloring], you might want to enable it to easily identify
important messages. Known programs that support coloring are

- PlatformIO serial monitor : Start the monitor with the `--raw` option

- PuTTY : Make sure to allow colours in the settings `Window-->Colours`

- picocom

These colors were chosen so that the importance becomes obvious immediately:

- `DEBUG` : blue
- `INFO` : green
- `WARNING` : yellow
- `ERROR` : red
- `CRITICAL` : red background and white font

Note that the PlatformIO monitor does not show the `CRITICAL` messages
correctly. You might see just the background with a dark font.

[ANSI coloring]: https://stackoverflow.com/a/33206814

### Examples

Check the [examples] or look at [serial-logger-meta]'s main code for an
interactive test program.

[serial-logger-meta]: https://github.com/addivaku/serial-logger-meta
[examples]: ./examples/

## Future development

This section collects topics that will/can become part of the logger library.

### Road map

This section contains the features that _will_ be implemented at some point.

### Ideas

This section contains some ideas that _might_ be integrated at some point.

- Optimize memory usage/handling/allocation

- Add the current time (`millis()`) to each log line

- Support `Stream`-like objects
