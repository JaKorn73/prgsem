
# PRG semestral project Jakub Kornel
This project is divided into two "separate" parts, which are computation module and module for handling user input and managing printing out.

## Deployment

To download dependencies for this project on ubuntu run:

```bash
  apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

To compile it simply run:
```bash
  make
```

If you would like to compile only one part simply add `comp` or `prgsem`. For cleaning all binaries run `clean`.

For running the program, you have to first run `./create_pipes` and after that in two separate windows `./comp` and `./prgsem`.
## Features

- basic fractal computation
- complete GUI (menu)
- 


## Usage/Examples

This app supports couple commands. You cannot enter them directly into bash since this was old feature but when focused with mouse on the popup.

Here is a list of commands:
 - `g` - prints out modul version (MSG_GET_VERSION)
 - `s` - sets parameters for computation (MSG_SET_COMPUTE)
 - `1` - spustí výpočet (MSG_COMPUTE)
 - `a` - aborts running computation (MSG_ABORT)
 - `r` - resets chunk id to 0
 - `l` - smaže aktuální obsah výpočtu (bufferu)
 - `p` - překreslí obsah okna aktuálním stavem výpočtu (bufferem)
 - `c` - computes fractal and prints it out
 - `q` - ends the program
 - `KEY_UP` - increase the number `n` of iterations (this makes computation more precise)
 - `KEY_DOWN` - decrease number `n`

Also this app provides menu when you click on the red box at the bottom, you get simple menu popup, where you can setup all needed parameters.

For exiting the application you can also click on the cross in top right corner.

If you would like to resize the window, you can do that by catching edge and putting it to desired width and height. Then run `0` to refresh the app to adjust itself.