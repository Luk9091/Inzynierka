import sys
import default
from pathlib import Path


def setPicoAddress(args: list[str]):
    if args[0] == "localhost":
        default.PICO_IP = args.pop(0)
        default.PC_IP = "localhost"
    else:
        default.PICO_IP = f"{default.ROUTER_IP}.{args.pop(0)}"


    if len(args) > 0:
        default.UDP_CLIENT_PORT = int(args.pop(0))
    if len(args) > 0 and default.PC_IP != "localhost":
        if args[0] == "localhost":
            default.PC_IP = args.pop(0)
        else:
            default.PC_IP = f"{default.ROUTER_IP}.{args.pop(0)}"
    if len(args) > 0:
        default.UDP_SERVER_PORT = int(args.pop(0))


def param_noNewLine(args: list[str]):
    default.END_LINE = ""
    args.remove("--noNewLine")

def param_save(args: list[str]):
    default.STORE_DATA = True

    try:
        if not args[args.index("--save")+1].startswith("-"):
            default.OUT_FILE_PATH = Path(args.pop(args.index("--save") + 1))
        else:
            default.OUT_FILE_PATH = Path("log.txt")
    except IndexError:
        default.OUT_FILE_PATH = Path("log.txt")
    args.remove("--save")

def param_outPort(args: list[str]):
    default.UDP_CLIENT_PORT = args.pop(args.index("--outPort") + 1)
    args.remove("--outPort")

def param_inPort(args: list[str]):
    default.UDP_SERVER_PORT = args.pop(args.index("--inPort") + 1)
    args.remove("--inPort")

def param_binDisplay(args: list[str]):
    default.DISPLAY_BINARY = True
    args.remove("--bin")

def param_file(args: list[str]):
    default.READ_FILE_PATH = args.pop(args.index("--file") + 1)
    args.remove("--file")

def param_fileDelay(args: list[str]):
    default.READ_FILE_DELAY = float(args.pop(args.index("--delay") + 1))
    args.remove("--delay")

def param_help(args: list[str]):
    print("Usage: python3 UDP_serial.py [PICO_ADDRESS] [PICO_PORT] [PC_ADDRESS] [PC_PORT]")
    print("Options:")
    print("  --noNewLine")
    print("  --save [FILE]")
    print("  --outPort [PORT]")
    print("  --inPort [PORT]")
    print("  --bin")
    print("  --file [FILE]")
    print("  --delay [SECONDS]")

    sys.exit(0)


params = {
    "--noNewLine": param_noNewLine,
    "--save": param_save,
    "--outPort": param_outPort,
    "--inPort": param_inPort,
    "--bin": param_binDisplay,
    "--file": param_file,
    "--delay": param_fileDelay,

    "--help": param_help
}



def param_parser():
    args = sys.argv[1:]
    if len(args) == 1 and args[0].count(" "):
        args = args[0].split(" ")
    arg_cp = args.copy()
    for arg in arg_cp:
        try:
            params[arg](args)
        except KeyError:
            pass
    
    if len(args) > 0:
        setPicoAddress(args)

    default.PC_ADDRESS = (default.PC_IP, default.UDP_SERVER_PORT)
    default.PICO_ADDRESS = (default.PICO_IP, default.UDP_CLIENT_PORT)