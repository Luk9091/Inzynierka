import default

def save(string: str) -> None:
    if default.OUT_FILE_PATH is None:
        return
    if string.endswith("\r"):
        string = string[:-1]
    if string.endswith("\n"):
        string = string[:-1]

    with open(default.OUT_FILE_PATH, "a") as file:
        file.write(rf"{string}" + "\n")

