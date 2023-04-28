# t5sp-session-fix

A Plutonium T5SP server plugin to work around the session error.
 
- After 3 session modify fails, it will try to recreate the session.
- After 3 session create fails, it will try to reinitiate the connection with the Plutonium servers.

It hasn't been tested on very long server running sessions yet and might be unstable, your feedback is welcome!

The plugin skeleton is based on [t5-gsc-utils](https://github.com/fedddddd/t5-gsc-utils).

## Installation
### Required Plutonium version: <ins>r3417</ins>
Download or compile the [latest release](https://github.com/Nahelam/t5sp-session-fix/releases/latest/) and put it in the `%LOCALAPPDATA%\Plutonium\storage\t5\plugins` folder.
