# maclid
- Display controls and lid sync for Macbooks on Linux
- No additional drivers required 

## Prerequisites
- ACPI daemon
  - `sudo apt install acpid`

## Building
- Build
  - `make`
- Install
  - `sudo cp maclid /usr/local/bin/maclid`

## Register ACPI Events
- Set up event handler
  - `sudo cp maclid-event /etc/acpi/events/maclid-event`
- Restart ACPI daemon
  - `sudo systemctl restart acpid`

## Usage
`maclid blank|unblank|sync /dev/ttyX`
- Turn /dev/tty1 off
  - `maclid blank /dev/tty1`
- Turn /dev/tty1 on
  - `maclid unblank /dev/tty1`
