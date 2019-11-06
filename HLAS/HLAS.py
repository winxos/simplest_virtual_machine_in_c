# coding:utf-8
# HLAS FOR HLVM project
# PYTHON 3.7
# winxos 20191105
import glob
import sys

keywords = {
    "ldr": 0x01,
    "str": 0x02,
    "ldri": 0x03,
    "add": 0x04,
    "sub": 0x05,
    "mul": 0x06,
    "div": 0x07,
    "mod": 0x08,
    "inc": 0x09,
    "dec": 0x0a,
    "neg": 0x0b,
    "and": 0x0c,
    "or": 0x0d,
    "xor": 0x0e,
    "b": 0x0f,
    "bl": 0x10,
    "bz": 0x11,
    "bn": 0x12,
    "int": 0x13,
    "halt": 0x14,
    "push": 0x15,
    "pop": 0x16,
    "slp": 0x17,
    "ret": 0x18,
};
single_operators = {"inc", "dec", "halt", "push", "pop", "ret", "add", "sub", "mul", "div", "and", "or", "xor"}
var_table = {}
label_table = {}


def pretranslate(src_code):
    after_code = []
    for line in src_code:
        cmds = str(line).split()
        if len(cmds) > 0:
            if cmds[0] == "dim":  # vars
                if len(cmds) == 2:
                    cmds.append("0")
                if len(cmds) > 2:
                    if cmds[1] in var_table:
                        print("[ERR] VAR %s ALREADY DEFINED." % cmds[1])
                        return
                    var_table[cmds[1]] = cmds[2]
            elif cmds[0].startswith("."):  # label
                if cmds[0] in label_table:
                    print("[ERR] LABEL %s ALREADY DEFINED." % cmds[0])
                    return
                label_table[cmds[0]] = len(after_code)
            elif cmds[0].startswith(";"):  # comment
                pass
            else:  # normal code
                after_code.append(cmds)

    for i, n in enumerate(var_table):  # append variables to the end
        after_code.append(var_table[n])
        var_table[n] = len(after_code) - 1
    return after_code


def translate(codes):
    ml = []
    for index, code in enumerate(codes):
        if index >= len(codes) - len(var_table):
            for i in (codes[-len(var_table):]):  # the variables
                ml.append(int(i))
            break
        operator = keywords[code[0]]
        operand = 0
        if code[0] == "b" or code[0] == "bl" or code[0] == "bn" or code[0] == "bz":
            if len(code) > 1:
                label = "." + code[1]
                if label in label_table:
                    operand = label_table[label]
                else:
                    print("[ERR] LABEL %S NOT FOUND." % code[1])
                    return
            else:
                print("[ERR] LINE % MISS LABEL." % index)
                return
        else:
            if len(code) > 1:
                if code[1] in var_table:
                    operand = var_table[code[1]]
                elif code[0] == "ldri" or code[0] == "slp":
                    operand = int(code[1])
                else:
                    print("[ERR] VAR %s NOT FOUND." % code[1])
                    return
            else:
                if code[0] not in single_operators:  # single operand
                    print("[ERR] LINE %d MISS OPERAND." % index)
                    return
        ml.append((operator << 24) + operand)
    return ml


def deal_sal(n):
    fn = str(n).split(".")[0] + ".vm"
    try:
        with open(n) as fi, open(fn, "w") as fo:
            c = translate(pretranslate(fi.read().split("\n")))
            for i in c:
                # fo.write("%08x\n"%i)
                fo.write("%x %x\n" % (i >> 24, i & 0x00ffffff))
            print("[success] ASSEMBLY FILE %s TO %s" % (n, fn))
    except IOError as e:
        print("[error] %s" % str(e))


def console():
    print("HLVM ASSEMBLY TOOL v1.2")
    print("\t\twinxos 20191105")
    print("\n")
    print("ls\t\t[list *.as]")
    print("as xx\t\t[assembly xx.as to xx.vm]")
    print("exit\t\t[exit program]")
    while True:
        cmd = str(input("HLAS>"))
        cmds = cmd.split(" ")
        if cmds[0] == "ls":
            print("\n".join(glob.glob(r"*.as")))
        if cmds[0] == "as":
            deal_sal(cmds[1] + ".as")
        if cmds[0] == "exit":
            sys.exit()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        deal_sal(sys.argv[1])
    else:
        console()
