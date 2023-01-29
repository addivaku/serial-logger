# serial-logger

![IDE](https://img.shields.io/badge/IDE-VS%20Code-blue?logo=visual-studio-code)
![cpp](https://img.shields.io/badge/Language-C%2B%2B-%2300599C?logo=C%2B%2B)
![Framework](https://img.shields.io/badge/Framework-Arduino-%2300979D?logo=arduino)
![PlatformStm](https://img.shields.io/badge/Platform-STM32-%2303234B?logo=stmicroelectronics)
![PlatformAvr](https://img.shields.io/badge/Platform-Atmel%20AVR-red)

A simple and lightweight logging tool for the `Arduino` framework. Designed as
`Platform IO` library, it provides the user with the ability to output logging
details via the serial console. The user can decide about the log level and
which serial instance is used. It can even disable all output.

## Features

The logger prints arbitrary log details to the serial output. The output adheres
to the following pattern:

```log
[<log level>] content
```

The user can choose between the following log levels:

- debug
- info
- warning
- error
- critical

The listing starts from the most verbose level and ends at the most quiet one.
Any level contains all following levels, too. If the user chooses `warning`,
messages designated for the `error` and `critical` level are printed, too.
Messages flagged with `info` and `debug` are discarded.

The user can disable all logging output.

## Integration

This section shows you how to use the library.

### IDE integration

The library can be integrated in any `Platform IO` project. Just add this code
to your project's `lib` folder and add the following include

### Interface usage

The module is wrapped in the `logging` namespace. The most important classes are

1. the `logging::Level` class which defines the log levels as an enumeration and
2. the `logging::Logger` class which contains the whole interface.

The logger is implemented as a pure static class. To use it, you do not need
to instantiate an object. Instead, you need to assign it a reference to an
instance of the `HardwareSerial` class via the `logging::Logger::setSerial`
interface. If you set `nullptr` (default case), all log requests are discarded.

To set the log level, you can use the `logging::Logger::setLevel` interface.
Note that the default level is `kInfo`. Use `logging::Logger::loglevel` to add
the current log level configuration to the logs.

The following _pseudo code_ shows two different possibilities to flag a message
with a level:

1. `logging::Logger::log(<level>, <message>)`
2. `logging::Logger::<level>(<message>)`

The second option most probably requires an explanation. Each log level exposes
a method that has the same name and flags the incoming message with the
respective level. Basically, those methods are wrappers for the `::log` method
to simplify the usage.

Initially, each log method was designed to accept the same arguments as the
`HardwareSerial::printxx` methods. Hence, you find the template

```C++
template <classname T>
static void logging::Logger::log(logging::Level level, T data);
```

for the generic `log` method. The same exists for the level-specific methods.
The latest extension adds an interface that allows the `printf` syntax:

```C++
static void logging::Logger::log(logging::Level level, const char * fmt, ...);
static void logging::Logger::debug(const char * fmt, ...); 
...
```

### Examples

Check the [examples] or look at [serial-logger-meta]'s main code for an
interactive test program.

[serial-logger-meta]: https://github.com/addivaku/rotary-encoder-meta
[examples]: ./examples/

## Future development

This section collects topics that will/can become part of the logger library.

### Road map

This section contains the features that _will_ be implemented at some point.

### Ideas

This section contains some ideas that might be integrated at some point.

- Optimize memory usage/handling/allocation

- Add the current time (`millis()`) to each log line
