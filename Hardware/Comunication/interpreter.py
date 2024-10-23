def interpretation(lines: list[str]) -> list[str]:
    new_lines = []
    for line in lines:
        new_lines.append(line.strip("\n"))
    return new_lines