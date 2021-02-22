#!/usr/bin/env python3
""" riscvdata.py - RISC-V instruction data

Used in the generator

@author: Cade Brown <cade@kscript.org>

"""

# Returns instruction kind, from instruction name
def get_kind(name):
    for i in insts:
        if i[0] == name:
            return i[1]

    raise KeyError(name)

exts = {

'RV32I': [
    # (name, kind, opcode, f3, f7)
    ('lui',    'U',  0b0110111,  0b000,  0b0000000),
    ('auipc',  'U',  0b0010111,  0b000,  0b0000000),
    ('jal',    'J',  0b1101111,  0b000,  0b0000000),
    ('jalr',   'I',  0b1100111,  0b000,  0b0000000),
    ('beq',    'B',  0b1100011,  0b000,  0b0000000),
    ('bne',    'B',  0b1100011,  0b001,  0b0000000),
    ('blt',    'B',  0b1100011,  0b100,  0b0000000),
    ('bge',    'B',  0b1100011,  0b101,  0b0000000),
    ('bltu',   'B',  0b1100011,  0b110,  0b0000000),
    ('bgeu',   'B',  0b1100011,  0b111,  0b0000000),
    ('lb',     'I',  0b0000011,  0b000,  0b0000000),
    ('lh',     'I',  0b0000011,  0b001,  0b0000000),
    ('lw',     'I',  0b0000011,  0b010,  0b0000000),
    ('lbu',    'I',  0b0000011,  0b100,  0b0000000),
    ('lhu',    'I',  0b0000011,  0b101,  0b0000000),
    ('sb',     'S',  0b0100011,  0b000,  0b0000000),
    ('sh',     'S',  0b0100011,  0b001,  0b0000000),
    ('sw',     'S',  0b0100011,  0b010,  0b0000000),
    ('addi',   'I',  0b0010011,  0b000,  0b0000000),
    ('slti',   'I',  0b0010011,  0b010,  0b0000000),
    ('sltiu',  'I',  0b0010011,  0b011,  0b0000000),
    ('xori',   'I',  0b0010011,  0b100,  0b0000000),
    ('ori',    'I',  0b0010011,  0b110,  0b0000000),
    ('andi',   'I',  0b0010011,  0b111,  0b0000000),
    ('slli',   'I',  0b0010011,  0b001,  0b0000000),
    ('srli',   'I',  0b0010011,  0b101,  0b0000000), # 'srli' and 'srai' are encoded the same, but the immediate value differs. Therefore, we only include 'srli' in the table, and the executor and encoder must solve that problem. SEE: https://stackoverflow.com/questions/39489318/risc-v-implementing-slli-srli-and-srai
    #('srai',   'I',  0b0010011,  0b101,  0b0000000),
    ('add',    'R',  0b0110011,  0b000,  0b0000000),
    ('sub',    'R',  0b0110011,  0b000,  0b0100000),
    ('sll',    'R',  0b0110011,  0b001,  0b0000000),
    ('slt',    'R',  0b0110011,  0b010,  0b0000000),
    ('sltu',   'R',  0b0110011,  0b011,  0b0000000),
    ('xor',    'R',  0b0110011,  0b100,  0b0000000),
    ('srl',    'R',  0b0110011,  0b101,  0b0000000),
    ('sra',    'R',  0b0110011,  0b101,  0b0010000),
    ('or',     'R',  0b0110011,  0b110,  0b0000000),
    ('and',    'R',  0b0110011,  0b111,  0b0000000),
    #('fence',  'y',  0b0000000,  0b000,  0b0000000),
    #('ecall',  'y',  0b0000000,  0b000,  0b0000000),
    #('ebreak', 'y',  0b0000000,  0b000,  0b0000000),
],
}

# List of all instructions
insts = sum((exts[k] for k in exts), [])
