from pathlib import Path


RUN: bool = True


MAX_UDP_PACKET_SIZE: int = 128

UDP_SERVER_PORT: int = 4444
UDP_CLIENT_PORT: int = 4445

ROUTER_IP: int  = "10.42.0"
PC_IP: int      = f"{ROUTER_IP}.1"
PICO_IP: int    = f"{ROUTER_IP}.22"

PC_ADDRESS:   tuple[str, int] | None = None
PICO_ADDRESS: tuple[str, int] | None = None

END_LINE: str = "\n"
STORE_DATA: bool = False
DISPLAY_BINARY: bool = False

READ_FILE_PATH: Path = None
READ_FILE_DELAY: float = 0.1

OUT_FILE_PATH: Path = None


