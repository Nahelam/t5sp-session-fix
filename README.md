# t5sp-session-fix

A Plutonium T5SP server plugin to work around the session error.
 
- After 3 consecutive session modify fails, it will try to recreate the session.
- After 3 consecutive session create fails, it will try to reinitiate the connection with the Plutonium servers.

The plugin skeleton is based on [t5-gsc-utils](https://github.com/fedddddd/t5-gsc-utils).

## Installation
Download or compile the [latest release](https://github.com/Nahelam/t5sp-session-fix/releases/latest/) and put it in the `%LOCALAPPDATA%\Plutonium\storage\t5\plugins` folder.
