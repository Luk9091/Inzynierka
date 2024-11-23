from time import sleep
from enum import Enum, IntEnum, auto
from colorama import Fore, Back, Style
import log
import socket
import default


def send(string: str, udp_socket: socket.socket) -> None:
    if udp_socket is None:
        return
    string = string.encode()
    data = bytearray([*string, *b'\x00'])
    udp_socket.sendto(data, default.PICO_ADDRESS)

def removeTillChar(string: str):
    for i in range(len(string)):
        if string[i] != ' ':
            return string[i:]
    return string

def getVar(data: list[str], variables: dict[str, float]) -> tuple[bool, float, float]:
        first = data[0]
        try:
            first = variables[first]
        except KeyError:
            if first.isnumeric():
                first = float(first)
            elif '"' in second:
                second = second.replace('"', '')
            else:
                return False, 0, 0

        try:
            second = data[2]
            second = variables[second]
        except IndexError:
            return True, first, 0
        except KeyError:
            if second.isnumeric():
                second = float(second)
            elif '"' in second:
                second = second.replace('"', '')
            else:
                return False, 0, 0
        
        return True, first, second


key_words = {

}


def interpretation(lines: list[str], socket):
    stack = []
    variables: dict[str, float] = {}
    loop_address: dict[str, int] = {}

    for i in range(len(lines)):
        lines[i] = removeTillChar(lines[i]).strip("\n")
        line = lines[i]

        if line.endswith(":"):
            loop_address[line[:-1]] = i
            continue
        # if "loop" in line:
        #     try:
        #         loop_address[line.split(' ')[1]] = program_counter
        #     except IndexError:
        #         print(f"{Fore.RED}Error: loop must have a name in line {lines.index(line)}{Style.RESET_ALL}")
        #         return
        #     continue
                
        if any([
            "==" in line, "!=" in line, "if" in line, ">" in line, "<" in line, ">=" in line, "<=" in line
        ]):
            pass
        elif "=" in line:
            words = line.split(' ')
            variable = words[words.index('=') - 1]
            if not variable in variables:
                try:
                    value = float(words[words.index('=') + 1])
                except IndexError:
                    value = 0
                except ValueError:
                    value = words[variable]

                variables[variable] = value


    # print(variables)
    program_counter: int = 0
    while program_counter < len(lines):
        line = lines[program_counter]
        program_counter = program_counter + 1

        if any([
            line.endswith(":"), line == "", line == "\n"
        ]):
            continue

        if "$" in line:
            words = line.split(' ')
            new_line = ""
            for word in words:
                if "$" in word:
                    try:
                        new_line = new_line + str(variables[word[1:]])
                    except IndexError:
                        print(f"{Fore.RED}Error: not existing variable:{Style.RESET_ALL} {line}")
                        return
                else:
                    new_line = new_line + word
                new_line = new_line + " "
            line = new_line[:-1]


        if "jump" in line:
            try:
                stack.append(program_counter)
                program_counter = loop_address[line.split(' ')[1]] + 1
            except KeyError:
                print(f"{Fore.RED}Error: loop address not found:{Style.RESET_ALL} {line}")
                return
            continue

        if "end" in line:
            try:
                program_counter = stack.pop()
            except IndexError:
                return
            
            continue

        if "print" in line:
            if '"' in line:
                data = line.strip("print ")
                data = data.replace('"', '')
                print(data)
                log.save(data)
                continue

            if line.split(' ')[1] in variables:
                print(variables[line.split(' ')[1]])
                continue

            print(f"{Fore.RED}Error: variable not found:{Style.RESET_ALL} {line}")
            return

        if "sleep" in line:
            sleep(float(line.split(' ')[1]))
            continue

        if "if" in line:
            data = line.split(' ')[1:]
            success, first, second = getVar(data, variables)
            if not success:
                print(f"{Fore.RED}Error: variable not found:{Style.RESET_ALL} {line}")
                return

            if "==" in line:
                if not first == second:
                    program_counter = program_counter + 1
            elif "!=" in line:
                if not first != second:
                    program_counter = program_counter + 1
            elif ">" in line:
                if not first > second:
                    program_counter = program_counter + 1
            elif "<" in line:
                if not first < second:
                    program_counter = program_counter + 1
            elif ">=" in line:
                if not first >= second:
                    program_counter = program_counter + 1
            elif "<=" in line:
                if not first <= second:
                    program_counter = program_counter + 1
            else:
                if not first:
                    program_counter = program_counter + 1
            continue

        if "=" in line.split(' '):
            words = line.split(' ')
            set_data = words[words.index('=') - 1]
            data = words[words.index('=') + 1:]

            success, first, second = getVar(data, variables)
            if not success:
                print(f"{Fore.RED}Error: variable not found:{Style.RESET_ALL} {line}")
                return

            if "+" in data:
                if type(first) == str or type(second) == str:
                    print(f"{Fore.RED}Error: wrong type:{Style.RESET_ALL} {line}")
                    return

                variables[set_data] = first + second
            elif "-" in data:
                if type(first) == str or type(second) == str:
                    print(f"{Fore.RED}Error: wrong type:{Style.RESET_ALL} {line}")
                    return
                variables[set_data] = first - second
            elif "*" in data:
                if type(first) == str or type(second) == str:
                    print(f"{Fore.RED}Error: wrong type:{Style.RESET_ALL} {line}")
                    return
                variables[set_data] = first * second
            elif "/" in data:
                if type(first) == str or type(second) == str:
                    print(f"{Fore.RED}Error: wrong type:{Style.RESET_ALL} {line}")
                    return
                variables[set_data] = first / second
            else:
                variables[set_data] = first

            continue

        print(f">>> {line}")
        send(line, socket)

    print(Fore.GREEN + "Program finished" + Style.RESET_ALL)


if __name__ == "__main__":
    lines = [
    "distance = 10",
    "f $distance",
    ]

    try:
        interpretation(lines, None)
    except (KeyboardInterrupt, EOFError) as e:
        if isinstance(e, KeyboardInterrupt):
            print()
        print(Fore.RED + "Exiting..." + Style.RESET_ALL)
        pass