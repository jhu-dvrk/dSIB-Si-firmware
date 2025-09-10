# dSIB-Si firmware

## Prerequisites

Compile:
- `arduino-cli` https://arduino.github.io/arduino-cli/1.2/
- `make`

Upload:
- `dfu-util`


## Compile

```
make build
```

## Upload

Hold down `DFU` button when you connect USB, then release `DFU` button.

```
make dfu
```
